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
#include <values.h>

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
      maxB.push_back(255.0);
      minB.push_back(0);
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
Imagen::corregirL(bool ajustarRangoDinamicoPorBanda)
{

  gfloat maxAj = MINFLOAT;
  gfloat minAj = MAXFLOAT;
  maxB.clear();
  minB.clear();
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    if (vectorBanda[i]->cargada)
      {
        gfloat maxBanda = 255.0 * cabecera->gainBias[i].gain
            + cabecera->gainBias[i].bias;
        gfloat minBanda = cabecera->gainBias[i].bias;
        g_print("Banda %i maximo: %f minimo: %f\n", i + 1, maxBanda, minBanda);
        maxB.push_back(maxBanda);
        minB.push_back(minBanda);
        if (maxBanda > maxAj)
          maxAj = maxBanda;

        if (minBanda < minAj)
          minAj = minBanda;
      }
    else
      {
        maxB.push_back(0);
        minB.push_back(0);
      }

  g_print("Todas las Bandas maximo: %f minimo: %f\n", maxAj, minAj);
  // calculo un gain y un bias para normalizar a 255
  gfloat bNorm = 255.0 / (1 - (maxAj / minAj));
  gfloat gNorm = (255.0 - bNorm) / maxAj;

  //  g_print("max: %f min: %f \n", (max,min,gc,bc);
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    {
      if (vectorBanda[i]->cargada)
        {
          if (ajustarRangoDinamicoPorBanda)
            {
              bNorm = 255.0 / (1 - (maxB[i] / minB[i]));
              gNorm = (255.0- bNorm) / maxB[i];
            }
          //calculo nuevos b y g para ajustar la luminancia y normalizar en un solo paso
          gfloat bAjustado = cabecera->gainBias[i].bias * gNorm + bNorm;
          gfloat gAjustado = (cabecera->gainBias[i].gain
              + cabecera->gainBias[i].bias) * gNorm + bNorm - bAjustado;
          for (int ii = 0; ii < cabecera->alto; ii++)

            for (int iii = 0; iii < cabecera->ancho; iii++)

              vectorBanda[i]->matriz[ii][iii] = vectorBanda[i]->matriz[ii][iii]
                  * gAjustado + bAjustado;

        }
    }
}

void
Imagen::corregirRHO(bool ajustarRangoDinamicoPorBanda)
{

  gdouble d = 1 - 0.0167 * cos((2 * PI
      * (cabecera->getFecha().get_day_of_year() + 1 - 3)) / 365);

  gdouble titaSat = 0;
  gdouble titaSol = 90 - cabecera->getTitaSol();
  gdouble phiSat = 0;
  gdouble phiSol = cabecera->getPhiSol();
  g_print("titaSat: %f phiSat: %f titaSol: %f phiSol: %f \n", titaSat, phiSat,
      titaSol, phiSol);
  titaSat = titaSat * PI / 180;
  phiSat = phiSat * PI / 180;
  titaSol = titaSol * PI / 180;//zenit
  phiSol = phiSol * PI / 180; //azimut

  gdouble cosenoMas = cos(titaSat) * cos(titaSol) - sin(titaSat) * sin(titaSol)
      * cos(phiSat - phiSol);
  gdouble cosenoMenos = -cos(titaSat) * cos(titaSol) - sin(titaSat) * sin(
      titaSol) * cos(phiSat - phiSol);

  gdouble faseMas = 0.75 * (1 + cosenoMas * cosenoMas);
  gdouble faseMenos = 0.75 * (1 + cosenoMenos * cosenoMenos);
  g_print("Distancia tierra sol: %f \n", d);
  g_print("titaSat: %f phiSat: %f titaSol: %f phiSol: %f \n", titaSat, phiSat,
      titaSol, phiSol);
  g_print("Coseno Mas: %f Coseno Menos: %f  \n", cosenoMas, cosenoMenos);
  g_print("Fase Mas: %f Fase Menos: %f  \n", faseMas, faseMenos);

  maxB.clear();
  minB.clear();
  gfloat maxAj = MINFLOAT;
  gfloat minAj = MAXFLOAT;

  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    if (vectorBanda[i]->cargada)
      {
        gfloat maxBanda = 255.0 * cabecera->gainBias[i].gain
            + cabecera->gainBias[i].bias;
        maxBanda = (d * d * PI * (maxBanda)) / (cos(titaSol)
            * cabecera->solct[i]);
        gfloat minBanda = cabecera->gainBias[i].bias;
        minBanda = (d * d * PI * (minBanda)) / (cos(titaSol)
            * cabecera->solct[i]);
        g_print("Banda %i maximo: %f minimo: %f\n", i + 1, maxBanda, minBanda);
        maxB.push_back(maxBanda);
        minB.push_back(minBanda);
        if (maxBanda > maxAj)
          maxAj = maxBanda;

        if (minBanda < minAj)
          minAj = minBanda;
      }
    else
      {
        maxB.push_back(0);
        minB.push_back(0);

      }
  g_print("Todas las Bandas maximo: %f minimo: %f\n", maxAj, minAj);
  // calculo un gain y un bias para normalizar a 255
  gfloat bNorm = -((255.0 * minAj) / (maxAj - minAj));
  gfloat gNorm = 255.0 / (maxAj - minAj);

  //  g_print("max: %f min: %f \n", (max,min,gc,bc);
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    {
      if (vectorBanda[i]->cargada)
        {
          if (ajustarRangoDinamicoPorBanda)
            {
              // calculo un gain y un bias para normalizar a 255
              bNorm = -((255.0 * minB[i]) / (maxB[i] - minB[i]));
              gNorm = 255.0 / (maxB[i] - minB[i]);
            }

          gfloat bRef = minB[i];
          gfloat gRef = (maxB[i] - minB[i]) / 255.0;

          //calculo nuevos b y g para ajustar la luminancia y normalizar en un solo paso
          gfloat bAjustado = bRef * gNorm + bNorm;
          gfloat gAjustado = (gRef + bRef) * gNorm + bNorm - bAjustado;
          for (int ii = 0; ii < cabecera->alto; ii++)

            for (int iii = 0; iii < cabecera->ancho; iii++)

              vectorBanda[i]->matriz[ii][iii] = vectorBanda[i]->matriz[ii][iii]
                  * gAjustado + bAjustado;

        }
    }
}

void
Imagen::corregirRHOR(bool ajustarRangoDinamicoPorBanda)
{

  gdouble d = 1 - 0.0167 * cos((2 * PI
      * (cabecera->getFecha().get_day_of_year() + 1 - 3)) / 365);

  gdouble titaSat = 0;
  gdouble titaSol = 90 - cabecera->getTitaSol();
  gdouble phiSat = 0;
  gdouble phiSol = cabecera->getPhiSol();
  g_print("titaSat: %f phiSat: %f titaSol: %f phiSol: %f \n", titaSat, phiSat,
      titaSol, phiSol);
  titaSat = titaSat * PI / 180;
  phiSat = phiSat * PI / 180;
  titaSol = titaSol * PI / 180;//zenit
  phiSol = phiSol * PI / 180; //azimut

  gdouble cosenoMas = cos(titaSat) * cos(titaSol) - sin(titaSat) * sin(titaSol)
      * cos(phiSat - phiSol);
  gdouble cosenoMenos = -cos(titaSat) * cos(titaSol) - sin(titaSat) * sin(
      titaSol) * cos(phiSat - phiSol);

  gdouble faseMas = 0.75 * (1 + cosenoMas * cosenoMas);
  gdouble faseMenos = 0.75 * (1 + cosenoMenos * cosenoMenos);
  g_print("Distancia tierra sol: %f \n", d);
  g_print("titaSat: %f phiSat: %f titaSol: %f phiSol: %f \n", titaSat, phiSat,
      titaSol, phiSol);
  g_print("Coseno Mas: %f Coseno Menos: %f  \n", cosenoMas, cosenoMenos);
  g_print("Fase Mas: %f Fase Menos: %f  \n", faseMas, faseMenos);

  maxB.clear();
  minB.clear();

  gfloat maxAj = MINFLOAT;
  gfloat minAj = MAXFLOAT;

  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    if (vectorBanda[i]->cargada)
      {
        gfloat lr = ((cabecera->solct[i] * cabecera->taur[i]) / (4 * PI * d * d
            * cos(titaSat))) * (exp(-cabecera->taug[i] / cos(titaSat))) * (exp(
            -cabecera->taug[i] / cos(titaSol))) * (faseMenos + 0.052 * faseMas);
        g_print("LR Banda %i: %f\n", i + 1, lr);

        gfloat maxBanda = 255.0 * cabecera->gainBias[i].gain
            + cabecera->gainBias[i].bias;
        maxBanda = (d * d * PI * (maxBanda - lr)) / (cos(titaSol)
            * cabecera->solct[i]);
        gfloat minBanda = cabecera->gainBias[i].bias;
        minBanda = (d * d * PI * (minBanda - lr)) / (cos(titaSol)
            * cabecera->solct[i]);
        g_print("Banda %i maximo: %f minimo: %f\n", i + 1, maxBanda, minBanda);
        maxB.push_back(maxBanda);
        minB.push_back(minBanda);
        if (maxBanda > maxAj)
          maxAj = maxBanda;

        if (minBanda < minAj)
          minAj = minBanda;
      }
    else
      {
        maxB.push_back(0);
        minB.push_back(0);

      }
  g_print("Todas las Bandas maximo: %f minimo: %f\n", maxAj, minAj);
  // calculo un gain y un bias para normalizar a 255
  gfloat bNorm = -((255.0 * minAj) / (maxAj - minAj));
  gfloat gNorm = 255.0 / (maxAj - minAj);

  //  g_print("max: %f min: %f \n", (max,min,gc,bc);
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    {
      if (vectorBanda[i]->cargada)
        {
          if (ajustarRangoDinamicoPorBanda)
            {
              // calculo un gain y un bias para normalizar a 255
              bNorm = -((255.0 * minB[i]) / (maxB[i] - minB[i]));
              gNorm = 255.0 / (maxB[i] - minB[i]);
            }

          gfloat bRai = minB[i];
          gfloat gRai = (maxB[i] - minB[i]) / 255.0;

          //calculo nuevos b y g para ajustar la luminancia y normalizar en un solo paso
          gfloat bAjustado = bRai * gNorm + bNorm;
          gfloat gAjustado = (gRai + bRai) * gNorm + bNorm - bAjustado;
          for (int ii = 0; ii < cabecera->alto; ii++)

            for (int iii = 0; iii < cabecera->ancho; iii++)

              vectorBanda[i]->matriz[ii][iii] = vectorBanda[i]->matriz[ii][iii]
                  * gAjustado + bAjustado;

        }
    }
}

Glib::ustring
Imagen::getDirectorio()
{
  Glib::ustring::size_type inicio = nombreArchivo.length();

  inicio = nombreArchivo.rfind("/", inicio) + 1;

  if (inicio - 1 == nombreArchivo.length())
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
  for (unsigned int i = 0; i < vectorBanda.size(); i++)
    {
      if (vectorBanda[i]->cargada)
        {
          delete vectorBanda[i];
        }
    }

}
