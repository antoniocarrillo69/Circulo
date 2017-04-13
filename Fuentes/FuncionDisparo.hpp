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


#ifndef __FUNCIONDISPARO_HPP__
#define __FUNCIONDISPARO_HPP__

#include "DefinicionEcuacion.hpp"

// Indica el método numérico de integración activo
#define RUNGE-KUTTA





////////////////////////////////////////////////////////////////////////////////////////////////////
//      Clase que contiene los metodos numericos de integracion de ecuaciones diferenciales       //
////////////////////////////////////////////////////////////////////////////////////////////////////


class FuncionDisparo: public DefinicionEcuacion
{
   private:
                             
      long double            Y, Yn, T1, Cota;    // Variables temporales para la función de disparo


#ifdef BOYLAND
long double F(long double x);  // Funcion bimodal continua

long double fTriangular(long double x);  // Define una funcion triangular en el plano

long double fasesF(long double x);  // Funcion bimodal continua reducida a fases

long double B(long double t,long double x);  // Funcion escalon de Boyland

long double fasesB(long double t,long double x);  // Funcion de fases escalon de Boyland

long double B_intervalo(long double t,long double x);

int extremos(void); // Halla las coordenadas (x,y) de los puntos criticos de la funcion F

int halla_maximo(void);  // Halla las coordenadas (x,y) del punto maximo de la funcion F

int halla_minimo(void);

					// Halla las coordenadas (x,y) del punto minimo de la funcion F

int define(long double t);

					// define los extremos de los escalones de Boyland y su altura yB

long double FInv(int PtoInic);


int esBimodal;   // Bandera que indica si la funcion en turno es bimodal

int caso;  // Indica el orden en que se encuentran el maximo y el minimo de la funcion F

				// caso = 0  es una funcion similar a la funcion de Arnold

				// caso = 1 es una funcion similar a la funcion de Arnold multiplicada por -1

long double NumPtsF;  // Numero de puntos de la malla para buscar el maximo y el minimo de F

long double maxX,minX,maxY,minY;  // Coordenadas (x,y) del maximo y el minimo de F

long double xB1;  // Coordenada del extremo izquierdo del escalon de la funcion de Boyland

long double xB2;  // Coordenada del extremo derecho del escalon de la funcion de Boyland

long double yB;  // Valor del escalon de Boyland
#endif

#ifdef ANA
  long double Beta(const long double xx);
  long double R1(const long double x);
  long double R2(const long double x);
#endif
      
   public:

      ////////////////////////////////////////////////////////////////////////
      // Función de disparo
      ////////////////////////////////////////////////////////////////////////
        
                             // Indica el error en calculo (si este existe)
      unsigned int           Error_calculo;

                             // Indica el tipo de función de disparo
                             // (0) Normal
                             // (1) Boyland                             
      int                    Tipo_funcion_disparo;

                             // Parámetro para el cálculo de la funcion de Boyland valor [0,1]
      long double            Parametro_funcion_boyland;

                             // Constructor de la clase
                             FuncionDisparo(void);

                             // Cálculo de la función de disparo
      long double            Calcula(const long double T, const bool FI);
};

#endif

