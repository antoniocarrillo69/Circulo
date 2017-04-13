//////////////////////////////////////////////////////////////////////////////////////////////
// CIRCULO WINDOWS C++ 1.0                                                                  //
//                                                                                          //
// An�lisis y Dise�o:                                                                       //
//                                                                                          //
// Humberto Andres Carrillo Calvet                                                          //
// Antonio Carrillo Ledesma                                                                 //
// Oscar Garc�a Regis                                                                       //
// Miguel Angel Mendoza Reyes                                                               //
// Fernando Alberto Ongay Larios                                                            //
//                                                                                          //
// Programaci�n:                                                                            //
//                                                                                          //
// Humberto Andres Carrillo Calvet                                                          //
// Antonio Carrillo Ledesma                                                                 //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////



#include <vcl.h>
#include "LMensaje.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

__fastcall TLMensajeForm::TLMensajeForm(TComponent* Owner) : TForm(Owner)
{
}

 

// Abre el archivo por omision
void TLMensajeForm::Abrir_archivo(const char *tit, const char *arch)
{
   TITULO = tit;
   ARCHIVO = arch;
   Caption = TITULO;
   Editor->Clear();
   try {
      if(arch[0]) Editor->Lines->LoadFromFile(ARCHIVO);
   } catch (...) {};
}


