/*
 * testcontrolvelocidad.c
 *
 *  Created on: 12/03/2013
 *      Author: senkzer0
 */


/* file minunit_example.c */

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include "minunit.h"
 #include "controlvelocidad.h"
 #include "testcontrolvelocidad.h"

 int tests_run = 0;

 int velocidad = 0;

 static char * test_Init(){
	printf("TEST %d ==> INICIALIZANDO GPIO", tests_run+1);
	if(!initIO()){
		return 0;
	}
	else{
		return 1;
	}
 }
 static char * test_Carbon(){
	//2 seconds stopped
	printf("TEST %d ==> TREN DE CARBON\r\n",tests_run+1);
	printf("...Estableciendo velocidad: %d\r\n",0);
	setVelocidad(TREN_CARBON,FORWARD,0);
	usleep(2000000);
	printf("......Incremento lineal de velocidad\r\n");
	incrLinVel(5,TREN_CARBON,FORWARD);
	printf("......Decremento escalonado de velocidad\r\n");
	decEscVel(2, TREN_CARBON, FORWARD);
	printf("\r\n");
	return 0;
 }

 static char * test_Gasoil(){
	printf("TEST %d ==> TREN DE GASOIL\r\n",tests_run+1);
	printf("......Incremento escalonado de velocidad\r\n");
	incrEscVel(2, TREN_GASOIL,FORWARD);
	printf("......Decremento lineal de velocidad\r\n");
	decLinVel(5,TREN_GASOIL,FORWARD);
	printf("...Estableciendo velocidad: %d\r\n",0);
	setVelocidad(TREN_GASOIL,FORWARD,0);
	usleep(2000000);
	printf("\r\n");
	return 0;
 }

 static char * all_tests() {
	 int n;
	 mu_run_test(test_Init);
	 for(n=0; n<4; n++){
		mu_run_test(test_Carbon);
		mu_run_test(test_Gasoil);
	 }
	 return 0;
 }

 int main(int argc, char **argv) {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);

     return result != 0;
 }


 /**
  * AUXILIARY FUNCTIONS
  */
 void incrLinVel(int t_interval, char tren, char dir){
	 int i;
	 int initVel = 0;
	 for(i=0; i <=28 ; i++){
		 setVelocidad(tren,dir, initVel+i);
		 printf("...Estableciendo velocidad: %d\r\n",initVel+i);
		 usleep(t_interval*1000000/28);
	 }
 }

 void incrEscVel(int t_step, char tren, char dir){
	 int i;
	 int initVel=0;
	 for(i=0; i<=4; i++){
		 setVelocidad(tren,dir,initVel+(i*7));
		 printf("...Estableciendo velocidad: %d\r\n",initVel+(i*7));
		 usleep(t_step*1000000);
	 }
 }

 void decLinVel(int t_interval, char tren, char dir){
	 int i;
	 int initVel = 28;
	 for(i=0; i<=28; i++){
		 setVelocidad(tren,dir,initVel-i);
		 printf("...Estableciendo velocidad: %d\r\n",initVel-i);
		 usleep(t_interval*1000000/28);
	 }
 }

 void decEscVel(int t_step, char tren, char dir){
 	 int i;
 	 int initVel=28;
 	 for(i=0; i<=4; i++){
 		 setVelocidad(tren,dir,initVel-(i*7));
 		 printf("...Estableciendo velocidad: %d\r\n",initVel-(i*7));
 		 usleep(t_step*1000000);
 	 }
  }


