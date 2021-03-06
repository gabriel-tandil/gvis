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

#include "Cabecera_L7.h"

Cabecera_L7::Cabecera_L7() :
  Cabecera()
{
  solct.push_back(1970);
  solct.push_back(1843);
  solct.push_back(1555);
  solct.push_back(1047);
  solct.push_back(227.1);

  taur.push_back(0.17);
  taur.push_back(0.09);
  taur.push_back(0.05);
  taur.push_back(0.02);
  taur.push_back(0);

  taug.push_back(0.01);
  taug.push_back(0.03);
  taug.push_back(0.02);
  taug.push_back(0.02);
  taug.push_back(0);

  satelite = L7;
  tagAncho = "PIXELS PER LINE =";
  tagAlto = "LINES PER BAND =";
  tagTamanioPixel = "PIXEL SIZE = ";
  tagNombreArchivo = "FILENAME =";
}

Glib::ustring
Cabecera_L7::obtieneNombreArchivoCapa(Glib::ustring nombreCapa)
{
  if (nombreCapa == "L")
    return buscar(texto, tagNombreArchivo, 0, ".FST") + ".FST";
  if (nombreCapa == "H")
    return buscar(texto, tagNombreArchivo, 1, ".FST") + ".FST";

  int ocurrencia = atoi(nombreCapa.c_str()) - 1;
  return buscar(texto, tagNombreArchivo, ocurrencia, ".FST") + ".FST";
}

Cabecera_L7::~Cabecera_L7()
{
}
