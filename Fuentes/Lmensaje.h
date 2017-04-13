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



#ifndef LMensajeH
#define LMensajeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TLMensajeForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TMemo *Editor;
private:	// User declarations
    const char *ARCHIVO;
    const char *TITULO;

public:		// User declarations
    __fastcall TLMensajeForm(TComponent* Owner);

               // Abre el archivo por omision
    void       Abrir_archivo(const char *tit, const char *arch);
};
#endif
 
