#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/VDP_GRAPH1.h"
//Aquí se declaran constantes para poderlas utilizar
#include "fusion-c/header/io.h"
//Aquí se declaran los nuevos tipos de datos con typedef=byte, int8, etc, ver archivo
#include "fusion-c/header/newTypes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void leerSC2 (char *filename);
void mostrarSC2 ();
//static char my_interrupt( void );
static unsigned int count = 0;



void main(void){
	Screen(1);
	//PutText(1,1,"Loading ...",0);
	

	//InitInterruptHandler();
	//SetInterruptHandler( my_interrupt );
	
	while( true ) {
		count++;
		printf("%i\n\r",count);
    	//DisableInterrupt();
		if(count==1){
			//leerSC2("1.sc2");
			//mostrarSC2();
			printf("Has entrado en 2\n\r");
		}
		if(count==80){
			//Cls();
			//leerSC2("2.sc2");
			//mostrarSC2();
			printf("Has entrado en 3\n\r");
			count=0;
		}


    //EnableInterrupt();
  	}
  	//EndInterruptHandler();

	


	
	/*
	
	Cls();
	Beep();
	leerSC2("2.sc2");
	mostrarSC2();

		

	Cls();
	Beep();
	leerSC2("3.sc2");
	mostrarSC2();



	Cls();
	Beep();
	leerSC2("4.sc2");
	mostrarSC2();*/
}


static char my_interrupt( void ) {
	if( IsVsync() == 0 ) return 0;
	count++;
	return 1;
}

void leerSC2 (char *filename) {
	int file;	// Set a file handler variable
	//byte buffer[Cabecera_Fichero];

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