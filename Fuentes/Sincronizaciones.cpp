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



#include "Sincronizaciones.hpp"
#include "Man_arch.hpp"

// Retorna el numero de sincronizacion
int Sincronizaciones::Calcula(const bool  t_i)
{
   bool Found = false;        
   // Almacena el primer valor de las iteraciones para buscar las resonancias
   long double x, Frac_x, Int_x = 0.0, incX;
   // Almacena las iteraciones de la funcion de disparos
   long double New_x, FracNew_x, IntNew_x;
   // Auxiliar en las busquedas
   long double Ancla_x, FracAncla_x, IntAncla_x;
   //Det. la cantidad de sincros para un conj. de parametros especificos
   unsigned char Indice_sincronizaciones = 0;  
   // Definicion del arreglo de sincronizaciones
   long double IncInits;    
   unsigned int Posible_Q, Posible_P;
   unsigned int i;
   unsigned int Encontro;  //Determina si se ha encontrado alguna posible orbita periodica
   char xcad[1000];
   long double CondInicial;

   Manipulador_archivos *Gp = new Manipulador_archivos;
   Gp->Parametros("C:\\REPTMP.TMP",GRABA_ARCHIVO,200,"");
   Gp->Graba_linea("Reporte de Sincronizaciones");
   Gp->Graba_linea("");
   Encontro = FALSO;


   // Inicializa arreglo de sincronizaciones
   for (i = 0; i < MAYORSINCRONIZACION; i++)
   {
       Arreglo_Sincronizaciones_Q[i] = 0;
       Arreglo_Sincronizaciones_P[i] = 0;
   }

   //Barrido de Cond. Iniciales
   if (Res_sw_Cond_inicial_aleatoria) incX = 1.0/((long double) Num_condiciones_iniciales), IncInits = 0.0;
    else incX = 0.6, IncInits = Res_Condicion_Inicial;

   for ( ; IncInits < 1.0; IncInits += incX ) 
   {
      // Condicion inicial condición inicial aleatoria dentro del intervalo
      Frac_x = IncInits + (incX * (random(10)/10.0));   
      CondInicial = Frac_x;

      //  A partir de la Condicion Inicial se realiza un Transitorio de iteraciones sobre ella
      for(i = 0; i < Res_Long_Min_transitorios; i++) {  
          x = Frac_x;
          x = FuncionDisparo::Calcula(x,t_i) + Int_x;
          Frac_x = modfl(x,&Int_x);
      }

      Encontro = FALSO;
      Posible_Q = 0;
      FracNew_x = Frac_x;
      while( (Encontro == FALSO) && (Posible_Q <= Res_Long_max_ciclo))
      {
         x = Frac_x;
         x = FuncionDisparo::Calcula(x,t_i) + Int_x;
         Frac_x = modfl(x, &Int_x);
         //Almacena la posible longitud de la órbita periódica
         Posible_Q++;
         i ++;
         //Condicion para det. si hay órbita
         if ( (fabsl(Frac_x - FracNew_x) < 1e-3) || (fabsl(fabsl(Frac_x - FracNew_x)-1.0) < 1e-3) ) Encontro = VERDADERO;
      }
      if (Encontro != VERDADERO) continue;

      Encontro = FALSO;
      while(i <= Res_Long_Max_transitorios && (Encontro == FALSO) ) {
         Posible_Q = 0;
         FracNew_x = Frac_x;
         while( (Encontro == FALSO) && (Posible_Q <= Res_Long_max_ciclo))
         {
            x = Frac_x;
            x = FuncionDisparo::Calcula(x,t_i) + Int_x;
            Frac_x = modfl(x, &Int_x);
            //Almacena la posible longitud de la órbita periódica
            Posible_Q++;
            i ++;
            //Condicion para det. si hay órbita
            if ( (fabsl(Frac_x - FracNew_x) < Res_Tolerancia) || (fabsl(fabsl(Frac_x - FracNew_x)-1.0) < Res_Tolerancia) ) Encontro = VERDADERO;
         }
      }
      if (i > Res_Long_Max_transitorios) continue;

      //Inicia comprobacion de la resonancia hallada
      if ((Encontro == VERDADERO))
      {
          Encontro = FALSO;
          if ( (Posible_Q > 0) && (Posible_Q <= Res_Long_max_ciclo) )
	  {
	      //New_x=New_x+(Res_Tolerancia/10); //Perturbacion ligera si se desea
	      New_x = Ancla_x = Frac_x;
              //Ciclo de check (long. de Posible_Q) desde 1 hasta Posible_Q, pues ahora solo se recorrá la orbita que se presume haber encontrado
              for ( i = 1; i <= Posible_Q; i++) 
              { 
                   FracNew_x = modfl(New_x,&IntNew_x);
                   New_x = FuncionDisparo::Calcula(FracNew_x,t_i) + IntNew_x;
              }

              FracAncla_x = modfl(Ancla_x, &IntAncla_x);

              FracNew_x = modfl(New_x, &IntNew_x);
              //Condicion para det. si hay órbita
              if ( (fabsl(FracAncla_x - FracNew_x) < Res_Tolerancia) || (fabsl(fabsl(FracAncla_x - FracNew_x) - 1.0) < Res_Tolerancia) ) Encontro = VERDADERO;
                      
              // Se encontro resonancia y paso el check
              if ( (Encontro == VERDADERO) ) 
              {
                  // Determina la posible envolvencia asociada a la órbita encontrada para "Posible_Q" este ya se encuentra almacenado
                  Posible_P = IntNew_x - IntAncla_x;

                  // Posible P no debera de excede a Posible Q
                  if (Posible_P < 254) 
                  {
                     //Caso en que no se proporciona lista de sincronizaciones opciones de Grabado para Multisincronizacion
                     sprintf(xcad,"   [%u:%u] ->%1.16Lf",Posible_Q,Posible_P,CondInicial);
                     Gp->Graba_linea(xcad);
                     if (Indice_sincronizaciones > 0)
                     {
                        for (i = 0; i < Indice_sincronizaciones; i++)
                        {
                           if (Arreglo_Sincronizaciones_Q[i] == Posible_Q && Arreglo_Sincronizaciones_P[i] == Posible_P) break;
                        }
                        if (i == Indice_sincronizaciones)
                        {
                            Arreglo_Sincronizaciones_Q[Indice_sincronizaciones] = (unsigned char) Posible_Q;
                            Arreglo_Sincronizaciones_P[Indice_sincronizaciones] = (unsigned char) Posible_P;
                            Indice_sincronizaciones++;
                        }
                      } else {
                         Arreglo_Sincronizaciones_Q[Indice_sincronizaciones] = (unsigned char) Posible_Q;
                         Arreglo_Sincronizaciones_P[Indice_sincronizaciones] = (unsigned char) Posible_P;
                         Indice_sincronizaciones++;
                     }
                  }
              } // if de "Se encontro resonancia y paso el check"
	  } //termina "if( (Posible_Q>0L) && (Posible_Q<=Res_Long_max_ciclo) )"
      } //termina "if((Encontro==VERDADERO)) //Inicia comprob.(Check) de la resonancia hallada"
      if (!Res_sw_Cond_inicial_aleatoria) break;
   } //Termina for de "IncInit"

   // Almacenamiento de los valores de los parametros y sus sincronizaciones
   if (Indice_sincronizaciones > 0)
   {
      Found = true;
      Gp->Graba_linea("");
      Gp->Graba_linea("");
      sprintf(xcad,"Se encontraron %i sincronizacion(es) diferente(s):",Indice_sincronizaciones);
      Gp->Graba_linea(xcad);
      for (i = 0; i < Indice_sincronizaciones; i++)
      {
         sprintf(xcad,"   [%u:%u]",Arreglo_Sincronizaciones_Q[i],Arreglo_Sincronizaciones_P[i]);
         Gp->Graba_linea(xcad);
      } 
      Gp->Graba_linea("");
      Gp->Graba_linea("Con los parámetros:");
      for (i = 0; i < FuncionDisparo::Numero_parametros; i++)
      {
         sprintf(xcad," %s = %Lf",FuncionDisparo::Nombre_parametros[i],FuncionDisparo::P[i]);
         Gp->Graba_linea(xcad);
      } 
      

   }    

   delete Gp;
   return (Found);
}

