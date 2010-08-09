/*
 * This file is part of gvis.
 *
 * Copyright (C) 2008, 2009, 2010
 *    Gabriel Alvarez
 *
 * gvis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gvis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gvis.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Imagen.h"
#include <math.h>

#define PI 3.14159265

Imagen::Imagen(Glib::ustring archivoCab)
{
  tamanioBloque = 100;
  nombreArchivo = archivoCab;
  FILE *f;
  f = fopen(archivoCab.c_str(), "r");
  cabecera = Cabecera::crearCabecera(f);
  fclose(f);
  for (unsigned int i = 0; i < cabecera->getBandasPresentes().length(); ++i)
    {
      Banda* c = new Banda(cabecera->getBandasPresentes().substr(i, 1), false,
          this);
      vectorBanda.push_back(c);
    }
}

void
Imagen::cargarBandas()
{
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    {
      vectorBanda[i]->cargarBanda();
    }
}

void
Imagen::corregirL()
{

  gfloat maxAj = -1000;
  gfloat minAj = 1000;

  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    if (vectorBanda[i]->cargada)
      {
        // todo: siempre seran 0 y 255 los max y min ND de cada banda?
        gfloat maxBanda = 255 * cabecera->gainBias[i].gain
            + cabecera->gainBias[i].bias;
        gfloat minBanda = cabecera->gainBias[i].bias;
        if (maxBanda > maxAj)
          maxAj = maxBanda;

        if (minBanda < minAj)
          minAj = minBanda;
      }
  // calculo un gain y un bias para normalizar a 255
  gfloat bNorm = 255 / (1 - (maxAj / minAj));
  gfloat gNorm = (255 - bNorm) / maxAj;

  //  g_print("max: %f min: %f \n", (max,min,gc,bc);
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    {
      if (vectorBanda[i]->cargada)
        {
          //calculo nuevos b y g para ajustar la luminancia y normalizar en un solo paso
          gfloat bAjustado = cabecera->gainBias[i].bias * gNorm + bNorm;
          gfloat gAjustado = (cabecera->gainBias[i].gain
              + cabecera->gainBias[i].bias) * gNorm + bNorm - bAjustado;
          for (int ii = 0; ii < cabecera->alto; ii++)
            {
              for (int iii = 0; iii < cabecera->ancho; iii++)
                {
                  vectorBanda[i]->matriz[ii][iii]
                      = vectorBanda[i]->matriz[ii][iii] * gAjustado + bAjustado;
                }
            }
        }
    }
}

void
Imagen::corregirRHO()
{

  gfloat d = 1 - 0.0167 * cos(
      (2 * PI * (cabecera->getFecha().get_julian() - 3)) / 365);

  gfloat titaSat = 0;
  gfloat titaSol = 90 - cabecera->getTitaSol();
  gfloat phiSat = 0;
  gfloat phiSol = cabecera->getPhiSol();

  titaSat = titaSat * PI / 180;
  phiSat = phiSat * PI / 180;
  titaSol = titaSol * PI / 180;
  phiSol = phiSol * PI / 180;

  gfloat cosenoMas = cos(titaSat) * cos(titaSol) - sin(titaSat) * sin(titaSol)
      * cos(phiSat - phiSol);
  gfloat cosenoMenos = -cos(titaSat) * cos(titaSol) - sin(titaSat) * sin(
      titaSol) * cos(phiSat - phiSol);

  gfloat faseMas = 0.75 * (1 + cosenoMas * cosenoMas);
  gfloat faseMenos = 0.75 * (1 + cosenoMenos * cosenoMenos);

  std::vector<gfloat> lr;

  gfloat maxAj = -1000;
  gfloat minAj = 1000;

  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    if (vectorBanda[i]->cargada)
      {
        lr[i] = ((cabecera->solct[i] * cabecera->taur[i]) / (4 * PI * d * d
            * cos(titaSat))) * (exp(-cabecera->taug[i] / cos(titaSat))) * (exp(
            -cabecera->taug[i] / cos(titaSol))) * (faseMenos + 0.052 * faseMas);

        // todo: siempre seran 0 y 255 los max y min ND de cada banda?
        gfloat maxBanda = 255 * cabecera->gainBias[i].gain
            + cabecera->gainBias[i].bias;
        maxBanda = (d * d * PI * (maxBanda - lr[i])) / (cos(titaSol)
            * cabecera->solct[i]);
        gfloat minBanda = cabecera->gainBias[i].bias;
        minBanda = (d * d * PI * (minBanda - lr[i])) / (cos(titaSol)
            * cabecera->solct[i]);
        if (maxBanda > maxAj)
          maxAj = maxBanda;

        if (minBanda < minAj)
          minAj = minBanda;
      }
  // calculo un gain y un bias para normalizar a 255
  gfloat bNorm = 255 / (1 - (maxAj / minAj));
  gfloat gNorm = (255 - bNorm) / maxAj;

  //  g_print("max: %f min: %f \n", (max,min,gc,bc);
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    {
      if (vectorBanda[i]->cargada)
        {
          //calculo nuevos b y g para ajustar la radiancia y normalizar en un solo paso
          gfloat bAjustado = cabecera->gainBias[i].bias * gNorm + bNorm;
          gfloat gAjustado = (cabecera->gainBias[i].gain
              + cabecera->gainBias[i].bias) * gNorm + bNorm - bAjustado;
          for (int ii = 0; ii < cabecera->alto; ii++)
            {
              for (int iii = 0; iii < cabecera->ancho; iii++)
                {
                  vectorBanda[i]->matriz[ii][iii]
                      = vectorBanda[i]->matriz[ii][iii] * gAjustado + bAjustado;
                }
            }
        }
    }
}

Glib::ustring
Imagen::getDirectorio()
{

  Glib::ustring::size_type inicio = nombreArchivo.length();

  // TODO cambiar / para version windows
  inicio = nombreArchivo.rfind("/", inicio) + 1;

  if (inicio == nombreArchivo.length())
    inicio = nombreArchivo.rfind("\\", inicio) + 1;
  Glib::ustring cadena = nombreArchivo.substr(0, inicio);

  return cadena;
}
int
Imagen::obtenerNumeroBanda(Glib::ustring cual)
{
  for (unsigned int var = 0; var < vectorBanda.size(); ++var)
    {
      if (vectorBanda[var]->nombre == cual)
        return var;
    }
  return -1;
}
Imagen::~Imagen()
{
}
