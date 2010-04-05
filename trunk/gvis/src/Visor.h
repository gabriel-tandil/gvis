/*
 * This file is part of gvis.
 *
 * Copyright (C) 2008, 2009, 2010
 * Gabriel Alvarez
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

#ifndef VISOR_H_
#define VISOR_H_

#include <gtkmm.h>
#include "util/Imagen.h"
#include "PantallaCapas.h"
#include "PantallaFalsoColor.h"
#include "Controlador.h"

class Visor : public Controlador
{
public:

  Visor();
  virtual
  ~Visor();

  virtual bool
  on_scrollHorizontal_change(Gtk::ScrollType st, double v);
  virtual void
  on_ventana_show();
  virtual void
  on_abrir_clik();
  virtual void
  on_verCabecera_clik();
  virtual void
  on_configFalsoColor_clik();
  virtual bool
  on_dibujo_rClik(GdkEventButton* evento);
  virtual bool
  on_dibujo_expose(GdkEventExpose* evento);
  Gtk::Menu* menuEmergenteDibujo;
  Gtk::ImageMenuItem* abrir;
  Gtk::ImageMenuItem* verCabecera;
  Gtk::ImageMenuItem* configFalsoColor;
  Gtk::Window* seleccionCapas;
  Gtk::VScrollbar* scrollVertical;
  Gtk::HScrollbar* scrollHorizontal;
  Gtk::DrawingArea* dibujo;
  Gtk::Window* ventanaCabecera;
  PantallaCapas* pantallaCapas;
  PantallaFalsoColor* pantallaFalsoColor;

  Imagen* imagen;
  Pintor* pintorPrincipal;

};

#endif /*VISOR_H_*/
