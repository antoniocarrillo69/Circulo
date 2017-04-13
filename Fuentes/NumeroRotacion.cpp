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



#include "NumeroRotacion.hpp"
#include "Man_arch.hpp"


// Retorna el numero de rotacion
int NumeroRotacion::Calcula(const bool  t_i)
{
   long double aux = 0.0, Temp1, incX, condini, xx, xcond;  
   int N;
   char xcad[3000];     

   Manipulador_archivos *Gp = new Manipulador_archivos;
   Gp->Parametros("C:\\REPTMP.TMP",GRABA_ARCHIVO,200,"");
   Gp->Graba_linea("Reporte del Número de Rotación");
   Gp->Graba_linea("");
   
   incX = 1.0/((long double) Num_condiciones_iniciales);
   for (condini= 0.0 ; condini < 1.0; condini += incX ) 
   {
      if (Rot_sw_Cond_inicial_aleatoria) Temp1 = condini + (incX * (random(10)/10.0));   
       else Temp1 = Rot_Condicion_Inicial;
      xcond = Temp1;

      N = 0;
      while ((N < Rot_Num_iteraciones) && !Error_calculo) {
         xx = FuncionDisparo::Calcula(Temp1,true);
         Temp1 = xx + aux;
         Temp1 = modfl(Temp1,&aux);
         N++;
      }
      if (!Error_calculo) {
//         Temp1 = modfl((Temp1 + aux)/ (long double) Rot_Num_iteraciones,&aux);
         if (Temp1 >= 0.0) Temp1 = modfl((Temp1 + aux)/ (long double) Rot_Num_iteraciones,&aux);
          else Temp1 = modfl((Temp1 + aux)/ (long double) Rot_Num_iteraciones,&aux) + 1.0;
      } else Temp1 = -10.0;
      sprintf(xcad,"   [%1.16Lf] <- %1.16Lf",Temp1,xcond);
      Gp->Graba_linea(xcad);
   }
   Gp->Graba_linea("");
   Gp->Graba_linea("Con los parámetros:");
   for (int y = 0; y < FuncionDisparo::Numero_parametros; y++) {
      sprintf(xcad," %s = %Lf",FuncionDisparo::Nombre_parametros[y],FuncionDisparo::P[y]);
      Gp->Graba_linea(xcad);
   } 
   delete Gp;
   return 1;
}


