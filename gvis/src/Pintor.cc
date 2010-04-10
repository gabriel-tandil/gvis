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

#include "Pintor.h"

Pintor::Pintor(Imagen* img, Gtk::DrawingArea* dibu)
  {
    dibujo = dibu;
    imagen = img;
    falsoColor.rojo = 0;
    falsoColor.verde = 0;
    falsoColor.azul = 0;
  }

void Pintor::refrescar()
  {
    int capaRoja = falsoColor.rojo;
    int capaVerde = falsoColor.verde;
    int capaAzul = falsoColor.azul;
    for (int i = 0; i < 100; i++)
      for (int ii = 0; ii < 100; ii++)
        {
        Capa c=imagen->vectorCapa[capaRoja];
        Capa::RegistroBloque rb=(*c.datos)[ii];
          area[i][ii].red =  rb.bloque[i]
              >> 8;
          area[i][ii].green
              = (*imagen->vectorCapa[capaVerde].datos)[ii].bloque[i] >> 8;
          area[i][ii].blue
              = (*imagen->vectorCapa[capaAzul].datos)[ii].bloque[i] >> 8;
        }

    dibujo->queue_draw();
  }

Pintor::~Pintor()
  {
    // TODO Auto-generated destructor stub
  }
