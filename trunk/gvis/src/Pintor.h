//    This file is part of gvis.
//
//    Copyright (C) 2008, 2009, 2010
//       Gabriel Alvarez
//    
//    gvis is free software: you can redistribute it and/or modify
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

#ifndef PINTOR_H_
#define PINTOR_H_

#include <gdkmm.h>
#include <gtkmm.h>
#include "util/Imagen.h"

class Pintor
  {
public:
  struct TFalsoColor
  {
    int rojo,verde,azul;
  };
  GdkColor area[600][600];
  TFalsoColor falsoColor;
  Pintor(Imagen* img,Gtk::DrawingArea* dibu);
  void refrescar();
  virtual ~Pintor();
private:
  Gtk::DrawingArea* dibujo;
  Imagen* imagen;
  };

#endif /* PINTOR_H_ */
