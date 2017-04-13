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



#include <string.h>
#include <stdio.h>
#include "Fechora.hpp"
#pragma hdrstop


#define MAX_LOG_FECHOR 100


// Retorna la fecha y hora actuales escrita con el sigiente formato:
// Si T_P es (0) el formato es:
//    Si ID es (1) el formato es [DD DE MMMMMMMM DE CCAA HH:MM:SS] en español
//    Si ID es (0) el formato es [DD OF MMMMMMMM OF CCAA HH:MM:SS] en ingles
// Si T_P es (1) el formato es:
//    Si ID es (1) el formato es [DDDDDDDD DD DE MMMMMMMM DE CCAA HH:MM:SS P.M.] en español
//    Si ID es (0) el formato es [DDDDDDDD DD OF MMMMMMMM OF CCAA HH:MM:SS P.M.] en ingles
// Si T_P es (2) el formato es:
//    ID no importa el formato es [DD/MM/AA HH:MM:SS]    
void Fechas_Horas::Fecha_y_hora(char *cad, const unsigned int t_p, const unsigned int id)
{
   char fecha[MAX_LOG_FECHOR+1], tiempo[MAX_LOG_FECHOR+1], xcad[MAX_LOG_FECHOR+1], fec_esc[MAX_LOG_FECHOR+1];
   int xtp = t_p;
   if (xtp == 2) xtp = 0;
   
   Fecha_hoy(fecha);
   Fecha_escrita(fecha, fec_esc, xtp, id);
   Hora_actual(tiempo, xtp, id);
   Trim(tiempo);
   if (t_p != 2) sprintf(xcad,"%s %s",fec_esc,tiempo);
    else sprintf(xcad,"%s %s",fecha,tiempo);
   strcpy(cad,xcad);
}
