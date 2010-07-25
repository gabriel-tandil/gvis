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

#ifndef CAPA_H_
#define CAPA_H_
#include <glibmm.h>
#include "Imagen.h"

class Imagen; //forward
class Banda
{
public:

  Banda(Glib::ustring, bool, Imagen*);
  //  Banda(const Banda &capa);
  virtual
  ~Banda();
  void
  cargarBanda();
  Glib::ustring nombre;
  gint16 anchoVista;
  gint16 altoVista;
  bool cargada;
  Imagen* imagen;
  typedef gint8 TBloque;
  struct RegistroBloque
  {
    TBloque* bloque;
    Glib::TimeVal momentoUso;
  };
  RegistroBloque* datos;
  guint8** matriz;
private:

};

#endif /*CAPA_H_*/
