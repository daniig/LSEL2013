/*
 * Name: 	main.h
 * Description:	Read some GPIO pins using wiringPi functions
 */
#ifndef MAIN_TREN_H
#define MAIN_TREN_H

struct train {
	char train_type;
	char current_sector;
	SRTIME current_time;
	char prev_sector;
	SRTIME prev_time;
};

// IRtracking function
void IRtracking(void *arg);

void mostrarDatos(void);

void cambiarVia(void);

int variableInit (void);

void catch_signal();

void disminuirVelocidad();

void aumentarVelocidad();

int interfaz_usuario(void);

void cleanup(void);

#endif
