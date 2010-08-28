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
  nivelZoom = 1;
  minRojo = 0;
  minVerde = 0;
  minAzul = 0;
  maxRojo = 255;
  maxVerde = 255;
  maxAzul = 255;
  mostrarBordes = false;
}

void
Pintor::setNivelZoom(int nivel)
{
  nivelZoom = nivel;
}

void
Pintor::setImagen(Imagen* img)
{
  imagen = img;
}

Gtk::DrawingArea*
Pintor::getDibujo()
{
  return dibujo;
}

bool
Pintor::on_dibujo_expose(GdkEventExpose* evento)
{

  if (imagen != NULL)
    {
      if (falsoColor.rojo == -1 && falsoColor.verde == -1 && falsoColor.azul
          == -1)
        {
        }
      else
      if (falsoColor.rojo >= 0 && falsoColor.verde >= 0 && falsoColor.azul >= 0)
        {
          if (minAzul == 0 && maxAzul == 255 && minRojo == 0 && maxRojo == 255
              && minVerde == 0 && maxVerde == 255)
            {

              pintarTresBandas(evento->area.x, evento->area.y,
                  evento->area.height, evento->area.width);
            }
          else
            {
              pintarTresBandasRangoAcotado(evento->area.x, evento->area.y,
                  evento->area.height, evento->area.width);
            }
        }
      else
        {
          if (minAzul == 0 && maxAzul == 255 && minRojo == 0 && maxRojo == 255
              && minVerde == 0 && maxVerde == 255)
            {
              pintarMenosBandas(evento->area.x, evento->area.y,
                  evento->area.height, evento->area.width);
            }
          else
            {
              pintarMenosBandasRangoAcotado(evento->area.x, evento->area.y,
                  evento->area.height, evento->area.width);
            }
        }

      g_print("Redibujando %i,%i->%i,%i\n", evento->area.x, evento->area.y,
          evento->area.x + evento->area.width, evento->area.y
              + evento->area.height);
      if (mostrarBordes)
        dibujarBordes(evento->area.x, evento->area.y, evento->area.height,
            evento->area.width);
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

void
Pintor::setMostrarBordes(bool mostrar)
{
  mostrarBordes = mostrar;
}

void
Pintor::reset()
{
  imagen = NULL;
  falsoColor.rojo = -1;
  falsoColor.verde = -1;
  falsoColor.azul = -1;
}

void
Pintor::dibujarBordes(int x, int y, int height, int width)
{
  Glib::RefPtr<Gdk::GC> refGC1 = Gdk::GC::create(dibujo->get_window());
  Gdk::Color color;
  color.set_red(255 * 255);
  color.set_green(255 * 255);
  color.set_blue(255 * 255);
  int id = desplazamientoY + y + 1;
  for (int i = y + 1; i < height - 1; i++)
    {
      int iid = desplazamientoX + x + 1;
      for (int ii = x + 1; ii < width - 1; ii++)
        {
          bool pinta = false;
          for (unsigned int iii = 0; iii < imagen->vectorBanda.size(); iii++)
            if (imagen->vectorBanda[iii]->cargada)
              {
                Banda* c = imagen->vectorBanda[iii];
                int valor = (abs((c->matriz[id - 1][iid - 1] + 2 * c->matriz[id
                    - 1][iid] + c->matriz[id - 1][iid + 1])
                    - (c->matriz[id + 1][iid - 1] + 2 * c->matriz[id + 1][iid]
                        + c->matriz[id + 1][iid + 1])) + abs(
                    (c->matriz[id - 1][iid + 1] + 2 * c->matriz[id][iid + 1]
                        + c->matriz[id + 1][iid + 1]) - (c->matriz[id - 1][iid
                        - 1] + 2 * c->matriz[id][iid - 1]
                        + c->matriz[id + 1][iid - 1]))) / 6;
                if (valor > 10)
                  pinta = true;
              }
          if (pinta)
            {
              refGC1->set_rgb_fg_color(color);
              dibujo->get_window()->draw_point(refGC1, ii, i);
            }
          iid += nivelZoom == 0 ? ii % 2 : nivelZoom;
        }
      id += nivelZoom == 0 ? i % 2 : nivelZoom;
    }
}

void
Pintor::pintarTresBandas(int x, int y, int height, int width)
{
  int capaRoja = falsoColor.rojo;
  int capaVerde = falsoColor.verde;
  int capaAzul = falsoColor.azul;
  Glib::RefPtr<Gdk::GC> refGC1 = Gdk::GC::create(dibujo->get_window());
  Gdk::Color color;
  int id = desplazamientoY + y;
  for (int i = y; i < height; i++)
    {
      int iid = desplazamientoX + x;
      for (int ii = x; ii < width; ii++)
        {

          Banda* c = imagen->vectorBanda[capaRoja];
          color.set_red(c->matriz[id][iid] * 255);
          c = imagen->vectorBanda[capaVerde];
          color.set_green(c->matriz[id][iid] * 255);
          c = imagen->vectorBanda[capaAzul];
          color.set_blue(c->matriz[id][iid] * 255);

          refGC1->set_rgb_fg_color(color);
          dibujo->get_window()->draw_point(refGC1, ii, i);
          iid += nivelZoom == 0 ? ii % 2 : nivelZoom;
        }
      id += nivelZoom == 0 ? i % 2 : nivelZoom;
    }
}

void
Pintor::pintarTresBandasRangoAcotado(int x, int y, int height, int width)
{
  int minRojo = this->minRojo;
  int minVerde = this->minVerde;
  int minAzul = this->minAzul;
  int maxRojo = this->maxRojo;
  int maxVerde = this->maxVerde;
  int maxAzul = this->maxAzul;
  int capaRoja = falsoColor.rojo;
  int capaVerde = falsoColor.verde;
  int capaAzul = falsoColor.azul;
  Glib::RefPtr<Gdk::GC> refGC1 = Gdk::GC::create(dibujo->get_window());
  Gdk::Color color;
  int id = desplazamientoY + y;
  for (int i = y; i < height; i++)
    {
      int iid = desplazamientoX + x;
      for (int ii = x; ii < width; ii++)
        {

          Banda* c = imagen->vectorBanda[capaRoja];
          color.set_red((c->matriz[id][iid] < maxRojo && c->matriz[id][iid]
              > minRojo) ? c->matriz[id][iid] * 255 : 0);
          c = imagen->vectorBanda[capaVerde];
          color.set_green((c->matriz[id][iid] < maxVerde && c->matriz[id][iid]
              > minVerde) ? c->matriz[id][iid] * 255 : 0);
          c = imagen->vectorBanda[capaAzul];
          color.set_blue((c->matriz[id][iid] < maxAzul && c->matriz[id][iid]
              > minAzul) ? c->matriz[id][iid] * 255 : 0);

          refGC1->set_rgb_fg_color(color);
          dibujo->get_window()->draw_point(refGC1, ii, i);
          iid += nivelZoom == 0 ? ii % 2 : nivelZoom;
        }
      id += nivelZoom == 0 ? i % 2 : nivelZoom;
    }
}

void
Pintor::pintarMenosBandas(int x, int y, int height, int width)
{
  int capaRoja = falsoColor.rojo;
  int capaVerde = falsoColor.verde;
  int capaAzul = falsoColor.azul;
  Glib::RefPtr<Gdk::GC> refGC1 = Gdk::GC::create(dibujo->get_window());
  Gdk::Color color;
  int id = desplazamientoY + y;
  for (int i = y; i < height; i++)
    {
      int iid = desplazamientoX + x;
      for (int ii = x; ii < width; ii++)
        {

          if (capaRoja >= 0)
            {
              Banda* c = imagen->vectorBanda[capaRoja];
              color.set_red(c->matriz[id][iid] * 255);
            }
          if (capaVerde >= 0)
            {
              Banda* c = imagen->vectorBanda[capaVerde];
              color.set_green(c->matriz[id][iid] * 255);
            }
          if (capaAzul >= 0)
            {
              Banda* c = imagen->vectorBanda[capaAzul];
              color.set_blue(c->matriz[id][iid] * 255);
            }

          refGC1->set_rgb_fg_color(color);
          dibujo->get_window()->draw_point(refGC1, ii, i);
          iid += nivelZoom == 0 ? ii % 2 : nivelZoom;
        }
      id += nivelZoom == 0 ? i % 2 : nivelZoom;
    }
}

void
Pintor::pintarMenosBandasRangoAcotado(int x, int y, int height, int width)
{
  int minRojo = this->minRojo;
  int minVerde = this->minVerde;
  int minAzul = this->minAzul;
  int maxRojo = this->maxRojo;
  int maxVerde = this->maxVerde;
  int maxAzul = this->maxAzul;
  int capaRoja = falsoColor.rojo;
  int capaVerde = falsoColor.verde;
  int capaAzul = falsoColor.azul;
  Glib::RefPtr<Gdk::GC> refGC1 = Gdk::GC::create(dibujo->get_window());
  Gdk::Color color;
  int id = desplazamientoY + y;
  for (int i = y; i < height; i++)
    {
      int iid = desplazamientoX + x;
      for (int ii = x; ii < width; ii++)
        {

          if (capaRoja >= 0)
            {
              Banda* c = imagen->vectorBanda[capaRoja];
              color.set_red((c->matriz[id][iid] < maxRojo && c->matriz[id][iid]
                  > minRojo) ? c->matriz[id][iid] * 255 : 0);
            }
          if (capaVerde >= 0)
            {
              Banda* c = imagen->vectorBanda[capaVerde];
              color.set_green((c->matriz[id][iid] < maxVerde
                  && c->matriz[id][iid] > minVerde) ? c->matriz[id][iid] * 255
                  : 0);
            }
          if (capaAzul >= 0)
            {
              Banda* c = imagen->vectorBanda[capaAzul];
              color.set_blue((c->matriz[id][iid] < maxAzul
                  && c->matriz[id][iid] > minAzul) ? c->matriz[id][iid] * 255
                  : 0);
            }

          refGC1->set_rgb_fg_color(color);
          dibujo->get_window()->draw_point(refGC1, ii, i);
          iid += nivelZoom == 0 ? ii % 2 : nivelZoom;
        }
      id += nivelZoom == 0 ? i % 2 : nivelZoom;
    }
}

Pintor::~Pintor()
{
  // TODO Auto-generated destructor stub
}
