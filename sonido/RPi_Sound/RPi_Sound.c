#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "./RPI_Sound.h"

int sampleSize;
char samples[TAMANO_BUFF_COPIA];

int bufSize;
char buf[TAMANO_BUFF_COPIA];

int main(int argc, char *argv[]) {
	int pid;
	int tubo[2];
	pipe(tubo);

	if(argc < 2) {
		fprintf(stderr, "No audio file specified.\nUse: %s <audioFile>\n", argv[0]);
		exit(1);
	} else {
		if((pid=fork()) == 0) { // Proceso Hijo - Productor
		    close(tubo[0]);					// cerramos la entrada del tubo
			// Recuerda: STDIN = 0, STDOUT = 1, STERR = 2
		    dup2(tubo[1], 1);				// Duplica y redirige la STDOUT a la salida del tubo
		    close(tubo[1]);					// Cierra la salida del tubo
		    getAudioSamples(argv[1]);
		    audioMaker();

		} else {	// Proceso Padre - Consumidor
		    close(tubo[1]);					// cierra la salida del tubo
		    dup2(tubo[0], 0);				// Duplica y redirige la STDIN a la entrada del tubo
		    close(tubo[0]);					// Cierra la entrada del tubo
		    audioPlayer();					// y ejecuta consumidor
		}
	}
	exit(0);
}


void audioPlayer() {
	if(DEBUG) printf("Launching audio player...\n");
	execlp("aplay", "aplay", "-r", "22100", NULL);
}

void getAudioSamples(char * audioFile) {
	int origen;

	if ((origen = open(audioFile, O_RDONLY)) < 0) {
		exit(1);// Si falla abrir el origen, error
	}

	sampleSize = read(origen, samples, TAMANO_BUFF_COPIA);

	if (sampleSize < 0) {
		fprintf(stderr, "Error reading from audio file\n");	// compruebo que la lectura no haya tenido error al terminar el while
		exit(1);
	}
	close(origen);		// cierro el archivo origen
	return;
}

void audioMaker() {
	int escritos;
	int i;
	int j;

	for(i=0; i <= 5; i++) {
		setSamplesSpeed(i);
		for(j=0; j<=4; j++) {
			if((escritos = write(1, buf, bufSize)) < 0) {
				fprintf(stderr, "Error copying from audio to FIFO\n"); 	// escribo y compruebo la escritura
				exit(1);
			}
		}
	}

	exit(0);
}

void setSamplesSpeed(int speed) {
	int i, j, k;
	k = 0;
	i = 0;

	if(speed == 0) {
		for(i = 0; i < TAMANO_BUFF_COPIA; i++) {
			buf[i] = 0;
		}
		bufSize = TAMANO_BUFF_COPIA;
	} else {
		while(k < sampleSize - 1) {
			for(j=0;j<(MAX_SPEED + 2 - speed); j++) {
				buf[i] = samples[k];
				bufSize = i;
				//fprintf(stderr, "k=%i i=%i j=%i\n", k, i, j);
				k++;
				i++;
			}
			k++;
		}
	}
}


