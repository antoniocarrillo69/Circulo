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

#ifndef ParamLenguasH
#define ParamLenguasH
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
class TVCap_Lenguas : public TForm
{
__published:	// IDE-managed Components
        TTabbedNotebook *TabbedNotebook1;
        TPanel *Panel1;
        TPanel *Panel2;
        TGroupBox *GroupBox2;
        TLabel *Label6;
        TLabel *Label7;
        TGroupBox *GroupBox1;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *EditHMin;
        TEdit *EditHMax;
        TEdit *EditVMin;
        TEdit *EditVMax;
        TPanel *Panel4;
        TLabel *Label8;
        TButton *Button1;
        TColorDialog *ColorDialog;
        TLabel *Label2;
        TEdit *EditA1;
        TStaticText *StaticText1;
        TListBox *ListBoxParametros;
        TStaticText *NombreParametro;
        TEdit *EditValorParametro;
        TLabel *Label1;
        TEdit *EditA3;
        TEdit *EditA5;
        TEdit *EditA6;
        TEdit *EditA7;
        TLabel *Label3;
        TLabel *Label9;
        TLabel *Label10;
        TEdit *EditA2;
        TLabel *Label11;
        TEdit *EditA4;
        TPanel *Panel3;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        TComboBox *ComboBox1;
        TComboBox *ComboBox2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ListBoxParametrosClick(TObject *Sender);
        void __fastcall EditValorParametroKeyPress(TObject *Sender,
          char &Key);
private:	// User declarations
public:		// User declarations
        bool Aceptar;

        __fastcall TVCap_Lenguas(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
 
