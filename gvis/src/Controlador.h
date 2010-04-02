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

#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <gtkmm.h>

#define CARPETA_UI "src/"
#define EXTENSION_UI ".ui"

class Controlador
  {
public:
  Controlador(Glib::ustring nombre);
  virtual ~Controlador();
  Gtk::Window& get_window() const
    {
      return *ventana;
    }
protected:
  Glib::RefPtr<Gtk::Builder> builder;
  Gtk::Window* ventana;

  };

#endif /*CONTROLADOR_H_*/
