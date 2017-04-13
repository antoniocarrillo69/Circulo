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


#ifndef EditorH
#define EditorH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <vcl\Clipbrd.hpp>
#include "..\libreria\Fechora.hpp"

//---------------------------------------------------------------------------
class TEditorForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MenuPrincipal;
        TMenuItem *MenuArchivo;
        TMenuItem *MenuArchivoCerrar;
   TMenuItem *Separador;
        TMenuItem *MenuArchivoConfiguraImpresora;
        TMenuItem *MenuArchivoImprimir;
        TMenuItem *N2;
        TMenuItem *MenuArchivoGrabaComo;
        TMenuItem *MenuArchivoGrabar;
        TMenuItem *MenuArchivoAbrir;
        TMenuItem *MenuArchivoNuevo;
        TMenuItem *MenuEdicion;
        TMenuItem *MenuEdicionRemplazar;
        TMenuItem *MenuEdicionBuscar;
        TMenuItem *N5;
        TMenuItem *MenuEdicionPegar;
        TMenuItem *MenuEdicionCopiar;
        TMenuItem *MenuEdicionCortar;
        TMenuItem *N6;
        TMenuItem *MenuEdicionDeshacer;
        TPanel *BarraDeHerramientas;
        TSpeedButton *IconoAbrir;
        TSpeedButton *IconoGrabar;
        TSaveDialog *SaveDialog;
        TOpenDialog *OpenDialog;
        TPrintDialog *PrintDialog;
        TMenuItem *MenuEdicionSeleccionaTodo;
        TPopupMenu *PopupMenu1;
        TMenuItem *PopMenuDeshacer;
        TMenuItem *PopMenuCortar;
        TMenuItem *PopMenuCopiar;
        TMenuItem *PopMenuPegar;
        TMenuItem *PopMenuSeleccionarTodo;
        TSpeedButton *IconoCortar;
        TSpeedButton *IconoCopiar;
        TSpeedButton *IconoPegar;
        TSpeedButton *IconoDeshacer;
        TSpeedButton *IconoImprimir;
        TStatusBar *BarraDeEstados;
        TMenuItem *MenuBloque;
        TMenuItem *N7;
        TMenuItem *MenuBloqueConvertirMayusculas;
        TMenuItem *MenuBloqueConvertirminusculas;
        TMenuItem *MenuConfigurar;
        TMenuItem *MenuConfigurarTipoLetra;
        TFontDialog *FontDialog;
        TMenuItem *MenuConfigurarLongitudEdicionFija;
        TMenuItem *MenuBloqueLeer;
        TMenuItem *MenuBloqueGrabar;
        TMenuItem *MenuBloqueImprimir;
        TMenuItem *N4;
        TMenuItem *MenuEdicionInsertarFecha;
        TMenuItem *MenuEdicionInsertarHora;
        TMenuItem *MenuEdicionInsertarFechaHora;
        TMenuItem *N3;
        TMenuItem *PopMenuInsertarFecha;
        TMenuItem *PopMenuInsertarHora;
        TMenuItem *PopMenuInsertarFechaHora;
        TSpeedButton *IconoInsertaFecha;
        TSpeedButton *IconoInsertaHora;
        TSpeedButton *InsertaInsertaFechaHora;
        TMenuItem *N8;
        TRichEdit *Editor;
        TFindDialog *FindDialog;
        TSpeedButton *IconoBuscar;
        TOpenDialog *OpenDialog1;
        TReplaceDialog *ReplaceDialog;
   TColorDialog *ColorDialog;
        TMenuItem *MenuConfigurarColorVentana;
   TMenuItem *N9;
   TMenuItem *Arch1;
   TMenuItem *Arch2;
   TMenuItem *Arch3;
   TMenuItem *Arch4;
        TMenuItem *MenuEdicionCopiarTodo;
        TMenuItem *PopMenuCopiarTodo;
        void __fastcall NuevoArchivo(TObject *Sender);
        
        void __fastcall GrabaArchivo(TObject *Sender);
        void __fastcall AbrirArchivo(TObject *Sender);
        void __fastcall GrabaComoArchivo(TObject *Sender);
        
        void __fastcall DeshaseCambios(TObject *Sender);
        void __fastcall SeleccionaTexto(TObject *Sender);
        void __fastcall CortaTexto(TObject *Sender);
        void __fastcall CopiaTexto(TObject *Sender);
        void __fastcall PegarTexto(TObject *Sender);
        void __fastcall SalirEditor(TObject *Sender);
        
        void __fastcall LongitudFija(TObject *Sender);
        void __fastcall MenuArchivoImprimirClick(TObject *Sender);
        
        void __fastcall ConfiguraPRN(TObject *Sender);
        void __fastcall Insertafecha1Click(TObject *Sender);
        void __fastcall Insertahora1Click(TObject *Sender);
        void __fastcall Insertafechayhora1Click(TObject *Sender);
        void __fastcall MenuBloqueConvertirMayusculasClick(TObject *Sender);
        void __fastcall MenuBloqueConvertirminusculasClick(TObject *Sender);
        void __fastcall MenuConfigurarTipoLetraClick(TObject *Sender);
        void __fastcall MenuBloqueLeerClick(TObject *Sender);
        
        void __fastcall FindDialogFind(TObject *Sender);
        void __fastcall MenuEdicionBuscarClick(TObject *Sender);
        void __fastcall ReplaceDialogReplace(TObject *Sender);
        void __fastcall MenuEdicionRemplazarClick(TObject *Sender);
        
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormDeactivate(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall MenuConfigurarColorVentanaClick(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall Arch1Click(TObject *Sender);
   void __fastcall Arch2Click(TObject *Sender);
   void __fastcall Arch3Click(TObject *Sender);
   void __fastcall Arch4Click(TObject *Sender);
        void __fastcall MenuEdicionCopiarTodoClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
        char        *ARCHIVO;
        char         TITULO[200];
        bool         st_solo_lectura;

              // El editor estara en modo de solo lectura
        void  Editor_solo_lectura(bool st);
              // Muestra el titulo de la aplicación
        void  Titulo(void);
              //Actualiza lista de archivos
        void  Actualiza_lista(void);


        void  __fastcall OnHint(TObject *Sender);
        void  __fastcall OnIdle(TObject *Sender, bool &Done);
        
public:		// User declarations

              // Abre el archivo por omision
        void  Abrir_archivo(char *arch, bool st_edicion);
              // Abre el archivo indicado
        void  Abrir_archivo_lista(const char *arch);
        
        __fastcall TEditorForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TEditorForm *EditorForm;
//---------------------------------------------------------------------------
#endif
