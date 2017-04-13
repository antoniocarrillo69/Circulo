// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en trámite 1999-2000
// Revisión  1.1-A


#include <vcl\vcl.h>
#include "Acecade.h"
#include "DefinicionEcuacion.hpp"
#include "ACX-WebBrowser.h"
#include "EnviarCorreo.h"
#pragma hdrstop


extern char *WWW;


//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TVAcercaDe::TVAcercaDe(TComponent* Owner) : TForm(Owner)
{
   // Nombre de la ecuación del sistema
   DefinicionEcuacion def_ecu;
   NombreSistema->Caption = def_ecu.Nombre_sistema;
   // Fecha de generacion de la versión
   FechaGeneracion->Caption = __DATE__;
   Label10->Caption = Application->Title;
   Caption = Application->Title;
}
//---------------------------------------------------------------------------

void __fastcall TVAcercaDe::Label10Click(TObject *Sender)
{
   TFormaWebBrowzer *vent = new TFormaWebBrowzer(this);
   if (vent) {
       vent->Parametros(WWW);
       vent->Show();
   }
}
//---------------------------------------------------------------------------

void __fastcall TVAcercaDe::Label15Click(TObject *Sender)
{
   TFormaWebBrowzer *vent = new TFormaWebBrowzer(this);
   if (vent) {
       vent->Parametros("http://www.dynamics.unam.edu/hcc");
       vent->Show();
   }
}
//---------------------------------------------------------------------------

void __fastcall TVAcercaDe::Label16Click(TObject *Sender)
{
   TFormaWebBrowzer *vent = new TFormaWebBrowzer(this);
   if (vent) {
       vent->Parametros("http://www.dynamics.unam.edu/acl");
       vent->Show();
   }
}
//---------------------------------------------------------------------------

void __fastcall TVAcercaDe::Label17Click(TObject *Sender)
{
   TFormaWebBrowzer *vent = new TFormaWebBrowzer(this);
   if (vent) {
       vent->Parametros("http://www.dynamics.unam.edu/DinamicaNoLineal/Integrantes/fol.html");
       vent->Show();
   }
}
//---------------------------------------------------------------------------

void __fastcall TVAcercaDe::Label18Click(TObject *Sender)
{
   TFormaWebBrowzer *vent = new TFormaWebBrowzer(this);
   if (vent) {
       vent->Parametros("http://www.fciencias.unam.mx/Docencia/PlantaAcademica/Directorio/GUGA531222/index.html");
       vent->Show();
   }
}
//---------------------------------------------------------------------------


void __fastcall TVAcercaDe::Label3Click(TObject *Sender)
{
   TFormaEnviarCorreo *vent = new TFormaEnviarCorreo(this);
   if (vent) {
       vent->Parametros("UserMail@dinamica1.fciencias.unam.mx","dinamica@www.dynamics.unam.edu",Application->Title.c_str());
       vent->ShowModal();
   }
}
//---------------------------------------------------------------------------

