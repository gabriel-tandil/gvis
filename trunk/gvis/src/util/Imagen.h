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

#ifndef IMAGEN_H_
#define IMAGEN_H_
#include "Banda.h"
#include "Cabecera.h"
#include <glibmm.h>
class Banda;
class Imagen
  {
public:
  typedef std::vector<Banda*> TVectorCapa;
  Imagen(Glib::ustring archivoCab);
  virtual ~Imagen();
  void cargarBandas();

  // imagen en radiancia
  void corregirL();
  // imagen en reflactancia
  void corregirRHO();
  // imagen en reflactancia Rayleigh
  void corregirRHOR();
  Glib::ustring getDirectorio();
  int obtenerNumeroBanda(Glib::ustring cual);
  TVectorCapa vectorBanda;
  Cabecera* cabecera;
  gint8 tamanioBloque;
  std::vector<gfloat> maxB;
  std::vector<gfloat> minB;
protected:
  Glib::ustring nombreArchivo;

  };
#endif /*IMAGEN_H_*/
