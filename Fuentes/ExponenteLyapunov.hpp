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


#ifndef __EXPONENTELYAPUNOV_HPP__
#define __EXPONENTELYAPUNOV_HPP__


#include "FuncionDisparo.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
//      Clase que contiene los metodos numericos de integracion de ecuaciones diferenciales       //
////////////////////////////////////////////////////////////////////////////////////////////////////


class ExponenteLyapunov: public FuncionDisparo
{

   public:
   
    long double              Condicion_inicial;
    bool                     Sw_cond_inicial_aleatoria;
    int                      Max_num_iteraciones;
    long double              Epsilon;
    long double              Paso_derivada;
        
      

                             // Constructor de la clase
                             ExponenteLyapunov(void)
                             {
                                Condicion_inicial = 0.0;
                                Sw_cond_inicial_aleatoria = true;
                                Max_num_iteraciones = 1000;
                                Epsilon = 0.0001;
                                Paso_derivada = 0.01;
                             }

                             // Retorna el exponente de Lyapunov
      long double            Calcula(const bool FI);
};

#endif

