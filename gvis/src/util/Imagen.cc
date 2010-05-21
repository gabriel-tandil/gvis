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
      Banda* c=new Banda(cabecera->getBandasPresentes().substr(i, 1),false, this);
        vectorBanda.push_back(c);
      }
  }

void Imagen::cargarBandas()
  {
    for (unsigned int i = 0; i < vectorBanda.size(); i++)
      {
        vectorBanda[i]->cargarCapa();
      }
  }

Glib::ustring Imagen::getDirectorio()
  {

    Glib::ustring::size_type inicio = nombreArchivo.length();

    // TODO cambiar / para version windows
    inicio = nombreArchivo.rfind("/", inicio) + 1;

    if (inicio == nombreArchivo.length())
      inicio = nombreArchivo.rfind("\\", inicio) + 1;
    Glib::ustring cadena = nombreArchivo.substr(0, inicio);

    return cadena;
  }
int Imagen::obtenerNumeroBanda(Glib::ustring cual){
  for (unsigned int var = 0; var < vectorBanda.size(); ++var) {
    if (vectorBanda[var]->nombre==cual)
      return var;
  }
  return -1;
}
Imagen::~Imagen()
  {
  }
