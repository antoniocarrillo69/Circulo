//////////////////////////////////////////////////////////////////////////////////////////////
// CIRCULO WINDOWS C++ 1.0                                                                  //
//                                                                                          //
// Análisis y Diseño:                                                                       //
//                                                                                          //
// Humberto Andres Carrillo Calvet                                                          //
// Antonio Carrillo Ledesma                                                                 //
// Oscar García Regis                                                                       //
// Miguel Angel Mendoza Reyes                                                               //
// Fernando Alberto Ongay Larios                                                            //
//                                                                                          //
// Programación:                                                                            //
//                                                                                          //
// Humberto Andres Carrillo Calvet                                                          //
// Antonio Carrillo Ledesma                                                                 //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CTRL_L_A_H__
#define __CTRL_L_A_H__


#include "Man_arch.hpp"
#ifndef __PROGRAMACION_WINDOWS__
   #define MAXPATH   1024
#else 
   #include <io.h>
#endif
#include <string.h>
#define NUM_MAX_ARCHIVOS   10

class Ctrl_lista_archivos: public Cadenas
{
   private:
      char    Nombre_lista[MAXPATH];
      char    Arreglo_nombres[NUM_MAX_ARCHIVOS][MAXPATH];
       
              // Inicializa el arreglo
      void    Inicializa_arreglo(void);
              // Graba la lista de archivos
      void    Graba_lista_archivos(void);
              // Lee la lista de archivos
      void    Lee_lista_archivos(void);
      
   public:

              // Constructor
              Ctrl_lista_archivos(void)
              { Inicializa_arreglo(); }

              // inicializa lista de archivos 
      void    Inicializa_lista_archivos(const char *path)
              { 
                memccpy(Nombre_lista,path,0,MAXPATH-1);
                Lee_lista_archivos();
              }  
              // Inserta un nombre al inicio de la lista
      void    Inserta_nombre(const char *arch);
              // Retorna el nombre indicado de la lista
      void    Retorna_nombre(const int i, char *arch, bool &st);
              // Indica si existe almenos un elemento en la lista
      bool    Existe_almenos_uno(void);
};

#endif
