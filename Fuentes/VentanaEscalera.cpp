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

#include <vcl.h>
#include <stdio.h>
#include <time.h>
#include <Printers.hpp>
#include "Acecade.h"
#include "Ayuda.h"
#include "VentanaEscalera.h"
#include "Sincronizaciones.hpp"
#include "NumeroRotacion.hpp"
#include "ExponenteLyapunov.hpp"
#include "LMensaje.h"
#include "Editor.h"
#include "VEditor.h"

#include "VentanaBifurcaciones.h"
#include "VentanaCirculo.h"
#include "VentanaToro.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"







//---------------------------------------------------------------------------
// Objetosglobales del sistema IyD
extern  Sincronizaciones       sincro;
extern  NumeroRotacion         num_rot;
extern  ExponenteLyapunov      exp_lyap;

// Variables globales de control de escenarios
extern  bool                   Limpiar_ventana;
extern  void                  *Ventana_activa;
extern  bool                   Calcular_sincronizaciones;
extern  bool                   Calcular_numero_rotacion;
extern  bool                   Calcular_exponente_lyapunov;  
extern  bool                   Configura_escenario;
extern  bool                   Calcula_escenario;
extern  bool                   Detener_Calculo;
extern  char                  *COMPANIA;
extern  char                  *E_MAIL;
extern  bool                   Zoom_manteniendo_aspecto;

extern const char *VL_TXT01   = "Arrastre el mouse para seleccionar el área";
extern TColor Colores[] = {
   clWhite,
   clMaroon,
   clGreen,
   clOlive,
   clNavy,
   clPurple,
   clTeal,
   clRed,
   clLime,
   clYellow,
   clBlue,
   clFuchsia,
   clAqua,
   clGray,
   clSilver,
   clBlack
};

// Valores Globales para el COPY and PAST
extern long double PARAMETROS[NUM_MAX_PARAMETROS];


// Constructor de la clase
__fastcall TFormaEscalera::TFormaEscalera(TComponent* Owner) : TForm(Owner)
{
   // Inicializa el indicador de recuadro
   Sw_Dibuja_rectangulo = false;
   Ventana_seleccionada = false;
   Sw_Recuadro_activo = true;

   Detener_calculo = false;
   Sw_cerrar_ventana = false;
   // Dimenciones de la ventana
   Dim_Vtn.Xi = 0.0;
   Dim_Vtn.Xf = 1.0;
   Dim_Vtn.Yi = 0.0;
   Dim_Vtn.Yf = 1.0;
   // Longitud de la ventana de pixeles
   Pix_x = 1024;
   Pix_y = 1024;
   // Inicializa la ventana de pixeles
   pix[0].Inicializa(Pix_x, Pix_y, false,Dim_Vtn);
   
   Color_fondo = 0xE2E2E2;
   // Tipo de inegración en este escenario
   Tipo_integracion = ADELANTE;
   Ptos_X = VTEscalera->Width;  // Puntos de la maya
   Primer_Eje = 0;              // Paramatro del primer eje
   Numero_iteraciones = 10000;  // Numero de iteraciones 
   Puntos_ventana = 2000;       // Puntos en la ventana
}


// Al cerrar la forma ...
void __fastcall TFormaEscalera::FormClose(TObject *Sender,TCloseAction &Action)
{
//   if(Tongues_P) delete [] Tongues_P;
//   if(Tongues_Q) delete [] Tongues_Q;
   Ventana_activa = NULL;
}

// Al solicitar cerrar la ventana ...
void __fastcall TFormaEscalera::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   CanClose = false;
   if (MessageBox(Handle,"Do you wish to close this window?","Toungues scenery",MB_YESNO + MB_ICONQUESTION)  == IDYES) Sw_cerrar_ventana = true;
}

// Al cambiar de tamaño ...
void __fastcall TFormaEscalera::FormResize(TObject *Sender)
{
//   Limpia_matriz_pixeles();
   // Calcula la escala de la ventana de visualizacion
   Escala.X = (VTEscalera->Width -1) / (long double) (Dim_Vtn.Xf - Dim_Vtn.Xi);
   Escala.Y = (VTEscalera->Height -1) / (long double) (Dim_Vtn.Yf - Dim_Vtn.Yi);
   // Grafica el contenido de la ventana
   Grafica(true);        
}     


///////////////////////////////////////////////////////////////////////////////
//  Definición de Comportamientos del Menu 
///////////////////////////////////////////////////////////////////////////////

// Menu Archivo->Cerrar
void __fastcall TFormaEscalera::MenuArchivoCerrarClick(TObject *Sender)
{
   Close();        
}


// Menu->Archivo->Grabar BMP
void __fastcall TFormaEscalera::MenuArchivoGrabarBMPClick(TObject *Sender)
{
   TRect   xRect = Rect(0,0,VTEscalera->Width,VTEscalera->Height);
   Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
   Bitmap->Width = VTEscalera->Width;
   Bitmap->Height = VTEscalera->Height;
   Bitmap->Canvas->CopyRect(xRect, VTEscalera->Canvas, xRect);
   SaveDialog->Title = "Save as ...";
   if (SaveDialog->Execute()) {
      Bitmap->SaveToFile(SaveDialog->FileName);           
   }
   VTEscalera->Canvas->Draw(0,0,Bitmap);
   delete Bitmap;
}

// Menu->Archivo->Imprimir
void __fastcall TFormaEscalera::MenuArchivoImprimirClick(TObject *Sender)
{
   TRect   xRect = Rect(0,0,VTEscalera->ClientWidth,VTEscalera->ClientHeight);
   Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
   Bitmap->Width = VTEscalera->ClientWidth;
   Bitmap->Height = VTEscalera->ClientHeight;
   Bitmap->Canvas->CopyRect(xRect, VTEscalera->Canvas, xRect);
   Bitmap->SaveToFile("Prn_img.bmp");
   delete Bitmap;
   Bitmap = new Graphics::TBitmap;
   Bitmap->LoadFromFile("Prn_img.bmp");
   Printer()->BeginDoc();
   Printer()->Canvas->TextOut(100,10,Application->Title);
   Printer()->Canvas->TextOut(100,100,Caption);
   Printer()->Canvas->StretchDraw(Rect(100,300,Printer()->PageWidth-100,Printer()->PageWidth-200),Bitmap);
   Printer()->Canvas->TextOut(100,(Printer()->PageWidth - 100),COMPANIA);
   Printer()->Canvas->TextOut(100,(Printer()->PageWidth - 10),E_MAIL);
   Printer()->EndDoc();
   delete Bitmap;
   unlink("Prn_img.bmp");
}


// Menu->Editar->Copiar parámetros
void __fastcall TFormaEscalera::MenuEdicionCopiarClick(TObject *Sender)
{
   fun_disp.RetornaParametros(PARAMETROS);   
}


// Menu->Editar->Pegar parámetros
void __fastcall TFormaEscalera::MenuEdicionPegarClick(TObject *Sender)
{
   fun_disp.AsignaParametros(PARAMETROS);   
}




// Menu->Calcular->Calcular Escalera
void __fastcall TFormaEscalera::MenuCalcularLenguasClick(TObject *Sender)
{
   Sw_Proceso_calculo = true;
   Sw_cerrar_ventana = false; 
   Detener_calculo = false;

   // Cambia el cursor del mouse
   TCursor antcursor = Cursor;
   Cursor = crHourGlass;


   long double Incx, x;
   long double aux = 0.0, Temp1, incX, condini;  
   int N, px , py;
   int                    Error_calculo;
   int                    Rot_Num_iteraciones;
   long double            Rot_Condicion_Inicial;

    
   // Valores por omision para el calculo del rotacional
   Error_calculo                 = 0;
   Rot_Condicion_Inicial         = random(10)/10.0;
   Rot_Num_iteraciones           = Numero_iteraciones;
   Incx = ((Dim_Vtn.Xf - Dim_Vtn.Xi) / ((long double) Puntos_ventana));  
   
   // Limpia la ventana de trabajo
   MenuVentanaLimpiarClick(this);
   for (x = Dim_Vtn.Xi; x <= Dim_Vtn.Xf; x += Incx ) {
      Application->ProcessMessages();
      if (Sw_cerrar_ventana || Detener_calculo) break;
      sprintf(xcad,"Calculating Devil's Staircase of the parameter %s = %2.8Lf (this process may last several minutes).", fun_disp.Nombre_parametros[Primer_Eje], x);
      BarraDeEstadosEscalera->SimpleText=(AnsiString) xcad;
      // Fija los parametros al sistema
      fun_disp.RetornaParametros(parm);   
      parm[Primer_Eje] = x;
      fun_disp.AsignaParametros(parm); 
        
      Temp1 = Rot_Condicion_Inicial;
      N = 0;
      while ((N < Rot_Num_iteraciones) && !Error_calculo) {
          Temp1 = fun_disp.Calcula(Temp1,Tipo_integracion) + aux;
          Temp1 = modfl(Temp1,&aux);
          N++;
      }
      if (!Error_calculo) {
         if (Temp1 >= 0.0) Temp1 = modfl((Temp1 + aux) / (long double) Rot_Num_iteraciones,&aux);
          else Temp1 = modfl(((Temp1 + aux) /(long double) Rot_Num_iteraciones),&aux) + 1.0;
      } 
      // Visualiza la sincronizacion encontrada
      pix[0].Asigna_valor(x,Temp1,true);
      px = (x - Dim_Vtn.Xi) * Escala.X;
      py = VTEscalera->Height - (((Temp1 - Dim_Vtn.Yi) * Escala.Y) + 1.0);
      VTEscalera->Canvas->Pixels[px][py] = Colores[16];
   }
   BarraDeEstadosEscalera->SimpleText=(AnsiString) " ";
   Cursor = antcursor;
   Sw_Proceso_calculo = false;
}

// Menu->Calcular->Numero de Rotación
void __fastcall TFormaEscalera::MenuCalcularNumeroRotacionClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double p[NUM_MAX_PARAMETROS];
   BarraDeEstadosEscalera->SimpleText = (AnsiString) "Calculating rotation number (this process may last several seconds).";
   // Recupera los parámetros por omisión
   fun_disp.RetornaParametros(p);
   // Cambiar parámetros
   num_rot.AsignaParametros(p);
   num_rot.Calcula(Tipo_integracion);
   TLMensajeForm *Ayuda = new TLMensajeForm(this);
   if(!Ayuda) return;
   Ayuda->Abrir_archivo("Rotational","C:\\REPTMP.TMP");
   Ayuda->ShowModal();

   Cursor = antcursor;
   BarraDeEstadosEscalera->SimpleText = (AnsiString) " ";
}

// Menu->Calcular->Sincronización
void __fastcall TFormaEscalera::MenuCalcularSincronizacinClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double p[NUM_MAX_PARAMETROS];
   BarraDeEstadosEscalera->SimpleText = (AnsiString) "Calculating synchronization (this process may last several seconds).";
   // Recupera los parámetros por omisión
   fun_disp.RetornaParametros(p);
   // Cambiar parámetros
   sincro.AsignaParametros(p);
   if (sincro.Calcula(Tipo_integracion)) {
      TLMensajeForm *Ayuda = new TLMensajeForm(this);
      if(!Ayuda) return;
      Ayuda->Abrir_archivo("Synchronization","C:\\REPTMP.TMP");
      Ayuda->ShowModal();
   } else ShowMessage("No se encontro sincronizaciones");
   Cursor = antcursor;
   BarraDeEstadosEscalera->SimpleText = (AnsiString) " ";
}

// Menu->Calcular->Exponente de Lyapunov
void __fastcall TFormaEscalera::MenuCalcularExpLyapunovClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   // Cambia el cursor del mouse
   Cursor = antcursor;
   long double p[NUM_MAX_PARAMETROS], rot;
   BarraDeEstadosEscalera->SimpleText = (AnsiString) "Calculating Lyapunov exponent (this process may last several seconds).";
   // Recupera los parámetros por omisión
   fun_disp.RetornaParametros(p);
   // Cambiar parámetros
   exp_lyap.AsignaParametros(p);
   rot = exp_lyap.Calcula(Tipo_integracion);
   char xcad[200];
   sprintf(xcad,"Lyapunov exponent: %Lf",rot);
   MessageBox(NULL,xcad,"Tongues scenery",MB_OK+MB_ICONINFORMATION);
   BarraDeEstadosEscalera->SimpleText = (AnsiString) " ";
   // Cambia el cursor del mouse
   Cursor = antcursor;
}

// Menu->ventana->limpiar
void __fastcall TFormaEscalera::MenuVentanaLimpiarClick(TObject *Sender)
{
   Limpia_matriz_pixeles();
   Grafica(true);
}



// Menu->Ventana->Zoom Out
void __fastcall TFormaEscalera::MenuVentanaZoomOutClick(TObject *Sender)
{
   // Almacena las dimensiones actuales
   if (Ind_zoom > 0) {
      Ind_zoom --;
      Dim_Vtn = Dim_zoom[Ind_zoom];
      pix[0].Cambia_dimension(Dim_Vtn, false);
      FormResize(this);
      MenuCalcularLenguasClick(this);
      if (Ind_zoom < 1) MenuVentanaZoomOut->Enabled = false;
   }
}

// Menu->Ventana->Dimensiones originales
void __fastcall TFormaEscalera::MenuVentanaOriginalDimentionsClick(TObject *Sender)
{
   Dim_Vtn = Dim_orig;
   Ind_zoom = 0;
   MenuVentanaZoomOut->Enabled = false;
   // Actualiza las dimensiones de las ventanas de trabajo
   pix[0].Cambia_dimension(Dim_Vtn, false);
   FormResize(this);
}


// Menu->Configurar->Parametros
void __fastcall TFormaEscalera::MenuConfigurarParametrosClick(TObject *Sender)
{
   configura = new TVCap_Escalera(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 0;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}

// Menu->Configurar->Dimensiones
void __fastcall TFormaEscalera::MenuConfigurarDimensionesClick(TObject *Sender)
{
   configura = new TVCap_Escalera(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 1;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}

// Menu->Configurar->Lenguas
void __fastcall TFormaEscalera::Tongues1Click(TObject *Sender)
{
   configura = new TVCap_Escalera(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 2;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}


// Menu->Configurar->Calculo
void __fastcall TFormaEscalera::MenuConfigurarCalculoClick(TObject *Sender)
{
   configura = new TVCap_Escalera(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 3;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}


// Acerca de ...
void __fastcall TFormaEscalera::MenuAyudaAcercadeClick(TObject *Sender)
{
   TVAcercaDe *Acercade = new TVAcercaDe(this);
   if (Acercade) {
      Acercade->ShowModal();
      delete Acercade;
   }
}

// Ayuda de TFormaLevantamientos
void __fastcall TFormaEscalera::MenuAyudaLevantamientosClick(TObject *Sender)
{
   TAyudaForm *Ayuda = new TAyudaForm(this);
   if (Ayuda) {
      Ayuda->Abrir_archivo("Tongues scenery","Tongues.hlp");
      Ayuda->Show();
   }
}



///////////////////////////////////////////////////////////////////////////////
//  Definición de Comportamientos de Lenguas
///////////////////////////////////////////////////////////////////////////////


// Grafica el mapa de pixeles
void TFormaEscalera::Grafica(bool limpia)
{
   if (Sw_cerrar_ventana) return;
   if (!pix[0].Ventana_activa()) return;

   unsigned int x, y, i;
   // Calcula la escala de la ventana de visualizacion con respecto a la ventana de pixeles
   C_2D esc;

   if (limpia) Limpiar_vantana();
   esc.X = VTEscalera->Width / (long double) Pix_x;
   esc.Y = VTEscalera->Height / (long double) Pix_y;
   // Visualiza la ventana de pixeles
   for(y = 0; y < Pix_y; y++) {
       for(x = 0; x < Pix_x; x++) {
           if(pix[0].Retorna_valor(x, y)) VTEscalera->Canvas->Pixels[x * esc.X][y * esc.Y] = Colores[16];
       }
   }
}

// Limpia la ventana de graficacion
void TFormaEscalera::Limpiar_vantana(void)
{
   VTEscalera->Canvas->Pen->Color = Color_fondo;
   VTEscalera->Canvas->Brush->Color = Color_fondo;
   VTEscalera->Canvas->Rectangle(0, 0, VTEscalera->Width, VTEscalera->Height);
}


///////////////////////////////////////////////////////////////////////////////
// Controla el movimiento del mouse y genereación del recuadro para el zoom del
// la ventana de Lenguas
///////////////////////////////////////////////////////////////////////////////
// Cuando el mouse es movido
void __fastcall TFormaEscalera::VTEscaleraMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
   if (Sw_Recuadro_activo) {
      if (Sw_Dibuja_rectangulo) {
         TPenMode mode;

         mode = VTEscalera->Canvas->Pen->Mode;
         VTEscalera->Canvas->Pen->Mode = pmNot;
      
         if (Sw_Dibuja_rectangulo_ant) {
            VTEscalera->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
            VTEscalera->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
            VTEscalera->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
            VTEscalera->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
            VTEscalera->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
            VTEscalera->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
            VTEscalera->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
            VTEscalera->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
         }
         VTEscalera->Canvas->MoveTo(Vt1.Xi,Vt1.Yi);
         VTEscalera->Canvas->LineTo(X,Vt1.Yi);
         VTEscalera->Canvas->MoveTo(Vt1.Xi,Vt1.Yi);
         VTEscalera->Canvas->LineTo(Vt1.Xi,Y);
         VTEscalera->Canvas->MoveTo(X,Y);
         VTEscalera->Canvas->LineTo(Vt1.Xi,Y);
         VTEscalera->Canvas->MoveTo(X,Y);
         VTEscalera->Canvas->LineTo(X,Vt1.Yi);
      
         Vt2.Xf = X;
         Vt2.Yf = Y;
         VTEscalera->Canvas->Pen->Mode = mode;
         Sw_Dibuja_rectangulo_ant = true;

         // Ajusta a que xVt tenga el inicio y el final de la ventana
         xVt.Xi = Vt1.Xi, xVt.Xf = Vt2.Xf, xVt.Yi = Vt1.Yi, xVt.Yf = Vt2.Yf;
         int  xtmp;
         if (xVt.Xi > xVt.Xf) xtmp = xVt.Xi, xVt.Xi = xVt.Xf, xVt.Xf = xtmp;
         if (xVt.Yi > xVt.Yf) xtmp = xVt.Yi, xVt.Yi = xVt.Yf, xVt.Yf = xtmp;
         // Ajusta los valores a la longitud de la ventana
         if (xVt.Xi < 0) xVt.Xi = 0;
         if (xVt.Yi < 0) xVt.Yi = 0;
         if (xVt.Xf > VTEscalera->Width) xVt.Xf = VTEscalera->Width;
         if (xVt.Yf > VTEscalera->Height) xVt.Yf = VTEscalera->Height;
         // Visualiza los valores de la region seleccionada segun la dimención de la ventana
         Vs.Xi = xVt.Xi / Escala.X + Dim_Vtn.Xi;
         Vs.Yi = (-(xVt.Yi - VTEscalera->Height) / Escala.Y) + Dim_Vtn.Yi;
         Vs.Xf = xVt.Xf / Escala.X + Dim_Vtn.Xi;
         Vs.Yf = (-(xVt.Yf - VTEscalera->Height) / Escala.Y) + Dim_Vtn.Yi;
         sprintf(xcad,"%s: (%3.5Lf, %3.5Lf, %3.5Lf, %3.5Lf)",VL_TXT01,Vs.Xi,Vs.Yf,Vs.Xf,Vs.Yi);
         BarraDeEstadosEscalera->SimpleText = (AnsiString) xcad;
      } else {
         Vs.Xf = X, Vs.Yf = Y;
         Vs.Xi = Vs.Xf / Escala.X + Dim_Vtn.Xi;
         Vs.Yi = (-(Vs.Yf - VTEscalera->Height) / Escala.Y) + Dim_Vtn.Yi;

//         sprintf(Msg,"Mouse position: (%3.8Lf, %3.8Lf)", Vs.Xi, Vs.Yi);
         sprintf(Msg,"%s=%3.12Lf, %3.12Lf", fun_disp.Nombre_parametros[Primer_Eje], Vs.Xi, Vs.Yi);
         BarraDeEstadosEscalera->SimpleText = (AnsiString) Msg;
      }    
   }
}


// Cuando el mouse es presionado
void __fastcall TFormaEscalera::VTEscaleraMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Sw_Recuadro_activo) {
      if (Button == mbLeft) {
         Vt2.Xf = Vt1.Xf = Vt2.Xi = Vt1.Xi = X;
         Vt2.Yf = Vt1.Yf = Vt2.Yi = Vt1.Yi = Y;
         Sw_Dibuja_rectangulo = true;
         Sw_Dibuja_rectangulo_ant = false;
         Ventana_seleccionada = false;
      }
   }
}

// Cuando el mouse es soltado
void __fastcall TFormaEscalera::VTEscaleraMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Sw_Recuadro_activo) {
      if (Button == mbLeft) {
         TPenMode mode;
         mode = VTEscalera->Canvas->Pen->Mode;
         VTEscalera->Canvas->Pen->Mode = pmNot;
         VTEscalera->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
         VTEscalera->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
         VTEscalera->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
         VTEscalera->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
         VTEscalera->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
         VTEscalera->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
         VTEscalera->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
         VTEscalera->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
         VTEscalera->Canvas->Pen->Mode = mode;
         Vt1.Xf = X;
         Vt1.Xf = Y;
         Sw_Dibuja_rectangulo = false;
         Sw_Dibuja_rectangulo_ant = false;
         BarraDeEstadosEscalera->SimpleText = (AnsiString) " ";
         Ventana_seleccionada = true;
         // Revisa si realmente se solicito el zoom
         if (abs(Vt2.Xf - Vt2.Xi) > 5 && abs(Vt2.Yf - Vt2.Yi) > 5) {
            // Pregunta si se desea hacer el zoom
            if (MessageBox(Handle,"Do you wish to zoom?","Bifurcations scenery",MB_YESNO + MB_ICONQUESTION)  == IDYES) {
               // Almacena las dimensiones actuales
               Dim_zoom[Ind_zoom] = Dim_Vtn;
               if ((Ind_zoom + 1) < NUM_MAX_DIM_ZOOM) Ind_zoom ++;
               MenuVentanaZoomOut->Enabled = true;
               long double incx = fabsl(Vs.Xf - Vs.Xi);
               long double incy = fabsl(Vs.Yf - Vs.Yi);
               long double inc  = (incx > incy ? incx : incy);
               // Actualiza las dimensiones de las ventanas de trabajo
               Dim_Vtn.Xi = Vs.Xi;
               Dim_Vtn.Yi = Vs.Yf;
               
               if (Zoom_manteniendo_aspecto) {
                  Dim_Vtn.Xf = Vs.Xi + inc;
                  Dim_Vtn.Yf = Vs.Yf + inc;
                } else {
                  Dim_Vtn.Xf = Vs.Xf;
                  Dim_Vtn.Yf = Vs.Yi;
               }
               pix[0].Cambia_dimension(Dim_Vtn, false);
               FormResize(this);
               MenuCalcularLenguasClick(this);
            } else {
               if (MessageBox(Handle,"Do you wish to calculate only this region?","Bifurcations scenery",MB_YESNO + MB_ICONQUESTION)  == IDYES) {
                  MenuCalcularLenguasClick(this);
               }
            }
         }
      }
   }
}

// Pasa los valores de configuración de la ventana de captura
void TFormaEscalera::PasarValoresConfiguracion(void)
{
   ////////////////////////////////////////////////////////
   // Parámetros
   ////////////////////////////////////////////////////////
   // Parámetros
   configura->ListBoxParametros->Items->Clear();
   for( int i = 0; i < fun_disp.Numero_parametros; i++) {
      sprintf(xcad,"%-6s = %1.9Lf",fun_disp.Nombre_parametros[i],fun_disp.P[i] );
      configura->ListBoxParametros->Items->Add(xcad);
   }
   ////////////////////////////////////////////////////////
   // Dimensiones
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%Lf",Dim_Vtn.Xi);
   configura->EditHMin->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",Dim_Vtn.Xf);
   configura->EditHMax->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",Dim_Vtn.Yi);
   configura->EditVMin->Text = (AnsiString) xcad;
   sprintf(xcad,"%Lf",Dim_Vtn.Yf);
   configura->EditVMax->Text = (AnsiString) xcad;
   ////////////////////////////////////////////////////////
   // Lenguas
   ////////////////////////////////////////////////////////
   configura->ComboBox1->Items->Clear();
   for( int i = 0; i < fun_disp.Numero_parametros_bifurcar; i++) {
      configura->ComboBox1->Items->Add(fun_disp.Nombre_parametros[i]);
   }
   configura->ComboBox1->ItemIndex = Primer_Eje;
   ////////////////////////////////////////////////////////
   // Calcular
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%u",Numero_iteraciones);
   configura->EditA1->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",Puntos_ventana);
   configura->EditA2->Text = (AnsiString) xcad;
}

// Retorna los valores de configuración de la ventana de captura
void TFormaEscalera::RetornarValoresConfiguracion(void)
{
   if (!configura->Aceptar) return;
   ////////////////////////////////////////////////////////
   // Parámetros
   ////////////////////////////////////////////////////////
   for (int i = 0; i < fun_disp.Numero_parametros; i++) {
       unsigned int i1, i2;
       char xcad[100], xcad1[100];
       strcpy(xcad1,configura->ListBoxParametros->Items->Strings[i].c_str());
       // Valor del parámetro
       for (i2 = 0, i1 = 9; i1 < strlen(xcad1); i2++, i1++) xcad[i2] = xcad1[i1];
       xcad[i2] = 0;
       fun_disp.P[i] = _atold(xcad);
   }
   ////////////////////////////////////////////////////////
   // Dimensiones
   ////////////////////////////////////////////////////////
   Dim_Vtn.Xi = _atold(configura->EditHMin->Text.c_str());       
   Dim_Vtn.Xf = _atold(configura->EditHMax->Text.c_str());       
   Dim_Vtn.Yi = _atold(configura->EditVMin->Text.c_str());       
   Dim_Vtn.Yf = _atold(configura->EditVMax->Text.c_str());       
   ////////////////////////////////////////////////////////
   // Lenguas
   ////////////////////////////////////////////////////////
   Primer_Eje = configura->ComboBox1->ItemIndex;
   ////////////////////////////////////////////////////////
   // Calcular
   ////////////////////////////////////////////////////////
   Numero_iteraciones = atoi(configura->EditA1->Text.c_str());
   Puntos_ventana = atoi(configura->EditA2->Text.c_str());
   /////////////////////////////////////////////////////////
   // Al terminar
   /////////////////////////////////////////////////////////
   pix[0].Cambia_dimension(Dim_Vtn, false);
   FormResize(this);
}

void __fastcall TFormaEscalera::Timer1Timer(TObject *Sender)
{
   // Almacena la ventana activa
   if (Focused()) {
      Ventana_activa = this;
   }   
   // Revisa si es la ventana activa 
   if (Ventana_activa == this) {
      // Calcula el número de rotacion
      if (Calcular_numero_rotacion) {
          Calcular_numero_rotacion = false;
          MenuCalcularNumeroRotacionClick(this);
      }
      // Calcula la sincronización
      if (Calcular_sincronizaciones) {
          Calcular_sincronizaciones = false;
          MenuCalcularSincronizacinClick(this);
      }
      // Calcula el exponente de Luapunov
      if (Calcular_exponente_lyapunov) {
          Calcular_exponente_lyapunov = false;
          MenuCalcularExpLyapunovClick(this);
      }
      // Configura el escenario
      if (Configura_escenario) {
          Configura_escenario = false;
          MenuConfigurarParametrosClick(this);
      }
      // Calcula
      if (Calcula_escenario) {
          Calcula_escenario = false;
          MenuCalcularLenguasClick(this);
      }
      // Detener cálculo
      if (Detener_Calculo) {
          Detener_Calculo = false;
          MenuCalcularDetenerelcalculoClick(this);
      }
      // Limpia la ventana
      if (Limpiar_ventana) {
          Limpiar_ventana = false;
          MenuVentanaLimpiarClick(this);
      }
   }
   // Controla que no este calculando para cerrar la ventana
   if (Sw_cerrar_ventana) MenuCalcularDetenerelcalculoClick(this);
   if (Sw_cerrar_ventana && !Sw_Proceso_calculo) {
      Ventana_activa = NULL;
      Free();
   }
}

void __fastcall TFormaEscalera::MenuCalcularDetenerelcalculoClick(TObject *Sender)
{
   Detener_calculo = true;        
}

// Grafica el contenido de la ventana
void __fastcall TFormaEscalera::VTEscaleraPaint(TObject *Sender)
{
   Grafica(true);        
}

// OnHint
void  __fastcall TFormaEscalera::OnHint(TObject *Sender)
{
   BarraDeEstadosEscalera->SimpleText = Application->Hint;
}

// Al activar la forma
void __fastcall TFormaEscalera::FormActivate(TObject *Sender)
{
   Application->OnHint = OnHint;
}

// Al desactivar la forma
void __fastcall TFormaEscalera::FormDeactivate(TObject *Sender)
{
   Application->OnHint = NULL;
}



void __fastcall TFormaEscalera::Copyparameters1Click(TObject *Sender)
{
   long double parm[NUM_MAX_PARAMETROS];
   fun_disp.RetornaParametros(parm);   
   parm[Primer_Eje] = Vs.Xi;
   fun_disp.AsignaParametros(parm);   
}
//---------------------------------------------------------------------------

void __fastcall TFormaEscalera::ShowSincronization1Click(TObject *Sender)
{
   long double parm[NUM_MAX_PARAMETROS];
   fun_disp.RetornaParametros(parm);   
   parm[Primer_Eje] = Vs.Xi;
   fun_disp.AsignaParametros(parm);   
   MenuCalcularSincronizacinClick(this);
}


void __fastcall TFormaEscalera::ShowRotational1Click(TObject *Sender)
{
   long double parm[NUM_MAX_PARAMETROS];
   fun_disp.RetornaParametros(parm);   
   parm[Primer_Eje] = Vs.Xi;
   fun_disp.AsignaParametros(parm);   
   MenuCalcularNumeroRotacionClick(this);
}




// Lanza una ventana del escenario Círculo
void __fastcall TFormaEscalera::Torus1Click(TObject *Sender)
{
   TFormaToro *vent_tor = new TFormaToro(this);
   if(vent_tor) {
      for( int i = 0; i < fun_disp.Numero_parametros; i++) vent_tor->fun_disp.P[i] = fun_disp.P[i];
      vent_tor->fun_disp.P[Primer_Eje] = Vs.Xi;
      vent_tor->Show();
   }
}

// Lanza una ventana del escenario Círculo
void __fastcall TFormaEscalera::Circle1Click(TObject *Sender)
{
   TFormaCirculo *vent_cir = new TFormaCirculo(this);
   if(vent_cir) {
      for( int i = 0; i < fun_disp.Numero_parametros; i++) vent_cir->fun_disp.P[i] = fun_disp.P[i];
      vent_cir->fun_disp.P[Primer_Eje] = Vs.Xi;
      vent_cir->Show();
   }
}

// Lanza una ventana del escenario Bifurcacion
void __fastcall TFormaEscalera::Bifurcations1Click(TObject *Sender)
{
   TFormaBifurcaciones *vent_bif = new TFormaBifurcaciones(this);
   if(vent_bif) {
      for( int i = 0; i < fun_disp.Numero_parametros; i++) vent_bif->fun_disp.P[i] = fun_disp.P[i];
      vent_bif->fun_disp.P[Primer_Eje] = Vs.Xi;
      vent_bif->Show();
   }
}



