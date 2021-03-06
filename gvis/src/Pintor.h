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
    int rojo, verde, azul;
  };
  TFalsoColor falsoColor;
  int minRojo;
  int minVerde;
  int minAzul;
  int maxRojo;
  int maxVerde;
  int maxAzul;
  int desplazamientoX, desplazamientoY;
  int nivelZoom;
  bool mostrarBordes;
  Pintor(Gtk::DrawingArea* dibu);
  bool
  on_dibujo_expose(GdkEventExpose* evento);
  virtual
  ~Pintor();
  void
  setImagen(Imagen* img);
  Gtk::DrawingArea*
  getDibujo();
  void
  reset();
  void
  setNivelZoom(int nivel);
  void
  setDesplazamientoX(int desp);
  void
  setDesplazamientoY(int desp);
  void
  setMostrarBordes(bool mostrar);
private:
  Gtk::DrawingArea* dibujo;
  Imagen* imagen;
  void
  dibujarBordes(int x, int y, int height, int width);
  void
  pintarTresBandas(int x, int y, int height, int width);
  void
  pintarMenosBandas(int x, int y, int height, int width);
  void
  pintarTresBandasRangoAcotado(int x, int y, int height, int width);
  void
  pintarMenosBandasRangoAcotado(int x, int y, int height, int width);

};

#endif /* PINTOR_H_ */
