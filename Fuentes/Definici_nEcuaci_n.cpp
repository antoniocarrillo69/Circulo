//////////////////////////////////////////////////////////////////////////////////////////////
// CIRCULO WINDOWS C++ 1.0                                                                  //
//                                                                                          //
// An�lisis y Dise�o:                                                                       //
//                                                                                          //
// Humberto Andres Carrillo Calvet                                                          //
// Antonio Carrillo Ledesma                                                                 //
// Oscar Garc�a Regis                                                                       //
// Miguel Angel Mendoza Reyes                                                               //
// Fernando Alberto Ongay Larios                                                            //
//                                                                                          //
// Programaci�n:                                                                            //
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
   // N�mero de par�metros
   Numero_parametros          = 3;
   Numero_parametros_bifurcar = 3;
   // Nombre de los par�metros (si el nombre de el par�metro excede de 6 letras
   // hay que indicar el tama�o maximo de este en captura de par�metros en cada escenario)
   Nombre_parametros[0] = "a";
   Nombre_parametros[1] = "b";
   Nombre_parametros[2] = "c";
   
   // Valores por omisi�n de los parametros
   P[0] = 0.5;
   P[1] = 0.45;
   P[2] = 0.45;
   // Ecuaci�n en formato texto
   Ecuacion_texto = "x = x + a + b * sin(2.0 * M_PI * x) + c * cos(2.0 * M_PI * x)";
#endif
#ifdef ANA
   Nombre_sistema       = "ANA";
   // N�mero de par�metros
   Numero_parametros          = 3;
   Numero_parametros_bifurcar = 3;
   // Nombre de los par�metros (si el nombre de el par�metro excede de 6 letras
   // hay que indicar el tama�o maximo de este en captura de par�metros en cada escenario)
   Nombre_parametros[0] = "a";
   Nombre_parametros[1] = "b";
   Nombre_parametros[2] = "c";
   
   // Valores por omisi�n de los parametros
   P[0] = 0.5;
   P[1] = 0.45;
   P[2] = 0.45;
   // Ecuaci�n en formato texto
   Ecuacion_texto = "x = x + a + b * sin(2.0 * M_PI * x)";
#endif
#ifdef ARNOLD
   Nombre_sistema       = "Arnold's Classical Maps of Circunference";
   // N�mero de par�metros
   Numero_parametros          = 2;
   Numero_parametros_bifurcar = 2;
   // Nombre de los par�metros (si el nombre de el par�metro excede de 6 letras
   // hay que indicar el tama�o maximo de este en captura de par�metros en cada escenario)
   Nombre_parametros[0] = "a";
   Nombre_parametros[1] = "b";
   
   // Valores por omisi�n de los parametros
   P[0] = 0.5;
   P[1] = 0.45;
   // Ecuaci�n en formato texto
   Ecuacion_texto = "x = x + a + b * sin(2.0 * M_PI * x)";
#endif
}



// Asigna valor a los par�metros
void DefinicionEcuacion::AsignaParametros(const long double parm[NUM_MAX_PARAMETROS])
{
   for (int i = 0; i < Numero_parametros; i++) P[i] = parm[i];
}

// Retorna el valor de los par�metros
void DefinicionEcuacion::RetornaParametros(long double *parm)
{
   for (int i = 0; i < Numero_parametros; i++) parm[i] = P[i];
#ifdef BOYLAND   
   Preparada=-1;
#endif   
}


