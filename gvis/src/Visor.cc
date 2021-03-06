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
  nivelZoom = 1;
  imagen = NULL;
  etiquetasBandas = NULL;
  builder->get_widget("mostrarBordes", mostrarBordes);
  builder->get_widget("scrollHorizontal", scrollHorizontal);
  builder->get_widget("scrollVertical", scrollVertical);
  builder->get_widget("dibujo", dibujo);
  builder->get_widget("abrir", abrir);
  builder->get_widget("acercar", acercar);
  builder->get_widget("alejar", alejar);
  builder->get_widget("verCabecera", verCabecera);
  builder->get_widget("salir", salir);
  builder->get_widget("configFalsoColor", configFalsoColor);
  builder->get_widget("ventanaCabecera", ventanaCabecera);
  builder->get_widget("ventanaFirmaEspectral", ventanaFirmaEspectral);
  builder->get_widget("statusBar", statusBar);

  Gdk::Color color; //hago el area de dibujo negra
  color.parse("black");
  dibujo->modify_bg(Gtk::STATE_NORMAL,color);

  pintorPrincipal = new Pintor(dibujo);
  mostrarBordes->signal_activate().connect(sigc::mem_fun(*this,
      &Visor::on_mostrarBordes_clik));
  scrollHorizontal->signal_change_value().connect(sigc::mem_fun(*this,
      &Visor::on_scrollHorizontal_change));
  scrollVertical->signal_change_value().connect(sigc::mem_fun(*this,
      &Visor::on_scrollVertical_change));
  ventana->signal_key_press_event().connect(sigc::mem_fun(*this,
      &Visor::on_key_press));
  abrir->signal_activate().connect(sigc::mem_fun(*this, &Visor::on_abrir_clik));

  acercar->signal_activate().connect(sigc::mem_fun(*this,
      &Visor::on_acercar_clik));
  alejar->signal_activate().connect(
      sigc::mem_fun(*this, &Visor::on_alejar_clik));

  configFalsoColor->signal_activate().connect(sigc::mem_fun(*this,
      &Visor::on_configFalsoColor_clik));
  verCabecera->signal_activate().connect(sigc::mem_fun(*this,
      &Visor::on_verCabecera_clik));
  salir->signal_activate().connect(sigc::mem_fun(*this, &Visor::on_salir_clik));
  dibujo->signal_button_press_event().connect(sigc::mem_fun(*this,
      &Visor::on_dibujo_Apreta));
  dibujo->signal_button_release_event().connect(sigc::mem_fun(*this,
      &Visor::on_dibujo_Suelta));
  dibujo->signal_motion_notify_event().connect(sigc::mem_fun(*this,
      &Visor::on_dibujo_Mueve));
  dibujo->signal_expose_event().connect(sigc::mem_fun(*pintorPrincipal,
      &Pintor::on_dibujo_expose));
  dibujo->signal_size_allocate().connect(sigc::mem_fun(*this,
      &Visor::on_dibujo_cambia_tamanio));
  pantallaCargarImagen = new PantallaCargarImagen();
  pantallaFalsoColor = new PantallaFalsoColor();
  scrollHorizontal->get_adjustment()->set_page_size(200);
  scrollHorizontal->get_adjustment()->set_step_increment(50);
  scrollHorizontal->get_adjustment()->set_lower(0);
  scrollVertical->get_adjustment()->set_page_size(200);
  scrollVertical->get_adjustment()->set_step_increment(50);
  scrollVertical->get_adjustment()->set_lower(0);

}

void
Visor::on_mostrarBordes_clik()
{
  pintorPrincipal->setMostrarBordes(mostrarBordes->get_active());
  dibujo->queue_draw();
}

bool
Visor::on_key_press(GdkEventKey* evento)
{
  if (*evento->string == '+')
    acercarZoom();
  if (*evento->string == '-')
    alejarZoom();
  return true;
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
      gfloat nivelZoomAjustado = nivelZoom == 0 ? 0.5 : nivelZoom;
      //scrollHorizontal->set_value()
      scrollHorizontal->get_adjustment()->set_upper(imagen->cabecera->ancho
          - dibujo->get_width() * nivelZoomAjustado);
      scrollVertical->get_adjustment()->set_upper(imagen->cabecera->alto
          - dibujo->get_height() * nivelZoomAjustado);
      if (scrollHorizontal->get_adjustment()->get_value()
          > scrollHorizontal->get_adjustment()->get_upper()
              + dibujo->get_width())
        {
          scrollHorizontal->get_adjustment()->set_value(
              scrollHorizontal->get_adjustment()->get_upper());
        }
      if (scrollVertical->get_adjustment()->get_value()
          > scrollVertical->get_adjustment()->get_upper()
              + dibujo->get_height())
        {
          scrollVertical->get_adjustment()->set_value(
              scrollVertical->get_adjustment()->get_upper());
        }
    }
}

bool
Visor::on_dibujo_Apreta(GdkEventButton* evento)
{
  if (evento->button == 1)
    {
      if (imagen != NULL)
        {
          Gtk::Table* tablaFirmaEspectral;

          builder->get_widget("tablaFirmaEspectral", tablaFirmaEspectral);
          if (etiquetasBandas != NULL)
            {
              tablaFirmaEspectral->remove(*etiquetasBandas);
              delete etiquetasBandas;
            }
          etiquetasBandas = new Gtk::HBox(true, 0);
          tablaFirmaEspectral->attach(*etiquetasBandas, 1, 2, 1, 2, Gtk::EXPAND
              | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0);

          for (unsigned int i = 0; i < imagen->vectorBanda.size(); i++)
            if (imagen->vectorBanda[i]->cargada)
              {
                Banda* c = imagen->vectorBanda[i];
                Gtk::Label* etiqueta = new Gtk::Label(c->nombre, false);
                etiquetasBandas->pack_start(*etiqueta, true, true, 0);

              }
          tablaFirmaEspectral->show_all_children(true);
        }
      actualizaFirmaEspectral(evento->x, evento->y);
      ventanaFirmaEspectral->show();
      int x, y;
      ventanaFirmaEspectral->get_position(x, y);
      ventanaFirmaEspectral->move(x + 200, y);
    }
  return true;
}

bool
Visor::on_dibujo_Suelta(GdkEventButton* evento)
{
  if (evento->button == 1)
    {
      ventanaFirmaEspectral->hide();
      actualizarBarraEstado();
    }
  return true;
}

void
Visor::actualizaFirmaEspectral(int ex, int ey)
{
  Gtk::Curve* firmaEspectral;
  builder->get_widget("firmaEspectral", firmaEspectral);
  std::vector<float> temp;
  if (imagen != NULL)
    {
      for (unsigned int i = 0; i < imagen->vectorBanda.size(); i++)
        if (imagen->vectorBanda[i]->cargada)

          {
            gfloat nivelZoomAjustado = nivelZoom == 0 ? 0.5 : nivelZoom;
            Banda* c = imagen->vectorBanda[i];
            int y = scrollVertical->get_adjustment()->get_value() + ey
                * nivelZoomAjustado;
            int x = scrollHorizontal->get_adjustment()->get_value() + ex
                * nivelZoomAjustado;
            actualizarBarraEstado(x, y);
            temp.push_back(c->matriz[y][x]);
          }
    }
  Glib::ArrayHandle<float> array1(temp);
  firmaEspectral->set_vector(array1);

}

bool
Visor::on_dibujo_Mueve(GdkEventMotion* evento)
{
  if (ventanaFirmaEspectral->is_visible())
    actualizaFirmaEspectral(evento->x, evento->y);
  return true;
}

void
Visor::on_salir_clik()
{
  ventana->hide();
}

void
Visor::actualizarBarraEstado(int x, int y)
{
  statusBar->pop(1);
  statusBar->pop(2);
  gchar* msg;
  if (nivelZoom == 1)
    msg = g_strdup_printf("Nivel de Zoom: x 1");
  else if (nivelZoom == 0)
    msg = g_strdup_printf("Nivel de Zoom: x 2");
  else
    msg = g_strdup_printf("Nivel de Zoom: x 1/%d", nivelZoom);

  statusBar->push(msg, 1);
  if (x != -1)
    {
      msg = g_strdup_printf("x: %d y: %d", x, y);
      for (unsigned int i = 0; i < imagen->vectorBanda.size(); i++)
        if (imagen->vectorBanda[i]->cargada)
          {
            Banda* c = imagen->vectorBanda[i];
            msg = g_strdup_printf("%s banda %d valor: %f valor digital: %d",
                msg, i + 1, ((c->matriz[y][x] / 255.0) * (imagen->maxB[i]
                    - imagen->minB[i])) + imagen->minB[i], c->matriz[y][x]);

          }
      statusBar->push(msg, 2);
    }
}

void
Visor::on_acercar_clik()
{
  acercarZoom();
}

void
Visor::acercarZoom()
{
  if (nivelZoom > 0)
    nivelZoom--;
  acercar->set_sensitive(nivelZoom != 0);
  ajustarMaximoDesplazamiento();
  pintorPrincipal->setNivelZoom(nivelZoom);
  actualizarBarraEstado();
  dibujo->queue_draw();
}

void
Visor::alejarZoom()
{
  nivelZoom++;
  acercar->set_sensitive(nivelZoom != 0);
  ajustarMaximoDesplazamiento();
  pintorPrincipal->setNivelZoom(nivelZoom);
  actualizarBarraEstado();
  dibujo->queue_draw();

}

void
Visor::on_alejar_clik()
{
  alejarZoom();
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
      if (imagen != NULL)
        {
          delete imagen;
          pintorPrincipal->reset();
          pantallaFalsoColor->reset();
        }
      imagen = new Imagen(dialog.get_filename());
      pantallaCargarImagen->mostrar(imagen);
      pintorPrincipal->setImagen(imagen);

      ajustarMaximoDesplazamiento();

      configFalsoColor->set_sensitive(true);
      verCabecera->set_sensitive(true);
      actualizarBarraEstado();
    }

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
