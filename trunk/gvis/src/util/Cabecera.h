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

#ifndef CABECERA_H_
#define CABECERA_H_
#include <glibmm.h>
#include <stdio.h>

class Cabecera
{
public:
  Cabecera();
  Cabecera(const Cabecera&);
  void
  cargarArchivo(Glib::ustring);
  virtual
  ~Cabecera();
  static Cabecera*
  crearCabecera(FILE* f);
  static Glib::ustring
  buscar(Glib::ustring texto, Glib::ustring tag);
  static Glib::ustring
  buscar(Glib::ustring texto, Glib::ustring tag, unsigned int ocurrencia,
      Glib::ustring separador);
  Glib::ustring
  getBandasPresentes();
  Glib::ustring
  getTexto();
  Glib::Date::Date
  getFecha();
  gfloat
  getTitaSol();
  gfloat
  getPhiSol();

  enum
  {// satelites
    L5, L7
  };
  enum
  {// gain/bias
    GAIN, BIAS
  };
  struct TGainBias
  {
    gfloat gain;
    gfloat bias;
  };
  virtual Glib::ustring
  obtieneNombreArchivoCapa(Glib::ustring);//TODO: llevar a una jerarquia de capas por satelite
  guint16 ancho;
  guint16 alto;
  std::vector<TGainBias> gainBias;
protected:
  Glib::ustring tagSatelite;
  Glib::ustring tagFecha;
  Glib::ustring tagAncho;
  Glib::ustring tagAlto;
  Glib::ustring tagTamanioPixel;
  Glib::ustring tagBandas;
  Glib::ustring tagGainsBiases;
  Glib::ustring tagTitaSol;
  Glib::ustring tagPhiSol;
  gfloat titaSol;
  gfloat phiSol;

  Glib::ustring texto;
  guint8 satelite;
  Glib::Date::Date fecha;
  gfloat tamanioPixel;
  Glib::ustring bandasPresentes;
};

#endif /*CABECERA_H_*/
