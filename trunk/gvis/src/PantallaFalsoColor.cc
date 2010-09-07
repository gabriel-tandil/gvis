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

#include "PantallaFalsoColor.h"
#include <stdio.h>
#include <gtk/gtkcombobox.h>

PantallaFalsoColor::PantallaFalsoColor() :
  Controlador("falsoColor")
{
  imagen = NULL;
  builder->get_widget("falsoColorRojo", falsoColorRojo);
  builder->get_widget("falsoColorVerde", falsoColorVerde);
  builder->get_widget("falsoColorAzul", falsoColorAzul);
  builder->get_widget("aceptar_falsoColor", aceptar_falsoColor);
  builder->get_widget("cancelar_falsoColor", cancelar_falsoColor);
  builder->get_widget("minRojo", minRojo);
  builder->get_widget("minVerde", minVerde);
  builder->get_widget("minAzul", minAzul);
  builder->get_widget("maxRojo", maxRojo);
  builder->get_widget("maxVerde", maxVerde);
  builder->get_widget("maxAzul", maxAzul);
  maxRojo->set_range(0, 255);
  maxRojo->set_increments(1, 5);
  maxRojo->set_value(255);
  maxVerde->set_range(0, 255);
  maxVerde->set_increments(1, 5);
  maxVerde->set_value(255);
  maxAzul->set_range(0, 255);
  maxAzul->set_increments(1, 5);
  maxAzul->set_value(255);
  minRojo->set_range(0, 255);
  minRojo->set_increments(1, 5);
  minVerde->set_range(0, 255);
  minVerde->set_increments(1, 5);
  minAzul->set_range(0, 255);
  minAzul->set_increments(1, 5);
  aceptar_falsoColor->signal_clicked().connect(sigc::mem_fun(*this,
      &PantallaFalsoColor::on_aceptar_falsoColor_clik));
  cancelar_falsoColor->signal_clicked().connect(sigc::mem_fun(*this,
      &PantallaFalsoColor::on_cancelar_falsoColor_clik));
  ventana->signal_show().connect(sigc::mem_fun(*this,
      &PantallaFalsoColor::on_ventana_show));

  rojo_refListStore = Gtk::ListStore::create(m_text_columns);
  verde_refListStore = Gtk::ListStore::create(m_text_columns);
  azul_refListStore = Gtk::ListStore::create(m_text_columns);
  falsoColorRojo->set_model(rojo_refListStore);
  falsoColorVerde->set_model(verde_refListStore);
  falsoColorAzul->set_model(azul_refListStore);
  falsoColorRojo->pack_start(m_text_columns.nombre);
  falsoColorVerde->pack_start(m_text_columns.nombre);
  falsoColorAzul->pack_start(m_text_columns.nombre);
}

void
PantallaFalsoColor::mostrar(Pintor* p, Imagen*i)
{
  pintor = p;
  if (imagen == NULL){
    imagen = i;
    armarCombos();
  }
  ventana->show();
}

void
PantallaFalsoColor::reset()
{
  imagen = NULL;
  pintor = NULL;
}

void
PantallaFalsoColor::on_ventana_show()
{

}

void
PantallaFalsoColor::on_aceptar_falsoColor_clik()
{
  pintor->falsoColor.rojo = imagen->obtenerNumeroBanda(get_active_text(
      falsoColorRojo));
  pintor->falsoColor.verde = imagen->obtenerNumeroBanda(get_active_text(
      falsoColorVerde));
  pintor->falsoColor.azul = imagen->obtenerNumeroBanda(get_active_text(
      falsoColorAzul));
  pintor->minRojo = minRojo->get_value_as_int();
  pintor->maxRojo = maxRojo->get_value_as_int();
  pintor->minVerde = minVerde->get_value_as_int();
  pintor->maxVerde = maxVerde->get_value_as_int();
  pintor->minAzul = minAzul->get_value_as_int();
  pintor->maxAzul = maxAzul->get_value_as_int();

  pintor->getDibujo()->queue_draw();
  ventana->hide();
}

void
PantallaFalsoColor::on_cancelar_falsoColor_clik()
{
  ventana->hide();
}

void
PantallaFalsoColor::armarCombos()
{
  rojo_refListStore->clear();
  verde_refListStore->clear();
  azul_refListStore->clear();
  for (unsigned int i = 0; i < imagen->vectorBanda.size(); ++i)
    {
      combo_add_item(imagen->vectorBanda[i]);
    }

}

void
PantallaFalsoColor::combo_add_item(const Banda* capa)
{
  if (capa->cargada)
    {
      Gtk::TreeRow row = *(rojo_refListStore->append());
      row[m_text_columns.nombre] = capa->nombre;
      row = *(verde_refListStore->append());
      row[m_text_columns.nombre] = capa->nombre;
      row = *(azul_refListStore->append());
      row[m_text_columns.nombre] = capa->nombre;

    }
}

Glib::ustring
PantallaFalsoColor::get_active_text(Gtk::ComboBox* combo) const
{
  Glib::ustring result;

  //Get the active row:
  Gtk::TreeModel::iterator active_row = combo->get_active();
  if (active_row)
    {
      Gtk::TreeModel::Row row = *active_row;
      result = row[m_text_columns.nombre];
    }

  return result;
}

PantallaFalsoColor::~PantallaFalsoColor()
{
}
