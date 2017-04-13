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




////////////////////////////////////////////////////////////////
// + Falta controlar si es posible escribir en la unidad indicada
// + Revisar si puede adicionar texto a un archivo existente
////////////////////////////////////////////////////////////////



#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "Man_arch.hpp"

#ifndef __PROGRAMACION_WINDOWS__
   #include <unistd.h>
   #define MAXPATH   1024
#else 
   #include <io.h>
   #include <string.h>
   #define W_OK      06
   #define R_OK      04
#endif
#pragma hdrstop




#define TABULADOR    8



// Posibles estados del manipulador de archivos
// (0)   Sin iniciacion
// (1)   Inicio de archivo
// (2)   Fin de archivo
// (80)  No se puede leer el archivo
// (81)  No se puede leer y grabar sobre el archivo
// (97)  Error al abrir 
// (98)  Error al leer
// (99)  Error al grabar

// Constructor del manipulador de archivos
Manipulador_archivos::Manipulador_archivos(void)
{
   Estado = 0;
   DELIMITADOR = Asigna_puntero(4);
   DELIMITADOR[0] = 13;
   DELIMITADOR[1] = 10;
   DELIMITADOR[2] = 0;
   ARCHIVO = NULL;
   RUTINA = NULL;
   ARCHIVO_TRABAJO = NULL;
}


// Destructor del manipulador de archivos
Manipulador_archivos::~Manipulador_archivos()
{
   // Cierra el archivo de trabajo
   if (ARCHIVO_TRABAJO) fclose(ARCHIVO_TRABAJO);
   if (ARCHIVO) delete []ARCHIVO;
   if (RUTINA) delete []RUTINA;
   delete []DELIMITADOR;
   Estado = 0;
}


// constructor del manipulador de archivos
void Manipulador_archivos::Parametros(const char *archivo, const int modo, const unsigned int lg_max, const char *rutina)
{
   MODO = modo;
   LG_MAX = lg_max;
      
   ARCHIVO = Asigna_puntero(archivo);
   RUTINA  = Asigna_puntero(rutina);
   // Establece el tipo de atributo para apertura del archivo
   char *ATRIBUTO;
   if (MODO == LEE_ARCHIVO)      ATRIBUTO = "rb";
   if (MODO == GRABA_ARCHIVO)    ATRIBUTO = "wb";
   if (MODO == ADICIONA_ARCHIVO) ATRIBUTO = "ab";   // "ab+"
   // Si no encuentra el archivo y el modo es de lectura manda mesage de error
   if (MODO == LEE_ARCHIVO) {
      if (access(ARCHIVO,R_OK)) {
         Estado = 80; // No se puede leer el archivo
      }   
   }
   if (MODO == ADICIONA_ARCHIVO) {
      // Revisa si se puede escribir y leer el archivo
      if (access(ARCHIVO,W_OK)) {
         Estado = 81; // No se puede leer y grabar sobre el archivo
      }
   }

   if (!Estado) {
      // Apertura del archivo
      ARCHIVO_TRABAJO = fopen(ARCHIVO,ATRIBUTO);
      if (!ARCHIVO_TRABAJO) {
         Estado = 97; // Error al abrir
      }   
   }
}


// Cuenta el numero de caracteres y lineas dentro del archivo si T_P es 1
// Si T_P es cero retorna el numero de caracteres reales
void Manipulador_archivos::Longitud_archivo(long &lg_archivo, long &nm_lineas, const int t_p)
{
   lg_archivo = nm_lineas = Estado = 1;
   rewind(ARCHIVO_TRABAJO);
   while (!feof(ARCHIVO_TRABAJO)) {
      car = (char)fgetc(ARCHIVO_TRABAJO);
      if (feof(ARCHIVO_TRABAJO)) break;
      if (ferror(ARCHIVO_TRABAJO)) {
         Estado = 98; // Error al leer
      }
      if (t_p) {
//         if (!car || car == 7 || car == 13 || car == 255) continue;
         if (!car || car == 7 || car == 13) continue;
         if (car == 9) {
            lg_archivo += TABULADOR;
            continue;
         }
         if (car == 10) nm_lineas ++;
      }
      lg_archivo++;
   }
   rewind(ARCHIVO_TRABAJO);
}



// Lee la siguiente linea del archivo, retornando la longitud de la linea
unsigned int Manipulador_archivos::Lee_linea(char *cadena)
{
   cadena[0] = 0;
   char *pcad = cadena;
   unsigned int longtmp = 0;
   while (!feof(ARCHIVO_TRABAJO)) {
      car = (char)fgetc(ARCHIVO_TRABAJO);
      if (feof(ARCHIVO_TRABAJO)) break;
      if (ferror(ARCHIVO_TRABAJO)) {
         Estado = 98; // Error al lear
      }
//      if (car == 0 || car == 7 || car == 13 || car == 255) continue;
      if (!car || car == 7 || car == 13) continue;
      if (car == 10 || longtmp >= LG_MAX) break;
      if (car == 9) {
         if (longtmp + TABULADOR < LG_MAX) {
            for (int i = 0; i < TABULADOR; i++) *pcad = 32, pcad++;
            longtmp += TABULADOR;
         }
         continue;
      }
      *pcad = car, pcad++, longtmp++;
   }
   *pcad = 0;
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
   return longtmp;
}


// Graba una cadena como una linea del archivo, retornando la longitud de esta
unsigned int Manipulador_archivos::Graba_linea(const char *cadena)
{
   unsigned int inttmp, longtmp;
   inttmp = strlen(cadena);
   if (inttmp) longtmp = fwrite(cadena,1,inttmp,ARCHIVO_TRABAJO);
   fwrite(DELIMITADOR,1,2,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
   return longtmp;
}

// Busca un numero de linea determinado en el archivo
unsigned int Manipulador_archivos::Busca_linea(const unsigned int linea)
{
   unsigned int longtmp = 1;
   rewind(ARCHIVO_TRABAJO);
   if (linea < 2) return 1;
   while (!feof(ARCHIVO_TRABAJO)) {
      car =(char)fgetc(ARCHIVO_TRABAJO);
      if (ferror(ARCHIVO_TRABAJO)) {
         Estado = 98; // Error al leer
      }
      if (car == 10) {
         if (longtmp >= linea) break;
          else longtmp++;
      }
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
   return longtmp;
}




// Lee el numero de caracteres indicados en N_C y los deja en la cadena CADENA, retornando  el numero de caracteres leidos
unsigned int Manipulador_archivos::Lee_caracteres(char *cadena, const unsigned int n_c)
{
   unsigned int longtmp = fread(cadena,1,(unsigned int) n_c,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 98; // Error al leer
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
   return longtmp;
}


// Graba una una cadena de caracteres, retornando el numero de caracteres grabados
unsigned int Manipulador_archivos::Graba_caracteres(const char *cadena, const unsigned int n_c)
{
   unsigned int longtmp = fwrite(cadena,1,n_c,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
   return longtmp;
}


// Lee un entero
void Manipulador_archivos::Lee_entero(int &num)
{
   fread(&inttmp,sizeof(inttmp),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 98; // Error al leer
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
   num = inttmp;
}

// Lee un entero sin signo
void Manipulador_archivos::Lee_entero(unsigned int &num)
{
   fread(&uinttmp,sizeof(uinttmp),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 98; // Error al leer
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
   num = uinttmp;
}

// Lee un caracter sin signo
void Manipulador_archivos::Lee_caracter(char &car)
{
   fread(&cartmp,1,1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 98; // Error al leer
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
   car = cartmp;
}


// Lee un entero largo
void Manipulador_archivos::Lee_entero_largo(long &num)
{
   fread(&longtmp,sizeof(longtmp),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 98; // Error al leer
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
   num = longtmp;
}

// Lee un flotante double
void Manipulador_archivos::Lee_double(double &num)
{
   fread(&num,sizeof(num),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 98; // Error al leer
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
}


// Lee un flotante double
void Manipulador_archivos::Lee_ldouble(long double &num)
{
   fread(&num,sizeof(num),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 98; // Error al leer
   }
   if (feof(ARCHIVO_TRABAJO)) Estado = 2;  // Fin de archivo
}


// Graba un entero
void Manipulador_archivos::Graba_entero(const int ent)
{
   fwrite(&ent,sizeof(ent),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
}

// Graba un entero largo
void Manipulador_archivos::Graba_entero_largo(const long int ent)
{
   fwrite(&ent,sizeof(ent),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
}


// Graba un entero sin signo
void Manipulador_archivos::Graba_entero(const unsigned int ent)
{
   fwrite(&ent,sizeof(ent),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
}

// Graba caracter sin signo
void Manipulador_archivos::Graba_caracter(const char car)
{
   fwrite(&car,1,1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
}

// Graba un flotante double 
void Manipulador_archivos::Graba_double(const double num)
{
   fwrite(&num,sizeof(num),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
}

// Graba un flotante long double 
void Manipulador_archivos::Graba_ldouble(const long double num)
{
   fwrite(&num,sizeof(num),1,ARCHIVO_TRABAJO);
   if (ferror(ARCHIVO_TRABAJO)) {
      Estado = 99; // Error al grabar
   }
}

#define LG_BUFF 10240
// Copia el archivo indicado como fuente al archivo objeto
// Retornando (0) Termino satisfactoriamente
//            (1) Error apertura de archivo fuente
//            (2) Error apertura de archivo objeto
int Manipulador_archivos::Copia_archivo(const char *archivo_fuente, const char *archivo_objeto)
{
   unsigned int lg;
   Manipulador_archivos Arc_lect;
   Arc_lect.Parametros(archivo_fuente,LEE_ARCHIVO,LG_BUFF,"COPIA ARCHIVO");
   if (Arc_lect.Retorna_estado()) return 1; // Error apertura de archivo fuente

   Manipulador_archivos Arc_grab;
   Arc_grab.Parametros(archivo_objeto,GRABA_ARCHIVO,LG_BUFF,"COPIA ARCHIVO");
   if (Arc_grab.Retorna_estado()) return 2; // Error apertura de archivo objeto

   char *buff = new char[LG_BUFF+3];
   while(Arc_lect.Retorna_estado() != 2) {
      lg = Arc_lect.Lee_caracteres(buff,LG_BUFF);
      Arc_grab.Graba_caracteres(buff,lg);
   }
   delete []buff;
   return 0; // Termino satisfactoriamente
}


// Concatena el archivo indicado como fuente al archivo objeto
// Retornando (0) Termino satisfactoriamente
//            (1) Error apertura de archivo fuente
//            (2) Error apertura de archivo objeto
int Manipulador_archivos::Concatena_archivo(const char *archivo_fuente, const char *archivo_objeto)
{
   unsigned int lg;
   Manipulador_archivos Arc_lect;
   Arc_lect.Parametros(archivo_fuente,LEE_ARCHIVO,LG_BUFF,"CONCATENA ARCHIVO");
   if (Arc_lect.Retorna_estado()) return 1; // Error apertura de archivo fuente

   Manipulador_archivos Arc_grab;
   Arc_grab.Parametros(archivo_objeto,ADICIONA_ARCHIVO,LG_BUFF,"CONCATENA ARCHIVO");
   if (Arc_grab.Retorna_estado()) return 2; // Error apertura de archivo objeto

   
   char *buff = new char[LG_BUFF+3];
   while(Arc_lect.Retorna_estado() != 2) {
      lg = Arc_lect.Lee_caracteres(buff,LG_BUFF);
      Arc_grab.Graba_caracteres(buff,lg);
   }
   delete []buff;
   return 0; // Termino satisfactoriamente
}

// Remueve el archivo especificado en el path, en caso de tener permisos de solo lectura es cambiado a archivo normal. Despues de borra el archivo, si tp es distinto de cero reescribe este para evitar su recuperación.
// Retornando (0) Termino satisfactoriamente
//            (1) No existe el archivo
//            (2) No se puede borrar el archivo
int Manipulador_archivos::Remueve_archivo(const char *arch, const int tp)
{
   int st = 0;

   // Reescribe el archivo para evitar su recuperación
   if(tp) {
      char *msg = "Error no se puede borrar el archivo:";
      int handle = creat(arch,O_CREAT);
      if (handle >= 0) {
         write(handle,msg,strlen(msg));
         close(handle);
      }
   }
   // Revisa si existe el archivo
   if (!access(arch,R_OK)) {
      // Revisa si tiene permiso para ser borrado
      if (access(arch,W_OK)) chmod(arch, S_IWRITE);
      // Borra el archivo
      if (unlink(arch)) st = 2; // no se puede borrar el archivo
   } else st = 1; // No existe el archivo
   return st;
}

// Renombra el archivo indicado como fuente al archivo objeto
// Retornando (0) Termino satisfactoriamente
//            (1) No pudo duplicar el archivo
int Manipulador_archivos::Renombra_archivo(const char *archivo_fuente, const char *archivo_objeto)
{
   if (!Copia_archivo(archivo_fuente, archivo_objeto)) {
       if (!Remueve_archivo(archivo_fuente, 0)) return 0; // termino correctamente
   }
   return 1; // No pudo duplicar el archivo
}

   
// Retorna un nombre de archivo con extención EXT
// La extención no incluye el punto
void Manipulador_archivos::Cambia_ext_path(const char *path, const char *ext, char *pathmod)
{
   char xpath[MAXPATH];
   int lg = Trim(path,xpath) -1;
   while(lg) {
     if(xpath[lg] != '.') {  
        if(xpath[lg] == '\\' || xpath[lg] == ':' || xpath[lg] == '/') lg = 0;
         else lg --;                         
     } else xpath[lg] = 0, lg = 0;
   }
   sprintf(pathmod,"%s.%s",xpath,ext);
}

// Retorna un nombre de archivo con extención EXT
// La extención no incluye el punto
void Manipulador_archivos::Cambia_ext_path(char *path, const char *ext)
{
   char xpath[MAXPATH];
   int lg = Trim(path,xpath) -1;
   while(lg) {
      if(xpath[lg] != 46) {
         if(xpath[lg] == 92 || xpath[lg] == 58) lg = 0;
          else lg --;
      } else xpath[lg] = 0, lg = 0;
   }
   sprintf(path,"%s.%s",xpath,ext);
}


// Construye una trayectoria
void Manipulador_archivos::Construye_trayectoria(const char *tray, const char *arch, const char *ext, char *xpath)
{  
   char tmpPath[MAXPATH+1], tmpArch[MAXPATH+1];

   Cambia_ext_path(arch, ext, tmpArch);
   sprintf(tmpPath,"%s\\%s",tray,tmpArch);
   memccpy(xpath,tmpPath,0,MAXPATH);
}

void Manipulador_archivos::Construye_trayectoria(const char *tray, const char *arch, char *xpath)
{  
   char tmpPath[MAXPATH+1];

   sprintf(tmpPath,"%s\\%s",tray,arch);
   memccpy(xpath,tmpPath,0,MAXPATH);
}

// Adiciona a una cadena con terminador nulo los caracteres LF y LN
void Manipulador_archivos::Convierte_linea(char *xcad)
{
   char del[4];
   del[0] = 13;
   del[1] = 10;
   del[2] = 0;
   strcat(xcad,del);
}

// Ajusta el nombre de archivo para visualizacion a la longitud indicada
void Manipulador_archivos::AjustaNombreArchivo(const char *cad, char *xcad, unsigned int lg)
{
   if (strlen(cad) < lg) memccpy(xcad,cad,0,lg);
    else {
//////////////////////////////////////////////////////////////////////////////////////    
//#error Falta hacer lo necesario para ajustar la visualización  [Primeros 10 caracteres] [...] [Final del texto]
      memccpy(xcad,cad,0,lg);
//////////////////////////////////////////////////////////////////////////////////////    
   }
}



/*
void Remueve_archivos(const char *files, const int tp);
// Remueve los archivos especificados en el path, en caso de tener permisos de solo
// lectura y/o oculto son cambiados a archivo normal. Despues de borra el archivo
// Si tp es (1) reescribe este para evitar su recuperaci¢n.

#include "r_esp.hpp"
#include "r_var.hpp"
#include "nucleo.hpp"


   #include <string.h>
   #include <stdio.h>
   #include <dos.h>
   #include <io.h>
   #include <dir.h>
   #include <sys\stat.h>




void Remueve_archivos(const char *files, const int tp)
{
   unsigned int attr, handle, na = 0, i = 0;
   char arch[MAXPATH];
   na = n_archivos_directorios(files,_A_ARCH);
   for(i = 0; i < na; i++) {
      nombre_archivo_directorio(files,_A_ARCH,!i,arch);
      arch[14] = 0;
      // Revisa los atributos del archivos
      if(_dos_getfileattr(arch,&attr) != 0) {
         // Quita atributo HIDEN
         if(attr & FA_HIDDEN) {
            attr &= ~FA_HIDDEN;
           _dos_setfileattr(arch,attr);
         }
         // Quita atributo de READ ONLY
         if(attr & FA_RDONLY) {
            attr &= ~FA_RDONLY;
           _dos_setfileattr(arch,attr);
         }
         // Reescribe el archivo
         if(tp) {
            handle = creat(arch,S_IREAD | S_IWRITE);
            write(handle,RUTINA,8);
            close(handle);
         }
      }
      // Borra el archivo
//      if(unlink(arch)) NCO->Ctrl_error(7,arch,RUTINA);
      unlink(arch)
   }
}

*/
   /*
   char *msg = "Error no se puede borrar el archivo:";
   int done, handle;
   unsigned int attr;
   struct ffblk ffblk;
   done = findfirst(files,&ffblk,FA_ARCH | FA_HIDDEN | FA_SYSTEM | FA_RDONLY);
   while(!done) {
      // Quita atributo HIDEN
      _dos_getfileattr(ffblk.ff_name,&attr);
      if(attr & FA_HIDDEN) {
         attr &= ~FA_HIDDEN;
        _dos_setfileattr(ffblk.ff_name,attr);
      }
      // Quita atributo de READ ONLY
      _dos_getfileattr(ffblk.ff_name,&attr);
      if(attr & FA_RDONLY) {
         attr &= ~FA_RDONLY;
        _dos_setfileattr(ffblk.ff_name,attr);
      }
      if(tp) {
         // Reescribe el archivo
         handle = creat(ffblk.ff_name,S_IREAD | S_IWRITE);
         write(handle,msg,8);
         close(handle);
      }
      // Borra el archivo
      if(unlink(ffblk.ff_name)) ctrl_err(7,ffblk.ff_name,"REMUEVE ARCHIVO");
      done = findnext(&ffblk);
   }
*/



