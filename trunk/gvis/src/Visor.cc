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

#include "Visor.h"

Visor::Visor() :
  Controlador("visor")
{
  imagen = NULL;
  builder->get_widget("scrollHorizontal", scrollHorizontal);
  builder->get_widget("scrollVertical", scrollVertical);
  builder->get_widget("dibujo", dibujo);
  builder->get_widget("abrir", abrir);
  builder->get_widget("verCabecera", verCabecera);
  builder->get_widget("salir", salir);
  builder->get_widget("configFalsoColor", configFalsoColor);
  builder->get_widget("ventanaCabecera", ventanaCabecera);
  //   builder->get_widget("menuEmergenteDibujo",menuEmergenteDibujo);
  pintorPrincipal = new Pintor(dibujo);

  scrollHorizontal->signal_change_value().connect(sigc::mem_fun(*this,
      &Visor::on_scrollHorizontal_change));
  scrollVertical->signal_change_value().connect(sigc::mem_fun(*this,
      &Visor::on_scrollVertical_change));
  ventana->signal_show().connect(sigc::mem_fun(*this, &Visor::on_ventana_show));
  abrir->signal_activate().connect(sigc::mem_fun(*this, &Visor::on_abrir_clik));
  configFalsoColor->signal_activate().connect(sigc::mem_fun(*this,
      &Visor::on_configFalsoColor_clik));
  verCabecera->signal_activate().connect(sigc::mem_fun(*this,
      &Visor::on_verCabecera_clik));
  salir->signal_activate().connect(sigc::mem_fun(*this, &Visor::on_salir_clik));
  dibujo->signal_button_press_event().connect(sigc::mem_fun(*this,
      &Visor::on_dibujo_rClik));
  dibujo->signal_expose_event().connect(sigc::mem_fun(*pintorPrincipal,
      &Pintor::on_dibujo_expose));
  dibujo->signal_size_allocate().connect(sigc::mem_fun(*this,
      &Visor::on_dibujo_cambia_tamanio));
  pantallaCapas = new PantallaCapas();
  pantallaFalsoColor = new PantallaFalsoColor();
  scrollHorizontal->get_adjustment()->set_page_size(200);
  scrollHorizontal->get_adjustment()->set_step_increment(50);
  scrollHorizontal->get_adjustment()->set_lower(0);
  scrollVertical->get_adjustment()->set_page_size(200);
  scrollVertical->get_adjustment()->set_step_increment(50);
  scrollVertical->get_adjustment()->set_lower(0);
}

void
Visor::on_ventana_show()
{

}

bool
Visor::on_scrollHorizontal_change(Gtk::ScrollType st, double v)
{
  pintorPrincipal->setDesplazamientoX(
      scrollHorizontal->get_adjustment()->get_value());
  g_print("%f \n", scrollHorizontal->get_adjustment()->get_value());
  dibujo->queue_draw();
  return true;
}

bool
Visor::on_scrollVertical_change(Gtk::ScrollType st, double v)
{
  g_print("%f \n", scrollVertical->get_adjustment()->get_value());

  pintorPrincipal->setDesplazamientoY(
      scrollVertical->get_adjustment()->get_value());

  dibujo->queue_draw();
  return true;
}

void
Visor::on_dibujo_cambia_tamanio(Gtk::Allocation rec)
{
  ajustarMaximoDesplazamiento();

}
void
Visor::ajustarMaximoDesplazamiento()
{
  if (imagen)
    {
      scrollHorizontal->get_adjustment()->set_upper(imagen->cabecera->ancho
          - dibujo->get_width());
      scrollVertical->get_adjustment()->set_upper(imagen->cabecera->alto
          - dibujo->get_height());
    }
}

bool
Visor::on_dibujo_rClik(GdkEventButton* evento)
{
  if (evento->button == 1)
    {
      menuEmergenteDibujo->show();
    }

  return true;
}

void
Visor::on_salir_clik()
{

  ventana->hide();
}

void
Visor::on_abrir_clik()
{

  Gtk::FileChooserDialog dialog("Seleccione archivo de Cabecera...",
      Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*ventana);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  const int response = dialog.run();
  if (response == Gtk::RESPONSE_OK)
    {
      imagen = new Imagen(dialog.get_filename());
    }
  pantallaCapas->mostrar(imagen);
  pintorPrincipal->setImagen(imagen);

  ajustarMaximoDesplazamiento();

  configFalsoColor->set_sensitive(true);
  verCabecera->set_sensitive(true);

  dibujo->queue_draw();

}

void
Visor::on_configFalsoColor_clik()
{

  pantallaFalsoColor->mostrar(pintorPrincipal, imagen);

}

void
Visor::on_verCabecera_clik()
{
  Gtk::TextView* textoCabecera;
  Gtk::Entry* bandas;
  Gtk::Entry* fecha;
  builder->get_widget("bandas", bandas);
  builder->get_widget("fecha", fecha);
  builder->get_widget("textoCabecera", textoCabecera);

  Glib::RefPtr<Gtk::TextBuffer> buffer = textoCabecera->get_buffer();

  buffer->set_text(imagen->cabecera->getTexto().substr(0,
      imagen->cabecera->getTexto().length() - 1));
  bandas->set_text(imagen->cabecera->getBandasPresentes());
  fecha->set_text(imagen->cabecera->getFecha().format_string("%x"));
  ventanaCabecera->show();
}

Visor::~Visor()
{
}
