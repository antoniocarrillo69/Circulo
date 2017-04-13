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



#include "FuncionDisparo.hpp"
#include <math.h>


// Constructor de la clase
FuncionDisparo::FuncionDisparo(void)
{
   // Variable para controlar el error numerico
   Error_calculo = 0;
#ifdef BOYLAND
   // Indica el tipo de función de disparo (0) Normal (1) Boyland                             
   Tipo_funcion_disparo = 0;
   // Parámetro para el cálculo de la funcion de Boyland valor [0,1]
   Parametro_funcion_boyland = 0.5;
   // Bandera que indica el estado de los preliminares para calcular B(t,x)
   Preparada=-1;
   // Numero de puntos de la malla para buscar el maximo y el minimo de F     
   NumPtsF = 5000.0;  
#endif   
}


// Función que encuentra el siguiente disparo
long double FuncionDisparo::Calcula(const long double T, const bool FI)
{
   Error_calculo = 0;
#ifndef ANA   
   #ifdef BOYLAND
   if (!Tipo_funcion_disparo) return (Ecuacion(T));
   return (B(Parametro_funcion_boyland,T));
   #else
   return (Ecuacion(T));
   #endif
#else 

   #ifdef GLASS
   long double x_entera;
   long double xx = modfl(T + P[1], &x_entera);
   return (Beta(xx)+x_entera);
//   return (Beta(R1(xx))+x_entera);
   #endif
   #ifdef FLASH
   return (Beta(Beta(R1(T))));
   #endif
   #ifdef OBSCURIDADLUZ1
   return (Beta(R2(Beta(R1(T)))));
   #endif
   #ifdef OBSCURIDADLUZ3
   return (Beta(R2(Beta(R1(T)))));
   #endif
   #ifdef ESQUELETO1
   return (Beta(Beta(R2(Beta(Beta(R1(T)))))));
   #endif
   #ifdef ESQUELETO3
   return (Beta(Beta(R2(Beta(Beta(R1(T)))))));
   #endif
#endif   
}

#ifdef ANA
long double FuncionDisparo::Beta(const long double xx)
{
   long double x_entera;
   long double x, res = 0.0;

   x = modfl(xx, &x_entera);
   
   if (P[0] > 0.0 && P[0] < 1.0) {
      long double xd = (1.0 / (2.0 * M_PI)) * acosl(P[0]);
      
      if (x >= 0.0 && x < xd ) res = (0.5 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))));
      if (x > xd && x < (1.0 -xd)) res = (1.0 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))));
      if (x > (1.0 -xd) && x < 1.0) res = (1.5 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))));

      if (x == xd) res = (3.0/4.0);
      if (x == (1.0 - xd)) res =  (5.0/4.0);
   }
   if (P[0] > 1.0) {
      if (x >= 0.0 && x < 0.5) res = (1.0 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))));
      if (x >= 0.5 && x < 1.0) res = (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x))));
   }
   return (res);//(res + x_entera);

/*
   x = modfl(xx, &x_entera);

   
   if (P[0] > 0.0 && P[0] < 1.0) {
      long double xd = (1.0 / (2.0 * M_PI)) * acosl(P[0]);
      if ((x >= 0.0 && x < xd) || (x > (1.0 - xd) && x < 1.0)) res = (0.5 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))));
      if (x > xd && x < 0.5) res = (1.0 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))));
      if (x > 0.5 && x < (1.0 -xd)) res = (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x))));
      if (x == xd) res = (3.0/4.0);
      if (x == (1.0 - xd)) res =  (1.0/4.0);
      if (x == 0.5) res = 0.0;
   }
   if (P[0] > 1.0) {
      if (x >= 0.0 && x < 0.5) res = (1.0 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))));
      if (x >= 0.5 && x < 1.0) res = (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x))));
   }
   return  (res + x_entera);

*/   
/*
   if (P[0] > 0.0 && P[0] < 1.0) {
      long double xd = (1.0 / (2.0 * M_PI)) * acosl(P[0]);
      if ((x >= 0.0 && x < xd) || (x > (1.0 - xd) && x < 1.0)) return ((0.5 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))))+x_entera);
      if (x > xd && x < 0.5) return ((1.0 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))))+x_entera);
      if (x > 0.5 && x < (1.0 -xd)) return ((((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x))))+x_entera);
      if (x == xd) return ((3.0/4.0)+x_entera);
      if (x == (1.0 - xd)) return ((1.0/4.0)+x_entera);
      if (x == 0.5) return (0.0+x_entera);
   }
   if (P[0] > 1.0) {
      if (x >= 0.0 && x < 0.5) return ((1.0 + (((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x)))))+x_entera);
      if (x >= 0.5 && x < 1.0) return ((((-1.0)/(2.0*M_PI))*atanl(sinl(2.0*M_PI*x)/(P[0]-cosl(2.0*M_PI*x))))+x_entera);
   }
   return (0.0+x_entera);
*/   
}

long double FuncionDisparo::R1(const long double x)
{
   long double x_modulo1;
   long double x_entera;
#ifdef GLASS   
   long double t1 = P[1];
#endif
#ifdef FLASH
   long double t1 = P[1];
#endif
#ifdef OBSCURIDADLUZ1
   long double t1 = (P[2]/(P[2]+1.0)) * P[1];
#endif   
#ifdef OBSCURIDADLUZ3
   long double t1 = P[1];
#endif
#ifdef ESQUELETO1
   long double t1 = (P[2]/(P[2]+1.0)) * P[1];
#endif   
#ifdef ESQUELETO3
   long double t1 = P[1];
#endif

//   x_modulo1 = modfl((x+t1), &x_entera);
//   return (x_modulo1+x_entera);
   return (x+t1);
//   return (x_modulo1);
}

long double FuncionDisparo::R2(const long double x)
{
   long double x_modulo1;
   long double x_entera;
#ifdef OBSCURIDADLUZ1
   long double t2 = (P[1]/(P[2]+1.0));
   x_modulo1 = modfl((x+t2), &x_entera);
#endif   
#ifdef OBSCURIDADLUZ3
   long double t2 = P[2];
   x_modulo1 = modfl((x+t2), &x_entera);
#endif   
#ifdef ESQUELETO1
   long double t2 = (P[1]/(P[2]+1.0));
   x_modulo1 = modfl((x+t2), &x_entera);
#endif   
#ifdef ESQUELETO3
   long double t2 = P[2];
   x_modulo1 = modfl((x+t2), &x_entera);
#endif   
   return (x_modulo1+x_entera);
}

#endif


#ifdef BOYLAND  
long double FuncionDisparo::B(long double t,long double x)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Retorna el valor de una funcion escalon de Boyland evaluada en el punto x
//      t          : es el parametro que indica cual funcion escalon deseamos utilizar
//      t = 0      : es la funcion escalon inferior de Boyland
//      t = 1      : es la funcion escalon superior de Boyland
//      0 < t < 1  : es una funcion escalon intermedia de Boyland
////////////////////////////////////////////////////////////////////////////////////////////////////
{

	long double ret;
	long double aux,aux2;

	aux = modfl(x,&aux2);
	ret = B_intervalo(t,aux) + aux2;
	return(ret);
}



long double FuncionDisparo::fasesB(long double t,long double x)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Retorna el valor en fase de una funcion escalon de Boyland evaluada en el punto x
// Esta funcion sirve para calcular las funciones de Bpylan en el toro
//      t          : es el parametro que indica cual funcion escalon deseamos utilizar
//      t = 0      : es la funcion escalon inferior de Boyland
//      t = 1      : es la funcion escalon superior de Boyland
//      0 < t < 1  : es una funcion escalon intermedia de Boyland
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	long double ret;
	long double aux,aux2;

	aux = modfl(x,&aux2);
	ret = modfl(B_intervalo(t,aux),&aux2);
	return(ret);
}



long double FuncionDisparo::B_intervalo(long double t,long double x)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcula la funcion escalon de Boyland correspondiente al real t y evaluada en el punto x de [0,1]
// Solamente esta definida en el intervalo. La funcion B(t,x,param) se encarga de extender esta
// funcion a todo el plano
//      t          : es el parametro que indica cual funcion escalon deseamos utilizar
//      t = 0      : es la funcion escalon inferior de Boyland
//      t = 1      : es la funcion escalon superior de Boyland
//      0 < t < 1  : es una funcion escalon intermedia de Boyland
////////////////////////////////////////////////////////////////////////////////////////////////////
{

	long double ret;
	long double aux,aux2;

	if (Preparada == -1)
	{
		esBimodal = extremos();
		Preparada = 0;
	}

	if ((Preparada==0) && (esBimodal))
	{
		define(t);
		Preparada = 1;
	}

	if ((Preparada == 1) && (esBimodal))
	{
		aux = modfl(x,&aux2);
		if (x < 0.0) aux += 1.0;
		if ((aux < xB1) || (aux > xB2)) ret = Ecuacion(aux);
		else ret = yB;
	} else ret = Ecuacion(x);
	return(ret);

}



int FuncionDisparo::extremos(void)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Halla los puntos donde la funcion F de disparos introducida por el usuario tiene su maximo y
// su minimo
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	int temp;

	temp = (halla_maximo() && halla_minimo());
	return(temp);
}



int FuncionDisparo::halla_maximo(void)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Halla el punto donde la funcion F de disparos tiene su minimo
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	int i;
	int maxFound = 0;
	int found = 0;
	long double x1,x2,x3,y1,y2,y3;
	long double inc = 1.0/NumPtsF;

	x1 = 0.0;
	y1 = Ecuacion(x1);
	x2 = inc;
	y2 = Ecuacion(x2);
	x3 = 2.0*inc;
	y3 = Ecuacion(x3);
	if ((y1 <= y2) && (y2 <= y3)) caso = 0;
	else caso = 1;
	for (i=2; i<NumPtsF; i++)
	{
		x3 = i*inc;
		y3 = Ecuacion(x3);
		found = ((y1 < y2) && (y2 > y3));
		if (found)
		{
			maxFound++;
			if (maxFound == 1)
			{
				maxX = x2;
				maxY = y2;
			}
		}
		x1 = x2;
		y1 = y2;
		x2 = x3;
		y2 = y3;
	}

	if (maxFound == 1)
	{
		found = 1;
		if (caso == 0)
		{
			if (maxY > Ecuacion(1.0))
			{
				maxX = 1.0;
				maxY = Ecuacion(1.0);
			}
		}
		else if (caso == 1)
		{
			if (maxY > Ecuacion(0.0))
			{
				maxX = 0.0;
				maxY = Ecuacion(0.0);
			}
		}
	}
	else found = 0;
	return(found);
}



int FuncionDisparo::halla_minimo(void)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Halla el punto donde la funcion F de disparos tiene su minimo
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	int i;
	int minFound = 0;
	int found = 0;
	long double x1,x2,x3,y1,y2,y3;
	long double inc = 1.0/NumPtsF;

	x1 = 0.0;
	y1 = Ecuacion(x1);
	x2 = inc;
	y2 = Ecuacion(x2);
	x3 = 2.0*inc;
	y3 = Ecuacion(x3);
	if ((y1 <= y2) && (y2 <= y3)) caso = 0;
	else caso = 1;
	for (i=2; i<NumPtsF; i++)
	{
		x3 = i*inc;
		y3 = Ecuacion(x3);
		found = ((y1 > y2) && (y2 < y3));
		if (found)
		{
			minFound++;
			if (minFound == 1)
			{
				minX = x2;
				minY = y2;
			}
		}
		x1 = x2;
		y1 = y2;
		x2 = x3;
		y2 = y3;
	}

	if (minFound == 1)
	{
		found = 1;
		if (caso == 0)
		{
			if (minY < Ecuacion(0.0))
			{
				minX = 0.0;
				minY = Ecuacion(0.0);
			}
		}
		else if (caso == 1)
		{
			if (minY < Ecuacion(1.0))
			{
				minX = 1.0;
				minY = Ecuacion(1.0);
			}
		}
	}
	else found = 0;
	return(found);
}



int FuncionDisparo::define(long double t)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Halla los extremos xB1,xB2 de la funcion escalon de Boyland y su altura yB
//   xB1: el extremo izquierdo del intervalo donde la funcion se hace constante
//   xB2: el extremo derecho del intervalo donde la funcion se hace constante
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	int temp=0;
	long double f0 = Ecuacion(0.0),f1 = Ecuacion(1.0);

	yB = minY + (maxY - minY)*t;
	if (caso == 0)  // F es similar a la funcion de Arnold
	{
		if (yB < f0)
		{
			xB1 = 0.0;
			xB2 = FInv(1);
			temp = 1;
		}
		else
		{
			xB1 = FInv(0);
			xB2 = FInv(1);
			temp = 1;
		}
	}
	else   // F es similar a la funcion de Arnold multiplicada por -1
	{
		if (yB < f1)
		{
			xB1 = FInv(0);
			xB2 = 1.0;
			temp = 1;
		}
		else
		{
			xB1 =FInv(0);
			xB2 =FInv(1);
			temp = 1;
		}
	}
	return(temp);
}



long double FuncionDisparo::FInv(int PtoInic)
////////////////////////////////////////////////////////////////////////////////////////////////////
// Halla los puntos xB1 y xB2 que definen los extremos del intervalo donde la funcion de Boyland se
// hace constante
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	int i=0;
	int found;
	long double inc = 1.0/NumPtsF;
	long double eps = 10.0/NumPtsF;
	long double x;

	do
	{
		PtoInic == 0 ? x = i*inc : x = 1.0 - i*inc;
		found = (fabsl(yB - Ecuacion(x))<eps);
		i++;
	} while ((!found) && (i<NumPtsF));
	return(x);
}






/*
long double fasesF(long double x,long double *param)

////////////////////////////////////////////////////////////////////////////////////////////////////

// Funcion bimodal y continua

////////////////////////////////////////////////////////////////////////////////////////////////////

{

	long double aux,ret = modfl(F(x,param),&aux);

	return(ret);

}



long double F(long double x,long double *param)

////////////////////////////////////////////////////////////////////////////////////////////////////

// Funcion bimodal y continua (es la que introduce el usuario)

////////////////////////////////////////////////////////////////////////////////////////////////////

{

	long double ret = x + param[0] + param[1]*sinl(2.0*M_PI*x);

	return(ret);

}



long double F(long double x,long double *param)

////////////////////////////////////////////////////////////////////////////////////////////////////

// Otra funcion bimodal y continua (junto con fTriangular es la funcion triangular en el plano)

////////////////////////////////////////////////////////////////////////////////////////////////////

{

	long double ret,aux,aux2;

	aux2 = modfl(x,&aux);

	ret = fTriangular(aux2,param) + aux;

	return(ret);

}



long double fTriangular(long double x,long double *param)

////////////////////////////////////////////////////////////////////////////////////////////////////

// Funcion bimodal, continua y triangular definida solo en el intervalo [0,1]

////////////////////////////////////////////////////////////////////////////////////////////////////

{

	long double temp;

	if ((x>=0.0) && (x<1.0/3.0)) temp = 2.0*x;

	else if ((x>=1.0/3.0) && (x<2.0/3.0)) temp = -x + 1.0;

	else if ((x>=2.0/3.0) && (x<=1.0)) temp = 2.0*x - 1.0;

	return(temp);

}



long double F3(long double x,long double *param)

////////////////////////////////////////////////////////////////////////////////////////////////////

// Una funcion que no es bimodal pero si continua (para verificar que el programa no

// calcule las lenguas de Boyland de una funcion que no es bimodal)

////////////////////////////////////////////////////////////////////////////////////////////////////

{

	long double ret = x + param[0] + param[1]*sinl(4.0*M_PI*x);

	return(ret);

}


*/

#endif
