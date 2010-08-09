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

#include "Cabecera_L5.h"

Cabecera_L5::Cabecera_L5() :
  Cabecera()
{
  solct.push_back(1957);
  solct.push_back(1829);
  solct.push_back(1557);
  solct.push_back(1047);
  solct.push_back(219.3);

  taur.push_back(0.1543);
  taur.push_back(0.0812);
  taur.push_back(0.0451);
  taur.push_back(0.0174);
  taur.push_back(0.0011);

  taug.push_back(0.01);
  taug.push_back(0.03);
  taug.push_back(0.02);
  taug.push_back(0.02);
  taug.push_back(0);

  satelite = L5;
  tagAncho = "PIXELS PER LINE= ";
  tagAlto = "LINES PER IMAGE= ";
  tagTamanioPixel = "PIXEL SIZE =";
  tagGainsBiases = "GAINS/BIASES =  ";
  tagTitaSol = "SUN ELEVATION =";
  tagPhiSol = "SUN AZIMUTH = ";
}

Glib::ustring
Cabecera_L5::obtieneNombreArchivoCapa(Glib::ustring nombreBanda)
{
  return "BAND" + nombreBanda + ".dat";
}

Cabecera_L5::~Cabecera_L5()
{

}
