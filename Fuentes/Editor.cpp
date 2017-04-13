// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#include <vcl.h>
#include <stdlib.h>
#include <io.h>
#include <stdio.h>
#include "man_arch.hpp"
#include "Editor.h"
#include "ctrl_l_a.hpp"
//#include "..\\Alarma\\trayec.h"
#pragma hdrstop

#define ARCHIVO_CFG_EDITOR "C:\\EDITOR.CFG"


// Objeto global
static Ctrl_lista_archivos cla;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#ifdef _IDIOMA_ESPANOL_
   #define TXT01 "Editor de texto ASCII"
   #define TXT02 "Buscar"
   #define TXT03 "Fin del documento"
   #define TXT04 "Error en solicitud de memoria dinámica"
   #define TXT05 "¿Desea terminar la actual edición?"
   #define TXT06 "El archivo actual ha sido cambiado, ¿se graba?"
   #define TXT07 "¿Desea iniciar un nuevo archivo?"
   #define TXT08 "¿Desea sobreescribir el archivo?"
   #define TXT09 "El archivo tiene permiso de solo lectura"
   #define TXT10 "&Longitud de edición fija"
   #define TXT11 "&Longitud de edición variable"
   #define TXT12 "Editor de texto ASCII ( %s ) Versión 1.0a"
#else
   #define TXT01 "Editor de texto ASCII"
   #define TXT02 "Find"
   #define TXT03 "End of document"
   #define TXT04 "Error en solicitud de memoria dinámica"
   #define TXT05 "¿Desea terminar la actual edición?"
   #define TXT06 "El archivo actual ha sido cambiado, ¿se graba?"
   #define TXT07 "¿Desea iniciar un nuevo archivo?"
   #define TXT08 "¿Desea sobreescribir el archivo?"
   #define TXT09 "El archivo tiene permiso de solo lectura"
   #define TXT10 "&Longitud de edición fija"
   #define TXT11 "&Longitud de edición variable"
   #define TXT12 "Editor de texto ASCII ( %s ) Versión 1.0a"
#endif



__fastcall TEditorForm::TEditorForm(TComponent* Owner) : TForm(Owner)
{
#ifdef _IDIOMA_ESPANOL_

#else

#endif
}

// Al crear la forma
void __fastcall TEditorForm::FormCreate(TObject *Sender)
{
   cla.Inicializa_lista_archivos(ARCHIVO_CFG_EDITOR);
   Actualiza_lista();

   ARCHIVO = NULL;
   st_solo_lectura = false;
   SaveDialog->FileName = "";
   Titulo();
}


// Al cerrar la forma
void __fastcall TEditorForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   CanClose = false;   
   if (!st_solo_lectura) {
      if (Editor->Modified) {
          if (MessageBox(Handle,TXT05,TXT01,MB_YESNO + MB_ICONQUESTION)  == IDYES) {
             if (MessageBox(Handle,TXT06,TXT01,MB_YESNO + MB_ICONQUESTION) == IDYES) GrabaArchivo(Sender);
          } else return;
      }
   }
//   NCO.Activa_Borrado(PtrEditor);   
   CanClose = true;
}


// Al activar la forma
void __fastcall TEditorForm::FormActivate(TObject *Sender)
{
   Application->OnHint = OnHint;
   Application->OnIdle = OnIdle;
}

// Al desactivar la forma
void __fastcall TEditorForm::FormDeactivate(TObject *Sender)
{
   Application->OnIdle = NULL;
   Application->OnHint = NULL;
}


// OnHint
void  __fastcall TEditorForm::OnHint(TObject *Sender)
{
   BarraDeEstados->SimpleText = Application->Hint;
}

// OnIdle 
void  __fastcall TEditorForm::OnIdle(TObject *Sender, bool &Done)
{
   bool st;
   if (!st_solo_lectura) {
      st = Editor->SelLength > 0;
      // Activa o desactiva la opcion de Cortar
      MenuEdicionCortar->Enabled = st;
      IconoCortar->Enabled = st;
      PopMenuCortar->Enabled = st;
      // Activa o desactiva la opcion de Copiar
      MenuEdicionCopiar->Enabled = st; 
      IconoCopiar->Enabled = st;
      PopMenuCopiar->Enabled = st;
      // Activa o desactiva la opcion de Pegar 
      st = Clipboard()->HasFormat(CF_TEXT);
      MenuEdicionPegar->Enabled = st;
      IconoPegar->Enabled = st;
      PopMenuPegar->Enabled = st;
      // Activa o desactiva la opcion de Deshacer
      st = Editor->Modified;
      MenuEdicionDeshacer->Enabled = st;
      IconoDeshacer->Enabled = st;
      PopMenuDeshacer->Enabled = st;
      // Activa o desactiva la opcion de Grabar y Grabar como ...
      MenuArchivoGrabar->Enabled = st;
      IconoGrabar->Enabled = st;
   }
   // Activa o desactiva la opcion de Seleccionar todo
   st = Editor->Lines->Count > 0;
   MenuEdicionSeleccionaTodo->Enabled = st;
   PopMenuSeleccionarTodo->Enabled = st;
}
 

// Nuevo archivo en el editor
void __fastcall TEditorForm::NuevoArchivo(TObject *Sender)
{
   if (MessageBox(Handle,TXT07,TXT01,MB_YESNOCANCEL + MB_ICONQUESTION) == IDYES) {
      if (Editor->Modified) {
         int res = MessageBox(Handle,TXT06,TXT01,MB_YESNOCANCEL + MB_ICONQUESTION);
         if (res == IDYES) GrabaArchivo(Sender);
         if (res == IDCANCEL) return;
      }
      if (Editor->Lines->Count > 0) Editor->Clear();
      SaveDialog->FileName = "";
      Titulo();
   }
}

// Graba archivo del editor
void __fastcall TEditorForm::GrabaArchivo(TObject *Sender)
{
   if (Editor->Modified) {
      if (SaveDialog->FileName != "") {
         Editor->Lines->SaveToFile(SaveDialog->FileName);
         Editor->Modified = false;
         OpenDialog->FileName = SaveDialog->FileName;
      } else GrabaComoArchivo(Sender);
      Titulo();
   }
}

// Abre archivo del editor
void __fastcall TEditorForm::AbrirArchivo(TObject *Sender)
{
   bool ro = false;
   if (Editor->Modified) {
      int res = MessageBox(Handle,TXT06,TXT01,MB_YESNOCANCEL + MB_ICONQUESTION);
      if (res == IDYES) GrabaArchivo(Sender);
      if (res == IDCANCEL) return;
   }        
   OpenDialog->FileName = "";
   if (OpenDialog->Execute()) {
      if (Editor->Lines->Count > 0) Editor->Clear();
      // Revisa que el archivo exista y pueda ser leido
      if (access(OpenDialog->FileName.c_str(),04) == 0) {
         // Revisa si puede ser modificado
         if (access(OpenDialog->FileName.c_str(),02) != 0) ro = true;
         // Lee el archivo
         Editor->Lines->LoadFromFile(OpenDialog->FileName);
      }
      Editor_solo_lectura(ro);
//      NCO.Bitacora("Abrir archivo en editor",OpenDialog->FileName.c_str(),"");
      SaveDialog->FileName = OpenDialog->FileName;
      cla.Inserta_nombre(OpenDialog->FileName.c_str());
      Actualiza_lista();
   }
   Titulo();
}

// Graba archivo como ... 
void __fastcall TEditorForm::GrabaComoArchivo(TObject *Sender)
{
   SaveDialog->Title = "Graba como ...";
   if (SaveDialog->Execute()) {
      if (access(SaveDialog->FileName.c_str(),00) == 0) {
         if (access(SaveDialog->FileName.c_str(),02) == 0) {
             if (MessageBox(Handle,TXT08,TXT01,MB_YESNO + MB_ICONQUESTION) == IDYES) {
                Editor->Lines->SaveToFile(SaveDialog->FileName);
             }
         } else MessageBox(Handle,TXT09,TXT01,MB_OK + MB_ICONEXCLAMATION);
      } else Editor->Lines->SaveToFile(SaveDialog->FileName);
   }
   OpenDialog->FileName = SaveDialog->FileName;
   Editor->Modified = false;
   Titulo();
}

             
// Imprime el archivo
void __fastcall TEditorForm::MenuArchivoImprimirClick(TObject *Sender)
{
   Editor->Print(TITULO);
}

// Configura la impresora
void __fastcall TEditorForm::ConfiguraPRN(TObject *Sender)
{
   PrintDialog->Execute();
}

// Deshase los cambios en el editor                    
void __fastcall TEditorForm::DeshaseCambios(TObject *Sender)
{
   SendMessage(Editor->Handle,WM_UNDO,0,0);        
}


// Corta el texto seleccionado del editor
void __fastcall TEditorForm::CortaTexto(TObject *Sender)
{
   Editor->CutToClipboard();        
}

// Copia el texto del editor
void __fastcall TEditorForm::CopiaTexto(TObject *Sender)
{  
   Editor->CopyToClipboard();
}

// Pega texto al editor
void __fastcall TEditorForm::PegarTexto(TObject *Sender)
{
   Editor->PasteFromClipboard();       
}

// Seleciona todo el texto del editor
void __fastcall TEditorForm::SeleccionaTexto(TObject *Sender)
{
   Editor->SelectAll();        
}


// Copia todo el contenido
void __fastcall TEditorForm::MenuEdicionCopiarTodoClick(TObject *Sender)
{
   Editor->SelectAll();                
   Editor->CopyToClipboard();        
}


// El editor tiene longitud fija
void __fastcall TEditorForm::LongitudFija(TObject *Sender)
{
   if (Editor->WordWrap) {
      // Longitud variable
      Editor->WordWrap = false;
      Editor->ScrollBars = ssBoth;
      MenuConfigurarLongitudEdicionFija->Caption = TXT10;
    } else {
      // Longitud fija
      Editor->WordWrap = true;     
      Editor->ScrollBars = ssVertical;
      MenuConfigurarLongitudEdicionFija->Caption = TXT11;
   }
}

// El editor tiene longitud variable// Inserta fecha
void __fastcall TEditorForm::Insertafecha1Click(TObject *Sender)
{
   char fecha[20], fec_esc[50];
   Manipulador_archivos ma;
   Fechas_Horas fh;
   
   fh.Fecha_hoy(fecha);
   fh.Fecha_escrita(fecha, fec_esc, 1, 1);
   ma.Convierte_linea(fec_esc);
   Editor->SetSelTextBuf(fec_esc);
}

// Inserta hora
void __fastcall TEditorForm::Insertahora1Click(TObject *Sender)
{
   char tiempo[20];
   Manipulador_archivos ma;
   Fechas_Horas fh;
   
   fh.Hora_actual(tiempo, 1, 1);
   fh.Trim(tiempo);
   ma.Convierte_linea(tiempo);
   Editor->SetSelTextBuf(tiempo);
}

// Inserta fecha y hora
void __fastcall TEditorForm::Insertafechayhora1Click(TObject *Sender)
{
   char xcad[100];
   Manipulador_archivos ma;
   Fechas_Horas fh;
   
   fh.Fecha_y_hora(xcad, 1, 1);
   ma.Convierte_linea(xcad);
   Editor->SetSelTextBuf(xcad);
}

// Lee un archivo y lo inserta en la actual posición
void __fastcall TEditorForm::MenuBloqueLeerClick(TObject *Sender)
{
   OpenDialog1->FileName = "";
   if (OpenDialog1->Execute()) {
      char xcad[310];
      // Lee la configuracion del usuario
      Manipulador_archivos ma;
      ma.Parametros(OpenDialog1->FileName.c_str(),LEE_ARCHIVO,300);
      // Revisa que pueda abrir correctamente el archivo
      if (ma.Retorna_estado()) return;
      while(!ma.Retorna_estado()) {
         ma.Lee_linea(xcad);
         if (!ma.Retorna_estado()) {
            ma.Convierte_linea(xcad);
            Editor->SetSelTextBuf(xcad);
         }
      } 
   }
}


// Convierte el bloque en mayusculas
void __fastcall TEditorForm::MenuBloqueConvertirMayusculasClick(TObject *Sender)
{
   Cadenas c;
   int Size = Editor->SelLength; 
   Size++;                      
   char *Buffer = new char[Size];  
   Editor->GetSelTextBuf(Buffer,Size); 
   c.Convierte_mayusculas(Buffer);
   Editor->SetSelTextBuf(Buffer);        
   delete Buffer;
}

// Convierte el bloque en minusculas
void __fastcall TEditorForm::MenuBloqueConvertirminusculasClick(TObject *Sender)
{
   Cadenas c;
   int Size = Editor->SelLength; 
   Size++;                      
   char *Buffer = new char[Size];  
   Editor->GetSelTextBuf(Buffer,Size); 
   c.Convierte_minusculas(Buffer);
   Editor->SetSelTextBuf(Buffer);        
   delete Buffer;
}

// Configura el font del editor
void __fastcall TEditorForm::MenuConfigurarTipoLetraClick(TObject *Sender)
{
   FontDialog->Font = Editor->Font;
   if(FontDialog->Execute()) Editor->Font = FontDialog->Font;
}

// Cierra el editor
void __fastcall TEditorForm::SalirEditor(TObject *Sender)
{
   Close();
}


// Abre el archivo por omision
void TEditorForm::Abrir_archivo(char *arch, bool st_edicion)
{
   bool ro  = st_edicion;
   ARCHIVO = arch;
   
   OpenDialog->FileName = ARCHIVO;
   Editor->Clear();
   // Revisa si tiene nombre de archivo 
   if (arch[0]) {
      // Revisa que el archivo exista y pueda ser leido
      if (access(ARCHIVO,04) == 0) {
         // Revisa si puede ser modificado
         if (access(ARCHIVO,02) != 0) ro = true;
         // Lee el archivo
         Editor->Lines->LoadFromFile(OpenDialog->FileName);
      }
   }
   Editor_solo_lectura(ro);
   SaveDialog->FileName = OpenDialog->FileName;
   Show();
}


// El editor estara en modo de solo lectura
void TEditorForm::Editor_solo_lectura(bool st)
{  
   st_solo_lectura = st;
   Editor->ReadOnly = st; 
   if (st) {
     // Iconos
     IconoAbrir->Enabled = false;
     IconoGrabar->Enabled = false;
     IconoCortar->Enabled = false;
     IconoPegar->Enabled = false;
     IconoDeshacer->Enabled = false;
     // Menu
     MenuArchivoNuevo->Enabled = false;
     MenuArchivoAbrir->Enabled = false;
     MenuArchivoGrabar->Enabled = false;
     MenuEdicionDeshacer->Enabled = false;
     MenuEdicionCortar->Enabled = false;
     MenuEdicionPegar->Enabled = false;
//     Remplazar->Enabled = false;
     MenuEdicionInsertarFecha->Enabled = false;
     MenuEdicionInsertarHora->Enabled = false;
     MenuEdicionInsertarFechaHora->Enabled = false;
     MenuBloqueConvertirMayusculas->Enabled = false;
     MenuBloqueConvertirminusculas->Enabled = false;
     MenuBloqueLeer->Enabled = false;
     MenuBloqueGrabar->Enabled = false;
     MenuBloqueImprimir->Enabled = false;
     // Popup menu
     PopMenuDeshacer->Enabled = false;
     PopMenuCortar->Enabled = false;
     PopMenuPegar->Enabled = false;
     PopMenuInsertarFecha->Enabled = false;
     PopMenuInsertarHora->Enabled = false;
     PopMenuInsertarFechaHora->Enabled = false;
   } else {
     // Iconos   
     IconoAbrir->Enabled = true;
     IconoGrabar->Enabled = true;
     IconoCortar->Enabled = true;
     IconoPegar->Enabled = true;
     IconoDeshacer->Enabled = true;
     // Menu
     MenuArchivoNuevo->Enabled = true;
     MenuArchivoAbrir->Enabled = true;
     MenuArchivoGrabar->Enabled = true;
     MenuEdicionDeshacer->Enabled = true;
     MenuEdicionCortar->Enabled = true;
     MenuEdicionPegar->Enabled = true;
//     Remplazar->Enabled = true;
     MenuEdicionInsertarFecha->Enabled = true;
     MenuEdicionInsertarHora->Enabled = true;
     MenuEdicionInsertarFechaHora->Enabled = true;
     MenuBloqueConvertirMayusculas->Enabled = true;
     MenuBloqueConvertirminusculas->Enabled = true;
     MenuBloqueLeer->Enabled = true;
     MenuBloqueGrabar->Enabled = true;
     MenuBloqueImprimir->Enabled = true;
     // Popup menu
     PopMenuDeshacer->Enabled = true;
     PopMenuCortar->Enabled = true;
     PopMenuPegar->Enabled = true;
     PopMenuInsertarFecha->Enabled = true;
     PopMenuInsertarHora->Enabled = true;
     PopMenuInsertarFechaHora->Enabled = true;
   }
}

// Muestra el titulo de la aplicacion
void TEditorForm::Titulo(void)
{
   if (SaveDialog->FileName == "") strcpy(TITULO,TXT01);
    else sprintf(TITULO,TXT12, SaveDialog->FileName.c_str());
   Caption = TITULO;
}


void __fastcall TEditorForm::MenuEdicionBuscarClick(TObject *Sender)
{
    FindDialog->FindText = Editor->SelText;
    FindDialog->Execute();
}


void __fastcall TEditorForm::FindDialogFind(TObject *Sender)
{
    Show();
    TSearchTypes st;
    TFindDialog *cd;
    int newpos;

    if ((cd = dynamic_cast<TFindDialog *> (Sender)) == 0) MessageBox(NULL, TXT04, TXT02, MB_OK + MB_ICONSTOP);

    if (cd->Options.Contains(frMatchCase)) st << stMatchCase;
    if (cd->Options.Contains(frWholeWord)) st << stWholeWord;

    if (Editor->SelLength) Editor->SelStart += 1;

    newpos = Editor->FindText(cd->FindText, Editor->SelStart, Editor->Text.Length(), st);

    if (newpos != -1) {
        Editor->SelStart = newpos;
        Editor->SelLength = cd->FindText.Length();
     } else {
        MessageBox(NULL, TXT03, TXT02, MB_OK + MB_ICONINFORMATION);
        Editor->SelStart = 0;
    }
}


void __fastcall TEditorForm::ReplaceDialogReplace(TObject *Sender)
{
    if (Editor->SelLength == 0) FindDialogFind(Sender);
    else {
        Editor->SelText =  ReplaceDialog->ReplaceText;
        FindDialogFind(Sender);
    }

    if (ReplaceDialog->Options.Contains(frReplaceAll)) {
        while (Editor->SelLength !=0) {
           ReplaceDialogReplace(Sender);
        }
    }
}


void __fastcall TEditorForm::MenuEdicionRemplazarClick(TObject *Sender)
{
    ReplaceDialog->FindText = Editor->SelText;
    ReplaceDialog->Execute();
}


// Configura el color de la ventana
void __fastcall TEditorForm::MenuConfigurarColorVentanaClick(TObject *Sender)
{
   ColorDialog->Color = Editor->Color;
   if(ColorDialog->Execute()) Editor->Color = ColorDialog->Color;
}


void TEditorForm::Abrir_archivo_lista(const char *xarch)
{
   bool ro = false;
   if (Editor->Modified) {
      int res = MessageBox(Handle,TXT06,TXT01,MB_YESNOCANCEL + MB_ICONQUESTION);
      if (res == IDYES) GrabaArchivo(this);
      if (res == IDCANCEL) return;
   }        
   if(Editor->Lines->Count > 0) Editor->Clear();
   // Revisa que el archivo exista y pueda ser leido
   if (access(xarch,04) == 0) {
      // Revisa si puede ser modificado
      if (access(xarch,02) != 0) ro = true;
      // Lee el archivo
      Editor->Lines->LoadFromFile(xarch);
   }
   Editor_solo_lectura(ro);
//   NCO.Bitacora("Abrir archivo en editor",xarch,"");
   SaveDialog->FileName = xarch;
   cla.Inserta_nombre(xarch);
   Actualiza_lista();
   Titulo();
}

void TEditorForm::Actualiza_lista(void)
{
   char arch[MAXPATH];
   bool st;
   
   cla.Retorna_nombre(0,arch, st);
   Arch1->Caption = arch;
   Arch1->Visible = st;

   cla.Retorna_nombre(1,arch, st);
   Arch2->Caption = arch;
   Arch2->Visible = st;

   cla.Retorna_nombre(2,arch, st);
   Arch3->Caption = arch;
   Arch3->Visible = st;
    
   cla.Retorna_nombre(3,arch, st);
   Arch4->Caption = arch;
   Arch4->Visible = st;

   Separador->Visible = cla.Existe_almenos_uno();
}


void __fastcall TEditorForm::Arch1Click(TObject *Sender)
{
   Abrir_archivo_lista(Arch1->Caption.c_str());   
}

void __fastcall TEditorForm::Arch2Click(TObject *Sender)
{
   Abrir_archivo_lista(Arch2->Caption.c_str());
}

void __fastcall TEditorForm::Arch3Click(TObject *Sender)
{
   Abrir_archivo_lista(Arch3->Caption.c_str());
}

void __fastcall TEditorForm::Arch4Click(TObject *Sender)
{
   Abrir_archivo_lista(Arch4->Caption.c_str());   
}


void __fastcall TEditorForm::FormClose(TObject *Sender,TCloseAction &Action)
{
   Application->OnIdle = NULL;
   Application->OnHint = NULL;
   Free();        
}

