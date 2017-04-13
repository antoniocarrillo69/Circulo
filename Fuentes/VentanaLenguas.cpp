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
#include "VentanaLenguas.h"
#include "Sincronizaciones.hpp"
#include "NumeroRotacion.hpp"
#include "ExponenteLyapunov.hpp"
#include "LMensaje.h"
#include "Editor.h"
#include "VEditor.h"

#include "VentanaBifurcaciones.h"
#include "VentanaCirculo.h"
#include "VentanaToro.h"
#include "VentanaEscalera.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"



//#define MAYA_CON_ALEATORIOS 



#define FIN 9999.0 //Determina el final del archivo de almacenamiento
#define CLAVE_ENCABEZADO "ARNOLD16"  // ARNOLD16
//                                            ^^
//                                            ||__ Parámetros
//                                            |___ Versión

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

const char *VL_TXT01   = "Arrastre el mouse para seleccionar el área";
TColor Colores[] = {
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
__fastcall TFormaLenguas::TFormaLenguas(TComponent* Owner) : TForm(Owner)
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
   for (int i = 0; i < 16; i++) pix[i].Inicializa(Pix_x, Pix_y, false,Dim_Vtn);
   
//   Color_fondo = clSilver;
   Color_fondo = 0xE2E2E2;
   // Tipo de inegración en este escenario
   Tipo_integracion = ADELANTE;
   // Inicialización de valores de languas
   Tipo_region_calcular = 1;
///////////////////////////////////////////////////////////////////////////////////
// Calculo de Lenguas   
   // Variables que pueden ser configurables
   Tolerancia = 1e-5;          // Tolerancia en los calculos de sincronizaciones
   Tolerancia_maxima = 1e-3;   // Tolerancia para descartar en los cálculos de sincronizaciones
   ItsTrans = 100;             // Cantidad de Iteraciones mínimas
   Num_max_trans = 1000;      // Cantidad Maxima de Iteraciones del Periodo Transitorio
   SizeOrbits = 10;            // Tamaño máximo de orbitas a detectar
   NCondInits = 10;            // Cantidad de conds. iniciales aleatorias elegidas para cada conj. de param. determin.
   Ptos_X = VTLenguas->Width;  // Puntos de la maya
   Ptos_Y = VTLenguas->Height; // Puntos de la maya
   Primer_Eje = 0;             // Paramatro del primer eje
   Segundo_Eje = 1;            // Paramatro del segundo eje
   // Cota superior de la región de difeomorfismos en lenguas
   Cota_superior_region_difeomorfismos = (1.0 / (2.0 * M_PI));
   // Factor usado en la zona de difeomorfismos
   Factor_usado_zona_difeomorfismos = 50;
   // Indica el máximo de multiestabilidad buscado al calcular lenguas
   Maxima_estabilidad_buscada_al_calculo_lenguas = 2;
///////////////////////////////////////////////////////////////////////////////////   
   // Indica la máxima multiestabilidad encontrada en la lectura de un archivo de lenguas
   Indice_multiestabilidad = 0;
///////////////////////////////////////////////////////////////////////////   
   Optimiza_ptos_calcular = false;
}


// Al cerrar la forma ...
void __fastcall TFormaLenguas::FormClose(TObject *Sender,TCloseAction &Action)
{
   if(Tongues_P) delete [] Tongues_P;
   if(Tongues_Q) delete [] Tongues_Q;
   Ventana_activa = NULL;
}

// Al solicitar cerrar la ventana ...
void __fastcall TFormaLenguas::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   CanClose = false;
   if (MessageBox(Handle,"Do you wish to close this window?","Toungues scenery",MB_YESNO + MB_ICONQUESTION)  == IDYES) Sw_cerrar_ventana = true;
}

// Al cambiar de tamaño ...
void __fastcall TFormaLenguas::FormResize(TObject *Sender)
{
//   Limpia_matriz_pixeles();
   // Calcula la escala de la ventana de visualizacion
   Escala.X = (VTLenguas->Width -1) / (long double) (Dim_Vtn.Xf - Dim_Vtn.Xi);
   Escala.Y = (VTLenguas->Height -1) / (long double) (Dim_Vtn.Yf - Dim_Vtn.Yi);
   // Grafica el contenido de la ventana
   Grafica(true);        
}     


///////////////////////////////////////////////////////////////////////////////
//  Definición de Comportamientos del Menu 
///////////////////////////////////////////////////////////////////////////////

// Menu Archivo->Cerrar
void __fastcall TFormaLenguas::MenuArchivoCerrarClick(TObject *Sender)
{
   Close();        
}


// Menu->Archivo->Grabar BMP
void __fastcall TFormaLenguas::MenuArchivoGrabarBMPClick(TObject *Sender)
{
   TRect   xRect = Rect(0,0,VTLenguas->Width,VTLenguas->Height);
   Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
   Bitmap->Width = VTLenguas->Width;
   Bitmap->Height = VTLenguas->Height;
   Bitmap->Canvas->CopyRect(xRect, VTLenguas->Canvas, xRect);
   SaveDialog->Title = "Save as ...";
   if (SaveDialog->Execute()) {
      Bitmap->SaveToFile(SaveDialog->FileName);           
   }
   VTLenguas->Canvas->Draw(0,0,Bitmap);
   delete Bitmap;
}

// Menu->Archivo->Imprimir
void __fastcall TFormaLenguas::MenuArchivoImprimirClick(TObject *Sender)
{
   TRect   xRect = Rect(0,0,VTLenguas->ClientWidth,VTLenguas->ClientHeight);
   Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
   Bitmap->Width = VTLenguas->ClientWidth;
   Bitmap->Height = VTLenguas->ClientHeight;
   Bitmap->Canvas->CopyRect(xRect, VTLenguas->Canvas, xRect);
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
void __fastcall TFormaLenguas::MenuEdicionCopiarClick(TObject *Sender)
{
   fun_disp.RetornaParametros(PARAMETROS);   
}


// Menu->Editar->Pegar parámetros
void __fastcall TFormaLenguas::MenuEdicionPegarClick(TObject *Sender)
{
   fun_disp.AsignaParametros(PARAMETROS);   
}




// Menu->Calcular->Calcular lenguas todas
void __fastcall TFormaLenguas::SelectTongues1Click(TObject *Sender)
{
   bool st = false;
   TVentanaCaptura *vent_cap = new TVentanaCaptura(this);
   if(vent_cap) {
      vent_cap->Titulo("Select Tongues");
      vent_cap->Archivo_a_leer("C:\\CONFIG.DAT");
      vent_cap->ShowModal();
      st = vent_cap->st;
      delete vent_cap;
   }
   if (!st) return;
   Tipo_lenguas = 1;
   Control_calculo_lenguas();
}

// Menu->Calcular->Calcular lenguas seleccionadas
void __fastcall TFormaLenguas::MenuCalcularLenguasClick(TObject *Sender)
{
   Tipo_lenguas = 2;
   Control_calculo_lenguas();
}

// Menu->Calcular->Numero de Rotación
void __fastcall TFormaLenguas::MenuCalcularNumeroRotacionClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double p[NUM_MAX_PARAMETROS];
   BarraDeEstadosLenguas->SimpleText = (AnsiString) "Calculating rotation number (this process may last several seconds).";
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
   BarraDeEstadosLenguas->SimpleText = (AnsiString) " ";
}

// Menu->Calcular->Sincronización
void __fastcall TFormaLenguas::MenuCalcularSincronizacinClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   long double p[NUM_MAX_PARAMETROS];
   BarraDeEstadosLenguas->SimpleText = (AnsiString) "Calculating synchronization (this process may last several seconds).";
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
   BarraDeEstadosLenguas->SimpleText = (AnsiString) " ";
}

// Menu->Calcular->Exponente de Lyapunov
void __fastcall TFormaLenguas::MenuCalcularExpLyapunovClick(TObject *Sender)
{
   TCursor antcursor = Cursor;
   // Cambia el cursor del mouse
   Cursor = crHourGlass;
   // Cambia el cursor del mouse
   Cursor = antcursor;
   long double p[NUM_MAX_PARAMETROS], rot;
   BarraDeEstadosLenguas->SimpleText = (AnsiString) "Calculating Lyapunov exponent (this process may last several seconds).";
   // Recupera los parámetros por omisión
   fun_disp.RetornaParametros(p);
   // Cambiar parámetros
   exp_lyap.AsignaParametros(p);
   rot = exp_lyap.Calcula(Tipo_integracion);
   char xcad[200];
   sprintf(xcad,"Lyapunov exponent: %Lf",rot);
   MessageBox(NULL,xcad,"Tongues scenery",MB_OK+MB_ICONINFORMATION);
   BarraDeEstadosLenguas->SimpleText = (AnsiString) " ";
   // Cambia el cursor del mouse
   Cursor = antcursor;
}

// Menu->ventana->limpiar
void __fastcall TFormaLenguas::MenuVentanaLimpiarClick(TObject *Sender)
{
   Limpia_matriz_pixeles();
   Grafica(true);
}



// Menu->Ventana->Zoom Out
void __fastcall TFormaLenguas::MenuVentanaZoomOutClick(TObject *Sender)
{
   // Almacena las dimensiones actuales
   if (Ind_zoom > 0) {
      Ind_zoom --;
      Dim_Vtn = Dim_zoom[Ind_zoom];
      for (int i = 0; i < 16; i++) pix[i].Cambia_dimension(Dim_Vtn, false);
      FormResize(this);
      Control_calculo_lenguas();
      if (Ind_zoom < 1) MenuVentanaZoomOut->Enabled = false;
   }
}

// Menu->Ventana->Dimensiones originales
void __fastcall TFormaLenguas::MenuVentanaOriginalDimentionsClick(TObject *Sender)
{
   Dim_Vtn = Dim_orig;
   Ind_zoom = 0;
   MenuVentanaZoomOut->Enabled = false;
   // Actualiza las dimensiones de las ventanas de trabajo
   for (int i = 0; i < 16; i++) pix[i].Cambia_dimension(Dim_Vtn, false);
   FormResize(this);
}


// Menu->Configurar->Parametros
void __fastcall TFormaLenguas::MenuConfigurarParametrosClick(TObject *Sender)
{
   configura = new TVCap_Lenguas(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 0;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}

// Menu->Configurar->Dimensiones
void __fastcall TFormaLenguas::MenuConfigurarDimensionesClick(TObject *Sender)
{
   configura = new TVCap_Lenguas(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 1;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}

// Menu->Configurar->Lenguas
void __fastcall TFormaLenguas::Tongues1Click(TObject *Sender)
{
   configura = new TVCap_Lenguas(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 2;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}


// Menu->Configurar->Calculo
void __fastcall TFormaLenguas::MenuConfigurarCalculoClick(TObject *Sender)
{
   configura = new TVCap_Lenguas(this);
   if (configura) {
      PasarValoresConfiguracion();
      configura->TabbedNotebook1->PageIndex = 3;
      configura->ShowModal();
      RetornarValoresConfiguracion();
      delete configura;
   }
}


// Acerca de ...
void __fastcall TFormaLenguas::MenuAyudaAcercadeClick(TObject *Sender)
{
   TVAcercaDe *Acercade = new TVAcercaDe(this);
   if (Acercade) {
      Acercade->ShowModal();
      delete Acercade;
   }
}

// Ayuda de TFormaLevantamientos
void __fastcall TFormaLenguas::MenuAyudaLevantamientosClick(TObject *Sender)
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
void TFormaLenguas::Grafica(bool limpia)
{
   if (Sw_cerrar_ventana) return;
   if (!pix[0].Ventana_activa()) return;

   unsigned int x, y, i;
   // Calcula la escala de la ventana de visualizacion con respecto a la ventana de pixeles
   C_2D esc;

   if (limpia) Limpiar_vantana();
   esc.X = VTLenguas->Width / (long double) Pix_x;
   esc.Y = VTLenguas->Height / (long double) Pix_y;
   // Visualiza la ventana de pixeles
   for(i = 0; i < 16; i++) {
      for(y = 0; y < Pix_y; y++) {
          for(x = 0; x < Pix_x; x++) {
              if(pix[i].Retorna_valor(x, y)) VTLenguas->Canvas->Pixels[x * esc.X][y * esc.Y] = Colores[i];
          }
      }
   } 
}

// Limpia la ventana de graficacion
void TFormaLenguas::Limpiar_vantana(void)
{
   VTLenguas->Canvas->Pen->Color = Color_fondo;
   VTLenguas->Canvas->Brush->Color = Color_fondo;
   VTLenguas->Canvas->Rectangle(0, 0, VTLenguas->Width, VTLenguas->Height);
}


///////////////////////////////////////////////////////////////////////////////
// Controla el movimiento del mouse y genereación del recuadro para el zoom del
// la ventana de Lenguas
///////////////////////////////////////////////////////////////////////////////
// Cuando el mouse es movido
void __fastcall TFormaLenguas::VTLenguasMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
   if (Sw_Recuadro_activo) {
      if (Sw_Dibuja_rectangulo) {
         TPenMode mode;

         mode = VTLenguas->Canvas->Pen->Mode;
         VTLenguas->Canvas->Pen->Mode = pmNot;
      
         if (Sw_Dibuja_rectangulo_ant) {
            VTLenguas->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
            VTLenguas->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
            VTLenguas->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
            VTLenguas->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
            VTLenguas->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
            VTLenguas->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
            VTLenguas->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
            VTLenguas->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
         }
         VTLenguas->Canvas->MoveTo(Vt1.Xi,Vt1.Yi);
         VTLenguas->Canvas->LineTo(X,Vt1.Yi);
         VTLenguas->Canvas->MoveTo(Vt1.Xi,Vt1.Yi);
         VTLenguas->Canvas->LineTo(Vt1.Xi,Y);
         VTLenguas->Canvas->MoveTo(X,Y);
         VTLenguas->Canvas->LineTo(Vt1.Xi,Y);
         VTLenguas->Canvas->MoveTo(X,Y);
         VTLenguas->Canvas->LineTo(X,Vt1.Yi);
      
         Vt2.Xf = X;
         Vt2.Yf = Y;
         VTLenguas->Canvas->Pen->Mode = mode;
         Sw_Dibuja_rectangulo_ant = true;

         // Ajusta a que xVt tenga el inicio y el final de la ventana
         xVt.Xi = Vt1.Xi, xVt.Xf = Vt2.Xf, xVt.Yi = Vt1.Yi, xVt.Yf = Vt2.Yf;
         int  xtmp;
         if (xVt.Xi > xVt.Xf) xtmp = xVt.Xi, xVt.Xi = xVt.Xf, xVt.Xf = xtmp;
         if (xVt.Yi > xVt.Yf) xtmp = xVt.Yi, xVt.Yi = xVt.Yf, xVt.Yf = xtmp;
         // Ajusta los valores a la longitud de la ventana
         if (xVt.Xi < 0) xVt.Xi = 0;
         if (xVt.Yi < 0) xVt.Yi = 0;
         if (xVt.Xf > VTLenguas->Width) xVt.Xf = VTLenguas->Width;
         if (xVt.Yf > VTLenguas->Height) xVt.Yf = VTLenguas->Height;
         // Visualiza los valores de la region seleccionada segun la dimención de la ventana
         Vs.Xi = xVt.Xi / Escala.X + Dim_Vtn.Xi;
         Vs.Yi = (-(xVt.Yi - VTLenguas->Height) / Escala.Y) + Dim_Vtn.Yi;
         Vs.Xf = xVt.Xf / Escala.X + Dim_Vtn.Xi;
         Vs.Yf = (-(xVt.Yf - VTLenguas->Height) / Escala.Y) + Dim_Vtn.Yi;
         sprintf(xcad,"%s: (%3.5Lf, %3.5Lf, %3.5Lf, %3.5Lf)",VL_TXT01,Vs.Xi,Vs.Yf,Vs.Xf,Vs.Yi);
         BarraDeEstadosLenguas->SimpleText = (AnsiString) xcad;
      } else {
         Vs.Xf = X, Vs.Yf = Y;
         Vs.Xi = Vs.Xf / Escala.X + Dim_Vtn.Xi;
         Vs.Yi = (-(Vs.Yf - VTLenguas->Height) / Escala.Y) + Dim_Vtn.Yi;

         sprintf(Msg,"%s=%3.12Lf, %s=%3.12Lf", fun_disp.Nombre_parametros[Primer_Eje], Vs.Xi, fun_disp.Nombre_parametros[Segundo_Eje], Vs.Yi);
         BarraDeEstadosLenguas->SimpleText = (AnsiString) Msg;
      }    
   }
}


// Cuando el mouse es presionado
void __fastcall TFormaLenguas::VTLenguasMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
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
void __fastcall TFormaLenguas::VTLenguasMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Sw_Recuadro_activo) {
      if (Button == mbLeft) {
         TPenMode mode;
         mode = VTLenguas->Canvas->Pen->Mode;
         VTLenguas->Canvas->Pen->Mode = pmNot;
         VTLenguas->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
         VTLenguas->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
         VTLenguas->Canvas->MoveTo(Vt2.Xi,Vt2.Yi);
         VTLenguas->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
         VTLenguas->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
         VTLenguas->Canvas->LineTo(Vt2.Xi,Vt2.Yf);
         VTLenguas->Canvas->MoveTo(Vt2.Xf,Vt2.Yf);
         VTLenguas->Canvas->LineTo(Vt2.Xf,Vt2.Yi);
         VTLenguas->Canvas->Pen->Mode = mode;
         Vt1.Xf = X;
         Vt1.Xf = Y;
         Sw_Dibuja_rectangulo = false;
         Sw_Dibuja_rectangulo_ant = false;
         BarraDeEstadosLenguas->SimpleText = (AnsiString) " ";
         Ventana_seleccionada = true;
         Tipo_region_calcular = 1;
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
               for (int i = 0; i < 16; i++) pix[i].Cambia_dimension(Dim_Vtn, false);
               FormResize(this);
               Tipo_region_calcular = 1;
               Control_calculo_lenguas();
            } else {
               if (MessageBox(Handle,"Do you wish to calculate only this region?","Bifurcations scenery",MB_YESNO + MB_ICONQUESTION)  == IDYES) {
                  Tipo_region_calcular = 2;
                  Control_calculo_lenguas();
                  Tipo_region_calcular = 1;
               }
            }
         }
      }
   }
}

// Pasa los valores de configuración de la ventana de captura
void TFormaLenguas::PasarValoresConfiguracion(void)
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

   configura->ComboBox2->Items->Clear();
   for( int i = 0; i < fun_disp.Numero_parametros_bifurcar; i++) {
      configura->ComboBox2->Items->Add(fun_disp.Nombre_parametros[i]);
   }
   configura->ComboBox2->ItemIndex = Segundo_Eje;
   ////////////////////////////////////////////////////////
   // Calcular
   ////////////////////////////////////////////////////////
   sprintf(xcad,"%u",ItsTrans);
   configura->EditA1->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",Num_max_trans);
   configura->EditA2->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",SizeOrbits);
   configura->EditA3->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",NCondInits);
   configura->EditA4->Text = (AnsiString) xcad;
   sprintf(xcad,"%1.6Le",Tolerancia);
   configura->EditA5->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",Ptos_X);
   configura->EditA6->Text = (AnsiString) xcad;
   sprintf(xcad,"%u",Ptos_Y);
   configura->EditA7->Text = (AnsiString) xcad;
}

// Retorna los valores de configuración de la ventana de captura
void TFormaLenguas::RetornarValoresConfiguracion(void)
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
   Segundo_Eje = configura->ComboBox2->ItemIndex;
   ////////////////////////////////////////////////////////
   // Calcular
   ////////////////////////////////////////////////////////
   ItsTrans = atoi(configura->EditA1->Text.c_str());
   Num_max_trans = atoi(configura->EditA2->Text.c_str());
   SizeOrbits = atoi(configura->EditA3->Text.c_str());
   NCondInits = atoi(configura->EditA4->Text.c_str());
   Tolerancia = _atold(configura->EditA5->Text.c_str());
   Ptos_X = atoi(configura->EditA6->Text.c_str());
   Ptos_Y = atoi(configura->EditA7->Text.c_str());
   /////////////////////////////////////////////////////////
   // Al terminar
   /////////////////////////////////////////////////////////
   for (int i = 0; i < 16; i++) pix[i].Cambia_dimension(Dim_Vtn, false);
   FormResize(this);
}

void __fastcall TFormaLenguas::Timer1Timer(TObject *Sender)
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
          Control_calculo_lenguas();
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

void __fastcall TFormaLenguas::MenuCalcularDetenerelcalculoClick(TObject *Sender)
{
   Detener_calculo = true;        
}

// Grafica el contenido de la ventana
void __fastcall TFormaLenguas::VTLenguasPaint(TObject *Sender)
{
   Grafica(true);        
}

// OnHint
void  __fastcall TFormaLenguas::OnHint(TObject *Sender)
{
   BarraDeEstadosLenguas->SimpleText = Application->Hint;
}

// Al activar la forma
void __fastcall TFormaLenguas::FormActivate(TObject *Sender)
{
   Application->OnHint = OnHint;
}

// Al desactivar la forma
void __fastcall TFormaLenguas::FormDeactivate(TObject *Sender)
{
   Application->OnHint = NULL;
}


// Menu->Archivo->Carga archivo de lenguas
void __fastcall TFormaLenguas::MenuArchivoLoadFileofTonguesClick(TObject *Sender)
{
   if (!OpenDialogLenguas->Execute()) return;
   strcpy(Archivo_lenguas,OpenDialogLenguas->FileName.c_str());

   if (Encontrar_todas_sincronizaciones()) return;

   // Lectura del archivo de encabezado
   if (Leer_Encabezado()) return;

    //Det. la cantidad de sincros para un conj. de parametros especificos
    unsigned char Indice_sincronizaciones = 0;
   
    // Definicion del arreglo de sincronizaciones
    unsigned char xarreglo_sincronizaciones_Q;
    unsigned char xarreglo_sincronizaciones_P;

    //Parametros (A y B); LeeLongDb se utiliza para capturar valores de tipo long double
    float A, B;


    //Se lee la informacion correspondiente a cada uno de los valores de los parámetros
    //y sus posibles sincronizaciones
    unsigned int i, xi;
    int px, py;
    while (1)
    {
       //Lectura del Parametro A
       fread(&A, sizeof(A), 1, InField);
       //Condicion de que no se ha llegado al final del archivo
       if (A == FIN) break;
       //Lectura del Parametro B
       fread(&B, sizeof(B), 1, InField);
       //Lectura del Numero de Sincronizaciones
       fread(&Indice_sincronizaciones, sizeof(Indice_sincronizaciones), 1, InField);
       //Lectura de las Sincronizaciones
       for (i = 0; i < Indice_sincronizaciones; i++)
       {
          fread(&xarreglo_sincronizaciones_Q, sizeof(xarreglo_sincronizaciones_Q), 1, InField);
          fread(&xarreglo_sincronizaciones_P, sizeof(xarreglo_sincronizaciones_P), 1, InField);
          // Visualización de las lenguas
          if (A >= Dim_Vtn.Xi && A <= Dim_Vtn.Xf && B >= Dim_Vtn.Yi && B <= Dim_Vtn.Yf) {
             for (xi = 0; xi < SizeTgs; xi++) {
                if (Tongues_P[xi] == xarreglo_sincronizaciones_P && Tongues_Q[xi] == xarreglo_sincronizaciones_Q) {
                   pix[xi%16].Asigna_valor(A,B,true);
                   px = (A - Dim_Vtn.Xi) * Escala.X;
                   py = VTLenguas->Height - (((B - Dim_Vtn.Yi) * Escala.Y) + 1.0);
                   VTLenguas->Canvas->Pixels[px][py] = Colores[xi%16];
                   break; 
                }
             }
          }
       }
    } 
    //Cierra el Archivo que fue leido
    fclose(InField);

    // Visualiza las sincronizaciones encontradas
    char xcad1[300];
    TLMensajeForm *Ayuda = new TLMensajeForm(this);
    if(!Ayuda) return; 
    Ayuda->Caption = "Sincronizations found";
    Ayuda->Editor->Lines->Add("");
   
    unsigned int Count = 0;
    if (N_Sincro_encontradas) {
       while (Count < N_Sincro_encontradas) {
          sprintf(xcad1,"   %u:%u", Tongues_P[Count], Tongues_Q[Count]);
          Ayuda->Editor->Lines->Add(xcad1);
          Count ++;
       } 
    }
    sprintf(xcad1,"   %u sincronizations were founded", N_Sincro_encontradas);
    Ayuda->Editor->Lines->Add("");
    Ayuda->Editor->Lines->Add(xcad1);
    Ayuda->Show();

}

void __fastcall TFormaLenguas::LoadFileofTonguesSelect1Click(TObject *Sender)
{
   if (!OpenDialogLenguas->Execute()) return;
   strcpy(Archivo_lenguas,OpenDialogLenguas->FileName.c_str());
   
   bool st = false;
   TVentanaCaptura *vent_cap = new TVentanaCaptura(this);
   if(vent_cap) {
      vent_cap->Titulo("Select Tongues");
      vent_cap->Archivo_a_leer("C:\\CONFIG.DAT");
      vent_cap->ShowModal();
      st = vent_cap->st;
      delete vent_cap;
   }
   if (!st) return;
//   Limpia_matriz_pixeles();
//   Grafica(true);
   // Inicializa los arreglos de trabajo
   delete[] Tongues_Q;
   delete[] Tongues_P;
   Tongues_P = new unsigned int[MAX_ARREGLO];
   Tongues_Q = new unsigned int[MAX_ARREGLO];

   Genera_lista_sincronizaciones();
   Leer_Archivo_lenguas();

   delete[] Tongues_Q;
   delete[] Tongues_P;
}



void __fastcall TFormaLenguas::ShowIndexofMultiestabilidad1Click(TObject *Sender)
{
   if (!OpenDialogLenguas->Execute()) return;
   strcpy(Archivo_lenguas,OpenDialogLenguas->FileName.c_str());

   char xcad[200];

   sprintf(xcad,"%i",Indice_multiestabilidad);
   AnsiString Cad = xcad;
   if (InputQuery("Tongues", "Index of Multistability:", Cad)) {
      if (atoi(Cad.c_str()) >= 0) Indice_multiestabilidad = atoi(Cad.c_str());
       else return;
   } else return;

   
   if (Encontrar_todas_sincronizaciones()) return;

   // Lectura del archivo de encabezado
   if (Leer_Encabezado()) return;

    //Det. la cantidad de sincros para un conj. de parametros especificos
    unsigned char Indice_sincronizaciones = 0;
   
    // Definicion del arreglo de sincronizaciones
    unsigned char xarreglo_sincronizaciones_Q;
    unsigned char xarreglo_sincronizaciones_P;

    //Parametros (A y B); LeeLongDb se utiliza para capturar valores de tipo long double
    float A, B;


    //Se lee la informacion correspondiente a cada uno de los valores de los parámetros
    //y sus posibles sincronizaciones
    unsigned int i, xi;
    int px, py;
    while (1)
    {
       //Lectura del Parametro A
       fread(&A, sizeof(A), 1, InField);
       //Condicion de que no se ha llegado al final del archivo
       if (A == FIN) break;
       //Lectura del Parametro B
       fread(&B, sizeof(B), 1, InField);
       //Lectura del Numero de Sincronizaciones
       fread(&Indice_sincronizaciones, sizeof(Indice_sincronizaciones), 1, InField);
       //Lectura de las Sincronizaciones
       for (i = 0; i < Indice_sincronizaciones; i++)
       {
          fread(&xarreglo_sincronizaciones_Q, sizeof(xarreglo_sincronizaciones_Q), 1, InField);
          fread(&xarreglo_sincronizaciones_P, sizeof(xarreglo_sincronizaciones_P), 1, InField);
       }
       // Visualización de las lenguas
       if (Indice_multiestabilidad == 0) {
          // Visualiza todas las lenguas
          pix[Indice_sincronizaciones%16].Asigna_valor(A,B,true);
          px = (A - Dim_Vtn.Xi) * Escala.X;
          py = VTLenguas->Height - (((B - Dim_Vtn.Yi) * Escala.Y) + 1.0);
          VTLenguas->Canvas->Pixels[px][py] = Colores[Indice_sincronizaciones%16];
        } else {
          if (Indice_multiestabilidad == Indice_sincronizaciones) {
             // Visualiza las lenguas con estabilidad indicada sólamente
             pix[Indice_sincronizaciones%16].Asigna_valor(A,B,true);
             px = (A - Dim_Vtn.Xi) * Escala.X;
             py = VTLenguas->Height - (((B - Dim_Vtn.Yi) * Escala.Y) + 1.0);
             VTLenguas->Canvas->Pixels[px][py] = Colores[Indice_sincronizaciones%16];
          }
       }
    } 
    //Cierra el Archivo que fue leido
    fclose(InField);

    // Visualiza las sincronizaciones encontradas
    char xcad1[300];
    TLMensajeForm *Ayuda = new TLMensajeForm(this);
    if(!Ayuda) return; 
    Ayuda->Caption = "Sincronizations found";
    Ayuda->Editor->Lines->Add("");
   
    unsigned int Count = 0;
    if (N_Sincro_encontradas) {
       while (Count < N_Sincro_encontradas) {
          sprintf(xcad1,"   %u:%u", Tongues_P[Count], Tongues_Q[Count]);
          Ayuda->Editor->Lines->Add(xcad1);
          Count ++;
       } 
    }
    sprintf(xcad1,"   %u sincronizations were founded", N_Sincro_encontradas);
    Ayuda->Editor->Lines->Add("");
    Ayuda->Editor->Lines->Add(xcad1);
    Ayuda->Show();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lectura del archivo de lenguas
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFormaLenguas::Leer_Archivo_lenguas(void)
{
   // Lectura del archivo de encabezado
   if (Leer_Encabezado()) return;

    //Det. la cantidad de sincros para un conj. de parametros especificos
    unsigned char Indice_sincronizaciones = 0;
   
    // Definicion del arreglo de sincronizaciones
    unsigned char a,b;

    //Parametros (A y B); LeeLongDb se utiliza para capturar valores de tipo long double
    float A, B;

    //Se lee la informacion correspondiente a cada uno de los valores de los parámetros y sus posibles sincronizaciones
    unsigned int i, xi;
    int px, py;
    while (!feof(InField))
    {
       //Lectura del Parametro A
       fread(&A, sizeof(A), 1, InField);
       //Condicion de que no se ha llegado al final del archivo
       if (A == FIN) break;
       //Lectura del Parametro B
       fread(&B, sizeof(B), 1, InField);
       //Lectura del Numero de Sincronizaciones
       fread(&Indice_sincronizaciones, sizeof(Indice_sincronizaciones), 1, InField);

       //Lectura de las Sincronizaciones
       for (i = 0; i < Indice_sincronizaciones; i++)
       {
          fread(&a, sizeof(a), 1, InField);
          fread(&b, sizeof(b), 1, InField);
          // Visualización de las lenguas
          if (A >= Dim_Vtn.Xi && A <= Dim_Vtn.Xf && B >= Dim_Vtn.Yi && B <= Dim_Vtn.Yf) {
             for (xi = 0; xi < N_Sincro_encontradas; xi++) {
                if (Tongues_P[xi] == b && Tongues_Q[xi] == a) {
                   pix[xi%16].Asigna_valor(A,B,true);
                   px = (A - Dim_Vtn.Xi) * Escala.X;
                   py = VTLenguas->Height - (((B - Dim_Vtn.Yi) * Escala.Y) + 1.0);
                   VTLenguas->Canvas->Pixels[px][py] = Colores[xi%16];
                   break; 
                }
             }
          }
       }
    } //Termina while(1)
    //Cierra el Archivo que fue leido
    fclose(InField);

    // Visualiza las sincronizaciones encontradas
    char xcad1[300];
    TLMensajeForm *Ayuda = new TLMensajeForm(this);
    if(!Ayuda) return; 
    Ayuda->Caption = "Sincronizations found";
    Ayuda->Editor->Lines->Add("");
   
    unsigned int Count = 0;
    if (N_Sincro_encontradas) {
       while (Count < N_Sincro_encontradas) {
          sprintf(xcad1,"   %u:%u", Tongues_P[Count], Tongues_Q[Count]);
          Ayuda->Editor->Lines->Add(xcad1);
          Count ++;
       } 
    }
    sprintf(xcad1,"   %u sincronizations were founded", N_Sincro_encontradas);
    Ayuda->Editor->Lines->Add("");
    Ayuda->Editor->Lines->Add(xcad1);
    Ayuda->Show();
}

int TFormaLenguas::Encontrar_todas_sincronizaciones(void)
{
   // Lectura del archivo de encabezado
   if (Leer_Encabezado()) return 1;

    //Det. la cantidad de sincros para un conj. de parametros especificos
    unsigned char Indice_sincronizaciones = 0;
    unsigned int i;
   
    // Definicion del arreglo de sincronizaciones
    unsigned char *sincronizaciones_Q = new unsigned char[MAX_ARREGLO];
    unsigned char *sincronizaciones_P = new unsigned char[MAX_ARREGLO];
    unsigned int *Back_T_Q = new unsigned int[MAX_ARREGLO];
    unsigned int *Back_T_P = new unsigned int[MAX_ARREGLO];
    float A, B;
    unsigned int Count;
    N_Sincro_encontradas = 0;

    //Inicializacion de los elementos del arreglo
    for (i = 0; i < MAX_ARREGLO; i++) Back_T_Q[i] = Back_T_P[i] = 0;
    
    while (1)
    {
       //Lectura del Parametro A
       fread(&A, sizeof(A), 1, InField);
       //Condicion de que no se ha llegado al final del archivo
       if (A == FIN) break;
       //Lectura del Parametro B
       fread(&B, sizeof(B), 1, InField);
       //Lectura del Numero de Sincronizaciones para los paramatros (A, B)
       fread(&Indice_sincronizaciones, sizeof(Indice_sincronizaciones), 1, InField);
       //Lectura de las Sincronizaciones para los parametros (A, B)
       
       for (i = 0; i < Indice_sincronizaciones; i++)
       {
          fread(&sincronizaciones_Q[i], sizeof(sincronizaciones_Q[i]), 1, InField);
          fread(&sincronizaciones_P[i], sizeof(sincronizaciones_P[i]), 1, InField);
       }

       for (i = 0; i < Indice_sincronizaciones; i++)
       {
          Count = 0;
          do
          {
             if ( (sincronizaciones_Q[i] == Back_T_Q[Count]) && (sincronizaciones_P[i] == Back_T_P[Count]) ) break;
             Count ++;
          } while( Count <= N_Sincro_encontradas);
          if ( Count > N_Sincro_encontradas && N_Sincro_encontradas < MAX_ARREGLO)
          {
             Back_T_Q[N_Sincro_encontradas] = sincronizaciones_Q[i];
             Back_T_P[N_Sincro_encontradas] = sincronizaciones_P[i];
             N_Sincro_encontradas ++;
             Count = N_Sincro_encontradas+1;
          }
       }  // end para Indice_sincronizaciones
    } //Termina while(1)
    //Cierra el Archivo que fue leido
    fclose(InField);
    SizeTgs = N_Sincro_encontradas;
    //Proceso de Ordenacion en orden ascendente de Q
    if (SizeTgs) Ordenacion(Back_T_Q, Back_T_P);

    delete []sincronizaciones_Q;
    delete []sincronizaciones_P;
    delete []Back_T_Q; 
    delete []Back_T_P;
    return 0;
}

// Lee el encabezado del archivo de lenguas
int TFormaLenguas::Leer_Encabezado(void)
{
   float LeeLongDb = 0.0;
   //Arreglo utilizado para capturar textos del archivo de Lectura
   char Outer[400];

   InField = fopen(Archivo_lenguas,"rb");
   if ( InField == NULL ) return 1;

   ///////////  Clave de Archivo
   fread(Outer, 8, 1, InField);
   Outer[8] = 0;
   
   // Comprueba si el archivo de Apertura cumple con los requerimientos
   if ( strcmp(CLAVE_ENCABEZADO, Outer) != 0) return 1;
   
   /////Nombre del Archivo
   fread(Outer, 30, 1, InField);
   Outer[30] = 0;

   ///////////Comentarios
   fread(Outer, 300, 1, InField);
   Outer[300] = 0;
     
   ////////////// Seccion de Parametros a Variar
   fread(Outer, 100, 1, InField);
   Outer[100] = 0;

   //////////////Clave Numero de Sincronizaciones: Tipo de Algorit.
   fread(Outer, 20, 1, InField);
   Outer[20] = 0;

   ////////// C O N F I G U R A C I O N //////////////////
   fread(Outer, 100, 1, InField);
   Outer[100] = 0;
     
   /////// Dimensiones en X e Y Reales
   fread(Outer, 100, 1, InField);
   Outer[100] = 0;

   float xMinX, xMaxX, xMinY, xMaxY;

   fread(&xMinX, sizeof(xMinX), 1, InField);  //MinX
   fread(&xMinY, sizeof(xMinY), 1, InField);  //MinY
   fread(&xMaxX, sizeof(xMaxX), 1, InField);  //MaxX
   fread(&xMaxY, sizeof(xMaxY), 1, InField);  //MaxY

   // Actualiza las dimenciones de la ventana de trabajo
//   Dim_Vtn.Xi = xMinX;
//   Dim_Vtn.Xf = xMaxX;
//   Dim_Vtn.Yi = xMinY;
//   Dim_Vtn.Yf = xMaxY;
   // Calcula la escala de la ventana de visualizacion
//   Escala.X = (VTLenguas->Width -1) / (long double) (Dim_Vtn.Xf - Dim_Vtn.Xi);
//   Escala.Y = (VTLenguas->Height -1) / (long double) (Dim_Vtn.Yf - Dim_Vtn.Yi);

   /////// Cantidad de Pts en X e Y Fisicas
   fread(Outer, 100, 1, InField);
   Outer[100] = 0;

   unsigned int xPtsX, xPtsY;    

   fread(&xPtsX, sizeof(xPtsX), 1, InField);
   fread(&xPtsY, sizeof(xPtsY), 1, InField);
     
   /////// Longitud de Paso de Calculo
   fread(Outer, 50, 1, InField);
   Outer[50] = 0;

   fread(&LeeLongDb, sizeof(LeeLongDb), 1, InField);
   fread(&LeeLongDb, sizeof(LeeLongDb), 1, InField);
     
   /////// Ecuacion del Sistema
   fread(Outer, 100, 1, InField);
   Outer[100] = 0;
    
   //////// Formato de Almacenamiento de Sincronizaciones
   fread(Outer, 100, 1, InField);
   Outer[100] = 0;


   return 0;
}

// Rutina de ordenacion de Lenguas en Qs
void TFormaLenguas::Ordenacion(unsigned int *Back_T_Q, unsigned int *Back_T_P)
{
   unsigned int Natural_Q, Natural_P, Count = 0, sizeorbits = 0, i, sizeP = 0;

   if (Tongues_P) delete []Tongues_P;
   if (Tongues_Q) delete []Tongues_Q;

   Tongues_P = new unsigned int[SizeTgs];
   Tongues_Q = new unsigned int[SizeTgs];

   // Encuentra la maxima orbita
   for (i = 0; i < SizeTgs; i++) {
      if (Back_T_Q[i] > sizeorbits) sizeorbits = Back_T_Q[i];
      if (Back_T_P[i] > sizeP) sizeP = Back_T_P[i];
   }

   //Ordenacion de las Sincronizaciones
   for(Natural_Q = 1; Natural_Q <= sizeorbits; Natural_Q++) {
      for(Natural_P = 0; Natural_P <= sizeP; Natural_P++) {
         for(i = 0; i < SizeTgs; i++) {
            if( (Natural_Q == Back_T_Q[i]) && (Natural_P == Back_T_P[i]) ) {
               Tongues_Q[Count] = Back_T_Q[i];
               Tongues_P[Count] = Back_T_P[i];
               Count++;
            }
         }
      }
   }
}


// Rutina de ordenacion de Lenguas en Qs
void TFormaLenguas::Ordenacion_lenguas(unsigned int *Back_T_Q, unsigned int *Back_T_P)
{
   unsigned int Natural_Q, Natural_P, Count = 0, sizeorbits = 0, i, sizeP = 0;
   unsigned int *xTongues_P = new unsigned int[N_Sincro_encontradas+2];
   unsigned int *xTongues_Q = new unsigned int[N_Sincro_encontradas+2];

   // Encuentra la maxima orbita
   for (i = 0; i < N_Sincro_encontradas; i++) {
      if (Back_T_Q[i] > sizeorbits) sizeorbits = Back_T_Q[i];
      if (Back_T_P[i] > sizeP) sizeP = Back_T_P[i];
   }

   //Ordenacion de las Sincronizaciones
   for (Natural_Q = 1; Natural_Q <= sizeorbits; Natural_Q++) {
      for (Natural_P = 0; Natural_P <= sizeP; Natural_P++) {
         for (i = 0; i < N_Sincro_encontradas; i++) {
            if ( (Natural_Q == Back_T_Q[i]) && (Natural_P == Back_T_P[i]) ) {
               xTongues_Q[Count] = Back_T_Q[i];
               xTongues_P[Count] = Back_T_P[i];
               Count++;
               break;
            }
         }
      }
   }
   
   for (i = 0; i < N_Sincro_encontradas; i++) {
       Back_T_Q[i] = xTongues_Q[i];
       Back_T_P[i] = xTongues_P[i];
   }
   
   delete []xTongues_P;
   delete []xTongues_Q;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Grabado del archivo de languas
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Formato de Encabezado del Archivo
void TFormaLenguas::Grabar_encabezado_archivo(void)
{
   float Incx = 0.0, Incy = 0.0;
   
   Incx = (MaxX - MinX) / (PtsX);  //  ( C.R. / C.F.)
   Incy = (MaxY - MinY) / (PtsY);  //  ( C.R. / C.F.)

   char Outer[350];

   strcpy(Outer, CLAVE_ENCABEZADO);    ///////////Clave de Archivo
   fwrite(Outer, 8, 1, OutField);
   
   strcpy(Outer, "Nombre del Archivo:");   /////Nombre del Archivo
   fwrite(Outer, 30, 1, OutField);
   
   strcpy(Outer, "Seccion de Comentarios ..."); ///////////Comentarios
   fwrite(Outer, 300,1, OutField);
   
      ////////////// Seccion de Parametros a Variar
   strcpy(Outer, "Parametros: a  b");
   fwrite(Outer, 100, 1, OutField);
   
        //////////////Clave Numero de Sincronizaciones: Tipo de Algorit.
   strcpy(Outer, "Numero Definido");
   fwrite(Outer, 20,1, OutField);
   
          ////////// C O N F I G U R A C I O N //////////////////
   strcpy(Outer, "Oscilador Geometrico de Diente de Sierra");
   fwrite(Outer, 100,1, OutField);
   
          /////// Dimensiones en X e Y Reales
   strcpy(Outer, "Dimensiones (X-Y) Reales");
   fwrite(Outer, 100, 1, OutField);
   
   fwrite(&MinX, sizeof(MinX), 1, OutField);   // MinX
   fwrite(&MinY, sizeof(MinY), 1, OutField);   // MinY
   fwrite(&MaxX, sizeof(MaxX), 1, OutField);   // MaxX
   fwrite(&MaxY, sizeof(MaxY), 1, OutField);   // MaxY
   
          /////// Cantidad de Pts en X-Y
   strcpy(Outer, "Cantidad de Pts en X-Y");
   fwrite(Outer, 100, 1, OutField);
   
   fwrite(&PtsX, sizeof(PtsX), 1, OutField);   // PtsX
   fwrite(&PtsY, sizeof(PtsY), 1, OutField);   // PtsY

          /////// Longitud de Paso de Calculo
   strcpy(Outer, "Longitud de Paso de Calculo (X-Y)");
   fwrite(Outer, 50, 1, OutField);
   
   fwrite(&Incx, sizeof(Incx), 1, OutField);
   fwrite(&Incy, sizeof(Incy), 1, OutField);

          /////// Ecuacion del Sistema
   strcpy(Outer, "Ecuacion: f(x)=x+a+b*Sen(2*PI*x)");
   fwrite(Outer, 100, 1, OutField);

         //////// Formato de Almacenamiento de Sincronizaciones
   strcpy(Outer, "Par-X  Par-Y  #Sincros Valor-Qs  Valor-Ps");
   fwrite(Outer, 100, 1, OutField);
/////////////////////////////////////////////////////////////////////////////
// Falta grabar:
//   Lenguas solicitadas
//      # de lenguas solicitadas, P's , Q's,....
//   ......
/////////////////////////////////////////////////////////////////////////////
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculo de Lenguas 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFormaLenguas::Control_calculo_lenguas(void)
{
   time_t ti, tf;
// Para calculos finos poner un valor mayor a 100
   int xFACTOR;
   Sw_Proceso_calculo = true;
   Sw_cerrar_ventana = false; 
   Detener_calculo = false;
   time(&ti);

   
   // Cambia el cursor del mouse
   TCursor antcursor = Cursor;
   Cursor = crHourGlass;

   // Inicializa los arreglos de trabajo
   Tongues_P = new unsigned int[MAX_ARREGLO];
   Tongues_Q = new unsigned int[MAX_ARREGLO];

   PtsX = Ptos_X;
   PtsY = Ptos_Y;
   if (Tipo_region_calcular == 1) {
      // Limpia la ventana de trabajo
      MenuVentanaLimpiarClick(this);

      MinX = Dim_Vtn.Xi;
      MinY = Dim_Vtn.Yi;
      MaxX = Dim_Vtn.Xf;
      MaxY = Dim_Vtn.Yf;
   } else {
      MinX = Vs.Xi;
      MinY = Vs.Yf;
      MaxX = Vs.Xf;
      MaxY = Vs.Yi;
      if (Optimiza_ptos_calcular) {
         PtsX = ((VTLenguas->Width * (MaxX - MinX)) / (Dim_Vtn.Xf - Dim_Vtn.Xi)) + 50;
         PtsY = ((VTLenguas->Height * (MaxY - MinY)) / (Dim_Vtn.Yf - Dim_Vtn.Yi)) + 50;
      }
   }

   N_Sincro_encontradas = 0;
   Maxima_estabilidad_encontrada_al_calculo_lenguas = 0;
   Terminador = FIN;

   long double Incx, Incy, aaux, baux, aux, minauxY, maxauxY;
   // Eleccion de Lenguas a calcular
   Incx = ((MaxX - MinX) / ((long double) PtsX));  
   Incy = ((MaxY - MinY) / ((long double) PtsY));

   ///////////////////
   if (!SaveDialogLenguas->Execute()) return;
   memccpy(Archivo_lenguas,SaveDialogLenguas->FileName.c_str(),0,300);
   
   //  abrir archivo para almacenamiento
   OutField = fopen(Archivo_lenguas,"wb");
   if( OutField == NULL ) return;
   // Grabar encabezado de archivo de lenguas
   Grabar_encabezado_archivo();

   // inicializa la semilla de condiciones iniciales
   randomize();

   // Solicita memoria para inicializar el arreglo de Condiciones iniciales
   Condiciones_Iniciales = new long double [NCondInits+2];
   for (int i = 0; i <= NCondInits; i++) Condiciones_Iniciales[i] = i / (long double) NCondInits;

   // Solicita memoria para inicializar los arreglos de Ultimas Sincronizaciones encontradas
   Sincronizaciones_en_un_punto_Q = new unsigned int [Maxima_estabilidad_buscada_al_calculo_lenguas+2];
   Sincronizaciones_en_un_punto_P = new unsigned int [Maxima_estabilidad_buscada_al_calculo_lenguas+2];
   Arreglo_Sincronizaciones_Q = new unsigned char [Maxima_estabilidad_buscada_al_calculo_lenguas+2];
   Arreglo_Sincronizaciones_P = new unsigned char [Maxima_estabilidad_buscada_al_calculo_lenguas+2];
   
   for (int i = 0; i <= Maxima_estabilidad_buscada_al_calculo_lenguas; i++) {
      Sincronizaciones_en_un_punto_Q[i] = 0;
      Sincronizaciones_en_un_punto_P[i] = 0;
   }
// #error Poner codigo para almacenar las sincronizaciones calculadas en el ultimo claculo
// Tambien poner una forma de verificar que la sincronizacion calculada es real
// Usando las sincronizaciones del ultimo calculo

   
   // Revisa si existe lista de sincronizaciones
   if(Tipo_lenguas == 1) Genera_lista_sincronizaciones();
   
   if(Tipo_lenguas == 1) {
      if (MinY < Cota_superior_region_difeomorfismos) {
      
         if (MaxY > Cota_superior_region_difeomorfismos) maxauxY = Cota_superior_region_difeomorfismos;
          else maxauxY = MaxY;

         b =  MinY;
         // Region de difeomorfismos
         while (b < maxauxY) {
            Application->ProcessMessages();
            if (Sw_cerrar_ventana || Detener_calculo) break;
            sprintf(xcad,"Calculating Tongues of the parameter %s = %2.8Lf (this process may last several minutes).",fun_disp.Nombre_parametros[Segundo_Eje], b);            
            BarraDeEstadosLenguas->SimpleText=(AnsiString) xcad;
            // Selecciona un parametro dentro de la malla
            a = MinX;
            while (a <= MaxX) {
            xFACTOR = 1 + Factor_usado_zona_difeomorfismos * ((((Cota_superior_region_difeomorfismos - b)*100.0) / Cota_superior_region_difeomorfismos)/100.0);
#ifdef MAYA_CON_ALEATORIOS            
               aux = (random(10)/10.0);
               aaux = a + ((Incx/xFACTOR) * aux);
               baux = b + ((Incy/xFACTOR) * aux);
               Calcula_Lenguas_Solicitadas(a, b, aaux, baux, true);
#else               
               Calcula_Lenguas_Solicitadas(a, b, a, b, true);
#endif               
               a += (Incx/xFACTOR);
            }
            b += (Incy/xFACTOR);   
         }
         minauxY = Cota_superior_region_difeomorfismos;
      } else minauxY = MinY;

      // Region de no difeomorfismos
      for (b = minauxY; b <= MaxY; b += Incy ) {
         Application->ProcessMessages();
         if (Sw_cerrar_ventana || Detener_calculo) break;
         sprintf(xcad,"Calculating Tongues of the parameter %s = %2.8Lf, Multistabolity = %i (this process may last several minutes).",fun_disp.Nombre_parametros[Segundo_Eje], b,Maxima_estabilidad_encontrada_al_calculo_lenguas);
         BarraDeEstadosLenguas->SimpleText=(AnsiString) xcad;
         // Selecciona un parametro dentro de la malla
         for (a = MinX; a <= MaxX; a += Incx) {
#ifdef MAYA_CON_ALEATORIOS                     
            aux = (random(10)/10.0);
            aaux = a + (Incx * aux);
            baux = b + (Incy * aux);
            Calcula_Lenguas_Solicitadas(a, b, aaux, baux, false);
#else            
            Calcula_Lenguas_Solicitadas(a, b, a, b, false);
#endif
         }   
      }
    } else {
      if (MinY < Cota_superior_region_difeomorfismos) {
         if (MaxY > Cota_superior_region_difeomorfismos) maxauxY = Cota_superior_region_difeomorfismos;
          else maxauxY = MaxY;
         b =  MinY;
         // Region de difeomorfismos
         while (b < maxauxY) {
            Application->ProcessMessages();
            if (Sw_cerrar_ventana || Detener_calculo) break;
            sprintf(xcad,"Calculating Tongues of the parameter %s = %2.8Lf (this process may last several minutes).",fun_disp.Nombre_parametros[Segundo_Eje], b);            
            BarraDeEstadosLenguas->SimpleText=(AnsiString) xcad;
            // Selecciona un parametro dentro de la malla
            a = MinX;
            while (a <= MaxX) {
               xFACTOR = 1 + Factor_usado_zona_difeomorfismos * ((((Cota_superior_region_difeomorfismos - b)*100.0) / Cota_superior_region_difeomorfismos)/100.0);
#ifdef MAYA_CON_ALEATORIOS                     
               aux = (random(10)/10.0);
               aaux = a + ((Incx/xFACTOR) * aux);
               baux = b + ((Incy/xFACTOR) * aux);
               Calcula_Lenguas_Todas(a, b, aaux, baux, true);
#else            
               Calcula_Lenguas_Todas(a, b, a, b, true);
#endif
               a += (Incx/(xFACTOR));
            }
            b += (Incy/xFACTOR);   
         }
         minauxY = Cota_superior_region_difeomorfismos;
      } else minauxY = MinY;

      // Region de no difeomorfismos
      for (b = minauxY; b <= MaxY; b += Incy ) {
         Application->ProcessMessages();
         if (Sw_cerrar_ventana || Detener_calculo) break;
         sprintf(xcad,"Calculating Tongues of the parameter %s = %2.8Lf, Multistabolity = %i (this process may last several minutes).",fun_disp.Nombre_parametros[Segundo_Eje], b,Maxima_estabilidad_encontrada_al_calculo_lenguas);
         BarraDeEstadosLenguas->SimpleText=(AnsiString) xcad;
         // Selecciona un parametro dentro de la malla
         for (a = MinX; a <= MaxX; a += Incx) {
#ifdef MAYA_CON_ALEATORIOS                     
            aux = (random(10)/10.0);
            aaux = a + (Incx * aux);
            baux = b + (Incy * aux);
            Calcula_Lenguas_Todas(a, b, aaux, baux, false);
#else            
            Calcula_Lenguas_Todas(a, b, a, b, false);
#endif
         }   
      }
   }
   // Cerrar archivo
   fwrite(&Terminador, sizeof(Terminador), 1, OutField);
   fclose(OutField);
   Grafica(false);
   time(&tf);

   
   char xcad1[300];
   TLMensajeForm *Ayuda = new TLMensajeForm(this);
   if(!Ayuda) return;
   if(Tipo_lenguas != 1) Ayuda->Caption = "Sincronizations found";
   else Ayuda->Caption = "Sincronizations find";
   Ayuda->Editor->Lines->Add("");

   
   unsigned int Count = 0;
   if (N_Sincro_encontradas) {
      Ordenacion_lenguas(Tongues_Q, Tongues_P);
      while (Count < N_Sincro_encontradas) {
         sprintf(xcad1,"   %u:%u", Tongues_P[Count], Tongues_Q[Count]);
         Ayuda->Editor->Lines->Add(xcad1);
         Count ++;
      } 
   }
   if(Tipo_lenguas != 1) sprintf(xcad1,"   %u sincronizations were founded", N_Sincro_encontradas);
   else sprintf(xcad1,"   %u sincronizations were founded", N_Sincro_encontradas);
   Ayuda->Editor->Lines->Add("");
   Ayuda->Editor->Lines->Add(xcad1);
   sprintf(xcad1,"Maximum miltistability founded %i", Maxima_estabilidad_encontrada_al_calculo_lenguas);
   Ayuda->Editor->Lines->Add("");
   Ayuda->Editor->Lines->Add(xcad1);
   Ayuda->Editor->Lines->Add("");
   strcpy(xcad1,ctime(&ti));
   xcad1[24] = 0;
   Ayuda->Editor->Lines->Add(xcad1);
   strcpy(xcad1,ctime(&tf));
   xcad1[24] = 0;
   Ayuda->Editor->Lines->Add(xcad1);
   Ayuda->Show();
   delete[] Tongues_Q;
   delete[] Tongues_P;
   delete[] Sincronizaciones_en_un_punto_Q;
   delete[] Sincronizaciones_en_un_punto_P;
   delete[] Arreglo_Sincronizaciones_Q;
   delete[] Arreglo_Sincronizaciones_P;

   Tongues_P = NULL;
   Tongues_Q = NULL;

   BarraDeEstadosLenguas->SimpleText=(AnsiString) " ";
   Cursor = antcursor;
   Sw_Proceso_calculo = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Genera lista de sincronizaciones
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Genera la lista de sincronizaciones
void TFormaLenguas::Genera_lista_sincronizaciones(void)
{
   // Inicialización
   int i;
   N_Sincro_encontradas = 0;
   for (i = 0; i < MAX_ARREGLO; i++) {
      Tongues_P[i] = 0;
      Tongues_Q[i] = 0;
   }
   // Creación de la lista de sincronizaciones
   Leer_Especificaciones_Calculo();
   // Encuentra la mayor orbita
   SizeOrbits = 0;
   for (i = 0; i < N_Sincro_encontradas; i++) {
      if (Tongues_Q[i] > SizeOrbits) SizeOrbits= Tongues_Q[i];
   }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall TFormaLenguas::Copyparameters1Click(TObject *Sender)
{
   long double parm[NUM_MAX_PARAMETROS];
   fun_disp.RetornaParametros(parm);   
   parm[Primer_Eje] = Vs.Xi;
   parm[Segundo_Eje] = Vs.Yi;
   fun_disp.AsignaParametros(parm);   
}
//---------------------------------------------------------------------------

void __fastcall TFormaLenguas::ShowSincronization1Click(TObject *Sender)
{
   long double parm[NUM_MAX_PARAMETROS];
   fun_disp.RetornaParametros(parm);   
   parm[Primer_Eje] = Vs.Xi;
   parm[Segundo_Eje] = Vs.Yi;
   fun_disp.AsignaParametros(parm);   
   MenuCalcularSincronizacinClick(this);
}


void __fastcall TFormaLenguas::ShowRotational1Click(TObject *Sender)
{
   long double parm[NUM_MAX_PARAMETROS];
   fun_disp.RetornaParametros(parm);   
   parm[Primer_Eje] = Vs.Xi;
   parm[Segundo_Eje] = Vs.Yi;
   fun_disp.AsignaParametros(parm);   
   MenuCalcularNumeroRotacionClick(this);
}



// Lee las especificaciones del calculo
int TFormaLenguas::Leer_Especificaciones_Calculo(void)
{
   unsigned int *Back_T_P, *Back_T_Q; //Lenguas y Color Auxiliares
   unsigned int Resonancy[6];

   FILE *ApTongue;      //Apuntador al archivo de datos de sincronizaciones
   ApTongue = fopen("C:\\CONFIG.DAT","rt");
   if (!ApTongue) return 1;
   
   //Construccion de los arreglos para los Qs, Ps
   Back_T_Q = new unsigned int[MAX_ARREGLO];
   Back_T_P = new unsigned int[MAX_ARREGLO];

   Intern = 0;
   SizeTgs = 0;
   while (!feof(ApTongue)) {  
      //Obtiene Informacion de las Sincronizaciones
      Leer_Linea_Archivo(Resonancy,ApTongue);
      Analizar_Entrada(Resonancy, Back_T_Q, Back_T_P);
   } //Termina while(!THE_END)
   fclose(ApTongue);  //Cierra el archivo de informacion de Sincronizaciones
   // Ordenacion de las lenguas, considerando el periodo 
   Ordenacion(Back_T_P, Back_T_Q);
   N_Sincro_encontradas = Intern;
  
   delete []Back_T_Q;
   delete []Back_T_P;
   return 0;
}

//Analiza las entradas de la lectura del archivo de especificacion de Sincronizaciones
int TFormaLenguas::Analizar_Entrada(const unsigned int  *Resonancy, unsigned int *Back_T_Q, unsigned int *Back_T_P)
{
   unsigned int Cuenta;
   unsigned int i, xi;
   bool st;

   //Verifica que la informacion de las sincronizaciones sea correcta

   //Caso si existe sincronizacion o rango de sinc.
   if( (Resonancy[0]!=0) || (Resonancy[1]!=0) )
   {
       // caso cuando existe un rango de sincronizaciones
       if( (Resonancy[2]!=0) || (Resonancy[3]!=0) )
       {
           //Caso: Qs Fijos, Ps Varian
           if( Resonancy[0] == Resonancy[2] )
           {
               Cuenta = abs( Resonancy[3] - Resonancy[1]) + 1;
               for (i = 0; i < Cuenta; i++)
               {   
                   st = false;
                   for (xi = 0; xi < Intern; xi++) {
                       if ( Back_T_Q[xi] == Resonancy[0] && Back_T_P[xi] == (Resonancy[1] + i)) st = true;
                   }
                   if (!st) {
                      Back_T_Q[Intern] = Resonancy[0];
                      Back_T_P[Intern] = Resonancy[1] + i;
                      Intern++;   //Aumento en el indice de los arreglos
                      SizeTgs++;
                   }
               }
           }
           else
           {
              //Caso: Ps Fijos, Qs Varian
              if( Resonancy[1] == Resonancy[3] )
              {
                   Cuenta = abs( Resonancy[2] - Resonancy[0]) + 1;
                   for (i = 0; i < Cuenta; i++)
                   {
                       st = false;
                       for (xi = 0; xi < Intern; xi++) {
                           if ( Back_T_Q[xi] == (Resonancy[0] + i) && Back_T_P[xi] == Resonancy[1]) st = true;
                       }
                       if (!st) {
                          Back_T_Q[Intern]= Resonancy[0] + i;
                          Back_T_P[Intern]= Resonancy[1]; 

                          SizeTgs++;
                          Intern++;   //Aumento en el indice de los arreglos
                       }
                   }
              }
           }
       }
       else 
       { 
          for (xi = 0; xi < Intern; xi++) {
              if ( Back_T_Q[xi] == Resonancy[0] && Back_T_P[xi] == Resonancy[1]) return 0;
          }
          Back_T_Q[Intern] = Resonancy[0];
          Back_T_P[Intern] = Resonancy[1]; 
          
          Intern++;   //Aumento en el indice de los arreglos
          SizeTgs++;
       }
   }
   //Caso en que no existe sincronizacion o rango de sinc. 0:0
   else return 1;
   return 0;
}


// Funcion de lectura para el archivo de lenguas.
void TFormaLenguas::Leer_Linea_Archivo(unsigned int  *Resonancy, FILE *ApTongue)
{
   char entra[100], car;
   int i, END_LINE = DESACTIVADO, Indice = 0;
   THE_END = DESACTIVADO;

   //Inicializacion del arreglo de rango de sincronizaciones
   for (i = 0; i < 4; i++) Resonancy[i] = 0;
   i = 0;
   while ( !feof(ApTongue) && (!END_LINE) ) {
      car = fgetc(ApTongue);
      if (feof(ApTongue)) {
         if (i) {
            entra[i] = 0;
            Resonancy[Indice++] = atoi(entra);
         } 
         return;
      }
      if (car == ' ') continue;
      if (car == '\n') END_LINE = ACTIVADO;
      if ((car != ':') && (car != ',') && (car != '\n')) entra[i++] = car;
      else {
         entra[i] = 0;
	 if ( strcmp(entra, "...") ) {
            if (!THE_END) Resonancy[Indice++] = atoi(entra);
	    //Limpieza del buffer entra
            entra[0] = 0;
            i = 0;
           } else {
            entra[0] = 0;
            i = 0;
          }
      }
   }
}




// Función del calculo de Sincronizaciones Solicitadas   <<SINCROS == ACTIVADO>> 
void TFormaLenguas::Calcula_Lenguas_Solicitadas(const long double a, const long double b, const long double aaux, const long double baux, const bool difeomorfismo)
{
   // Ajusta los parametros
   fun_disp.RetornaParametros(parm1);   
   fun_disp.RetornaParametros(parm1);   
   parm1[Primer_Eje] = aaux;
   parm1[Segundo_Eje] = baux;
   fun_disp.AsignaParametros(parm1);   

   
   // Almacena el primer valor de las iteraciones para buscar las resonancias
   long double x, Frac_x, Int_x = 0.0, incX;
   // Almacena las iteraciones de la funcion de disparos
   long double New_x, FracNew_x, IntNew_x;
   // Auxiliar en las busquedas
   long double Ancla_x, FracAncla_x, IntAncla_x;
   //Det. la cantidad de sincros para un conj. de parametros especificos
   unsigned char Indice_sincronizaciones = 0;  
   long double IncInits;    
   unsigned int Posible_Q, Posible_P;
   unsigned int i;
   unsigned int Encontro;  //Determina si se ha encontrado alguna posible orbita periodica
   int Count;
   bool existen_sincronizaciones_en_calculo_anterior;


   // Inicializa arreglo de sincronizaciones
   for (i = 0; i < Maxima_estabilidad_buscada_al_calculo_lenguas; i++) {
       Arreglo_Sincronizaciones_Q[i] = 0;
       Arreglo_Sincronizaciones_P[i] = 0;
   }

   // Si es necesario inicializa el arreglo de las últimas sincronizaciones calculadas
   if (Sincronizaciones_en_un_punto_Q[0] == 0 && Sincronizaciones_en_un_punto_P[0] == 0) {
      existen_sincronizaciones_en_calculo_anterior = false;
      for (i = 0; i < Maxima_estabilidad_buscada_al_calculo_lenguas; i++) {
         Sincronizaciones_en_un_punto_Q[i] = 0;
         Sincronizaciones_en_un_punto_P[i] = 0;
      }
   } else existen_sincronizaciones_en_calculo_anterior = true;


  
   // Barrido de Condiciones iniciales
   for (int nci = 0; nci < NCondInits; nci++) {
      // No continua el barrido de condiciones iniciales si ya encontro la máxima multiestabilidad dada
      if (Indice_sincronizaciones >= Maxima_estabilidad_buscada_al_calculo_lenguas) break;
      if (difeomorfismo && Indice_sincronizaciones >= 1) break;

      // Condición inicial aleatoria dentro del intervalo
      Frac_x = Condiciones_Iniciales[nci] + ((Condiciones_Iniciales[nci+1]-Condiciones_Iniciales[nci]) * (random(10)/10.0));   
      
      //  A partir de la Condicion Inicial se realiza un Transitorio de iteraciones sobre ella
      Int_x = 0.0;
      for(i = 0; i < ItsTrans; i++) {  
          x = Frac_x;
          x = fun_disp.Calcula(x,Tipo_integracion) + Int_x;
          Frac_x = modfl(x,&Int_x);
      }

      Encontro = FALSO;
      Posible_Q = 0;
      FracNew_x = Frac_x;
      Int_x = 0.0;
      while( (Encontro == FALSO) && (Posible_Q <= SizeOrbits)) {
         x = Frac_x;
         x = fun_disp.Calcula(x,Tipo_integracion) + Int_x;
         Frac_x = modfl(x, &Int_x);
         //Almacena la posible longitud de la órbita periódica
         Posible_Q++;
         i ++;
         //Condicion para det. si hay órbita
         if ( (fabsl(Frac_x - FracNew_x) < Tolerancia_maxima) || (fabsl(fabsl(Frac_x - FracNew_x)-1.0) < Tolerancia_maxima) ) Encontro = VERDADERO;
      }
      if (Encontro != VERDADERO) continue;

      
      Encontro = FALSO;
      while(i <= Num_max_trans && (Encontro == FALSO) ) {
         Posible_Q = 0;
         FracNew_x = Frac_x;
         Int_x = 0.0;
         while( (Encontro == FALSO) && (Posible_Q <= SizeOrbits)) {
            x = Frac_x;
            x = fun_disp.Calcula(x,Tipo_integracion) + Int_x;
            Frac_x = modfl(x, &Int_x);
            //Almacena la posible longitud de la órbita periódica
            Posible_Q++;
            i ++;
            //Condicion para det. si hay órbita
            if ( (fabsl(Frac_x - FracNew_x) < Tolerancia) || (fabsl(fabsl(Frac_x - FracNew_x)-1.0) < Tolerancia) ) Encontro = VERDADERO;
         }
      }
      if (i > Num_max_trans) continue;

      
      //Inicia comprobacion de la resonancia hallada
      if ((Encontro == VERDADERO)) {
          Encontro = FALSO;
          if ( (Posible_Q > 0) && (Posible_Q <= SizeOrbits) ) {
	      //New_x=New_x+(Tolerancia/10); //Perturbacion ligera si se desea
	      New_x = Ancla_x = Frac_x;
              //Ciclo de check (long. de Posible_Q) desde 1 hasta Posible_Q, pues ahora solo se recorrá la orbita que se presume haber encontrado
              for ( i = 1; i <= Posible_Q; i++) { 
                   FracNew_x = modfl(New_x,&IntNew_x);
                   New_x = fun_disp.Calcula(FracNew_x,Tipo_integracion) + IntNew_x;
              }

              FracAncla_x = modfl(Ancla_x, &IntAncla_x);

              FracNew_x = modfl(New_x, &IntNew_x);
              //Condicion para det. si hay órbita
              if ( (fabsl(FracAncla_x - FracNew_x) < Tolerancia) || (fabsl(fabsl(FracAncla_x - FracNew_x) - 1.0) < Tolerancia) ) Encontro = VERDADERO;
                      
              // Se encontro resonancia y paso el check
              if (Encontro == VERDADERO)  {
                  // Determina la posible envolvencia asociada a la órbita encontrada para "Posible_Q" este ya se encuentra almacenado
                  Posible_P = IntNew_x - IntAncla_x;

                  // Posible P no debera de excede a Posible Q
                  if (Posible_P < 254) 
                  {
                     Count = 0;
                     while (Count < N_Sincro_encontradas) {
                        if ( Tongues_Q[Count] == Posible_Q && Tongues_P[Count] == Posible_P ) {

                           if (Indice_sincronizaciones > 0) {
                              for (i = 0; i < Indice_sincronizaciones; i++) {
                                 if (Arreglo_Sincronizaciones_Q[i] == Posible_Q && Arreglo_Sincronizaciones_P[i] == Posible_P) break;
                              }
                              if (i == Indice_sincronizaciones) {
                                 if (i < Maxima_estabilidad_buscada_al_calculo_lenguas) {
                                     Arreglo_Sincronizaciones_Q[Indice_sincronizaciones] = (unsigned char) Posible_Q;
                                     Arreglo_Sincronizaciones_P[Indice_sincronizaciones] = (unsigned char) Posible_P;
                                     Indice_sincronizaciones++;
                                 }
                              }
                            } else {
                               Arreglo_Sincronizaciones_Q[Indice_sincronizaciones] = (unsigned char) Posible_Q;
                               Arreglo_Sincronizaciones_P[Indice_sincronizaciones] = (unsigned char) Posible_P;
                               Indice_sincronizaciones++;
                           }
                           break;

                        }
                        Count ++;
                     } 
                  }
              } // if de "Se encontro resonancia y paso el check"
	  } //termina "if( (Posible_Q>0L) && (Posible_Q<=SizeOrbits) )"
      } //termina "if((Encontro==VERDADERO)) //Inicia comprob.(Check) de la resonancia hallada"
   } //Termina for de "IncInit"

   // Almacenamiento de los valores de los parametros y sus sincronizaciones
   if (Indice_sincronizaciones > 0)
   {
      int px, py;              
      int Count;
      float xa = (float) a , xb = (float) b;
      fwrite(&xa, sizeof(xa), 1, OutField);
      fwrite(&xb, sizeof(xb), 1, OutField);
      // grabar Indice_sincronizaciones
      fwrite(&Indice_sincronizaciones, sizeof(Indice_sincronizaciones), 1, OutField);

      for (i = 0; i < Indice_sincronizaciones; i++)
      {
         fwrite(&Arreglo_Sincronizaciones_Q[i], sizeof(Arreglo_Sincronizaciones_Q[i]), 1, OutField);
         fwrite(&Arreglo_Sincronizaciones_P[i], sizeof(Arreglo_Sincronizaciones_P[i]), 1, OutField);
         if (!existen_sincronizaciones_en_calculo_anterior) {
            Sincronizaciones_en_un_punto_Q[i] = Arreglo_Sincronizaciones_Q[i];
            Sincronizaciones_en_un_punto_P[i] = Arreglo_Sincronizaciones_P[i];
         }

         Count = 0;
         while (Count < N_Sincro_encontradas) {
            if ( Tongues_Q[Count] == Arreglo_Sincronizaciones_Q[i] && Tongues_P[Count] == Arreglo_Sincronizaciones_P[i] ) break;
            Count ++;
         } 

         pix[Count%16].Asigna_valor(a,b,true);
         px = (a - Dim_Vtn.Xi) * Escala.X;
         py = VTLenguas->Height - (((b - Dim_Vtn.Yi) * Escala.Y) + 1.0);
         VTLenguas->Canvas->Pixels[px][py] = Colores[Count%16];
      }     
      if (Indice_sincronizaciones > Maxima_estabilidad_encontrada_al_calculo_lenguas) Maxima_estabilidad_encontrada_al_calculo_lenguas = Indice_sincronizaciones;
   }    
}


//  Función del calculo de Todas las Posibles Sincronizaciones  <<SINCROS == DESACTIVADO>> 
void TFormaLenguas::Calcula_Lenguas_Todas(const long double a, const long double b, const long double aaux, const long double baux, const bool difeomorfismo)
{
   // Ajusta los parametros
   fun_disp.RetornaParametros(parm1);   
   fun_disp.RetornaParametros(parm1);   
   parm1[Primer_Eje] = aaux;
   parm1[Segundo_Eje] = baux;
   fun_disp.AsignaParametros(parm1);   

   
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
   bool existen_sincronizaciones_en_calculo_anterior;



   // Inicializa arreglo de sincronizaciones
   for (i = 0; i < Maxima_estabilidad_buscada_al_calculo_lenguas; i++) {
       Arreglo_Sincronizaciones_Q[i] = 0;
       Arreglo_Sincronizaciones_P[i] = 0;
   }

   // Si es necesario inicializa el arreglo de las últimas sincronizaciones calculadas
   if (Sincronizaciones_en_un_punto_Q[0] == 0 && Sincronizaciones_en_un_punto_P[0] == 0) {
      existen_sincronizaciones_en_calculo_anterior = false;
      for (i = 0; i < Maxima_estabilidad_buscada_al_calculo_lenguas; i++) {
         Sincronizaciones_en_un_punto_Q[i] = 0;
         Sincronizaciones_en_un_punto_P[i] = 0;
      }
   } else existen_sincronizaciones_en_calculo_anterior = true;
   
  
   //Barrido de Condiciones iniciales
   for (int nci = 0; nci < NCondInits; nci++) {
      // No continua el barrido de condiciones iniciales si ya encontro la máxima multiestabilidad dada
      if (Indice_sincronizaciones >= Maxima_estabilidad_buscada_al_calculo_lenguas) break;
      if (difeomorfismo && Indice_sincronizaciones >= 1) break;

      // Condicion inicial condición inicial aleatoria dentro del intervalo
      Frac_x = Condiciones_Iniciales[nci] + ((Condiciones_Iniciales[nci+1]-Condiciones_Iniciales[nci]) * (random(10)/10.0));   
      
      //  A partir de la Condicion Inicial se realiza un Transitorio de iteraciones sobre ella
      Int_x = 0.0;
      for(i = 0; i < ItsTrans; i++) {  
          x = Frac_x;
          x = fun_disp.Calcula(x,Tipo_integracion) + Int_x;
          Frac_x = modfl(x,&Int_x);
      }

      // Se revisa provisionalmente si esta convergiendo a algo
      Encontro = FALSO;
      Posible_Q = 0;
      FracNew_x = Frac_x;
      Int_x = 0.0;
      while( (Encontro == FALSO) && (Posible_Q <= SizeOrbits))
      {
         x = Frac_x;
         x = fun_disp.Calcula(x,Tipo_integracion) + Int_x;
         Frac_x = modfl(x, &Int_x);
         //Almacena la posible longitud de la órbita periódica
         Posible_Q++;
         i ++;
         //Condicion para det. si hay órbita
         if ( (fabsl(Frac_x - FracNew_x) < Tolerancia_maxima) || (fabsl(fabsl(Frac_x - FracNew_x)-1.0) < Tolerancia_maxima) ) Encontro = VERDADERO;
      }
      // Si no converge provisionalmente a algo se descarta la condición inicial
      if (Encontro != VERDADERO) continue;

      // Revisa si existe almenos una sincronización encontrada
      if (Indice_sincronizaciones) {
         // Revisa si la candidata es una de las que ya existian
         Posible_P = Int_x;
         for (i = 0; i < Indice_sincronizaciones; i++) {
            if (Arreglo_Sincronizaciones_Q[i] == Posible_Q && Arreglo_Sincronizaciones_P[i] == Posible_P) {
               // Descarta la candidata por ya existir
               Encontro = FALSO;
               break;
            }
         }
      }
      // Descarta la candidata por ya existir
      if (Encontro != VERDADERO) continue;

      // Hace el refinamiento de la sincronización candidata
      Encontro = FALSO;
      while(i <= Num_max_trans && (Encontro == FALSO) ) {
         Posible_Q = 0;
         FracNew_x = Frac_x;
         Int_x = 0.0;
         while( (Encontro == FALSO) && (Posible_Q <= SizeOrbits))
         {
            x = Frac_x;
            x = fun_disp.Calcula(x,Tipo_integracion) + Int_x;
            Frac_x = modfl(x, &Int_x);
            //Almacena la posible longitud de la órbita periódica
            Posible_Q++;
            i ++;
            //Condicion para det. si hay órbita
            if ( (fabsl(Frac_x - FracNew_x) < Tolerancia) || (fabsl(fabsl(Frac_x - FracNew_x)-1.0) < Tolerancia) ) Encontro = VERDADERO;
         }
      }
      if (i > Num_max_trans) continue;

      //Inicia comprobacion de la resonancia hallada
// #error falta el chequeo de la sincronizacion encontrada      
      if (Encontro == VERDADERO) {
          Encontro = FALSO;
          if ( (Posible_Q > 0) && (Posible_Q <= SizeOrbits) ) {
	      //New_x=New_x+(Tolerancia/10); //Perturbacion ligera si se desea
	      New_x = Ancla_x = Frac_x;
              //Ciclo de check (long. de Posible_Q) desde 1 hasta Posible_Q, pues ahora solo se recorrá la orbita que se presume haber encontrado
              for ( i = 1; i <= Posible_Q; i++) { 
                   FracNew_x = modfl(New_x,&IntNew_x);
                   New_x = fun_disp.Calcula(FracNew_x,Tipo_integracion) + IntNew_x;
              }

              FracAncla_x = modfl(Ancla_x, &IntAncla_x);

              FracNew_x = modfl(New_x, &IntNew_x);
              //Condicion para det. si hay órbita
              if ( (fabsl(FracAncla_x - FracNew_x) < Tolerancia) || (fabsl(fabsl(FracAncla_x - FracNew_x) - 1.0) < Tolerancia) ) Encontro = VERDADERO;
                      
              // Se encontro resonancia y paso el check
              if (Encontro == VERDADERO) {
                  // Determina la posible envolvencia asociada a la órbita encontrada para "Posible_Q" este ya se encuentra almacenado
                  Posible_P = IntNew_x - IntAncla_x;

                  // Posible P no debera de excede a Posible Q
                  if (Posible_P < 254)  {
                     //Caso en que no se proporciona lista de sincronizaciones opciones de Grabado para Multisincronizacion
                     if (Indice_sincronizaciones > 0) {
                        for (i = 0; i < Indice_sincronizaciones; i++) {
                           if (Arreglo_Sincronizaciones_Q[i] == Posible_Q && Arreglo_Sincronizaciones_P[i] == Posible_P) break;
                        }
                        if (i == Indice_sincronizaciones) {
                           if (i < Maxima_estabilidad_buscada_al_calculo_lenguas) {
                              Arreglo_Sincronizaciones_Q[Indice_sincronizaciones] = (unsigned char) Posible_Q;
                              Arreglo_Sincronizaciones_P[Indice_sincronizaciones] = (unsigned char) Posible_P;
                              Indice_sincronizaciones++;
                           }
                        }
                      } else {
                         Arreglo_Sincronizaciones_Q[Indice_sincronizaciones] = (unsigned char) Posible_Q;
                         Arreglo_Sincronizaciones_P[Indice_sincronizaciones] = (unsigned char) Posible_P;
                         Indice_sincronizaciones++;
                     }
                  }
              } // if de "Se encontro resonancia y paso el check"
	  } //termina "if( (Posible_Q>0L) && (Posible_Q<=SizeOrbits) )"
      } //termina "if((Encontro==VERDADERO)) //Inicia comprob.(Check) de la resonancia hallada"
   } //Termina for de "IncInit"

   // Almacenamiento de los valores de los parametros y sus sincronizaciones
   if (Indice_sincronizaciones > 0)
   {
      int px, py;              
      int Count;
      float xa = (float) a , xb = (float) b;
      fwrite(&xa, sizeof(xa), 1, OutField);
      fwrite(&xb, sizeof(xb), 1, OutField);
      // grabar Indice_sincronizaciones
      fwrite(&Indice_sincronizaciones, sizeof(Indice_sincronizaciones), 1, OutField);
      for (i = 0; i < Indice_sincronizaciones; i++) {
         // Graba la sincronización encontrada
         fwrite(&Arreglo_Sincronizaciones_Q[i], sizeof(Arreglo_Sincronizaciones_Q[i]), 1, OutField);
         fwrite(&Arreglo_Sincronizaciones_P[i], sizeof(Arreglo_Sincronizaciones_P[i]), 1, OutField);
         if (!existen_sincronizaciones_en_calculo_anterior) {
            Sincronizaciones_en_un_punto_Q[i] = Arreglo_Sincronizaciones_Q[i];
            Sincronizaciones_en_un_punto_P[i] = Arreglo_Sincronizaciones_P[i];
         }

         // Revisa si ya esta en el arreglo de lenguas en caso contrario la anexa
         Count = 0;
         while (Count < N_Sincro_encontradas) {
            if ( Tongues_Q[Count] == Arreglo_Sincronizaciones_Q[i] && Tongues_P[Count] == Arreglo_Sincronizaciones_P[i] ) break;
            Count ++;
         } 
         if ( Count >= N_Sincro_encontradas && N_Sincro_encontradas < MAX_ARREGLO) {
             Tongues_Q[N_Sincro_encontradas] = Arreglo_Sincronizaciones_Q[i];
             Tongues_P[N_Sincro_encontradas] = Arreglo_Sincronizaciones_P[i];
             Count = N_Sincro_encontradas;
             N_Sincro_encontradas ++;
         }
         // Visualiza la sincronizacion encontrada
         pix[Count%16].Asigna_valor(a,b,true);
         px = (a - Dim_Vtn.Xi) * Escala.X;
         py = VTLenguas->Height - (((b - Dim_Vtn.Yi) * Escala.Y) + 1.0);
         VTLenguas->Canvas->Pixels[px][py] = Colores[Count%16];
      }     
      if (Indice_sincronizaciones > Maxima_estabilidad_encontrada_al_calculo_lenguas) Maxima_estabilidad_encontrada_al_calculo_lenguas = Indice_sincronizaciones;
   }    
}


       
// Lanza una ventana del escenario Toro
void __fastcall TFormaLenguas::Torous1Click(TObject *Sender)
{
   TFormaToro *vent_tor = new TFormaToro(this);
   if(vent_tor) {
      for( int i = 0; i < fun_disp.Numero_parametros; i++) vent_tor->fun_disp.P[i] = fun_disp.P[i];
      vent_tor->fun_disp.P[Primer_Eje] = Vs.Xi;
      vent_tor->fun_disp.P[Segundo_Eje] = Vs.Yi;
      vent_tor->Show();
   }
}

// Lanza una ventana del escenario Círculo
void __fastcall TFormaLenguas::Circle1Click(TObject *Sender)
{
   TFormaCirculo *vent_cir = new TFormaCirculo(this);
   if(vent_cir) {
      for( int i = 0; i < fun_disp.Numero_parametros; i++) vent_cir->fun_disp.P[i] = fun_disp.P[i];
      vent_cir->fun_disp.P[Primer_Eje] = Vs.Xi;
      vent_cir->fun_disp.P[Segundo_Eje] = Vs.Yi;
      vent_cir->Show();
   }
}

// Lanza una ventana del escenario Bifurcacion
void __fastcall TFormaLenguas::Bifurcations1Click(TObject *Sender)
{
   TFormaBifurcaciones *vent_bif = new TFormaBifurcaciones(this);
   if(vent_bif) {
      for( int i = 0; i < fun_disp.Numero_parametros; i++) vent_bif->fun_disp.P[i] = fun_disp.P[i];
      vent_bif->fun_disp.P[Primer_Eje] = Vs.Xi;
      vent_bif->fun_disp.P[Segundo_Eje] = Vs.Yi;
      vent_bif->Show();
   }
}

// Lanza una ventana del escenario Escalera diablo
void __fastcall TFormaLenguas::DevilsStaircase1Click(TObject *Sender)
{
   TFormaEscalera *vent_esc = new TFormaEscalera(this);
   if(vent_esc) {
      for( int i = 0; i < fun_disp.Numero_parametros; i++) vent_esc->fun_disp.P[i] = fun_disp.P[i];
      vent_esc->fun_disp.P[Primer_Eje] = Vs.Xi;
      vent_esc->fun_disp.P[Segundo_Eje] = Vs.Yi;
      vent_esc->Show();
   }
}

