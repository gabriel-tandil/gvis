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

#ifndef PANTALLACAPAS_H_
#define PANTALLACAPAS_H_

#include <gtkmm.h>
#include "util/Imagen.h"
#include "Controlador.h"


class PantallaCapas : public Controlador
  {
public:

  PantallaCapas();
  virtual ~PantallaCapas();


  virtual void on_ventana_show();
  virtual void on_aceptar_seleccionCapas_clik();
  virtual void on_cancelar_seleccionCapas_clik();
  void armarListaCapas(Imagen* imagen);
  void mostrar(Imagen* imagen);
  Gtk::Button* aceptar_seleccionCapas;
  Gtk::Button* cancelar_seleccionCapas;
  Gtk::TreeView* listaCapas;
  Imagen* imagen;
  Glib::RefPtr<Gtk::ListStore> m_refListStore;
  void add_columns();
  void liststore_add_item(const Capa& foo);

  struct ModelColumns : public Gtk::TreeModelColumnRecord
    {
    Gtk::TreeModelColumn<bool> cargada;
    Gtk::TreeModelColumn<Glib::ustring> nombre;
    ModelColumns()
      {
        add(cargada);
        add(nombre);
      }
    };

  ModelColumns m_columns;
  };

#endif /*PANTALLACAPAS_H_*/
