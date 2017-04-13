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

#ifndef ParamGlobalesH
#define ParamGlobalesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Tabnotbk.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TVCap_General : public TForm
{
__published:	// IDE-managed Components
        TTabbedNotebook *TabbedNotebook1;
        TButton *Button1;
        TPanel *Panel2;
        TLabel *Label4;
        TEdit *EditR1;
        TPanel *Panel3;
        TPanel *Panel4;
        TStaticText *TituloCondicionInicialR;
        TEdit *EditR2;
        TLabel *Label10;
        TEdit *EditL2;
        TLabel *Label11;
        TEdit *EditL3;
        TStaticText *TituloCondicionInicialL;
        TEdit *EditL5;
        TEdit *EditL4;
        TStaticText *StaticText4;
        TCheckBox *CheckBoxL1;
        TCheckBox *CheckBoxR1;
        TLabel *Label1;
        TEdit *EditS1;
        TLabel *Label2;
        TEdit *EditS3;
        TStaticText *StaticText1;
        TEdit *EditS5;
        TCheckBox *CheckBoxS1;
        TEdit *EditS6;
        TStaticText *TituloCondicionInicialS;
        TEdit *EditS4;
        TLabel *Label3;
        TEdit *EditS2;
        TLabel *Label5;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall CheckBoxL1Click(TObject *Sender);
        void __fastcall CheckBoxS1Click(TObject *Sender);
        void __fastcall CheckBoxR1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        bool Aceptar;

        __fastcall TVCap_General(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVCap_General *VCap_General;
//---------------------------------------------------------------------------
#endif
