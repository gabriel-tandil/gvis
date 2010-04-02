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

#include "PantallaFalsoColor.h"
#include <stdio.h>
#include <gtk/gtkcombobox.h>

PantallaFalsoColor::PantallaFalsoColor():
Controlador("falsoColor")
  {
    builder->get_widget("falsoColorRojo", falsoColorRojo);
    builder->get_widget("falsoColorVerde", falsoColorVerde);
    builder->get_widget("falsoColorAzul", falsoColorAzul);
    builder->get_widget("aceptar_falsoColor", aceptar_falsoColor);
    builder->get_widget("cancelar_falsoColor",
        cancelar_falsoColor);
    aceptar_falsoColor->signal_clicked().connect(sigc::mem_fun(*this,
            &PantallaFalsoColor::on_aceptar_falsoColor_clik));
    cancelar_falsoColor->signal_clicked().connect(sigc::mem_fun(*this,
            &PantallaFalsoColor::on_cancelar_falsoColor_clik));
    ventana->signal_show().connect(sigc::mem_fun(*this,
            &PantallaFalsoColor::on_ventana_show));

    rojo_refListStore= Gtk::ListStore::create(m_text_columns);
    verde_refListStore= Gtk::ListStore::create(m_text_columns);
    azul_refListStore= Gtk::ListStore::create(m_text_columns);

  }

void PantallaFalsoColor::mostrar(Pintor* p, Imagen*i)
  {
    pintor = p;
    imagen = i;
    armarCombos();

    ventana->show();
  }

void PantallaFalsoColor::on_ventana_show()
  {

  }

void PantallaFalsoColor::on_aceptar_falsoColor_clik()
  {
    pintor->falsoColor.rojo = imagen->obtenerNumeroCapa(get_active_text(
        falsoColorRojo));
    pintor->falsoColor.verde = imagen->obtenerNumeroCapa(get_active_text(
        falsoColorVerde));
    pintor->falsoColor.azul = imagen->obtenerNumeroCapa(get_active_text(
        falsoColorAzul));

    ventana->hide();
  }

void PantallaFalsoColor::on_cancelar_falsoColor_clik()
  {
    ventana->hide();
  }

void PantallaFalsoColor::armarCombos()
  {

    std::for_each(imagen->vectorCapa.begin(), imagen->vectorCapa.end(),
        sigc::mem_fun(*this, &PantallaFalsoColor::combo_add_item));

  }

void PantallaFalsoColor::combo_add_item(const Capa& capa)
  {
    if (capa.cargada)
      {
        //      append_text(falsoColorRojo,capa.nombre);
        //      append_text(falsoColorVerde,capa.nombre);
        //      append_text(falsoColorAzul,capa.nombre);
        Gtk::TreeRow row = *(rojo_refListStore->append());
        row[m_text_columns.nombre] = capa.nombre;
        row = *(verde_refListStore->append());
        row[m_text_columns.nombre] = capa.nombre;
        row = *(azul_refListStore->append());
        row[m_text_columns.nombre] = capa.nombre;

      }
  }

void PantallaFalsoColor::append_text(Gtk::ComboBox* combo,
    const Glib::ustring& text)
  {
    gtk_combo_box_append_text(combo->gobj(), text.c_str());
  }

Glib::ustring PantallaFalsoColor::get_active_text(Gtk::ComboBox* combo) const
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
