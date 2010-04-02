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
#include "PantallaCapas.h"
#include <stdio.h>

PantallaCapas::PantallaCapas():
Controlador("capas")
  {
    builder->get_widget("listaCapas", listaCapas);
    builder->get_widget("aceptar_seleccionCapas", aceptar_seleccionCapas);
    builder->get_widget("cancelar_seleccionCapas",
        cancelar_seleccionCapas);
    aceptar_seleccionCapas->signal_clicked().connect(sigc::mem_fun(*this,
            &PantallaCapas::on_aceptar_seleccionCapas_clik));
    cancelar_seleccionCapas->signal_clicked().connect(sigc::mem_fun(*this,
            &PantallaCapas::on_cancelar_seleccionCapas_clik));
    ventana->signal_show().connect(sigc::mem_fun(*this,
            &PantallaCapas::on_ventana_show));

  }

void PantallaCapas::mostrar(Imagen* img)
  {
    imagen = img;
    listaCapas->remove_all_columns();
    armarListaCapas(imagen);
    add_columns(); // no se porque si no agrego las columnas despues de armar la lista de capas no quedan editables
    //por eso las saco y las pongo
    ventana->show();
  }

void PantallaCapas::on_ventana_show()
  {

  }

void PantallaCapas::on_aceptar_seleccionCapas_clik()
  {
    Glib::RefPtr<Gtk::TreeModel> refModel = listaCapas->_get_base_model();
    for (unsigned int var = 0; var < imagen->vectorCapa.size(); ++var)
      {
        char cad[255];
        sprintf(cad, "%6d", var);
        Gtk::TreeModel::iterator iter = refModel->get_iter(Glib::ustring(cad));
        Gtk::TreeRow row = *iter;
        imagen->vectorCapa[var].cargada = row[m_columns.cargada];
      }

    imagen->cargarCapas();
    ventana->hide();
  }

void PantallaCapas::on_cancelar_seleccionCapas_clik()
  {
    ventana->hide();
  }

void PantallaCapas::armarListaCapas(Imagen* imagen)
  {

    m_refListStore = Gtk::ListStore::create(m_columns);

    std::for_each(imagen->vectorCapa.begin(), imagen->vectorCapa.end(),
        sigc::mem_fun(*this, &PantallaCapas::liststore_add_item));
    listaCapas->set_model(m_refListStore);

  }

void PantallaCapas::add_columns()
  {

    int cols_count = listaCapas->append_column_editable("¿Cargada?",
        m_columns.cargada);
    Gtk::TreeViewColumn* pColumn = listaCapas->get_column(cols_count - 1);

    pColumn->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
    pColumn->set_fixed_width(80);
    pColumn->set_clickable();

    listaCapas->append_column("Nombre", m_columns.nombre);

  }

void PantallaCapas::liststore_add_item(const Capa& foo)
  {
    Gtk::TreeRow row = *(m_refListStore->append());

    row[m_columns.cargada] = foo.cargada;
    row[m_columns.nombre] = foo.nombre;
  }

PantallaCapas::~PantallaCapas()
  {
  }
