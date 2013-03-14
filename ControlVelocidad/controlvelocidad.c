#include <stdio.h>
#include <wiringPi.h>
#include "controlvelocidad.h"
#include "trama.h"

static TRM trama[N_TRENES];

/*
int main (void){
  printf ("Raspberry Pi blink\n") ;

  if (!initIO()){
	//for (;;){
		//putUno();
		//putCero();
		//putChar('A');
		//trama[0].address.field_add.add = 0x7A;	
		//trama[0].control.field_ctr.vel = STOP;
		//trama[0].control.field_ctr.dir = FORWARD;
		//setVelocidad(TREN_CARBON, FORWARD, 1);
		//printf("Caracter devuelto (1) ==> %d", getVelocidadCode(1));
	//}
	return 0;	
  }
  else{
	return 1;
  }
}
*/

int initIO(){
	if (wiringPiSetup () == -1){
		return 1;
	}
	else{
		pinMode (LINE_OUT, OUTPUT);
		return 0;
	}
}

void putUno(){
	digitalWrite (LINE_OUT, 1);	// On
    	usleep(DELAY);	// Delay 58 mS
    	digitalWrite (LINE_OUT, 0);	// Off
    	usleep(DELAY);	// Delay 58 mS
}

void putCero(){
	digitalWrite (LINE_OUT, 1);	// On 
    	usleep(2*DELAY);	// Delay 116 mS
    	digitalWrite (LINE_OUT, 0);	// Off
    	usleep(2*DELAY);	// Delay 116 mS
}

void putChar(char c){
	int i;
	for (i = 0; i < 8; i++){
		if((c >> (7-i) & 0x01) == 0x01){
			putUno();
		}
		else{
			putCero();
		}
	}
}

void putTrama(char tren){
	//Enviamos cabecera (12 unos)	
	int i;	
	for (i = 0; i < CABECERA_LENGHT; i++) {
		putUno();
	}
	//Separacion
	putCero();
	//Enviamos address
	putChar(trama[tren].address.all_add);
	//Separacion
	putCero();
	//Enviar velocidad y sentido
	putChar(trama[tren].control.all_ctr);
	//Separacion
	putCero();
	//Corrección de errores
	putChar(trama[tren].address.all_add ^ trama[tren].control.all_ctr);	
}

void setVelocidad(char tren, char direccion, char velocidad){
	trama[tren].address.field_add.un1 = 0;
	if (tren == TREN_CARBON){
		trama[tren].address.field_add.add = TREN_CARBON_ADDRESS;
	}
	else{
		trama[tren].address.field_add.add = TREN_GASOIL_ADDRESS;
	}
	trama[tren].control.field_ctr.un2 = 0b01;
	trama[tren].control.field_ctr.dir = direccion;
	trama[tren].control.field_ctr.vel = getVelocidadCode(velocidad);
	trama[tren].error = trama[tren].address.all_add ^ trama[tren].control.all_ctr;
	putTrama(tren);
}

char getVelocidadCode(char velocidad){
	char resultado;
	switch (velocidad) {
		case 1:
			resultado=STEP_1;
			break;
		case 2:
			resultado=STEP_2;
                        break;
		case 3:
			resultado=STEP_3;
                        break;
		case 4:
			resultado=STEP_4;
                        break;
		case 5:
			resultado=STEP_5;
                        break;
		case 6:
			resultado=STEP_6;
                        break;
		case 7:
			resultado=STEP_7;
                        break;
		case 8:
			resultado=STEP_8;
                        break;
		case 9:
			resultado=STEP_9;
                        break;
		case 10:
			resultado=STEP_10;
                        break;
		case 11:
			resultado=STEP_11;
                        break;
		case 12:
			resultado=STEP_12;
                        break;
		case 13:
			resultado=STEP_13;
                        break;
		case 14:
			resultado=STEP_14;
                        break;
		case 15:
			resultado=STEP_15;
                        break;
		case 16:
                        resultado=STEP_16;
                        break;
		case 17:
                        resultado=STEP_17;
                        break;
		case 18:
                        resultado=STEP_18;
                        break;
		case 19:
                        resultado=STEP_19;
                        break;
		case 20:
                        resultado=STEP_20;
                        break;
		case 21:
                        resultado=STEP_21;
                        break;
		case 22:
                        resultado=STEP_22;
                        break;
		case 23:
                        resultado=STEP_23;
                        break;
		case 24:
                        resultado=STEP_24;
                        break;
		case 25:
                        resultado=STEP_25;
                        break;
		case 26:
                        resultado=STEP_26;
                        break;
		case 27:
                        resultado=STEP_27;
                        break;
		case 28:
                        resultado=STEP_28;
                        break;
		case 0:
                        resultado=STOP;
                        break;
		default:
			resultado=STOP;
			break;
		}
	return resultado;
}
