//    This file is part of libvis.
//
//    Copyright (C) 2008, 2009, 2010
//       Gabriel Alvarez
//    
//    libvis is free software: you can redistribute it and/or modify
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

#ifndef CABECERA_L7_H_
#define CABECERA_L7_H_

#include "Cabecera.h"

class Cabecera_L7 : public Cabecera
  {
public:
  Cabecera_L7();
  virtual ~Cabecera_L7();
  virtual Glib::ustring obtieneNombreArchivoCapa(Glib::ustring);

  Glib::ustring tagNombreArchivo;
  };

#endif /*CABECERA_L5_H_*/
