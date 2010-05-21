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

#ifndef PANTALLAFALSOCOLOR_H_
#define PANTALLAFALSOCOLOR_H_

#include <gtkmm.h>
#include "util/Imagen.h"
#include "Pintor.h"
#include "Controlador.h"


class PantallaFalsoColor: public Controlador
  {
public:

  PantallaFalsoColor();
  virtual ~PantallaFalsoColor();
  virtual void on_ventana_show();
  virtual void on_aceptar_falsoColor_clik();
  virtual void on_cancelar_falsoColor_clik();
  void armarCombos();
  void combo_add_item(const Banda* capa);
  void mostrar(Pintor*p, Imagen* imagen);
  Glib::ustring get_active_text(Gtk::ComboBox* combo) const;
  Gtk::Button* aceptar_falsoColor;
  Gtk::Button* cancelar_falsoColor;
  Gtk::ComboBox* falsoColorRojo;
  Gtk::ComboBox* falsoColorVerde;
  Gtk::ComboBox* falsoColorAzul;
  Imagen* imagen;
  Pintor* pintor;
  Glib::RefPtr<Gtk::ListStore> rojo_refListStore;
  Glib::RefPtr<Gtk::ListStore> verde_refListStore;
  Glib::RefPtr<Gtk::ListStore> azul_refListStore;
protected:

    //Tree model columns:
    //These columns are used by the model that is created by the default constructor


  struct TextModelColumns : public Gtk::TreeModel::ColumnRecord
        {
          Gtk::TreeModelColumn<Glib::ustring> nombre;
          TextModelColumns()
            {
              add(nombre);
            }
        };

    TextModelColumns m_text_columns;

  };

#endif /*PantallaFalsoColor_H_*/
