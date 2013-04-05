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


struct train {
char train_type;
char current_sector;
SRTIME current_time;
char prev_sector;
SRTIME prev_time;
};

// Global variables
RT_TASK IRtracking_task, Actualiza_pantalla;
RT_MUTEX mutex_steam, mutex_diesel, mutex_pVia;
int pin, err_steam, err_diesel, err_pVia, s_count, d_count;
struct train steam_t, diesel_t;
char pVia, trenSeleccionado;
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
	//mvprintw(11, 3, "(0-9) Ajustar vel. tren selec. - (C) Cambio de vía - (T) Cambiar tren selec.");	
	mvprintw(11, 3, "(C) Cambio de via - (e) Exit/Salir");
	attroff(COLOR_PAIR(2));
	
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

	        mvaddch(5, 18, diesel_t.current_sector);
		mvaddch(6, 18, steam_t.current_sector);

		if (pVia == '0')
			mvprintw(9, 18, "Via interior");
		else{
			mvprintw(9, 18, "Via exterior");
		}

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
			
		}
	}
	system("reset");	// Reset del terminal
	return 0;
}


// Main
int main(int argc, char* argv[])
{

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
	initscr();			/* Start curses mode */
	//noecho();
	curs_set(0);
	imprimirInterfazInicial();

	rt_task_start(&IRtracking_task, &IRtracking, 0);
	rt_task_start(&Actualiza_pantalla, &Apantalla, 0);
	interfaz_usuario();	
	rt_task_delete(&Actualiza_pantalla);
	rt_task_delete(&IRtracking_task);
	endwin();			/* End curses mode		  */
	// cleanup();
	

	return 0;

}

// Borrado del MUTEX:
void cleanup (void)
{
	rt_mutex_delete(&mutex_steam);
	rt_mutex_delete(&mutex_diesel);
}
