/*
 * Name: 	IRtasks.c
 * Description:	Read some GPIO pins using wiringPi functions
 * Authors:	Andrés Madrid -  av.madrid@alumnos.upm.es
 *		Ramiro Utrilla - r.utrilla@alumnos.upm.es
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
#include "pin_config.h"


struct train {
	char train_type;
	char current_sector;
	SRTIME current_time;
	char prev_sector;
	SRTIME prev_time;
};

// Global variables
RT_TASK IRtracking_task;
//RT_TASK testVias_task;
RT_MUTEX mutex_steam, mutex_diesel, mutex_pVia;
int pin, err_steam, err_diesel, err_pVia;
struct train steam_t, diesel_t;
char pVia, trenSeleccionado;

// IRtracking function
void IRtracking(void *arg)
{
    RT_TASK *curtask;		// curtask y curtaskinfo se utilizan para heredar la información de la tarea actual
    RT_TASK_INFO curtaskinfo;	// para posteriormente indicar qué tarea se está ejecutando, etc. (ver función rt_task_inquire al final de la tarea)
    RTIME ticks_temp;

    rt_task_set_periodic(NULL, TM_NOW, 1000000000);

    while(1){

    // Blocks the while loop till the next periodic interruption
    rt_task_wait_period(NULL);

    // Acquire mutex_steam and mutex_diesel
    rt_mutex_acquire(&mutex_steam,TM_INFINITE);
    rt_mutex_acquire(&mutex_diesel,TM_INFINITE);

    // Read all the IR sensors
    // Los pins están puestos al tuntún, habría que ver exáctamente cómo los vamos a conectar y poner los numeros de manera concordante.

    // First point
    if (digitalRead(0) == HIGH)		// Bottom sensor
    {
      if (digitalRead(1) == HIGH){  	// Top sensor
        steam_t.prev_sector = steam_t.current_sector;
	steam_t.prev_time = steam_t.current_time;
	steam_t.current_sector = '1';
	ticks_temp = rt_timer_read();
	steam_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
      else{
        diesel_t.prev_sector = diesel_t.current_sector;
	diesel_t.prev_time = diesel_t.current_time;
	diesel_t.current_sector = '1';
	ticks_temp = rt_timer_read();
	diesel_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
    }

    // Second point
    if (digitalRead(2)==HIGH)		// Bottom sensor
    {
      if (digitalRead(3) == HIGH){	// Top sensor
        steam_t.prev_sector = steam_t.current_sector;
	steam_t.prev_time = steam_t.current_time;
	steam_t.current_sector = '2';
	ticks_temp = rt_timer_read();
	steam_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
      else{
        diesel_t.prev_sector = diesel_t.current_sector;
	diesel_t.prev_time = diesel_t.current_time;
	diesel_t.current_sector = '2';
	ticks_temp = rt_timer_read();
	diesel_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
    }

    // Third point
    if (digitalRead(4) == HIGH)		// Bottom sensor
    {
      if (digitalRead(5) == HIGH){	// Top sensor
        steam_t.prev_sector = steam_t.current_sector;
	steam_t.prev_time = steam_t.current_time;
	steam_t.current_sector = '3';
	ticks_temp = rt_timer_read();
	steam_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
      else{
        diesel_t.prev_sector = diesel_t.current_sector;
	diesel_t.prev_time = diesel_t.current_time;
	diesel_t.current_sector = '3';
	ticks_temp = rt_timer_read();
	diesel_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
    }

    // Fourth point
    if (digitalRead(6) == HIGH)		// Bottom sensor
    {
      if (digitalRead(7) == HIGH){	// Top sensor
        steam_t.prev_sector = steam_t.current_sector;
	steam_t.prev_time = steam_t.current_time;
	steam_t.current_sector = '4';
	ticks_temp = rt_timer_read();
	steam_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
      else{
        diesel_t.prev_sector = diesel_t.current_sector;
	diesel_t.prev_time = diesel_t.current_time;
	diesel_t.current_sector = '4';
	ticks_temp = rt_timer_read();
	diesel_t.current_time = rt_timer_ticks2ns(ticks_temp);
      }
    }

    // rt_printf("Chupame el orto:  .|. \n");

    // Release mutex_steam and mutex_diesel
    rt_mutex_release(&mutex_steam);
    rt_mutex_release(&mutex_diesel);



    // Inquire current task: return values information about the status of a given task
    //curtask = rt_task_self();
    //rt_task_inquire(curtask, &curtaskinfo);

    // Print task name
    //rt_printf("Task name : %s \n", curtaskinfo.name);
  }
}

void mostrarDatos(void)
{
  // Acquire mutex_steam and mutex_diesel
  rt_mutex_acquire(&mutex_steam,TM_INFINITE);
  rt_mutex_acquire(&mutex_diesel,TM_INFINITE);
//  rt_mutex_acquire(&mutex_pVia,TM_INFINITE);


  rt_printf("\nSteam train info - Sector: %c  Time: %i ns \n", steam_t.current_sector, steam_t.current_time);
  rt_printf("Diesel train info - Sector: %c  Time: %i ns \n", diesel_t.current_sector, diesel_t.current_time);
  rt_printf("Railroad track position: %c \n", pVia);


  // Release mutex_steam and mutex_diesel
  rt_mutex_release(&mutex_steam);
  rt_mutex_release(&mutex_diesel);
//  rt_mutex_release(&mutex_pVia);

}

void cambiarVia(void)
{
// rt_printf("Metodo cambio de via"); 
 if(pVia == '1')
 {
// rt_mutex_acquire(&mutex_pVia,TM_INFINITE);
    pVia = '0';
// rt_mutex_release(&mutex_pVia);
    digitalWrite (10,0) ; // Off
  }else{

 //rt_mutex_acquire(&mutex_pVia,TM_INFINITE);
    pVia = '1';
// rt_mutex_release(&mutex_pVia);
    digitalWrite (10,1) ; // On
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
    pVia = '0';
    return 0;
}



void catch_signal() {}


void disminuirVelocidad() {
  printf("Nueva velocidad tren %c: ...\n", trenSeleccionado);
}

void aumentarVelocidad() {
  printf("Nueva velocidad tren %c: ...\n", trenSeleccionado);
}


int interfaz_usuario(void)
{
  printf("INTERFAZ DE CONTROL DE TRENES\n");
  printf("(M)onitorizar - (1) (2) Seleccionar tren - (+)(-) Ajustar velocidad - Ajustar (c)ambio de vía - (E)xit/Salir\n");
  
  char in[64];
  while(1) {
    // Esperamos a recibir un carácter válido
    printf(">>");
    scanf("%s", in);
    switch (in[0]) {
      case 'm':
      case 'M':
  printf("\n");
  mostrarDatos();
  break;
      case '1':
  printf("\n");
  trenSeleccionado = 1;
  printf("Seleccionado tren 1\n");
  break;
      case '2':
  printf("\n");
  trenSeleccionado = 2;
  printf("Seleccionado tren 2\n");
  break;
      case '+':
  printf("\n");
  aumentarVelocidad();
  break;
      case '-':
  printf("\n");
  disminuirVelocidad();
  break;
      case 'c':
      case 'C':
  printf("\n");
  cambiarVia();
  break;
      case 'e':
      case 'E':
      return 0;
      default:
  printf("\n");
  printf("Comando no reconocido\n");
  break;
    }
  }
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

	// Se configuran los pines 0 al 8 como entrada:
	for (pin = 0 ; pin < 8 ; ++pin)
	pinMode (pin, INPUT) ;

	pinMode (10,OUTPUT);
      	digitalWrite (10,1) ; // On

	char  str[10] ;
	// Perform auto-init of rt_print buffers if the task doesn't do so
	rt_print_auto_init(1);

	// Lock memory : avoid memory swapping for this program
	mlockall(MCL_CURRENT|MCL_FUTURE);

//	rt_printf("start task\n");

	/*
	* Arguments: &task,
	*            name,
	*            stack size (0=default),
	*            priority,
	*            mode (FPU, start suspended, ...)
	*/

  	sprintf(str, "IRtracking");
	rt_task_create(&IRtracking_task, str, 0, 50, 0);


        //Creo un task para la prueba de las vias - AndresC
        //rt_task_start(&testVias_task; &test_vias, 0);


	/*
	* Arguments: &task,
	*            task function,
	*            function argument
	*/

	rt_task_start(&IRtracking_task, &IRtracking, 0);
  interfaz_usuario();
  rt_task_delete(&IRtracking_task);
//  cleanup();

    	return 0;

}

// Borrado del MUTEX:
void cleanup (void)
{
    rt_mutex_delete(&mutex_steam);
    rt_mutex_delete(&mutex_diesel);
}
