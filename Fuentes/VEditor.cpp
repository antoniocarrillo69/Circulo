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
#include <io.h>
#pragma hdrstop

#include "VEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVentanaCaptura *VentanaCaptura;
//---------------------------------------------------------------------------
__fastcall TVentanaCaptura::TVentanaCaptura(TComponent* Owner) : TForm(Owner)
{
   st = false;
}
//---------------------------------------------------------------------------
void __fastcall TVentanaCaptura::Close1Click(TObject *Sender)
{
   RichEdit1->Lines->SaveToFile(NOMBRE_ARCHIVO);
   st = true;
   Close();        
}
//---------------------------------------------------------------------------

void TVentanaCaptura::Archivo_a_leer(const char *arch)
{
   strcpy(NOMBRE_ARCHIVO,arch);
   if (!access(NOMBRE_ARCHIVO,06)) RichEdit1->Lines->LoadFromFile(NOMBRE_ARCHIVO);
}

void TVentanaCaptura::Titulo(const char *tit)
{
   Caption = tit;
}

/*

Uso de esta ventana:

   bool st = false;
   TVentanaCaptura *vent_cap = new TVentanaCaptura(this);     <-- Declaración del objeto
   if(vent_cap) {
      vent_cap->Titulo("Select Tongues");                     <-- Titulo de la ventana
      vent_cap->Archivo_a_leer("C:\\CONFIG.DAT");             <-- Archivo de trabajo
      vent_cap->ShowModal();
      st = vent_cap->st;
      delete vent_cap;                                        <-- estado de salida
   }
   if (!st) return;
*/
