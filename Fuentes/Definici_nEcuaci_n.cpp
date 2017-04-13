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



#include "DefinicionEcuacion.hpp"

// Constructor de la clase
DefinicionEcuacion::DefinicionEcuacion(void)
{
   // Nombre del sistema   
#ifdef TETRAESTABLE   
   Nombre_sistema       = "Tetraestable";
   // Número de parámetros
   Numero_parametros          = 3;
   Numero_parametros_bifurcar = 3;
   // Nombre de los parámetros (si el nombre de el parámetro excede de 6 letras
   // hay que indicar el tamaño maximo de este en captura de parámetros en cada escenario)
   Nombre_parametros[0] = "a";
   Nombre_parametros[1] = "b";
   Nombre_parametros[2] = "c";
   
   // Valores por omisión de los parametros
   P[0] = 0.5;
   P[1] = 0.45;
   P[2] = 0.45;
   // Ecuación en formato texto
   Ecuacion_texto = "x = x + a + b * sin(2.0 * M_PI * x) + c * cos(2.0 * M_PI * x)";
#endif
#ifdef ANA
   Nombre_sistema       = "ANA";
   // Número de parámetros
   Numero_parametros          = 3;
   Numero_parametros_bifurcar = 3;
   // Nombre de los parámetros (si el nombre de el parámetro excede de 6 letras
   // hay que indicar el tamaño maximo de este en captura de parámetros en cada escenario)
   Nombre_parametros[0] = "a";
   Nombre_parametros[1] = "b";
   Nombre_parametros[2] = "c";
   
   // Valores por omisión de los parametros
   P[0] = 0.5;
   P[1] = 0.45;
   P[2] = 0.45;
   // Ecuación en formato texto
   Ecuacion_texto = "x = x + a + b * sin(2.0 * M_PI * x)";
#endif
#ifdef ARNOLD
   Nombre_sistema       = "Arnold's Classical Maps of Circunference";
   // Número de parámetros
   Numero_parametros          = 2;
   Numero_parametros_bifurcar = 2;
   // Nombre de los parámetros (si el nombre de el parámetro excede de 6 letras
   // hay que indicar el tamaño maximo de este en captura de parámetros en cada escenario)
   Nombre_parametros[0] = "a";
   Nombre_parametros[1] = "b";
   
   // Valores por omisión de los parametros
   P[0] = 0.5;
   P[1] = 0.45;
   // Ecuación en formato texto
   Ecuacion_texto = "x = x + a + b * sin(2.0 * M_PI * x)";
#endif
}



// Asigna valor a los parámetros
void DefinicionEcuacion::AsignaParametros(const long double parm[NUM_MAX_PARAMETROS])
{
   for (int i = 0; i < Numero_parametros; i++) P[i] = parm[i];
}

// Retorna el valor de los parámetros
void DefinicionEcuacion::RetornaParametros(long double *parm)
{
   for (int i = 0; i < Numero_parametros; i++) parm[i] = P[i];
#ifdef BOYLAND   
   Preparada=-1;
#endif   
}


