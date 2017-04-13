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



#ifndef __FECHORA_HPP__
#define __FECHORA_HPP__

#include "Fechas.hpp"
#include "Tiempo.hpp"


class Fechas_Horas: public Fechas, public Tiempo
{

    private:
    

    public:

            // Retorna la fecha y hora actuales escrita con el sigiente formato:
            // Si T_P es (0) el formato es:
            //    Si ID es (1) el formato es [DD DE MMMMMMMM DE CCAA HH:MM:SS P.M.] en español
            //    Si ID es (0) el formato es [DD OF MMMMMMMM OF CCAA HH:MM:SS P.M.] en ingles
            // Si T_P es (1) el formato es:
            //    Si ID es (1) el formato es [DDDDDDDD DD DE MMMMMMMM DE CCAA HH:MM:SS P.M.] en español
            //    Si ID es (0) el formato es [DDDDDDDD DD OF MMMMMMMM OF CCAA HH:MM:SS P.M.] en ingles
       void Fecha_y_hora(char *cad, const unsigned int t_p, const unsigned int id);
    

};


#endif 

