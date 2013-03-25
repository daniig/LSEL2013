/*
 * RPI_SoundTest.h
 *
 *  Created on: 12/03/2013
 *      Author: merlin
 */

#ifndef RPI_SOUNDTEST_H_
  #define RPI_SOUNDTEST_H_
#endif /* RPI_SOUNDTEST_H_ */


#define DEBUG 1

#define MAX_SPEED 28			// Intervalos de velocidad
#define MIN_AUDIO_RATIO 120		// Esto define la frecuencia minima del sonido
#define MAX_AUDIO_RATIO 250		// Esto define la frecuencia maxima del sonido
#define SPEED_RATIO ((MAX_AUDIO_RATIO-MIN_AUDIO_RATIO)/MAX_SPEED)

#define TAMANO_BUFF_SAMPLE 19000	// Definimos la longitud maxima de las muestras de sonido
#define TAMANO_BUFF_COPIA 60000
//#define TAMANO_BUFF_COPIA  (TAMANO_BUFF_SAMPLE*(120/MIN_AUDIO_RATIO))	// Aseguramos la longitud del buffer de copia

void getAudioSamples(char * audioFile);
void audioPlayer();
void audioMaker();
void setSamplesSpeed(int speed);

