/*
 * Name: IRtasks.c
 * Description: Read some GPIO pins using wiringPi functions
 * Authors: Group 1 LSEL2013
 */

// Libraries
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include <native/mutex.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include <rtdk.h>
#include <ncurses.h>
#include <string.h>
#include "pin_config.h"
#include "main_tren.h"
#include "interfaz_usuario.h"

// Global variables
RT_TASK IRtracking_task, Actualiza_pantalla;
RT_MUTEX mutex_steam, mutex_diesel, mutex_pVia;
int pin, err_steam, err_diesel, err_pVia, s_count, d_count;
struct train steam_t, diesel_t;
char pVia;
char s0_state, s1_state, s2_state, s3_state;
RTIME s0_ticks_temp, s1_ticks_temp, s2_ticks_temp, s3_ticks_temp;

// IRtracking function
void IRtracking(void *arg)
{
	//RT_TASK *curtask;			// curtask y curtaskinfo se utilizan para heredar la información de la tarea actual
	//RT_TASK_INFO curtaskinfo;	// para posteriormente indicar qué tarea se está ejecutando, etc. (ver función rt_task_inquire al final de la tarea)
	//RTIME ticks_temp;

	rt_task_set_periodic(NULL, TM_NOW, IRT_PERIOD);

	while(1){

		// Blocks the while loop till the next periodic interruption
		rt_task_wait_period(NULL);

		// Acquire mutex_steam and mutex_diesel
		rt_mutex_acquire(&mutex_steam,TM_INFINITE);
		rt_mutex_acquire(&mutex_diesel,TM_INFINITE);

		// SECTOR 0 ----------------------------------------------------------------------------------------------------------
		// 'Idle' state
		if (s0_state=='I'){
			if (digitalRead(GPIO_IR0_ALTO) == HIGH){   // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save the time information
				s0_state = 'S';
				s0_ticks_temp = rt_timer_read();
			}
			else if (digitalRead(GPIO_IR0_BAJO) == HIGH){// Bottom sensor. HIGH = Diesel train or the first part of the Steam train.
				// Change to the 'Checking' state and save the time information
				s0_state = 'C';
				s0_ticks_temp = rt_timer_read();
			}
		}

		// 'Steam train' state
		else if (s0_state=='S'){
			if (digitalRead(GPIO_IR0_BAJO) == LOW){       // Bottom sensor. LOW = No train
				s_count++;
				if (s_count == IDLE_COUNT) {
					s_count = 0;
					s0_state = 'I';
					steam_t.prev_sector = steam_t.current_sector;
					steam_t.current_sector = '0';
					steam_t.prev_time = steam_t.current_time;
					steam_t.current_time = rt_timer_ticks2ns(s0_ticks_temp);
				}
			}
			else
				s_count = 0;
		}

		// 'Checking' state
		else if (s0_state=='C'){

			if (digitalRead(GPIO_IR0_ALTO) == HIGH){    // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save time information
				s0_state = 'S';
				s0_ticks_temp = rt_timer_read();
			}

			else if (digitalRead(GPIO_IR0_BAJO) == LOW){// Bottom sensor. LOW = Diesel train
				d_count++;
				if (d_count == IDLE_COUNT){
					d_count = 0;
					s0_state = 'I';
					diesel_t.prev_sector = diesel_t.current_sector;
					diesel_t.current_sector = '0';
					diesel_t.prev_time = diesel_t.current_time;
					diesel_t.current_time = rt_timer_ticks2ns(s0_ticks_temp);
				}
			} else {
				d_count = 0;
			}
		}

		// END OF SECTOR 0 ---------------------------------------------------------------------------------------------------

		// SECTOR 1 ----------------------------------------------------------------------------------------------------------
		// 'Idle' state
		if (s1_state=='I'){
			if (digitalRead(GPIO_IR1_ALTO) == HIGH){   // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save the time information
				s1_state = 'S';
				s1_ticks_temp = rt_timer_read();
			}
			else if (digitalRead(GPIO_IR1_BAJO) == HIGH){// Bottom sensor. HIGH = Diesel train or the first part of the Steam train.
				// Change to the 'Checking' state and save the time information
				s1_state = 'C';
				s1_ticks_temp = rt_timer_read();
			}
		}

		// 'Steam train' state
		else if (s1_state=='S'){
			if (digitalRead(GPIO_IR1_BAJO) == LOW){       // Bottom sensor. LOW = No train
				s_count++;
				if (s_count == IDLE_COUNT) {
					s_count = 0;
					s1_state = 'I';
					steam_t.prev_sector = steam_t.current_sector;
					steam_t.current_sector = '1';
					steam_t.prev_time = steam_t.current_time;
					steam_t.current_time = rt_timer_ticks2ns(s1_ticks_temp);
				}
			}
			else
				s_count = 0;
		}

		// 'Checking' state
		else if (s1_state=='C'){

			if (digitalRead(GPIO_IR1_ALTO) == HIGH){    // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save time information
				s1_state = 'S';
				s1_ticks_temp = rt_timer_read();
			}

			else if (digitalRead(GPIO_IR1_BAJO) == LOW){// Bottom sensor. LOW = Diesel train
				d_count++;
				if (d_count == IDLE_COUNT){
					d_count = 0;
					s1_state = 'I';
					diesel_t.prev_sector = diesel_t.current_sector;
					diesel_t.current_sector = '1';
					diesel_t.prev_time = diesel_t.current_time;
					diesel_t.current_time = rt_timer_ticks2ns(s1_ticks_temp);
				}
			} else {
				d_count = 0;
			}
		}
		// END OF SECTOR 1 ---------------------------------------------------------------------------------------------------

		// SECTOR 2 ----------------------------------------------------------------------------------------------------------
		// 'Idle' state
		if (s2_state=='I'){
			if (digitalRead(GPIO_IR2_ALTO) == HIGH){   // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save the time information
				s2_state = 'S';
				s2_ticks_temp = rt_timer_read();
			}
			else if (digitalRead(GPIO_IR2_BAJO) == HIGH){// Bottom sensor. HIGH = Diesel train or the first part of the Steam train.
				// Change to the 'Checking' state and save the time information
				s2_state = 'C';
				s2_ticks_temp = rt_timer_read();
			}
		}

		// 'Steam train' state
		else if (s2_state=='S'){
			if (digitalRead(GPIO_IR2_BAJO) == LOW){       // Bottom sensor. LOW = No train
				s_count++;
				if (s_count == IDLE_COUNT) {
					s_count = 0;
					s2_state = 'I';
					steam_t.prev_sector = steam_t.current_sector;
					steam_t.current_sector = '2';
					steam_t.prev_time = steam_t.current_time;
					steam_t.current_time = rt_timer_ticks2ns(s2_ticks_temp);
				}
			}
			else
				s_count = 0;
		}

		// 'Checking' state
		else if (s2_state=='C'){

			if (digitalRead(GPIO_IR2_ALTO) == HIGH){    // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save time information
				s2_state = 'S';
				s2_ticks_temp = rt_timer_read();
			}

			else if (digitalRead(GPIO_IR2_BAJO) == LOW){// Bottom sensor. LOW = Diesel train
				d_count++;
				if (d_count == IDLE_COUNT){
					d_count = 0;
					s2_state = 'I';
					diesel_t.prev_sector = diesel_t.current_sector;
					diesel_t.current_sector = '2';
					diesel_t.prev_time = diesel_t.current_time;
					diesel_t.current_time = rt_timer_ticks2ns(s2_ticks_temp);
				}
			} else {
				d_count = 0;
			}
		}
		// END OF SECTOR 2 ---------------------------------------------------------------------------------------------------

		// SECTOR 3 ----------------------------------------------------------------------------------------------------------
		// 'Idle' state
		if (s3_state=='I'){
			if (digitalRead(GPIO_IR3_ALTO) == HIGH){   // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save the time information
				s3_state = 'S';
				s3_ticks_temp = rt_timer_read();
			}
			else if (digitalRead(GPIO_IR3_BAJO) == HIGH){// Bottom sensor. HIGH = Diesel train or the first part of the Steam train.
				// Change to the 'Checking' state and save the time information
				s3_state = 'C';
				s3_ticks_temp = rt_timer_read();
			}
		}

		// 'Steam train' state
		else if (s3_state=='S'){
			if (digitalRead(GPIO_IR3_BAJO) == LOW){       // Bottom sensor. LOW = No train
				s_count++;
				if (s_count == IDLE_COUNT) {
					s_count = 0;
					s3_state = 'I';
					steam_t.prev_sector = steam_t.current_sector;
					steam_t.current_sector = '3';
					steam_t.prev_time = steam_t.current_time;
					steam_t.current_time = rt_timer_ticks2ns(s3_ticks_temp);
				}
			}
			else
				s_count = 0;
		}

		// 'Checking' state
		else if (s3_state=='C'){

			if (digitalRead(GPIO_IR3_ALTO) == HIGH){    // Top sensor. HIGH = Steam train
				// Change to the 'Steam train' state and save time information
				s3_state = 'S';
				s3_ticks_temp = rt_timer_read();
			}

			else if (digitalRead(GPIO_IR3_BAJO) == LOW){// Bottom sensor. LOW = Diesel train
				d_count++;
				if (d_count == IDLE_COUNT){
					d_count = 0;
					s3_state = 'I';
					diesel_t.prev_sector = diesel_t.current_sector;
					diesel_t.current_sector = '3';
					diesel_t.prev_time = diesel_t.current_time;
					diesel_t.current_time = rt_timer_ticks2ns(s3_ticks_temp);
				}
			} else {
				d_count = 0;
			}
		}
		// END OF SECTOR 3 ---------------------------------------------------------------------------------------------------

		// Release mutex_steam and mutex_diesel
		rt_mutex_release(&mutex_steam);
		rt_mutex_release(&mutex_diesel);

	}
}

void cambiarVia(void)
{
	if(pVia == '1')
	{
		pVia = '0';
		digitalWrite (GPIO_CAMBIO_VIA,0) ; // Off
	}else{
		pVia = '1';
		digitalWrite (GPIO_CAMBIO_VIA,1) ; // On
	}
}


// variableInit Function
int variableInit (void)
{
	// Inicialización estructuras train.
	steam_t.train_type = 'S';
	steam_t.current_sector = '0';
	steam_t.current_time = 0;
	steam_t.prev_sector = '0';
	steam_t.prev_time = 0;
	diesel_t.train_type = 'D';
	diesel_t.current_sector = '0';
	diesel_t.current_time = 0;
	diesel_t.prev_sector = '0';
	diesel_t.prev_time = 0;
	pVia = '1';
	s0_state = 'I';
	s1_state = 'I';
	s2_state = 'I';
	s3_state = 'I';
	s_count = 0;
	d_count = 0;
	return 0;
}

void catch_signal() {}

void disminuirVelocidad() {
	// Se encarga el otro grupo
}

void aumentarVelocidad() {
	// Se encarga el otro grupo

}

// Borrado del MUTEX:
void cleanup (void)
{
	rt_mutex_delete(&mutex_steam);
	rt_mutex_delete(&mutex_diesel);
}

// Main
int main(int argc, char* argv[]) {

	// Blocks interruptions and stuff
	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);
	/* Create a mutex; we could also have attempted to bind to some
	pre-existing object, using rt_mutex_bind() and rt_mutex_bind()
	instead of creating it. In any case, priority inheritance is
	automatically enforced for mutual exclusion locks. */

	err_steam = rt_mutex_create(&mutex_steam,"SteamMutex");
	err_diesel = rt_mutex_create(&mutex_diesel,"DieselMutex");
	err_pVia = rt_mutex_create(&mutex_pVia,"pViaMutex");


	// Función de configuración utilizada en el tutorial (?comprobar su utilidad):
	if (wiringPiSetup () == -1)
		exit (1) ;

	// Inicialización de todas las variables.
	if (variableInit () == -1)
		exit (1) ;

	// Se configuran los pines 0 al 7 como entrada -> Sensores IR
	for (pin = 0 ; pin < 8 ; pin++)
		pinMode (pin, INPUT);

	// Se configura el pin 10 como salida -> cambio de via
	pinMode (GPIO_CAMBIO_VIA,OUTPUT);
	digitalWrite (GPIO_CAMBIO_VIA,1) ; // Empieza por la via exterior

	char str[10] ;
	// Perform auto-init of rt_print buffers if the task doesn't do so
	rt_print_auto_init(1);

	// Lock memory : avoid memory swapping for this program
	mlockall(MCL_CURRENT|MCL_FUTURE);

	// rt_printf("start task\n");

	/*
	 * Arguments: &task,
	 * name,
	 * stack size (0=default),
	 * priority,
	 * mode (FPU, start suspended, ...)
	 */


	rt_task_create(&IRtracking_task, str, 0, 50, 0);
	rt_task_create(&Actualiza_pantalla, str, 0, 40, 0);
	initscr();			// Start curses mode
	//noecho();
	curs_set(0);
	imprimirInterfazInicial();

	rt_task_start(&IRtracking_task, &IRtracking, 0);
	rt_task_start(&Actualiza_pantalla, &Apantalla, 0);
	interfaz_usuario();	
	rt_task_delete(&Actualiza_pantalla);
	rt_task_delete(&IRtracking_task);
	endwin();			// End curses mode
	// cleanup();


	return 0;

}



