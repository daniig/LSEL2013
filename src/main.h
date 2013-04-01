/*
 * Name: 	main.h
 * Description:	Read some GPIO pins using wiringPi functions
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
RT_MUTEX mutex_steam, mutex_diesel, mutex_pVia;
int pin, err_steam, err_diesel, err_pVia;
struct train steam_t, diesel_t;
char pVia, trenSeleccionado;

// IRtracking function
void IRtracking(void *arg);

void mostrarDatos(void);

void cambiarVia(void);

int variableInit (void);

void catch_signal();

void disminuirVelocidad();

void aumentarVelocidad();

int interfaz_usuario(void);

int main(int argc, char* argv[]);

void cleanup(void);
