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


#ifndef __NUMEROROTACION_HPP__
#define __NUMEROROTACION_HPP__


#include "FuncionDisparo.hpp"
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////////////////
//         Clase que contiene la rutina para el calculo del numero de Rotación        //
////////////////////////////////////////////////////////////////////////////////////////


class NumeroRotacion: public FuncionDisparo 
{
   public:

                             // Indica el error en calculo (si este existe)
      int                    Error_calculo;
      int                    Rot_Num_iteraciones;
      long double            Rot_Condicion_Inicial;
      bool                   Rot_sw_Cond_inicial_aleatoria;
      int                    Num_condiciones_iniciales;

    
                             // Constructor de la clase
                             NumeroRotacion(void)
                             {
                                // Valores por omision para el calculo del rotacional
                                Rot_Num_iteraciones           = 100000;
                                Error_calculo                 = 0;
                                Rot_Condicion_Inicial         = 0.0;
                                Rot_sw_Cond_inicial_aleatoria = true;
                                Num_condiciones_iniciales     = 10;
                             }
                             
                             // Retorna el numero de rotacional
      int                    Calcula(const bool  t_i);
};

#endif

