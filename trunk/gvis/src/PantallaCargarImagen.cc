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
#include "PantallaCargarImagen.h"
#include <stdio.h>

PantallaCargarImagen::PantallaCargarImagen() :
  Controlador("cargarImagen")
{
  builder->get_widget("listaBandas", listaBandas);
  builder->get_widget("aceptar", aceptar);
  builder->get_widget("cancelar", cancelar);
  builder->get_widget("ND", nd);
  builder->get_widget("L", l);
  builder->get_widget("RHO", rho);
  builder->get_widget("RHOR", rhor);
  builder->get_widget("ajustarPorBanda", ajustarPorBanda);
  builder->get_widget("ajustarPorImagen", ajustarPorImagen);
  aceptar->signal_clicked().connect(sigc::mem_fun(*this,
      &PantallaCargarImagen::on_aceptar_clik));
  cancelar->signal_clicked().connect(sigc::mem_fun(*this,
      &PantallaCargarImagen::on_cancelar_clik));
  ventana->signal_show().connect(sigc::mem_fun(*this,
      &PantallaCargarImagen::on_ventana_show));

}

void
PantallaCargarImagen::mostrar(Imagen* img)
{
  imagen = img;
  listaBandas->remove_all_columns();
  armarListaBandas(imagen);
  add_columns(); // no se porque si no agrego las columnas despues de armar la lista de capas no quedan editables
  //por eso las saco y las pongo
  ventana->show();
}

void
PantallaCargarImagen::on_ventana_show()
{

}

void
PantallaCargarImagen::on_aceptar_clik()
{
  Glib::RefPtr<Gtk::TreeModel> refModel = listaBandas->_get_base_model();
  for (unsigned int var = 0; var < imagen->vectorBanda.size(); ++var)
    {
      char cad[255];
      sprintf(cad, "%6d", var);
      Gtk::TreeModel::iterator iter = refModel->get_iter(Glib::ustring(cad));
      Gtk::TreeRow row = *iter;
      imagen->vectorBanda[var]->cargada = row[m_columns.cargada];
    }

  imagen->cargarBandas();
  if (l->get_active())
    imagen->corregirL(ajustarPorBanda->get_active());
  if (rho->get_active())
    imagen->corregirRHO(ajustarPorBanda->get_active());
  if (rhor->get_active())
    imagen->corregirRHOR(ajustarPorBanda->get_active());

  ventana->hide();
}

void
PantallaCargarImagen::on_cancelar_clik()
{
  ventana->hide();
}

void
PantallaCargarImagen::armarListaBandas(Imagen* imagen)
{

  m_refListStore = Gtk::ListStore::create(m_columns);

  for (unsigned int i = 0; i < imagen->vectorBanda.size(); ++i)
    {
      liststore_add_item(imagen->vectorBanda[i]);
    }
  listaBandas->set_model(m_refListStore);

}

void
PantallaCargarImagen::add_columns()
{

  int cols_count = listaBandas->append_column_editable("¿Cargada?",
      m_columns.cargada);
  Gtk::TreeViewColumn* pColumn = listaBandas->get_column(cols_count - 1);

  pColumn->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
  pColumn->set_fixed_width(80);
  pColumn->set_clickable();

  listaBandas->append_column("Nombre", m_columns.nombre);

}

void
PantallaCargarImagen::liststore_add_item(const Banda* foo)
{
  Gtk::TreeRow row = *(m_refListStore->append());

  row[m_columns.cargada] = foo->cargada;
  row[m_columns.nombre] = foo->nombre;
}

PantallaCargarImagen::~PantallaCargarImagen()
{
}
