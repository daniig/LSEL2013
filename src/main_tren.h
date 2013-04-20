/*
 * Name: 	main.h
 * Description:	Read some GPIO pins using wiringPi functions
 */
#ifndef MAIN_TREN_H
#define MAIN_TREN_H

#include <native/mutex.h>

struct train {
	char train_type;
	char current_sector;
	SRTIME current_time;
	char prev_sector;
	SRTIME prev_time;
};

extern RT_MUTEX mutex_steam, mutex_diesel;
extern struct train steam_t, diesel_t;
extern char pVia;

void cambiarVia();

#endif
