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



#ifndef __DefinicionEcuacion_HPP__
#define __DefinicionEcuacion_HPP__


//#define BOYLAND


#define ARNOLD
//#define TETRAESTABLE
//#define ANA
//   #define GLASS
//   #define FLASH
//   #define OBSCURIDADLUZ1
//   #define OBSCURIDADLUZ3
//   #define ESQUELETO1
//   #define ESQUELETO3



extern "C" {
   #include <math.h>
}

#define NUM_MAX_PARAMETROS 3


/////////////////////////////////////////////////////////////////////////////////
//         Clase que contiene la definicion de la ecuaci�n del sistema         //
/////////////////////////////////////////////////////////////////////////////////

class DefinicionEcuacion {

   public:
                           // Nombre del sistema
      char                *Nombre_sistema;                  
                           // N�mero de par�metros
      char                 Numero_parametros;               
                           // N�mero de par�metros a bifurcar
      char                 Numero_parametros_bifurcar;               
                           // Nombre de par�metros
      char                *Nombre_parametros[NUM_MAX_PARAMETROS];            
                           // Ecuaci�n en formato texto
      char                *Ecuacion_texto;
                           // Par�metros del sistema
      long double          P[NUM_MAX_PARAMETROS];


#ifdef BOYLAND   
      int                  Preparada;   // Bandera que indica el estado de los preliminares para calcular B(t,x)
                                        //  -1 si todavia no han sido hallados el maximo y el minimo de F
                                        //  0  si ya fueron hallados el maximo y el minimo de F, pero no yB, x1 y x2
                                        //  1 el escalon de Boyland ya esta completamente definido

#endif       
                           // Constructor de la clase
                           DefinicionEcuacion(void);
                           // Asigna valor a los par�metros
      void                 AsignaParametros(const long double parm[NUM_MAX_PARAMETROS]);
                           // Retorna el valor de los par�metros
      void                 RetornaParametros(long double *parm);
      
                           // Ecuacion del sistema 
      long double          Ecuacion(const long double x)
                           { 
#ifdef TETRAESTABLE   
                              return ( x + P[0] + (P[1] * sinl(2.0 * M_PI * x)) + (P[2] * sinl(4.0 * M_PI * x)));
#endif       
#ifdef ARNOLD   
                              return ( x + P[0] + (P[1] * sinl(2.0 * M_PI * x)));
#endif       
                           } 
                           
};

#endif







