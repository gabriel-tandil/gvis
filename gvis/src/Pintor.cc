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

Pintor::Pintor(Gtk::DrawingArea* dibu)
{
  dibujo = dibu;
  falsoColor.rojo = -1;
  falsoColor.verde = -1;
  falsoColor.azul = -1;
  desplazamientoX = 0;
  desplazamientoY = 0;
  nivelZoom=1;
}

void
Pintor::setNivelZoom(int nivel){
  nivelZoom=nivel;
}

void
Pintor::setImagen(Imagen* img)
{
  imagen = img;
}

bool
Pintor::on_dibujo_expose(GdkEventExpose* evento)
{
  int capaRoja = falsoColor.rojo;
  int capaVerde = falsoColor.verde;
  int capaAzul = falsoColor.azul;
  Glib::RefPtr<Gdk::GC> refGC1 = Gdk::GC::create(dibujo->get_window());
  Gdk::Color color;

  refGC1->set_rgb_fg_color(color);
  g_print("Redibujando %i,%i->%i,%i\n", evento->area.x, evento->area.y,
      evento->area.x + evento->area.width, evento->area.y + evento->area.height);
  int id = desplazamientoY + evento->area.y;
  for (int i = evento->area.y; i < evento->area.height; i++)
    {
      int iid = desplazamientoX + evento->area.x;
      for (int ii = evento->area.x; ii < evento->area.width; ii++)
        {

          //  g_print("Redibujando %i,%i\n", i, ii);
          if (capaRoja >= 0)
            {
              Banda* c = imagen->vectorBanda[capaRoja];
              //            Banda::RegistroBloque* rb = &c->datos[i];
              //            Banda::TBloque b=rb->bloque[ii];
              //           color.set_red(b >> 8);
              //   gushort s=c->matriz[i][ii];
              color.set_red(c->matriz[id][iid] * 255);
            }
          if (capaVerde >= 0)
            {
              Banda* c = imagen->vectorBanda[capaVerde];
              //            Banda::RegistroBloque* rb = &c->datos[i];
              //            Banda::TBloque b=rb->bloque[ii];
              //           color.set_red(b >> 8);
              color.set_green(c->matriz[id][iid] * 255);
            }
          if (capaAzul >= 0)
            {
              Banda* c = imagen->vectorBanda[capaAzul];
              //            Banda::RegistroBloque* rb = &c->datos[i];
              //            Banda::TBloque b=rb->bloque[ii];
              //           color.set_red(b >> 8);
              color.set_blue(c->matriz[id][iid] * 255);
            }

          refGC1->set_rgb_fg_color(color);
          dibujo->get_window()->draw_point(refGC1, ii, i);
          iid+=nivelZoom;
        }
      id+=nivelZoom;
    }
  return false;
}

void
Pintor::setDesplazamientoX(int desp)
{
  desplazamientoX = desp;
}
void
Pintor::setDesplazamientoY(int desp)
{
  desplazamientoY = desp;
}

Pintor::~Pintor()
{
  // TODO Auto-generated destructor stub
}
