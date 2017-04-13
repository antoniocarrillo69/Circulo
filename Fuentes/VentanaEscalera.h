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

#ifndef VentanaEscaleraH
#define VentanaEscaleraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include "V_pixel.hpp"
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "FuncionDisparo.hpp"
#include "ParamEscalera.h"

#define ADELANTE    1
#define ATRAZ       0
#define NUM_MAX_DIM_ZOOM       10
#define MAYORSINCRONIZACION   255  // Si se cambia este valor hay que modificar el tipo de variable de Arreglo_Sincronizaciones_Q y _P
////////////////////////////////////////////////////////////////////////////////////
// #error Estas deveran ser dinamicas
#define MAX_ARREGLO          1600
///////////////////////////////////////////////////////////////////////////////////
#define DESACTIVADO 0   
#define ACTIVADO  1
#define FALSO 0
#define VERDADERO 1

extern TColor Colores[];


//---------------------------------------------------------------------------
class TFormaEscalera : public TForm
{
   __published:	// IDE-managed Components
        TStatusBar *BarraDeEstadosEscalera;
        TSavePictureDialog *SaveDialog;
        TMainMenu *MenuPrincipal;
        TMenuItem *MenuArchivo;
        TMenuItem *MenuArchivoGrabarBMP;
        TMenuItem *MenuArchivoImprimir;
        TMenuItem *Separador4;
        TMenuItem *MenuArchivoGrabarConfiguracion;
        TMenuItem *MenuArchivoLeerConfiguracion;
        TMenuItem *MenuArchivoReportedeConfiguracion;
        TMenuItem *Separador2;
        TMenuItem *MenuArchivoCerrar;
        TMenuItem *MenuEdicion;
        TMenuItem *MenuEdicionCopiar;
        TMenuItem *MenuEdicionPegar;
        TMenuItem *MenuCalcular;
        TMenuItem *MenuCalcularLenguas;
        TMenuItem *MenuCalcularNumeroRotacion;
        TMenuItem *MenuCalcularSincronizacin;
        TMenuItem *MenuCalcularExpLyapunov;
        TMenuItem *Separador3;
        TMenuItem *MenuCalcularDetenerelcalculo;
        TMenuItem *MenuVentana;
        TMenuItem *MenuVentanaLimpiar;
        TMenuItem *MenuConfigurar;
        TMenuItem *MenuConfigurarParametros;
        TMenuItem *MenuConfigurarDimenciones;
        TMenuItem *MenuConfigurarCalculo;
        TMenuItem *MenuAyuda;
        TMenuItem *MenuAyudaAcercade;
        TMenuItem *Separador1;
        TMenuItem *MenuAyudaLevantamientos;
        TTimer *Timer1;
        TMenuItem *Separador5;
        TPanel *Panel1;
        TPaintBox *VTEscalera;
        TMenuItem *Separador7;
        TOpenDialog *OpenDialogLenguas;
        TSaveDialog *SaveDialogLenguas;
        TMenuItem *MenuVentanaZoomOut;
        TMenuItem *MenuVentanaOriginalDimentions;
        TPopupMenu *PopupMenu1;
        TMenuItem *Copyparameters1;
        TMenuItem *ShowSincronization1;
        TMenuItem *Tongues1;
        TMenuItem *ShowRotational1;
        TMenuItem *N1;
        TMenuItem *Torus1;
        TMenuItem *Circle1;
        TMenuItem *Bifurcations1;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall MenuArchivoCerrarClick(TObject *Sender);
        void __fastcall MenuAyudaLevantamientosClick(TObject *Sender);
        void __fastcall MenuAyudaAcercadeClick(TObject *Sender);
        void __fastcall MenuVentanaLimpiarClick(TObject *Sender);
        void __fastcall MenuCalcularNumeroRotacionClick(TObject *Sender);
        void __fastcall MenuCalcularSincronizacinClick(TObject *Sender);
        void __fastcall MenuCalcularExpLyapunovClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall MenuConfigurarCalculoClick(TObject *Sender);
        void __fastcall MenuArchivoGrabarBMPClick(TObject *Sender);
        void __fastcall MenuArchivoImprimirClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall MenuConfigurarParametrosClick(TObject *Sender);
        void __fastcall MenuConfigurarDimensionesClick(TObject *Sender);
        void __fastcall VTEscaleraMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall MenuCalcularDetenerelcalculoClick(TObject *Sender);
        void __fastcall VTEscaleraPaint(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormDeactivate(TObject *Sender);
        void __fastcall VTEscaleraMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall VTEscaleraMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall MenuVentanaOriginalDimentionsClick(TObject *Sender);
        void __fastcall MenuVentanaZoomOutClick(TObject *Sender);
        void __fastcall Copyparameters1Click(TObject *Sender);
        void __fastcall ShowSincronization1Click(TObject *Sender);
        void __fastcall MenuCalcularLenguasClick(TObject *Sender);
        void __fastcall MenuEdicionCopiarClick(TObject *Sender);
        void __fastcall MenuEdicionPegarClick(TObject *Sender);
        void __fastcall Tongues1Click(TObject *Sender);
        void __fastcall ShowRotational1Click(TObject *Sender);
        void __fastcall Torus1Click(TObject *Sender);
        void __fastcall Circle1Click(TObject *Sender);
        void __fastcall Bifurcations1Click(TObject *Sender);
   private:	// User declarations
                               
        // Puntero a la ventana de configuracion
        TVCap_Escalera        *configura;
        char                   xcad[200];

        // Variables para mostrar la posición del mouse dentro de la ventana
        Dimension_Ventana      Vs;
        char                   Msg[200];

        Arreglo_pixeles        pix[1]; // Clase que contiene los graficos generados   

        C_2D                   Escala; // Escala del gráfico
        unsigned int           Pix_x;
        unsigned int           Pix_y; 
        Dimension_Ventana      Dim_Vtn;
        TColor                 Color_fondo;

        bool                   Tipo_integracion;
        bool                   Detener_calculo;
        bool                   Sw_Proceso_calculo;
        bool                   Sw_cerrar_ventana;
       
        // Variables necesarias para dibujar recuadro de selecccion de una area 
        Definicion_Ventana     Vt1, Vt2, xVt;
        bool                   Ventana_seleccionada;
        bool                   Sw_Dibuja_rectangulo;
        bool                   Sw_Dibuja_rectangulo_ant;
        bool                   Sw_Recuadro_activo;
        int                    Ind_zoom;
        Dimension_Ventana      Dim_zoom[NUM_MAX_DIM_ZOOM];
        Dimension_Ventana      Dim_orig;

                               // Constructor de la clase 
                               Levantamientos(void);

        
        void                   Grafica(bool limpia);
        void                   Limpiar_vantana(void);
        void                   Limpia_matriz_pixeles(void)
                               {
                                  pix[0].Asigna_todo_array(false);
                               }
        void                   PasarValoresConfiguracion(void);
        void                   RetornarValoresConfiguracion(void);

        void                   __fastcall OnHint(TObject *Sender);


        int Ptos_X;
        int Primer_Eje;
        int Numero_iteraciones;
        int Puntos_ventana;

           
        // Variables para calculo de lenguas
        long double parm[NUM_MAX_PARAMETROS];



        
   public:		// User declarations

        // Funcion de Disparo
        FuncionDisparo         fun_disp;


        __fastcall TFormaEscalera(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif

