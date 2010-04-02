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

#include "Cabecera_L7.h"

Cabecera_L7::Cabecera_L7() :
  Cabecera()
  {
    satelite=L7;
    tagAncho="PIXELS PER LINE =";
    tagAlto="LINES PER BAND =";
    tagTamanioPixel="PIXEL SIZE = ";
    tagNombreArchivo="FILENAME =";
  }

 Glib::ustring Cabecera_L7::obtieneNombreArchivoCapa(Glib::ustring nombreCapa){
  if (nombreCapa=="L")
    return buscar(texto,tagNombreArchivo,0,".FST")+".FST";
  if (nombreCapa=="H")
    return buscar(texto,tagNombreArchivo,1,".FST")+".FST";

  int ocurrencia =atoi(nombreCapa.c_str())-1;
    return buscar(texto,tagNombreArchivo,ocurrencia,".FST")+".FST";
}

Cabecera_L7::~Cabecera_L7()
  {
  }
