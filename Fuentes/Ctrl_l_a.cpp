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


#include "Ctrl_l_a.hpp"
//#pragma hdrstop

// Inicializa el arreglo
void Ctrl_lista_archivos::Inicializa_arreglo(void)
{ 
   for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) strcpy(Arreglo_nombres[i],""); 
}


void Ctrl_lista_archivos::Graba_lista_archivos(void)
{
   Manipulador_archivos ma;
   ma.Parametros(Nombre_lista, GRABA_ARCHIVO, MAXPATH, "");
   if (!ma.Retorna_estado()) {
      for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) {
         if(Arreglo_nombres[i][0]) ma.Graba_linea(Arreglo_nombres[i]);
      }
   }
}
              
void Ctrl_lista_archivos::Lee_lista_archivos(void)
{
   Inicializa_arreglo();
   Manipulador_archivos ma;
   ma.Parametros(Nombre_lista, LEE_ARCHIVO, MAXPATH, "");
   if (!ma.Retorna_estado()) {
     int i = 0;
     char xarch[MAXPATH];
     while (ma.Retorna_estado() != 2) {
        ma.Lee_linea(xarch);
        memccpy(Arreglo_nombres[i],xarch,0,MAXPATH-1);
        i++;
        if (i >= NUM_MAX_ARCHIVOS) break;
     }
   } 
}
      
// Inserta un nombre al inicio de la lista
void Ctrl_lista_archivos::Inserta_nombre(const char *arch)
{
   for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) {
       if (Compara_ignorando_minusculas(arch, Arreglo_nombres[i]))  Arreglo_nombres[i][0] = 0;
   }
   // Despalza los nombres al siguiente indice
   for (int i = (NUM_MAX_ARCHIVOS - 1); i > 0; i--) {
    memccpy(Arreglo_nombres[i], Arreglo_nombres[i-1],0,MAXPATH-1);
   }
   // Copia el nuevo indice
   memccpy(Arreglo_nombres[0], arch,0,MAXPATH-1);
   // Graba la lista de archivos
   Graba_lista_archivos();
   // Lee la nueva lista omitiendo los nulos
   Lee_lista_archivos();
}


// Retorna el nombre indicado de la lista
void Ctrl_lista_archivos::Retorna_nombre(const int i, char *arch, bool &st)
{ 
   if ( i < NUM_MAX_ARCHIVOS) memccpy(arch,Arreglo_nombres[i],0,MAXPATH-1);
   if ( arch[0]) st = true;
    else st = false;
}

              // Indica si existe almenos un elemento en la lista
bool Ctrl_lista_archivos::Existe_almenos_uno(void)
{
   bool st = false;
   for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) {
      if(Arreglo_nombres[i][0]) st = true;
   }
   return st;
}        

