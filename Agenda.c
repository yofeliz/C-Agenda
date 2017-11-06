////////////////////////////////////////////////////////////////////////
// PROYECTO AGENDA                                                    //
////////////////////////////////////////////////////////////////////////
// - Programa que gestiona un array de estructuras.                   //
// - Las estructuras se salvan y se cargan a/de disco.                //
// - El array de estructuras se carga completo en memoria.            //
// * Notas:                                                           //
//    · Si el primer caracter del DNI de cada estructura              //
//     es '\0' el registro no existe; y viceversa.                    //
// - Autor: David Diaz Pescador <david.diaz.job@gmail.com>            //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// FICHEROS CABECERA                                                  //
////////////////////////////////////////////////////////////////////////
#include <stdio.h>                                                    //
#include <stdlib.h>                                                   //
#include <conio.h>                                                    //
#include <string.h>                                                   //
#include <ctype.h>                                                    //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// DEFINES                                                            //
////////////////////////////////////////////////////////////////////////
#define T_MAX    100          // Tamanho maximo de lista              //
#define DATAFILE "DATOS.DAT"  // Nombre de fichero de datos           //
#define DNI      9            // Longitud maxima de DNI               //
#define NOMBRE   11           // Longitud maxima de NOMBRE            //
#define APELLIDO 16           // Longitud maxima de APELLIDO          //
#define EMAIL    21           // Longitud maxima de EMAIL             //
#define TELEFONO 10           // Longitud maxima de TELEFONO          //
#define O_X      10           // Coordenada X origen.                 //
#define O_Y      3            // Coordenada Y origen.                 //
#define ANCHO_V  30           // Ancho de las ventanas menu.          //
#define ALTO_V   14           // Alto de las ventanas menu.           //
#define ANCHO_M  60           // Ancho de las ventanas mensaje.       //
#define ALTO_M   3            // Alto de las ventanas mensaje.        //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// VARIABLES GLOBALES                                                 //
////////////////////////////////////////////////////////////////////////
int    cargada=0;  // Indicador de lista cargada                      //
char   buffer[21]; // Buffer de teclado (maximo)                      //
struct registro {  // Estructura de datos de lista                    //
   char dni[DNI];            // DNI (campo clave)                     //
   char nombre[NOMBRE];      // Nombre                                //
   char apellido[APELLIDO];  // Apellido                              //
   char email[EMAIL];        // E-mail                                //
   char tlf[TELEFONO];       // Telefono                              //
} lista[T_MAX];              // Variable global lista                 //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// DEFINICION DE FUNCIONES                                            //
////////////////////////////////////////////////////////////////////////
int      showMenu(void);     // Muestra el menu                       //
void     iniLista(void);     // Inicializa la lista                   //
int      searchLibre(void);  // Busca sitio libre en la lista         //
void     insRegistro(void);  // Inserta en la lista                   //
void     delRegistro(void);  // Borra de la lista                     //
void     modRegistro(void);  // Modifica de la lista                  //
void     showLista(void);    // Muestra la lista                      //
void     loadLista(void);    // Carga la lista                        //
void     saveLista(void);    // Salva la lista                        //
long int searchDni(char *);  // Busqueda de DNI                       //
void     orderLista(void);   // Ordenacion de la lista                //
void     square(void);       // "Pinta" los cuadros.                  //
void     section(char *);    // Mensaje en recuadro SECCION           //
void     message(char *);    // Mensaje en recuadro MENSAJE           //
void     showRegistro(int);  // Muestra un registro                   //
void     searchLista(void);  // Busca un registro en la lista         //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// main()                                                             //
////////////////////////////////////////////////////////////////////////
// - Función principal.                                               //
// * Devuelve valores al sistema.                                     //
////////////////////////////////////////////////////////////////////////
int main(void) {                                                      //
   char sel;                   // Caracter de seleccion               //
   _setcursortype(_NOCURSOR);  // Oculta el cursor                    //
   iniLista();                 // Inicializa lista                    //
   while(1) {                  // Bucle infinito                      //
      sel = showMenu();      // Obtiene la seleccion del menu         //
      switch(sel) {                                                   //
         case 49:            // Numero 1                              //
            insRegistro();   // Inserta en la lista                   //
            break;                                                    //
         case 50:            // Numero 2                              //
            modRegistro();   // Modifica en la lista                  //
            break;                                                    //
         case 51:            // Numero 3                              //
            delRegistro();   // Borra de la lista                     //
            break;                                                    //
         case 52:            // Numero 4                              //
            searchLista();   // Busca registro                        //
            break;                                                    //
         case 53:            // Numero 5                              //
            showLista();     // Muestra la lista                      //
            break;                                                    //
         case 54:            // Numero 6                              //
            saveLista();     // Salva la lista a disco                //
            break;                                                    //
         case 55:            // Numero 7                              //
            loadLista();     // Carga la lista de disco               //
            break;                                                    //
         case 48:            // Numero 0                              //
            clrscr();        // Borra la pantalla                     //
            exit(0);         // Sale del bucle y del programa         //
      }//switch                                                       //
   }//while                                                           //
}//main                                                               //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// showMenu()                                                         //
////////////////////////////////////////////////////////////////////////
// - Muestra el menu de seleccion de operacion.                       //
// * Devuelve la seleccion (int).                                     //
////////////////////////////////////////////////////////////////////////
int showMenu(void) {                                                  //
   int sel, key=49, ope=1;                                            //
   square();                // Borra la pantalla y pinta recuadros    //
   do {                                                               //
      section("MENU PRINCIPAL");                                      //
      textcolor(7);                                                   //
      message("Seleccione una opcion");                               //
      gotoxy(O_X+3,O_Y+2);  // Insertar registro                      //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("1");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==1) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("INSERTAR");                                            //
      textcolor(7);                                                   //
      textbackground(0);                                              //
      cprintf("  registro.");                                         //
      gotoxy(O_X+3,O_Y+3);  // Modificar registro                     //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("2");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==2) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("MODIFICAR");                                           //
      textcolor(7);                                                   //
      textbackground(0);                                              //
      cprintf(" registro.");                                          //
      gotoxy(O_X+3,O_Y+4);                                            //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("3");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==3) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("ELIMINAR");                                            //
      textcolor(7);                                                   //
      textbackground(0);                                              //
      cprintf("  registro.");                                         //
      gotoxy(O_X+3,O_Y+5);  // Buscar registro                        //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("4");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==4) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("BUSCAR");                                              //
      textcolor(7);                                                   //
      textbackground(0);                                              //
      cprintf("    registro.");                                       //
      gotoxy(O_X+3,O_Y+6);  // Listar registros                       //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("5");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==5) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("LISTAR");                                              //
      textcolor(7);                                                   //
      textbackground(0);                                              //
      cprintf("    registros.");                                      //
      gotoxy(O_X+3,O_Y+7);  // Salvar registros                       //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("6");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==6) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("SALVAR");                                              //
      textcolor(7);                                                   //
      textbackground(0);                                              //
      cprintf("    registros.");                                      //
      gotoxy(O_X+3,O_Y+8);  // Cargar registros                       //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("7");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==7) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("CARGAR");                                              //
      textcolor(7);                                                   //
      textbackground(0);                                              //
      cprintf("    registros.");                                      //
      gotoxy(O_X+3,O_Y+9);  // Salir                                  //
      textcolor(12);                                                  //
      cprintf("[");                                                   //
      textcolor(15);                                                  //
      cprintf("0");                                                   //
      textcolor(12);                                                  //
      cprintf("] ");                                                  //
      if(ope==8) {          // Se resalta la opcion                   //
         textcolor(0);                                                //
         textbackground(10);                                          //
      } else {              // Se normaliza                           //
         textcolor(10);                                               //
         textbackground(0);                                           //
      }//if-else                                                      //
      cprintf("Salir.");                                              //
      gotoxy(O_X+3,O_Y+11);  // Seleccion                             //
      textcolor(15);                                                  //
      textbackground(0);                                              //
      cprintf(" #");                                                  //
      textcolor(7);                                                   //
      cprintf(" Seleccion: ");                                        //
      textcolor(12);                                                  //
      cprintf("[ ]");                                                 //
      textcolor(7);                                                   //
      printf("\b\b");                                                 //
      key = getche();                                                 //
      while(kbhit())        // Vacia buffer de teclado                //
         key = getch();                                               //
      if(key!=48 && key!=49 && key!=50 && key!=51 &&                  //
         key!=52 && key!=53 && key!=54 && key!=55 &&                  //
         key!=72 && key!=80 && key!=13) {  // Teclas permitidas       //
         textcolor(12);                                               //
         message("! Opcion no valida !");                             //
         getch();                                                     //
         while(kbhit())     // Vacia buffer de teclado                //
            getch();                                                  //
      }//if                                                           //
      switch(key) {                                                   //
         case 48:            // Numero 0                              //
         case 49:            // Numero 1                              //
         case 50:            // Numero 2                              //
         case 51:            // Numero 3                              //
         case 52:            // Numero 4                              //
         case 53:            // Numero 5                              //
         case 54:            // Numero 6                              //
         case 55:            // Numero 7                              //
            sel = key;       // Seleccion = tecla pulsada             //
            break;                                                    //
         case 72:            // Cursor hacia arriba                   //
            if(ope==1)       // Inicio menu                           //
               ope = 8;      // Fin menu                              //
            else                                                      //
               ope--;        // Decrementar menu                      //
            break;                                                    //
         case 80:            // Cursor hacia abajo                    //
            if(ope==8)       // Fin menu                              //
               ope = 1;      // Inicio menu                           //
            else                                                      //
               ope++;        // Incrementar menu                      //
            break;                                                    //
         case 13:            // Enter                                 //
            switch(ope) {                                             //
               case 1:                                                //
                  sel = 49;  // Numero 1                              //
                  break;                                              //
               case 2:                                                //
                  sel = 50;  // Numero 2                              //
                  break;                                              //
               case 3:                                                //
                  sel = 51;  // Numero 3                              //
                  break;                                              //
               case 4:                                                //
                  sel = 52;  // Numero 4                              //
                  break;                                              //
               case 5:                                                //
                  sel = 53;  // Numero 5                              //
                  break;                                              //
               case 6:                                                //
                  sel = 54;  // Numero 6                              //
                  break;                                              //
               case 7:                                                //
                  sel = 55;  // Numero 7                              //
                  break;                                              //
               case 8:                                                //
                  sel = 48;  // Numero 01                             //
                  break;                                              //
            }//switch                                                 //
            break;                                                    //
      }//switch                                                       //
   } while(sel!=48 && sel!=49 && sel!=50 && sel!=51 &&                //
           sel!=52 && sel!=53 && sel!=54 && sel!=55 && sel!=13);      //
   return sel;                                                        //
}//showMenu                                                           //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// iniLista()                                                         //
////////////////////////////////////////////////////////////////////////
// - Inicializa el array de estructuras a '\0' [dni].                 //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void iniLista(void) {                                                 //
   register int i;                                                    //
   for(i=0; i<T_MAX; i++)                                             //
      lista[i].dni[0] = '\0';                                         //
}//iniLista                                                           //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// searchLibre()                                                      //
////////////////////////////////////////////////////////////////////////
// - Busca un sitio libre dentro de la lista, es decir,               //
//  busca una estructura de la lista que no este ocupada.             //
// * Devuelve la primera posicion vacia de la lista.                  //
// * En caso de estar completa, devuelve -1.                          //
////////////////////////////////////////////////////////////////////////
int searchLibre(void) {                                               //
   register int pos;                                                  //
   for(pos=0; lista[pos].dni[0]!='\0' && pos<T_MAX; pos++);           //
   if(pos==T_MAX)                                                     //
      return -1;                                                      //
   return pos;                                                        //
}//searchLibre                                                        //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// insRegistro()                                                      //
////////////////////////////////////////////////////////////////////////
// - Introduce un registro en la lista.                               //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void insRegistro(void) {                                              //
   int hueco, dniRep=-1, dniLong=0, nomLong=0,                        //
              apeLong=0, emaLong=0, tlfLong=0, error=0;               //
   char dni[DNI], nombre[NOMBRE]="\0",                                //
        apellido[APELLIDO]="\0", email[EMAIL]="\0",                   //
        tlf[TELEFONO]="\0";                                           //
   cargada = 1;                    // Lista no vacia                  //
   section("INSERTAR REGISTRO");                                      //
   if((hueco=searchLibre())==-1){  // Busca hueco                     //
      textcolor(12);                                                  //
      message("! Lista Completa !");                                  //
      getch();                                                        //
      while(kbhit())               // Vacia buffer de teclado         //
         getch();                                                     //
      return;                                                         //
   }//if                                                              //
   textcolor(7);                                                      //
   message("Introduzca el DNI");                                      //
   buffer[0] = DNI;                // Maximos caracteres escritos     //
   do {                            // El DNI es el campo clave        //
      if(dniRep!=-1) {             // DNI repetido                    //
         textcolor(12);                                               //
         message("! DNI Repetido !");                                 //
      }//if                                                           //
      if(error!=0) {               // DNI no valido                   //
         textcolor(12);                                               //
         message("! DNI no valido !");                                //
      }//if                                                           //
      gotoxy(O_X+33,O_Y+2);                                           //
      textcolor(12);                                                  //
      cprintf("<");                                                   //
      textcolor(10);                                                  //
      cprintf("DNI");                                                 //
      textcolor(12);                                                  //
      cprintf(">");                                                   //
      gotoxy(O_X+33,O_Y+3);                                           //
      textcolor(4);                                                   //
      cprintf("  [        ]");                                        //
      printf("\b\b\b\b\b\b\b\b\b");                                   //
      textcolor(15);                                                  //
      strcpy(dni, strupr(cgets(buffer)));                             //
      dniLong = strlen(dni);                                          //
      dniRep = searchDni(dni);     // Control de repeticion           //
   } while((error=dniLong!=DNI-1) || (dniRep!=-1));                   //
   strcpy(lista[hueco].dni, dni);  // Se asigna el valor              //
   textcolor(7);                                                      //
   message("Introduzca el NOMBRE");                                   //
   buffer[0] = NOMBRE;             // Maximos caracteres escritos     //
   do {                                                               //
      if(error!=0) {               // Nombre no valido                //
         textcolor(12);                                               //
         message("! NOMBRE no valido !");                             //
      }//if                                                           //
      gotoxy(O_X+33,O_Y+4);                                           //
      textcolor(12);                                                  //
      cprintf("<");                                                   //
      textcolor(10);                                                  //
      cprintf("NOMBRE");                                              //
      textcolor(12);                                                  //
      cprintf(">");                                                   //
      gotoxy(O_X+33,O_Y+5);                                           //
      textcolor(4);                                                   //
      cprintf("  [          ]");                                      //
      printf("\b\b\b\b\b\b\b\b\b\b\b");                               //
      textcolor(15);                                                  //
      strcpy(nombre, strupr(cgets(buffer)));                          //
      nomLong = strlen(nombre);                                       //
   } while((error=nomLong<=0) || (error=nomLong>NOMBRE-1));           //
   strcpy(lista[hueco].nombre, nombre);  // Se asigna el valor        //
   textcolor(7);                                                      //
   message("Introduzca el APELLIDO");                                 //
   buffer[0] = APELLIDO;           // Maximos caracteres escritos     //
   do {                                                               //
      if(error!=0) {               // Apellido no valido              //
         textcolor(12);                                               //
         message("! APELLIDO no valido !");                           //
      }//if                                                           //
      gotoxy(O_X+33,O_Y+6);                                           //
      textcolor(12);                                                  //
      cprintf("<");                                                   //
      textcolor(10);                                                  //
      cprintf("APELLIDO");                                            //
      textcolor(12);                                                  //
      cprintf(">");                                                   //
      gotoxy(O_X+33,O_Y+7);                                           //
      textcolor(4);                                                   //
      cprintf("  [               ]");                                 //
      printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");                     //
      textcolor(15);                                                  //
      strcpy(apellido, strupr(cgets(buffer)));                        //
      apeLong = strlen(apellido);                                     //
   } while((error=apeLong<=0) || (error=apeLong>APELLIDO-1));         //
   strcpy(lista[hueco].apellido, apellido);  // Se asigna el valor    //
   textcolor(7);                                                      //
   message("Introduzca el E-MAIL");                                   //
   buffer[0] = EMAIL;              // Maximos caracteres escritos     //
   do {                                                               //
      if(error!=0) {               // E-mail no valido                //
         textcolor(12);                                               //
         message("! E-MAIL no valido !");                             //
      }//if                                                           //
      gotoxy(O_X+33,O_Y+8);                                           //
      textcolor(12);                                                  //
      cprintf("<");                                                   //
      textcolor(10);                                                  //
      cprintf("E-MAIL");                                              //
      textcolor(12);                                                  //
      cprintf(">");                                                   //
      gotoxy(O_X+33,O_Y+9);                                           //
      textcolor(4);                                                   //
      cprintf("  [                    ]");                            //
      printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");                     //
      printf("\b\b\b\b\b");                                           //
      textcolor(15);                                                  //
      strcpy(email, strupr(cgets(buffer)));                           //
      emaLong = strlen(email);                                        //
   } while((error=emaLong<=0) || (error=emaLong>EMAIL-1));            //
   strcpy(lista[hueco].email, email);  // Se asigna el valor          //
   textcolor(7);                                                      //
   message("Introduzca el TELEFONO");                                 //
   buffer[0] = TELEFONO;           // Maximos caracteres permitidos   //
   do {                                                               //
      if(error!=0) {               // Telefono no valido              //
         textcolor(12);                                               //
         message("! TELEFONO no valido !");                           //
      }//if                                                           //
      gotoxy(O_X+33,O_Y+10);                                          //
      textcolor(12);                                                  //
      cprintf("<");                                                   //
      textcolor(10);                                                  //
      cprintf("TELEFONO");                                            //
      textcolor(12);                                                  //
      cprintf(">");                                                   //
      gotoxy(O_X+33,O_Y+11);                                          //
      textcolor(4);                                                   //
      cprintf("  [         ]\b\b\b\b\b\b\b\b\b\b");                   //
      textcolor(15);                                                  //
      strcpy(tlf, strupr(cgets(buffer)));                             //
      tlfLong = strlen(tlf);                                          //
   } while((error=tlfLong<=0) || (error=tlfLong>TELEFONO-1));         //
   strcpy(lista[hueco].tlf, tlf);  // Se asigna el valor              //
   textcolor(10);                                                     //
   message("Registro guardado en memoria");                           //
   orderLista();                   // Ordenacion de lista             //
   getch();                                                           //
   while(kbhit())                  // Vacia buffer de teclado         //
      getch();                                                        //
}//insRegistro                                                        //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// delRegistro()                                                      //
////////////////////////////////////////////////////////////////////////
// - Borra una entrada de la lista por su DNI.                        //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void delRegistro(void) {                                              //
   char dni[DNI];                                                     //
   buffer[0] = DNI;                   // Maximos caracteres escritos  //
   section("ELIMINAR REGISTRO");                                      //
   textcolor(7);                                                      //
   message("Introduzca DNI: [        ]");                             //
   printf("\b\b\b\b\b\b\b\b\b");                                      //
   strcpy(dni, cgets(buffer));                                        //
   if((strlen(dni))==DNI-1) {         // Longitud correcta            //
      long int pos;                                                   //
      if((pos=searchDni(dni))!=-1) {  // Encontro un DNI igual        //
         lista[pos].dni[0] = '\0';                                    //
         textcolor(10);                                               //
         message("Registro eliminado");                               //
      } else {                        // No encontro coincidencias    //
         textcolor(12);                                               //
         message("! Registro no existente !");                        //
      }//if-else                                                      //
   } else {                           // DNI no valido                //
      textcolor(12);                                                  //
      message("! DNI no valido !");                                   //
   }//else                                                            //
   getch();                                                           //
   while(kbhit())                     // Vacia buffer de teclado      //
      getch();                                                        //
}//delRegistro                                                        //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// modRegistro()                                                      //
////////////////////////////////////////////////////////////////////////
// - Modifica una entrada de la lista por su DNI.                     //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void modRegistro(void) {                                              //
   int hueco, dniLong=0, dniRep=-1, nomLong=0,                        //
              apeLong=0, emaLong=0, tlfLong=0, error=0, mod=0;        //
   char dni[DNI]="\0", nombre[NOMBRE]="\0",                           //
        apellido[APELLIDO]="\0", email[EMAIL]="\0",                   //
        tlf[TELEFONO]="\0";                                           //
   buffer[0] = DNI;  // Maximos caracteres permitidos                 //
   section("MODIFICAR REGISTRO");                                     //
   textcolor(7);                                                      //
   message("Introduzca DNI: [        ]");                             //
   printf("\b\b\b\b\b\b\b\b\b");                                      //
   strcpy(dni, cgets(buffer));                                        //
   if((strlen(dni))==DNI-1) {                                         //
      long int pos;                                                   //
      if((pos=searchDni(dni))!=-1) {  // Encontro un DNI igual        //
         message("Introduzca el DNI (ENTER para conservar)");         //
         do {                  // El DNI es el campo clave            //
            if(dniRep!=-1) {   // DNI repetido                        //
               textcolor(12);                                         //
               message("! DNI Repetido !");                           //
            }//if                                                     //
            if(error!=0) {     // DNI no valido                       //
               textcolor(12);                                         //
               message("! DNI no valido !");                          //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+2);                                     //
            textcolor(12);                                            //
            cprintf("<");                                             //
            textcolor(10);                                            //
            cprintf("DNI");                                           //
            textcolor(12);                                            //
            cprintf(">");                                             //
            gotoxy(O_X+33,O_Y+3);                                     //
            textcolor(4);                                             //
            cprintf("  [");                                           //
            textcolor(7);                                             //
            textcolor(15);                                            //
            printf("%s", lista[pos].dni);                             //
            textcolor(4);                                             //
            cprintf("]");                                             //
            printf("\b\b\b\b\b\b\b\b\b");                             //
            mod = getch();                                            //
            while(kbhit())                                            //
               mod = getch();  // Vacia buffer de teclado             //
            if(mod==13) {      // Se pulso un ENTER - Sin cambios     //
               strcpy(dni, lista[pos].dni);                           //
               break;                                                 //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+3);                                     //
            textcolor(4);                                             //
            cprintf("  [        ]");                                  //
            printf("\b\b\b\b\b\b\b\b\b");                             //
            textcolor(15);                                            //
            strcpy(dni, strupr(cgets(buffer)));                       //
            dniLong = strlen(dni);                                    //
            dniRep = searchDni(dni);   // Control de repeticion       //
         } while((error=dniLong!=DNI-1) || (dniRep!=-1));             //
         strcpy(lista[pos].dni, dni);  // Se asigna el valor          //
         textcolor(7);                                                //
         message("Introduzca el NOMBRE (ENTER para conservar)");      //
         buffer[0] = NOMBRE;           // Maximos caracteres escritos //
         do {                                                         //
            if(error!=0) {             // Nombre no valido            //
               textcolor(12);                                         //
               message("! NOMBRE no valido !");                       //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+4);                                     //
            textcolor(12);                                            //
            cprintf("<");                                             //
            textcolor(10);                                            //
            cprintf("NOMBRE");                                        //
            textcolor(12);                                            //
            cprintf(">");                                             //
            gotoxy(O_X+33,O_Y+5);                                     //
            textcolor(4);                                             //
            cprintf("  [");                                           //
            textcolor(7);                                             //
            textcolor(15);                                            //
            printf("%-10s", lista[pos].nombre);                       //
            textcolor(4);                                             //
            cprintf("]");                                             //
            printf("\b\b\b\b\b\b\b\b\b\b\b");                         //
            mod = getch();                                            //
            while(kbhit())     // Vacia buffer de teclado             //
               mod = getch();                                         //
            if(mod==13) {      // Se pulso un ENTER - Sin cambios     //
               strcpy(nombre, lista[pos].nombre);                     //
               break;                                                 //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+5);                                     //
            textcolor(4);                                             //
            cprintf("  [          ]");                                //
            printf("\b\b\b\b\b\b\b\b\b\b\b");                         //
            textcolor(15);                                            //
            strcpy(nombre, strupr(cgets(buffer)));                    //
            nomLong = strlen(nombre);                                 //
         } while((error=nomLong<=0) || (error=nomLong>NOMBRE-1));     //
         strcpy(lista[pos].nombre, nombre);  // Se asigna el valor    //
         textcolor(7);                                                //
         message("Introduzca el APELLIDO (ENTER para conservar)");    //
         buffer[0] = APELLIDO;         // Maximos caracteres escritos //
         do {                                                         //
            if(error!=0) {             // Apellido no valido          //
               textcolor(12);                                         //
               message("! APELLIDO no valido !");                     //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+6);                                     //
            textcolor(12);                                            //
            cprintf("<");                                             //
            textcolor(10);                                            //
            cprintf("APELLIDO");                                      //
            textcolor(12);                                            //
            cprintf(">");                                             //
            gotoxy(O_X+33,O_Y+7);                                     //
            textcolor(4);                                             //
            cprintf("  [");                                           //
            textcolor(7);                                             //
            textcolor(15);                                            //
            printf("%-15s", lista[pos].apellido);                     //
            textcolor(4);                                             //
            cprintf("]");                                             //
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");               //
            mod = getch();                                            //
            while(kbhit())     // Vacia buffer de teclado             //
               mod = getch();                                         //
            if(mod==13) {      // Se pulso un ENTER - Sin cambios     //
               strcpy(apellido, lista[pos].apellido);                 //
               break;                                                 //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+7);                                     //
            textcolor(4);                                             //
            cprintf("  [               ]");                           //
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");               //
            textcolor(15);                                            //
            strcpy(apellido, strupr(cgets(buffer)));                  //
            apeLong = strlen(apellido);                               //
         } while((error=apeLong<=0) || (error=apeLong>APELLIDO-1));   //
         strcpy(lista[pos].apellido, apellido);// Se asigna el valor  //
         textcolor(7);                                                //
         message("Introduzca el E-MAIL (ENTER para conservar)");      //
         buffer[0] = EMAIL;            // Maximos caracteres escritos //
         do {                                                         //
            if(error!=0) {             // E-mail no valido            //
               textcolor(12);                                         //
               message("! E-MAIL no valido !");                       //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+8);                                     //
            textcolor(12);                                            //
            cprintf("<");                                             //
            textcolor(10);                                            //
            cprintf("E-MAIL");                                        //
            textcolor(12);                                            //
            cprintf(">");                                             //
            gotoxy(O_X+33,O_Y+9);                                     //
            textcolor(4);                                             //
            cprintf("  [");                                           //
            textcolor(7);                                             //
            textcolor(15);                                            //
            printf("%-20s", lista[pos].email);                        //
            textcolor(4);                                             //
            cprintf("]");                                             //
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");               //
            printf("\b\b\b\b\b");                                     //
            mod = getch();                                            //
            while(kbhit())     // Vacia buffer de teclado             //
               mod = getch();                                         //
            if(mod==13) {      // Se pulso un ENTER - Sin cambios     //
               strcpy(email, lista[pos].email);                       //
               break;                                                 //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+9);                                     //
            textcolor(4);                                             //
            cprintf("  [                    ]");                      //
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");               //
            printf("\b\b\b\b\b");                                     //
            textcolor(15);                                            //
            strcpy(email, strupr(cgets(buffer)));                     //
            emaLong = strlen(email);                                  //
         } while((error=emaLong<=0) || (error=emaLong>EMAIL-1));      //
         strcpy(lista[pos].email, email);  // Se asigna el valor      //
         textcolor(7);                                                //
         message("Introduzca el TELEFONO (ENTER para conservar)");    //
         buffer[0] = TELEFONO;         // Maximos caracteres escritos //
         do {                                                         //
            if(error!=0) {             // Telefono no valido          //
               textcolor(12);                                         //
               message("! TELEFONO no valido !");                     //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+10);                                    //
            textcolor(12);                                            //
            cprintf("<");                                             //
            textcolor(10);                                            //
            cprintf("TELEFONO");                                      //
            textcolor(12);                                            //
            cprintf(">");                                             //
            gotoxy(O_X+33,O_Y+11);                                    //
            textcolor(4);                                             //
            cprintf("  [");                                           //
            textcolor(7);                                             //
            textcolor(15);                                            //
            printf("%-9s", lista[pos].tlf);                           //
            textcolor(4);                                             //
            cprintf("]");                                             //
            printf("\b\b\b\b\b\b\b\b\b\b");                           //
            mod = getch();                                            //
            while(kbhit())     // Vacia buffer de teclado             //
               mod = getch();                                         //
            if(mod==13) {      // Se pulso un ENTER - Sin cambios     //
               strcpy(tlf, lista[pos].tlf);                           //
               break;                                                 //
            }//if                                                     //
            gotoxy(O_X+33,O_Y+11);                                    //
            textcolor(4);                                             //
            cprintf("  [         ]");                                 //
            printf("\b\b\b\b\b\b\b\b\b\b");                           //
            textcolor(15);                                            //
            strcpy(tlf, strupr(cgets(buffer)));                       //
            tlfLong = strlen(tlf);                                    //
         } while((error=tlfLong<=0) || (error=tlfLong>TELEFONO-1));   //
         strcpy(lista[pos].tlf, tlf);  // Se asigna el valor          //
         orderLista();                                                //
         textcolor(10);                                               //
         message("Registro modificado");                              //
      } else {                         // No encontro coincidencias   //
         textcolor(12);                                               //
         message("! Registro no existente !");                        //
      }//if-else                                                      //
   } else {                           // DNI no valido                //
      textcolor(12);                                                  //
      message("! DNI no valido !");                                   //
   }//else                                                            //
   getch();                                                           //
   while(kbhit())                     // Vacia buffer de teclado      //
      getch();                                                        //
}//modRegistro                                                        //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// showLista()                                                        //
////////////////////////////////////////////////////////////////////////
// - Muestra el array de estructuras en pantalla.                     //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void showLista(void) {                                                //
   register int i, c=0;                                               //
   section("LISTAR REGISTROS");                                       //
   textcolor(7);                                                      //
   message("Utilice los cursores para desplazarse - (ESC = salir)");  //
   for(i=0; i<T_MAX; i++)    // Se recorre la lista                   //
      if(lista[i].dni[0] == '\0')                                     //
         c++;                                                         //
   if(c==T_MAX) {            // Lista vacia                           //
      textcolor(12);                                                  //
      message("Lista vacia");                                         //
      getch();                                                        //
      while(kbhit())         // Vacia buffer de teclado               //
         getch();                                                     //
      return;                                                         //
   }//if                                                              //
   for(i=0; i<T_MAX; i++)    // Se busca el primero con datos         //
      if(lista[i].dni[0] != '\0')                                     //
         break;                                                       //
   do {                                                               //
      showRegistro(i);       // Se muestra el registro                //
      c = getch();                                                    //
      while(kbhit())         // Vacia buffer de teclado               //
         c = getch();                                                 //
      if(c==75) {            // Cursor izquierda                      //
         do {                                                         //
            if(i!=0)                                                  //
               i--;          // Registro anterior                     //
            else                                                      //
               i = T_MAX-1;  // Ultimo registro                       //
         } while(lista[i].dni[0] == '\0');                            //
      }//if                                                           //
      if(c==77) {            // Cursor derecha                        //
         do {                                                         //
            if(i!=T_MAX-1)                                            //
               i++;          // Registro siguiente                    //
            else                                                      //
               i = 0;        // Primer registro                       //
         } while(lista[i].dni[0] == '\0');                            //
      }//if                                                           //
   } while(c != 27);         // Hasta que c == ESC                    //
}//showLista                                                          //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// loadLista()                                                        //
////////////////////////////////////////////////////////////////////////
// - Carga el array de estructuras desde disco.                       //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void loadLista(void) {                                                //
   FILE *fichero;                                                     //
   register int i;                                                    //
   section("CARGAR REGISTROS");                                       //
   if(cargada==1) {          // Lista en memoria no vacia             //
      int sel;                                                        //
      do {                                                            //
         textcolor(7);                                                //
         message("Sobreescribir la lista en memoria? (S/N) [ ]");     //
          printf("\b\b");                                             //
         sel = toupper(getche());                                     //
         while(kbhit())      // Vacia buffer de teclado               //
            sel = toupper(getch());                                   //
      } while(sel!=83 && sel!=78);                                    //
      if(sel==78)                                                     //
         return;             // NO sobreescribir                      //
   }//if                                                              //
   if((fichero=fopen(DATAFILE, "rb")) == NULL) {  // Abre fichero     //
      textcolor(12);                                                  //
      message("! No se puede leer desde disco !");                    //
      getch();                                                        //
      while(kbhit())         // Vacia buffer de teclado               //
         getch();                                                     //
      return;                                                         //
   }//if                                                              //
   iniLista();               // Inicializa lista                      //
   for(i=0; i<T_MAX; i++) {  // Lectura de datos                      //
      if((fread(&lista[i], sizeof(struct registro),                   //
               1, fichero)) != 1) {                                   //
         if(feof(fichero))   // Fin de fichero                        //
            break;                                                    //
         textcolor(12);                                               //
         message("! Error de lectura !");                             //
      }//if                                                           //
   }//for                                                             //
   if(ftell(fichero)!=0) {                                            //
      cargada = 1;           // Lista cargada                         //
      textcolor(10);                                                  //
      message("Lista cargada en memoria");                            //
   } else {                                                           //
      cargada = 0;           // Lista vacia                           //
      textcolor(7);                                                   //
      message("Fichero de datos vacio - Lista vacia");                //
   }//if-else                                                         //
   fclose(fichero);          // Cierra fichero                        //
   getch();                                                           //
   while(kbhit())            // Vacia buffer de teclado               //
      getch();                                                        //
}//loadLista                                                          //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// saveLista()                                                        //
////////////////////////////////////////////////////////////////////////
// - Salva el array de estructuras a disco.                           //
// - Solo se salvan aquellos registros que "existen", es decir,       //
//  aquellos en los que dni[0]!='\0'.                                 //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void saveLista(void) {                                                //
   FILE *fichero;                                                     //
   register int i;                                                    //
   section("SALVAR REGISTROS");                                       //
   if((fichero=fopen(DATAFILE, "rb")) != NULL) {  // Abre fichero     //
      fseek(fichero, 0, SEEK_END);  // Final de fichero               //
      if(ftell(fichero) != 0) {     // Fichero no vacio               //
         int sel;                                                     //
         do {                                                         //
            textcolor(7);                                             //
            message("Sobreescribir la lista en disco? (S/N) [ ]");    //
            printf("\b\b");                                           //
            sel = toupper(getche());                                  //
            while(kbhit())    // Vacia buffer de teclado              //
               sel = toupper(getch());                                //
         } while(sel!=83 && sel!=78);                                 //
         if(sel==78) {                                                //
            fclose(fichero);  // NO sobreescribir                     //
            return;                                                   //
         }//if                                                        //
      }//if                                                           //
      fclose(fichero);        // Cierra fichero                       //
   }//if                                                              //
   if((fichero=fopen(DATAFILE, "wb")) == NULL) {  // Abre fichero     //
         textcolor(12);                                               //
         message("! No se puede escribir en disco !");                //
         getch();                                                     //
         while(kbhit())       // Vacia buffer de teclado              //
            getch();                                                  //
         return;                                                      //
   }//if                                                              //
   for(i=0; i<T_MAX; i++) {   // Escritura de datos                   //
      if(lista[i].dni[0] != '\0')                                     //
         if((fwrite(&lista[i], sizeof(struct registro),               //
                    1, fichero)) != 1)                                //
            message("! Error de escritura !");                        //
   }//for                                                             //
   fclose(fichero);           // Cierra fichero                       //
   textcolor(10);                                                     //
   message("Lista guardada en disco");                                //
   getch();                                                           //
   while(kbhit())             // Vacia buffer de teclado              //
      getch();                                                        //
}//saveLista                                                          //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// searchDni()                                                        //
////////////////////////////////////////////////////////////////////////
// - Comprueba si el parametro DNI existe en la lista.                //
// * Valores devueltos:                                               //
//   * pos: posicion de registro coincidente.                         //
//   *  -1: no hubo coincidencia.                                     //
////////////////////////////////////////////////////////////////////////
long int searchDni(char *dni) {                                       //
   register int i;                                                    //
   for(i=0; i<T_MAX; i++)                                             //
      if(strcmp(lista[i].dni, dni) == 0)                              //
         return i;                                                    //
   return -1;                                                         //
}//searchDni                                                          //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// orderLista()                                                       //
////////////////////////////////////////////////////////////////////////
// - Ordena la lista en memoria alfabeticamente por el nombre.        //
// - Recorre la lista desde el principio hacia el final.              //
// - Algoritmo de Ordenacion: BURBUJA.                                //
// * No devuelve parametros.                                          //
////////////////////////////////////////////////////////////////////////
void orderLista(void) {                                               //
   register int i,j;                                                  //
   struct registro temp;                                              //
   for(i=T_MAX; i>0; i--) {  // Recorrido DESDE el final              //
      for(j=0; j<i-1; j++) { // Recorrido HACIA el final              //
         if((strcmp(lista[j+1].nombre,lista[j].nombre)<0) &&          //
            lista[j+1].nombre[0] != '\0') {  // Intercambio           //
            temp = lista[j+1];                                        //
            lista[j+1] = lista[j];                                    //
            lista[j] = temp;                                          //
         }//if                                                        //
      }//for                                                          //
   }//for                                                             //
}//orderLista                                                         //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// square()                                                           //
////////////////////////////////////////////////////////////////////////
// - Muestra por pantalla los recuadros que hacen de marcos.          //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void square(void) {                                                   //
   int i,j,k;                                                         //
   int y=O_Y;                                                         //
   clrscr();               // Borra la pantalla                       //
   textcolor(2);                                                      //
   gotoxy(O_X,y);                                                     //
   for(i=0; i<2; i++) {                                               //
      cprintf("É");        // Esquina superior izquierda              //
      for(j=0; j<ANCHO_V-2; j++)                                      //
         cprintf("Í");     // Borde superior                          //
      cprintf("»");        // Esquina superior derecha                //
   }//for                                                             //
   for(i=0; i<ALTO_V-2; i++) {                                        //
      gotoxy(O_X,++y);                                                //
      for(j=0; j<2; j++) {                                            //
         cprintf("º");     // Borde lateral izquierdo                 //
         for(k=0; k<ANCHO_V-2; k++)                                   //
            cprintf(" ");  // Interior                                //
         cprintf("º");     // Borde lateral derecho                   //
      }//for                                                          //
   }//for                                                             //
   gotoxy(O_X,++y);                                                   //
   for(i=0; i<2; i++) {                                               //
      cprintf("È");        // Esquina inferior izquierda              //
      for(j=0; j<ANCHO_V-2; j++)                                      //
         cprintf("Í");     // Borde inferior                          //
      cprintf("¼");        // Esquina inferior derecha                //
   }//for                                                             //
   gotoxy(O_X,++y);                                                   //
   cprintf("É");           // Esquina superior izquierda              //
   for(i=0; i<ANCHO_M-2; i++)                                         //
      cprintf("Í");        // Borde superior                          //
   cprintf("»");           // Esquina superior derecha                //
   gotoxy(O_X,++y);                                                   //
   cprintf("º");           // Borde lateral izquierdo                 //
   for(i=0; i<ANCHO_M-2; i++)                                         //
      cprintf(" ");        // Interior                                //
   cprintf("º");           // Borde lateral derecho                   //
   gotoxy(O_X,++y);                                                   //
   cprintf("È");           // Esquina inferior izquierda              //
   for(i=0; i<ANCHO_M-2; i++)                                         //
      cprintf("Í");        // Interior                                //
   cprintf("¼");           // Esquina inferior derecha                //
   gotoxy(O_X,++y);                                                   //
   cprintf("É");           // Esquina superior izquierda              //
   for(i=0; i<ANCHO_M-2; i++)                                         //
      cprintf("Í");        // Borde superior                          //
   cprintf("»");           // Esquina superior derecha                //
   gotoxy(O_X,++y);                                                   //
   cprintf("º");           // Borde lateral izquierdo                 //
   for(i=0; i<ANCHO_M-2; i++)                                         //
      cprintf(" ");        // Interior                                //
   cprintf("º");           // Borde lateral derecho                   //
   gotoxy(O_X,++y);                                                   //
   cprintf("È");           // Esquina inferior izquierda              //
   for(i=0; i<ANCHO_M-2; i++)                                         //
      cprintf("Í");        // Borde inferior                          //
   cprintf("¼");           // Esquina inferior derecha                //
   textcolor(11);                                                     //
   gotoxy(O_X+7,O_Y);                                                 //
   cprintf(" MENU PRINCIPAL ");                                       //
   gotoxy(O_X+40,O_Y);                                                //
   cprintf(" REGISTRO ");                                             //
   gotoxy(20,23);                                                     //
   gotoxy(O_X+1,O_Y+14);                                              //
   cprintf(" SELECCION ");                                            //
   gotoxy(O_X+1,O_Y+17);                                              //
   cprintf(" MENSAJE ");                                              //
}//square                                                             //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// section()                                                          //
////////////////////////////////////////////////////////////////////////
// - Muestra un mensaje en el centro del recuadro SECCION.            //
// - Antes de mostrarlo, borra el contenido del recuadro.             //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void section(char *mensaje) {                                         //
   int x;                                                             //
   gotoxy(O_X+1,O_Y+15);                                              //
   for(x=0; x<58; x++)                                                //
      printf(" ");                                                    //
   x=(((ANCHO_M-2) - strlen(mensaje)) / 2) + O_X+1;  // Centrado      //
   gotoxy(x,O_Y+15);                                                  //
   textcolor(14);                                                     //
   cprintf("%s", mensaje);                                            //
}//section                                                            //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// message()                                                          //
////////////////////////////////////////////////////////////////////////
// - Muestra un mensaje en el recuadro MENSAJE.                       //
// - Antes de mostrarlo, borra el contenido del cuadro.               //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void message(char *mensaje) {                                         //
   int x;                                                             //
   gotoxy(O_X+1,O_Y+18);                                              //
   for(x=0; x<58; x++)                                                //
      printf(" ");                                                    //
   x=(((ANCHO_M-2) - strlen(mensaje)) / 2) + O_X+1;  // Centrado      //
   gotoxy(x,O_Y+18);                                                  //
   cprintf("%s", mensaje);                                            //
}//message                                                            //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// showRegistro()                                                     //
////////////////////////////////////////////////////////////////////////
// - Muestra un registro de la lista a partir de la posicion.         //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void showRegistro(int pos) {                                          //
   gotoxy(O_X+33,O_Y+2);                                              //
   textcolor(12);                                                     //
   cprintf("<");                                                      //
   textcolor(10);                                                     //
   cprintf("DNI");                                                    //
   textcolor(12);                                                     //
   cprintf(">");                                                      //
   gotoxy(O_X+33,O_Y+3);                                              //
   printf("            ");                                            //
   gotoxy(O_X+33,O_Y+3);                                              //
   textcolor(4);                                                      //
   cprintf("  [");                                                    //
   textcolor(15);                                                     //
   cprintf("%s", lista[pos].dni);                                     //
   textcolor(4);                                                      //
   cprintf("]");                                                      //
   gotoxy(O_X+33,O_Y+4);                                              //
   textcolor(12);                                                     //
   cprintf("<");                                                      //
   textcolor(10);                                                     //
   cprintf("NOMBRE");                                                 //
   textcolor(12);                                                     //
   cprintf(">");                                                      //
   gotoxy(O_X+33,O_Y+5);                                              //
   printf("              ");                                          //
   gotoxy(O_X+33,O_Y+5);                                              //
   textcolor(4);                                                      //
   cprintf("  [");                                                    //
   textcolor(15);                                                     //
   cprintf("%s", lista[pos].nombre);                                  //
   textcolor(4);                                                      //
   cprintf("]");                                                      //
   gotoxy(O_X+33,O_Y+6);                                              //
   textcolor(12);                                                     //
   cprintf("<");                                                      //
   textcolor(10);                                                     //
   cprintf("APELLIDO");                                               //
   textcolor(12);                                                     //
   cprintf(">");                                                      //
   gotoxy(O_X+33,O_Y+7);                                              //
   printf("                   ");                                     //
   gotoxy(O_X+33,O_Y+7);                                              //
   textcolor(4);                                                      //
   cprintf("  [");                                                    //
   textcolor(15);                                                     //
   cprintf("%s", lista[pos].apellido);                                //
   textcolor(4);                                                      //
   cprintf("]");                                                      //
   gotoxy(O_X+33,O_Y+8);                                              //
   textcolor(12);                                                     //
   cprintf("<");                                                      //
   textcolor(10);                                                     //
   cprintf("E-MAIL");                                                 //
   textcolor(12);                                                     //
   cprintf(">");                                                      //
   gotoxy(O_X+33,O_Y+9);                                              //
   printf("                        ");                                //
   gotoxy(O_X+33,O_Y+9);                                              //
   textcolor(4);                                                      //
   cprintf("  [");                                                    //
   textcolor(15);                                                     //
   cprintf("%s", lista[pos].email);                                   //
   textcolor(4);                                                      //
   cprintf("]");                                                      //
   gotoxy(O_X+33,O_Y+10);                                             //
   textcolor(12);                                                     //
   cprintf("<");                                                      //
   textcolor(10);                                                     //
   cprintf("TELEFONO");                                               //
   textcolor(12);                                                     //
   cprintf(">");                                                      //
   gotoxy(O_X+33,O_Y+11);                                             //
   printf("             ");                                           //
   gotoxy(O_X+33,O_Y+11);                                             //
   textcolor(4);                                                      //
   cprintf("  [");                                                    //
   textcolor(15);                                                     //
   cprintf("%s", lista[pos].tlf);                                     //
   textcolor(4);                                                      //
   cprintf("]");                                                      //
}//showRegistro                                                       //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// searchLista()                                                      //
////////////////////////////////////////////////////////////////////////
// - Busca un registro en la lista a partir del DNI.                  //
// * No devuelve valores.                                             //
////////////////////////////////////////////////////////////////////////
void searchLista(void) {                                              //
   char dni[DNI];                                                     //
   buffer[0] = DNI;                   // Maximos caracteres escritos  //
   section("BUSCAR REGISTRO");                                        //
   textcolor(7);                                                      //
   message("Introduzca DNI: [        ]");                             //
   printf("\b\b\b\b\b\b\b\b\b");                                      //
   strcpy(dni, cgets(buffer));                                        //
   if((strlen(dni))==DNI-1) {         // Longitud correcta            //
      long int pos;                                                   //
      if((pos=searchDni(dni))!=-1) {  // Encontro un DNI igual        //
         textcolor(10);                                               //
         message("Registro encontrado");                              //
         showRegistro(pos);           // Muestra el registro          //
      } else {                        // No encontro coincidencias    //
         textcolor(12);                                               //
         message("! Registro no existente !");                        //
      }//if-else                                                      //
   } else {                           // DNI no valido                //
      textcolor(12);                                                  //
      message("! DNI no valido !");                                   //
   }//else                                                            //
   getch();                                                           //
   while(kbhit())                     // Vacia buffer de teclado      //
      getch();                                                        //
}//searchLista                                                        //
////////////////////////////////////////////////////////////////////////