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
#include "PantallaCargarImagen.h"
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
  virtual bool
  on_scrollVertical_change(Gtk::ScrollType st, double v);
  virtual void
  on_abrir_clik();
  virtual void
  on_acercar_clik();
  virtual void
  on_alejar_clik();
  virtual void
  on_verCabecera_clik();
  virtual void
  on_salir_clik();
  virtual void
  on_configFalsoColor_clik();
  virtual bool
  on_dibujo_Apreta(GdkEventButton* evento);
  virtual bool
  on_dibujo_Suelta(GdkEventButton* evento);
  virtual bool
  on_dibujo_Mueve(GdkEventMotion* evento);
  virtual bool
  on_key_press(GdkEventKey* evento);
  virtual void
  on_dibujo_cambia_tamanio(Gtk::Allocation rec);
  virtual void
  on_mostrarBordes_clik();
  Gtk::ImageMenuItem* abrir;
  Gtk::ImageMenuItem* verCabecera;
  Gtk::ImageMenuItem* configFalsoColor;
  Gtk::ImageMenuItem* salir;
  Gtk::ImageMenuItem* acercar;
  Gtk::ImageMenuItem* alejar;
  Gtk::CheckMenuItem* mostrarBordes;
  Gtk::Window* seleccionCapas;
  Gtk::VScrollbar* scrollVertical;
  Gtk::HScrollbar* scrollHorizontal;
  Gtk::Statusbar* statusBar;
  Gtk::DrawingArea* dibujo;
  Gtk::HBox* etiquetasBandas;
  Gtk::Window* ventanaCabecera;
  Gtk::Window* ventanaFirmaEspectral;
  PantallaCargarImagen* pantallaCargarImagen;
  PantallaFalsoColor* pantallaFalsoColor;
  int nivelZoom;
  Imagen* imagen;
  Pintor* pintorPrincipal;
private:
  virtual void
  actualizaFirmaEspectral(int ex, int ey);
  void
  ajustarMaximoDesplazamiento();
  void
  actualizarBarraEstado(int x = -1, int y = -1);
  void
  acercarZoom();
  void
  alejarZoom();
};

#endif /*VISOR_H_*/
