// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Direcci�n: Amsterdam 312 col. Hip�dromo Condesa
// Tel�fono:  55-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite 1999-2000
// Revisi�n  1.1-A

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ACX-WebBrowser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TFormaWebBrowzer *FormaWebBrowzer;



//---------------------------------------------------------------------------
__fastcall TFormaWebBrowzer::TFormaWebBrowzer(TComponent* Owner) : TForm(Owner)
{
}

void TFormaWebBrowzer::Parametros(const char *path)
{
   wchar_t buff[400];
   MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, path, -1, buff, sizeof(buff));
   CppWebBrowser1->Navigate(buff,0,0,0,0);
}

void __fastcall TFormaWebBrowzer::FormClose(TObject *Sender, TCloseAction &Action)
{
   Free();
}


/*
//////////////////////////////////////////////////////////////////////////////////////////////////
// Forma de utilizar esta forma
   TFormaWebBrowzer *vent = new TFormaWebBrowzer(this);
   if (vent) {
       vent->Parametros("http://dinamica1.fciencias.unam.mx/dinamicaNoLineal/Integrantes/ACL/Index.html");
       vent->Show();
   }
//////////////////////////////////////////////////////////////////////////////////////////////////
*/

