//    This file is part of libvis.
//
//    Copyright (C) 2008, 2009, 2010
//       Gabriel Alvarez
//    
//    libvis is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Foobar is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CAPA_H_
#define CAPA_H_
#include <glibmm.h>
#include "Imagen.h"

class Imagen; //forward
class Capa
  {
public:

  Capa(Glib::ustring, bool, Imagen*);
  virtual ~Capa();
  void cargarCapa();
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
  std::vector<RegistroBloque>* datos;
private:

  };

#endif /*CAPA_H_*/
