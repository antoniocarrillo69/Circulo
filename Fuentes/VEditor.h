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

#ifndef VEditorH
#define VEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TVentanaCaptura : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Close1;
        TMemo *RichEdit1;
        void __fastcall Close1Click(TObject *Sender);
private:	// User declarations
        char NOMBRE_ARCHIVO[300];
public:		// User declarations
        bool st;
        void Archivo_a_leer(const char *arch);
        void Titulo(const char *tit);
        __fastcall TVentanaCaptura(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVentanaCaptura *VentanaCaptura;
//---------------------------------------------------------------------------
#endif
