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


//---------------------------------------------------------------------------
#ifndef ACX-WebBrowserH
#define ACX-WebBrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//#include <NMHTML.hpp>
#include <OleCtrls.hpp>
#include <Buttons.hpp>
#include "SHDocVw_OCX.h"
//---------------------------------------------------------------------------
class TFormaWebBrowzer : public TForm
{
__published:	// IDE-managed Components
        TCppWebBrowser *CppWebBrowser1;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TFormaWebBrowzer(TComponent* Owner);
        void Parametros(const char *path);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormaWebBrowzer *FormaWebBrowzer;
//---------------------------------------------------------------------------
#endif
