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
#include <stdlib.h>

#include "Cabecera.h"
#include "Cabecera_L5.h"
#include "Cabecera_L7.h"

Cabecera::Cabecera()
{
  tagSatelite = "SATELLITE =";
  tagFecha = "ACQUISITION DATE =";
  tagBandas = "BANDS PRESENT =";
  tagAncho = "";
  tagAlto = "";
  tagTamanioPixel = "";
}
Cabecera::Cabecera(const Cabecera&)
{
}

void
Cabecera::cargarArchivo(Glib::ustring tex)
{
  texto = tex;

  Glib::ustring cadena = buscar(texto, tagFecha);
  fecha = *new Glib::Date::Date((Glib::Date::Day) atoi(
      cadena.substr(6, 2).c_str()), (Glib::Date::Month) atoi(
      cadena.substr(4, 2).c_str()), (Glib::Date::Year) atoi(
      cadena.substr(0, 4).c_str()));

  cadena = buscar(texto, tagAncho);
  ancho = atoi(cadena.c_str());

  cadena = buscar(texto, tagAlto);
  alto = atoi(cadena.c_str());

  cadena = buscar(texto, tagTamanioPixel);
  tamanioPixel = strtod(cadena.c_str(), NULL);

  cadena = buscar(texto, tagTitaSol);
  titaSol = strtod(cadena.c_str(), NULL);

  cadena = buscar(texto, tagPhiSol);
  phiSol = strtod(cadena.c_str(), NULL);

  bandasPresentes = buscar(texto, tagBandas);

  cadena = buscar(texto, tagGainsBiases, 0, " VOLUME");

  std::vector<Glib::ustring> vLines = Glib::Regex::split_simple("  ", cadena);
  for (unsigned int i = 0; i < vLines.size(); i++)
    {
      int cSeparador = vLines.at(i).find_first_of('/');
      TGainBias gb;
      gb.gain = strtod((vLines.at(i).substr(0, cSeparador)).c_str(), NULL);
      cSeparador++;
      gb.bias = strtod((vLines.at(i).substr(cSeparador, vLines.at(i).length()
          - cSeparador)).c_str(), NULL);
      gainBias.push_back(gb);
    }

}

Cabecera*
Cabecera::crearCabecera(FILE* f)
{
  Cabecera* cabe = NULL;
  Glib::ustring tagSatelite = "SATELLITE =";
  Glib::ustring texto = *new Glib::ustring();
  gchar c;
  do
    {
      c = getc(f);
      texto += c;
    }
  while (c != EOF);
  Glib::ustring cadena = buscar(texto, tagSatelite);
  if (cadena.compare("L5") == 0)
    {
      cabe = new Cabecera_L5();

    }
  else if (cadena.compare("LANDSAT7") == 0)
    {
      cabe = new Cabecera_L7();
    }
  cabe->cargarArchivo(texto);
  return cabe;

}

Glib::ustring
Cabecera::obtieneNombreArchivoCapa(Glib::ustring nombreCapa)
{
  return nombreCapa;
}

Glib::ustring
Cabecera::buscar(Glib::ustring texto, Glib::ustring tag,
    unsigned int ocurrencia, Glib::ustring separador)
{
  Glib::ustring::size_type inicio = 0;
  for (unsigned int i = 0; i <= ocurrencia; ++i)
    {
      inicio = texto.find(tag, inicio) + tag.length();
    }
  if (inicio == 0)
    return NULL;
  Glib::ustring cadena = texto.substr(inicio, texto.find(separador, inicio)
      - inicio);

  return cadena;
}

Glib::ustring
Cabecera::buscar(Glib::ustring texto, Glib::ustring tag)
{
  return buscar(texto, tag, 0, " ");
}
Glib::ustring
Cabecera::getBandasPresentes()
{
  return bandasPresentes;
}

Glib::ustring
Cabecera::getTexto()
{
  return texto;
}

Glib::Date::Date
Cabecera::getFecha()
{
  return fecha;
}

gfloat
Cabecera::getPhiSol()
{
  return phiSol;
}
gfloat
Cabecera::getTitaSol()
{
  return titaSol;
}

Cabecera::~Cabecera()
{
  delete &fecha;
  delete &texto;
}
