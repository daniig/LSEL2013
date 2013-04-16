/*
 * interfaz_usuario.c
 *
 *  Created on: Apr 16, 2013
 *      Author: rpidev
 */

#include <native/task.h>
#include <native/timer.h>
#include <native/mutex.h>
#include <ncurses.h>
#include <stdlib.h>
#include "main_tren.h"

char trenSeleccionado = 0;

int interfaz_usuario(void)
{
	char in[64];
	while(1)
	{
		mvprintw(15, 3, ">>");
		clrtoeol();
		scanw("%s", in);

		if(in[0] >= '0' && in <= '9')
		{
			// Llamar a la función que actualiza la velocidad
			// ...
		}else if(in[0]  == 'c') {
			cambiarVia();
			// Si pVia = 0 seleciona la via interior

		}else if(in[0]  == 'e') {
			// No se puede acabar el programa con Ctl+C o Ctl+Z porque ncurses se vuelve loco y hay que rebootear la RPi
			// Hay que terminar siempre el programa con el comando "e"
			endwin();
			exit(0);

		}else if(in[0] == 't') {
			// Cambiamos el tren seleccionado (el tren cuya velocidad queremos cambiar)
			if (trenSeleccionado == 0) {
				trenSeleccionado = 1;
			} else {
				trenSeleccionado = 0;
			}
		}
	}
	system("reset");	// Reset del terminal
	return 0;
}

void imprimirInterfazInicial() {
	/* ----------------- COLORS SET -------------------
		COLOR_BLACK   0
        	COLOR_RED     1
        	COLOR_GREEN   2
        	COLOR_YELLOW  3
        	COLOR_BLUE    4
        	COLOR_MAGENTA 5
        	COLOR_CYAN    6
        	COLOR_WHITE   7				*/

	initscr();			/* Start curses mode 		*/
	if(has_colors() == FALSE)
	{	endwin();
	mvprintw(13, 3, "Your Terminal does not support color WTF");
	exit(1);
	}

	start_color();			/* Start color 			*/
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);

	attron(COLOR_PAIR(1));
	//mvprintw(2, 36, "CONTROL DE TRENES");
	mvprintw(2, 28, "CONTROL DE TRENES");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(4, 3, "Tren");
	mvprintw(4, 18, "Sector");
	mvprintw(4, 33, "Velocidad (cm/s)");
	mvprintw(5, 3, "DIESEL");
	mvprintw(5, 18, "*");
	mvprintw(5, 33, "*");
	mvprintw(6, 3, "VAPOR");
	mvprintw(6, 18, "*");
	mvprintw(6, 33, "*");
	mvprintw(8, 3, "Cambio via");	// Ojo: por alguna razón meter vocales acentuadas descuadra las coordenadas
	mvprintw(8, 18, "Estado");
	mvprintw(9, 18, "*");
	mvprintw(11, 3, "(c) Cambio de via - (t) Cambiar tren selec. - (0-9) Ajustar vel. tren - (e) Salir");
	attroff(COLOR_PAIR(2));

}

// sector: el sector por el que acaba de pasar el tren
double calcularVelocidad(char sector, SRTIME prevTime, SRTIME currTime) {
	double longitud;
	double velCmS;
	if(sector == '0' || sector == '2') {
		longitud = 40*3.1416;
	} else {
		longitud = 90;
	}
	if ((currTime == 0) || (prevTime == 0))
		velCmS = 0;
	else{
		SRTIME intervalo = currTime - prevTime;
		double intervDouble = (double) intervalo;
		velCmS = longitud/(intervDouble/1000000000);
	}
	return velCmS;
}

void Apantalla(void *arg)
{
	double VelDiesel, VelVapor;
	rt_task_set_periodic(NULL, TM_NOW, 500000000);

	while(1){
		rt_task_wait_period(NULL);
		// Acquire mutex_steam and mutex_diesel
		rt_mutex_acquire(&mutex_steam,TM_INFINITE);
		rt_mutex_acquire(&mutex_diesel,TM_INFINITE);
		// rt_mutex_acquire(&mutex_pVia,TM_INFINITE);

		// Escribimos el sector
		mvaddch(5, 18, diesel_t.current_sector);
		mvaddch(6, 18, steam_t.current_sector);
		// Escribimos la vía
		if (pVia == '0')
			mvprintw(9, 18, "Via interior");
		else{
			mvprintw(9, 18, "Via exterior");
		}
		// Escribimos la velocidad
		VelDiesel=calcularVelocidad(diesel_t.current_sector, diesel_t.prev_time, diesel_t.current_time);
		VelVapor=calcularVelocidad(steam_t.current_sector, steam_t.prev_time, steam_t.current_time);

		if (VelDiesel == 0)
			mvprintw(5, 33, "No Data");
		else
			mvprintw(5, 33, "%f", VelDiesel);

		if (VelVapor == 0)
			mvprintw(6, 33, "No Data");
		else
			mvprintw(6, 33, "%f", VelVapor);

		// Escribimos el selector de tren
		if(trenSeleccionado == 0) {
			mvaddch(5, 1, '*');
			mvaddch(6, 1, ' ');
		} else {
			mvaddch(6, 1, '*');
			mvaddch(5, 1, ' ');
		}

		init_pair(4, COLOR_YELLOW, COLOR_BLACK);

		attron(COLOR_PAIR(4));
		mvprintw(15, 3, ">>");
		attroff(COLOR_PAIR(4));



		/* ----------------- COLORS SET -------------------
		COLOR_BLACK   0
        	COLOR_RED     1
        	COLOR_GREEN   2
        	COLOR_YELLOW  3
        	COLOR_BLUE    4
        	COLOR_MAGENTA 5
        	COLOR_CYAN    6
        	COLOR_WHITE   7				*/

		//start_color();			/* Start color 			*/
		//init_pair(1, COLOR_RED, COLOR_BLACK);


		refresh();

		// Release mutex_steam and mutex_diesel
		rt_mutex_release(&mutex_steam);
		rt_mutex_release(&mutex_diesel);
		// rt_mutex_release(&mutex_pVia);
	}
}
