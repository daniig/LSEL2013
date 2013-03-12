/*
 * RPI_SoundTest.h
 *
 *  Created on: 12/03/2013
 *      Author: merlin
 */

#ifndef RPI_SOUNDTEST_H_
  #define RPI_SOUNDTEST_H_
#endif /* RPI_SOUNDTEST_H_ */

#define TAMANO_BUFF_COPIA 1024
#define FIFO_FILENAME "fifo"
#define DEBUG 1

void createFIFO();
void removeFIFO();
void moveAudioToFifo(char * audioFile);
