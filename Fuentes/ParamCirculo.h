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

#ifndef ParamCirculoH
#define ParamCirculoH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Tabnotbk.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TVCap_Circunferencia : public TForm
{
__published:	// IDE-managed Components
        TTabbedNotebook *TabbedNotebook1;
        TPanel *Panel1;
        TPanel *Panel3;
        TCheckBox *CheckBox1;
        TCheckBox *CheckBox2;
        TPanel *Panel5;
        TPanel *Panel6;
        TPanel *Panel4;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *Edit8;
        TEdit *Edit9;
        TGroupBox *GroupBox3;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TButton *Button1;
        TColorDialog *ColorDialog;
        TCheckBox *CheckBox5;
        TPanel *Panel9;
        TStaticText *StaticText1;
        TListBox *ListBoxParametros;
        TStaticText *NombreParametro;
        TEdit *EditValorParametro;
        TCheckBox *CheckBox3;
        TCheckBox *CheckBox4;
        TPanel *Panel7;
        TPanel *Panel8;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Panel5Click(TObject *Sender);
        void __fastcall Panel6Click(TObject *Sender);
        void __fastcall Panel9Click(TObject *Sender);
        void __fastcall ListBoxParametrosClick(TObject *Sender);
        void __fastcall EditValorParametroKeyPress(TObject *Sender,
          char &Key);
        void __fastcall Panel7Click(TObject *Sender);
        void __fastcall Panel8Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        bool Aceptar;

        __fastcall TVCap_Circunferencia(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
 
