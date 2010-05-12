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

#include "Capa.h"

Capa::Capa(Glib::ustring nom, bool carga, Imagen* i)
{
  imagen = i;
  nombre = nom;
  cargada = carga;
  anchoVista = 100;
  altoVista = 100;
}

//Capa::Capa(const Capa &capa){
//
//}

void
Capa::cargarCapa()
{
  srand(time(NULL));
  if (cargada)
    {
      FILE *f;
      g_print("Cargando ");
      g_print((imagen->getDirectorio()
          + imagen->cabecera->obtieneNombreArchivoCapa(nombre)).c_str());
      g_print("...\n");
      f = fopen((imagen->getDirectorio()
          + imagen->cabecera->obtieneNombreArchivoCapa(nombre)).c_str(), "rb");
      if (f != NULL)
        {
          matriz = new guint8*[imagen->cabecera->alto];
          // datos = new RegistroBloque[100];
          for (int i = 0; i < imagen->cabecera->alto; i++)
            {
              //    datos[i].momentoUso.assign_current_time();
              //   datos[i].bloque = new TBloque[100];
              matriz[i] = new guint8[imagen->cabecera->ancho];//(TBloque *)malloc(  imagen->tamanioBloque*sizeof(TBloque) );
              fread(matriz[i], sizeof(guint8), imagen->cabecera->ancho, f);

              // for (int ii = 0; ii < imagen->cabecera->ancho; ii++)
              //  {
              //   matriz[i][ii] = rand()%256;
              //              datos[i].bloque[ii] = rand();
              // }
            }

        }
      else
        {
          g_print("No se encontro el archivo ");
          g_print(imagen->cabecera->obtieneNombreArchivoCapa(nombre).c_str());
        }
      fclose(f);

      //        int tama = imagen->cabecera->ancho * imagen->cabecera->alto;
      //        int salto =0 /*std::min(imagen->cabecera->ancho, imagen->cabecera->alto)
      //            / std::min(anchoVista, altoVista) - imagen->tamanioBloque*/;
      //        int bloquesSalto =0 /*salto / imagen->tamanioBloque*/;
      //        int cantBloques = tama / imagen->tamanioBloque;
      //        datos = new RegistroBloque[tama / imagen->tamanioBloque];
      //        RegistroBloque * rb;
      //        FILE *f;
      //        g_print("Cargando ");g_print((imagen->getDirectorio()+ imagen->cabecera->obtieneNombreArchivoCapa(nombre)).c_str());g_print("...\n");
      //        f = fopen((imagen->getDirectorio()+ imagen->cabecera->obtieneNombreArchivoCapa(nombre)).c_str(),
      //            "rb");
      //        if (f!=NULL){
      //        for (int i = 0; i < cantBloques; i++)
      //          {
      //            rb=new RegistroBloque;
      //            TBloque* tb= new TBloque[imagen->tamanioBloque];//(TBloque *)malloc(  imagen->tamanioBloque*sizeof(TBloque) );
      //
      //            rb->momentoUso = *new Glib::TimeVal();
      //            rb->momentoUso.assign_current_time();
      //            fread(tb, sizeof(TBloque), imagen->tamanioBloque, f);
      //            rb->bloque =tb ;
      //            datos[i] = *rb;
      //            for (int j = 0; j < bloquesSalto; j++)
      //              {
      //                rb=new RegistroBloque;
      //                rb->bloque = NULL; //TODO: estudiar de usar una lista vinculada (o doblemente vinculada) para no tener todos los blancos
      //                //tiene pros y contras
      //                datos[++i] = *rb;
      //              }
      //            fseek(f, bloquesSalto * imagen->tamanioBloque, SEEK_CUR);
      //          }
      //        }else{
      //           g_print("No se encontro el archivo ");g_print(imagen->cabecera->obtieneNombreArchivoCapa(nombre).c_str());
      //        }
      //        fclose(f);
    }
}
Capa::~Capa()
{
  for (int i = 0; i < imagen->cabecera->alto; i++)
    {
      delete matriz[i];
    }
  delete matriz;
}
