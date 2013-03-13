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

#define TAMANO_BUFF_COPIA 28600
#define MAX_SPEED 5

void getAudioSamples(char * audioFile);
void audioPlayer();
void audioMaker();
void setSamplesSpeed(int speed);

