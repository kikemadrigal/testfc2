#include <stdio.h>
#include <string.h>
#include "fusion-c/header/newTypes.h"
#include "fusion-c/header/vars_msxDos.h"
#include "fusion-c/header/io.h"
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/VDP_GRAPH1.h"
#include "BJloadSC.h"

#define dirBaseTablaPatrones 	0x0000
#define dirBaseTablaNombres		0x1800
#define dirBaseTablaColor 		0x2000
#define TAM_TablaPatrones 		6144
#define TAM_TablaColor			6144
#define TAM_TablaNombres		768
#define Cabecera_Fichero		8

//unsigned char tablaPatrones[]; son 768 filas por 8 bytes cada fila
//unsigned char tablaColor[];
//unsigned char tablaNombres[];

//----------- VARIABLES GLOBALES -----------------
byte tab_Patrones_from_fichero[TAM_TablaPatrones];
byte tab_Color_from_fichero[TAM_TablaColor];
byte tab_Nombres_from_fichero[TAM_TablaNombres];


/********* DOS ERROR HANDLER ***************/
void errorDOS (unsigned char error) {
	switch (error) {
		case __INTER:	printf ("\n\rInternal error"); break;
		case _NORAM	:	printf ("\n\rNot Enough Memory"); break;
		case __IBDOS:	printf ("\n\rInvalid MSX-DOS Call"); break;
		case __IDRV:	printf ("\n\rInvalid drive"); break;
		case __IFNM:	printf ("\n\rInvalid filename"); break;
		case __IPATH:	printf ("\n\rInvalid pathname"); break;
		case __PLONG:	printf ("\n\rPathname too long"); break;
		case __NOFIL:	printf ("\n\rFile not found"); break;
		case __NODIR:	printf ("\n\rDirectory not found"); break;
		case __DRFUL:	printf ("\n\rRoot directory full"); break;
		case __DKFUL:	printf ("\n\rDisk full"); break;
		case __DUPF:	printf ("\n\rDuplicate filename"); break;
		case __DIRE:	printf ("\n\rInvalid directory move"); break;
		case __FILRO:	printf ("\n\rRead only file"); break;
		case __DINE:	printf ("\n\rDirectory not empty"); break;
		case __IATTR:	printf ("\n\rInvalid attributes"); break;
		case __DOT:		printf ("\n\rInvalid . or .. operation"); break;
		case __SYSX:	printf ("\n\rSystem file exists"); break;
		case __DIRX:	printf ("\n\rDirectory exists"); break;
		case __FILEX:	printf ("\n\rFile exists"); break;
		case __FOPEN:	printf ("\n\rFile already in use"); break;
		case __OV64K:	printf ("\n\rCannot transfer above 64K"); break;
		case __FILE:	printf ("\n\rFile allocation error"); break;
		case __EOF:		printf ("\n\rEnd of file"); break;
		case __ACCV:	printf ("\n\rFile access violation"); break;
		case __IPROC:	printf ("\n\rInvalid process id"); break; 
		case __NHAND:	printf ("\n\rNo spare file handles"); break;
		case __IHAND:	printf ("\n\rInvalid file handle"); break;
		case __NOPEN:	printf ("\n\rFile handle not open"); break;
		case __IDEV:	printf ("\n\rInvalid device operation"); break;
		case __IENV:	printf ("\n\rInvalid environment string"); break;
		case __ELONG:	printf ("\n\rEnvironment string too long"); break;
		case __IDATE:	printf ("\n\rInvalid date"); break;
		case _ITIME:	printf ("\n\rInvalid time"); break;
		case __RAMDX:	printf ("\n\rRAM disk (drive H:) already exists"); break;
		case __NRAMD:	printf ("\n\rRAM disk does not exist"); break;
		case __HDEAD:	printf ("\n\rFile handle has been deleted"); break;
		case __ISBFN:	printf ("\n\rInvalid sub-function number"); break;
	}
}

void crearSC2 (char *filename) {
	int file;

	printf("\n\rCreando fichero %s.\n\r", filename);
	Create (filename);
	file=Open(filename, O_WRONLY);
	Write(file, tablaPatrones, TAM_TablaPatrones);
	Write(file, tablaNombres, TAM_TablaNombres);
	Write(file, tablaColor, TAM_TablaColor);
	Close(file);
}

void leerSC2 (char *filename) {
	int file;	// Set a file handler variable
	byte buffer[Cabecera_Fichero];

	file=Open(filename, O_RDONLY);
	printf("\n\rFichero %s abierto.", filename);
	// Se salta la cabecera
	//Read(file, buffer, Cabecera_Fichero);	
	// Leyendo la tabla de patrones	
	Read(file, tab_Patrones_from_fichero, TAM_TablaPatrones);
	// Leyendo la tabla de nombres
	Read(file, tab_Nombres_from_fichero, TAM_TablaNombres);
	// Leyendo la tabla de colores
	Read(file, tab_Color_from_fichero, TAM_TablaColor);
	Close(file);
	printf("\n\rFichero %s leido.", filename);
}

void verificarGrabacion (void){
	unsigned int i;	

	// Comparando la version en memoria con la leida
	for (i=0; i<TAM_TablaPatrones; i++)
		if (tab_Patrones_from_fichero[i]!=tablaPatrones[i])
			printf ("\n\rError. Patrones no coinciden.");
	printf ("\n\r");
	// Comparando la version en memoria con la leida
	for (i=0; i<TAM_TablaNombres; i++)
		if (tab_Nombres_from_fichero[i]!=tablaNombres[i])
			printf ("\n\rError. Nombres no coinciden.");
	printf ("\n\r");
	for (i=0; i<TAM_TablaColor; i++) 
		if (tab_Color_from_fichero[i]!=tablaColor[i])
			printf ("\n\rError. Colores no coinciden.");
	printf ("\n\r");
}

void mostrarSC2 (){
	// Modo 2 graficos de alta resolucion
	Screen(2);	
	SetColors(1, 1, 1);	// pantalla en negro
	HideDisplay();
	// Tablas patrones, nombre (mapa) y color
	CopyRamToVram (&tab_Patrones_from_fichero[0], dirBaseTablaPatrones, TAM_TablaPatrones);
	CopyRamToVram (&tab_Nombres_from_fichero[0], dirBaseTablaNombres, TAM_TablaNombres);
	CopyRamToVram (&tab_Color_from_fichero[0], dirBaseTablaColor, TAM_TablaColor);
	ShowDisplay();
	WaitKey();
}

/***************** MAIN *******************/
void main(void) 
{
	char filename[]="4.sc2";
	
	Cls();
	printf ("\n\rVRAM size = %d KB", GetVramSize());
	printf ("\n\r");
	//_os_ver = 1;
	//--------- Creación del fichero desde BJloadSC.h -----------
	//StrCopy (filename, "loading.sc2");
	crearSC2 (filename);
	
	//--------- Leyendo fichero guardado -----------
	leerSC2 (filename);
	verificarGrabacion();

	mostrarSC2 ();
 
	// Vuelta al MSX-DOS
	Screen (0);
	SetColors(15, 4, 4);	// Colores iniciales
	Exit (0);

}