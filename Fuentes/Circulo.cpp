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
#pragma hdrstop
USERES("Circulo.res");
USEFORM("VentanaPrincipal.cpp", FormaPrincipal);
USEUNIT("FuncionDisparo.cpp");
USEUNIT("DefiniciónEcuación.cpp");
USEUNIT("Sincronizaciones.cpp");
USEUNIT("NumeroRotacion.cpp");
USEUNIT("ExponenteLyapunov.cpp");
USEFORM("VentanaBifurcaciones.cpp", FormaBifurcaciones);
USEFORM("VentanaCirculo.cpp", FormaCirculo);
USEFORM("VentanaToro.cpp", FormaToro);
USEFORM("VentanaLevantamientos.cpp", FormaLevantamientos);
USEFORM("Acecade.cpp", VAcercaDe);
USEFORM("ParamBifurcaciones.cpp", VCap_Bifurcaciones);
USEFORM("ParamToro.cpp", VCap_Toro);
USEFORM("ParamLevantamientos.cpp", VCap_Levantamientos);
USEFORM("ParamCirculo.cpp", VCap_Circunferencia);
USEFORM("ParamGlobales.cpp", VCap_General);
USEUNIT("..\Libreria\V_pixel.cpp");
USEFORM("..\Libreria\FORMAS\Ayuda.cpp", AyudaForm);
USEFORM("..\Libreria\FORMAS\Editor.cpp", EditorForm);
USEUNIT("..\Libreria\ctrl_l_a.cpp");
USEUNIT("..\Libreria\Cadenas.cpp");
USEUNIT("..\Libreria\Fechas.cpp");
USEUNIT("..\Libreria\fechora.cpp");
USEUNIT("..\Libreria\Man_arch.cpp");
USEUNIT("..\Libreria\Tiempo.cpp");
USEFORM("..\Libreria\FORMAS\ACX-WebBrowser.cpp", FormaWebBrowzer);
USEFORM("..\Libreria\FORMAS\EnviarCorreo.cpp", FormaEnviarCorreo);
USEFORM("..\Libreria\FORMAS\Lmensaje.cpp", LMensajeForm);
USEFORM("..\Libreria\FORMAS\VEditor.cpp", VentanaCaptura);
USEFORM("ParamLenguas.cpp", VCap_Lenguas);
USEFORM("VentanaLenguas.cpp", FormaLenguas);
USEFORM("VentanaEscalera.cpp", FormaEscalera);
USEFORM("ParamEscalera.cpp", VCap_Escalera);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "";
                 Application->CreateForm(__classid(TFormaPrincipal), &FormaPrincipal);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

