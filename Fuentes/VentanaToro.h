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


#ifndef VentanaToroH
#define VentanaToroH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "V_pixel.hpp"
#include "FuncionDisparo.hpp"
#include "ParamToro.h"

#define ADELANTE    1
#define ATRAZ       0
//---------------------------------------------------------------------------
class TFormaToro : public TForm
{
   __published:	// IDE-managed Components
        TStatusBar *BarraDeEstadosToro;
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
        TMenuItem *MenuCalcularToro;
        TMenuItem *MenuCalcularNumeroRotacion;
        TMenuItem *MenuCalcularSincronizacin;
        TMenuItem *MenuCalcularExpLyapunov;
        TMenuItem *Separador3;
        TMenuItem *MenuCalcularDetenerelcalculo;
        TMenuItem *MenuVentana;
        TMenuItem *MenuVentanaLimpiar;
        TMenuItem *MenuConfigurar;
        TMenuItem *MenuConfigurarParametros;
        TMenuItem *MenuConfigurarVisualizar;
        TMenuItem *MenuConfigurarCalculo;
        TMenuItem *MenuAyuda;
        TMenuItem *MenuAyudaAcercade;
        TMenuItem *Separador1;
        TMenuItem *MenuAyudaToro;
        TMenuItem *MenuCalcularIteracionesToro;
        TPopupMenu *PopupMenu1;
        TMenuItem *FijarCondicIninicial;
        TTimer *Timer1;
        TMenuItem *Separador5;
        TPanel *Panel1;
        TPaintBox *VTToro;
        TPanel *Panel2;
        TPaintBox *VVToro;
        TMenuItem *Atractor1;
        TMenuItem *N1;
        TMenuItem *Circle1;
        void __fastcall MenuArchivoCerrarClick(TObject *Sender);
        void __fastcall MenuAyudaAcercadeClick(TObject *Sender);
        void __fastcall MenuAyudaToroClick(TObject *Sender);
        void __fastcall MenuCalcularIteracionesenToroClick(
          TObject *Sender);
        void __fastcall MenuCalcularNumRotacionClick(TObject *Sender);
        void __fastcall MenuCalcularSincronizacionClick(TObject *Sender);
        void __fastcall MenuCalcularExpLyapunovClick(TObject *Sender);
        void __fastcall MenuCalcularToroClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall MenuVentanaLimpiarClick(TObject *Sender);
        void __fastcall MenuConfigurarCalculoClick(TObject *Sender);
        void __fastcall ManuArchivoGrabarBMPClick(TObject *Sender);
        void __fastcall MenuArchivoImprimirClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall MenuConfigurarParametrosClick(TObject *Sender);
        void __fastcall MenuConfigurarVisualizarClick(TObject *Sender);
        void __fastcall VTToroMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FijarCondicIninicialClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall MenuCalcularDetenerelcalculoClick(TObject *Sender);
        void __fastcall VTToroPaint(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormDeactivate(TObject *Sender);
        void __fastcall VTToroClick(TObject *Sender);
        void __fastcall Atractor1Click(TObject *Sender);
        void __fastcall Circle1Click(TObject *Sender);
   private:	// User declarations

        // Puntero a la ventana de configuracion
        TVCap_Toro            *configura;
        char                   xcad[200];

        // Variables para mostrar la posición del mouse dentro de la ventana
        Dimension_Ventana      Vs;
        char                   Msg[100];

        Arreglo_pixeles        pix[4];    // Clase que contiene los graficos generados
        C_2D                   Escala, escala;    // Escala del gráfico
        unsigned int           Pix_x;
        unsigned int           Pix_y;
        Dimension_Ventana      Dim_VtnT, Dim_VtnP;
        TColor                 Color_grafica[4];
        TColor                 Color_fondo;
        unsigned int           Puntos_trazar;
        unsigned int           Num_iteraciones;
        unsigned int           Num_trazos;
        C_2D                    Condicion_inicial;
        bool                   Tipo_integracion;
        bool                   Sw_Visualiza_referencia;
        bool                   Sw_Visualiza_toro;
        bool                   Sw_Visualiza_linea;
        bool                   Detener_calculo;
        bool                   Sw_Proceso_calculo;
        bool                   Sw_cerrar_ventana;

        void                   Calcula_toro(void);
        void                   Calcula_lineas_toro(void);
        void                   Calcula_referencia(void);

        void                   Grafica(bool limpia);
        void                   Limpiar_vantana(void);
        void                   Limpia_matriz_pixeles(void)
                               {
                                  pix[2].Asigna_todo_array(false);
                                  pix[3].Asigna_todo_array(false);
                               }
        void                   PasarValoresConfiguracion(void);
        void                   RetornarValoresConfiguracion(void);
        void                   PasarValorParametroSeleccionado(void);

        void                   __fastcall OnHint(TObject *Sender);
   public:		// User declarations

        // Funcion de disparo
        FuncionDisparo         fun_disp;

        __fastcall TFormaToro(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif

