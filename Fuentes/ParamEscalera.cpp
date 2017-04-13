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

#include <vcl\vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ParamEscalera.h"
#pragma hdrstop


//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

// Constructor de la ventana
__fastcall TVCap_Escalera::TVCap_Escalera(TComponent* Owner) : TForm(Owner)
{
   Aceptar = false;
}

// Al presionar el boton de aceptar
void __fastcall TVCap_Escalera::Button1Click(TObject *Sender)
{
   // Valida que sean distintos
   Aceptar = true;
   Close();        
}

// Activa la captura del parámetro seleccionado
void __fastcall TVCap_Escalera::ListBoxParametrosClick(TObject *Sender)
{
   unsigned int Log_max_param = 6; // longitud máxima del nombre del parámetro
   unsigned int i1, i2;
   int i;
   for (i = 0; i < ListBoxParametros->Items->Count; i++) {
       if (ListBoxParametros->Selected[i]) {
           char xcad[100], xcad1[100];
           // Nombre del parámetro
           strcpy(xcad1,ListBoxParametros->Items->Strings[i].c_str());
           for (i1 = 0; i1 < Log_max_param; i1++) { 
               if (xcad1[i1] != ' ') xcad[i1] = xcad1[i1];
                else break;
           }
           xcad[i1] = ':';
           xcad[i1+1] = 0;
           NombreParametro->Caption = xcad;
           // Valor del parámetro
           for (i2 = 0, i1 = (Log_max_param + 3); i1 < strlen(xcad1); i2++, i1++) { 
               xcad[i2] = xcad1[i1];
           }
           xcad[i2] = 0;
           EditValorParametro->Text = xcad;
           // Se hace visible el nombre y valor del parámetro
           NombreParametro->Visible = true;
           EditValorParametro->Visible = true;
           break;
       }
   }
}

// Cambia el valor del parámetro activo
void __fastcall TVCap_Escalera::EditValorParametroKeyPress(TObject *Sender, char &Key)
{
   if (Key == VK_RETURN) {
      Key = 0;
      PostMessage(Handle, WM_NEXTDLGCTL,0,0);
      char xcad[100], xcad1[100];
      strcpy(xcad1,NombreParametro->Caption.c_str());
      int i, lg;
      lg = strlen(xcad1);
      xcad1[lg-1] = 0;
      sprintf(xcad,"%-6s = %1.9Lf",xcad1,_atold(EditValorParametro->Text.c_str()));
      for (i = 0; i < ListBoxParametros->Items->Count; i++) {
          if (ListBoxParametros->Selected[i]) ListBoxParametros->Items->Strings[i] = xcad; 
      }    
      NombreParametro->Visible = false;
      EditValorParametro->Visible = false;     
   }
}



