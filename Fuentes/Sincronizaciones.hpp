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


#ifndef __SINCRONIZACIONES_HPP__
#define __SINCRONIZACIONES_HPP__


#include "FuncionDisparo.hpp"
#include <stdlib.h>

#define MAYORSINCRONIZACION   255  // Si se cambia este valor hay que modificar el tipo de variable de Arreglo_Sincronizaciones_Q y _P
#define FALSO 0
#define VERDADERO 1
#define ADELANTE    1
#define ATRAZ       0


//////////////////////////////////////////////////////////////////////////////////
//      Clase que contiene las rutinas para el calculo de sincronizaciones      //
//////////////////////////////////////////////////////////////////////////////////


class Sincronizaciones: public FuncionDisparo 
{
   private:

      unsigned int           Arreglo_Sincronizaciones_Q[MAYORSINCRONIZACION];
      unsigned int           Arreglo_Sincronizaciones_P[MAYORSINCRONIZACION];
      long double            Tolerancia;

   public:
        
      unsigned int           Res_Long_max_ciclo ;
      unsigned int           Res_Long_Max_transitorios;
      unsigned int           Res_Long_Min_transitorios;
      unsigned int           Num_condiciones_iniciales;
      long double            Res_Tolerancia;
      long double            Res_Condicion_Inicial;
      bool                   Res_sw_Cond_inicial_aleatoria;


      
                             // Constructor de la clase
                             Sincronizaciones(void)
                             {
                                Res_Long_Min_transitorios     = 500;
                                Res_Long_Max_transitorios     = 100000;
                                Res_Long_max_ciclo            = 250;
                                Res_Tolerancia                = 1e-7;
                                Num_condiciones_iniciales     = 10;
                                Res_Condicion_Inicial         = 0.0;
                                Res_sw_Cond_inicial_aleatoria = true;
                             }
                             // Retorna el numero de resonancias
      int                    Calcula(const bool  t_i);
};

#endif

