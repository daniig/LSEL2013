#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "./RPI_SoundTest.h"

char buf[TAMANO_BUFF_COPIA];

int main(int argc, char *argv[]) {
	int pid;
	if(argc < 2) {
		fprintf(stderr, "No audio file specified.\nUse: %s <audioFile>\n", argv[0]);
		exit(1);
	} else {
		createFIFO();
		if((pid=fork()) == 0) { // Proceso Hijo
			if(DEBUG) printf("Launching audio player...\n");
			execlp("aplay", "aplay", FIFO_FILENAME, NULL);
		} else {	// Proceso Padre
			moveAudioToFifo(argv[1]);
			removeFIFO();
			exit(0);
		}
	}
	exit(0);
}


void createFIFO() {
	int pid;
	int result;
	if((pid = fork()) == 0) { // Proceso Hijo
		execlp("mkfifo", "mkfifo", FIFO_FILENAME, NULL);
	} else {	// Proceso Padre
		wait(&result);
		if(result != 0) {
			fprintf(stderr, "Error creating FIFO file\n");
			exit(1);
		} else {
			if(DEBUG) printf("Fifo created\n");
			return;
		}
	}
	return;
}

void removeFIFO() {
	int pid;
	int result;
	if((pid = fork()) == 0) { // Proceso Hijo
		execlp("rm", "rm", FIFO_FILENAME, NULL);
	} else {	// Proceso Padre
		wait(&result);
		if(result != 0) {
			fprintf(stderr, "Error deleting FIFO file\n");
			exit(1);
		} else {
			if(DEBUG) printf("Fifo deleted\n");
			return;
		}
	}
	return;
}

void moveAudioToFifo(char * audioFile) {
	// AQUÍ VAMOS PASANDO LAS MUESTRAS DESDE 'audioFile' A 'fifo'
	// test
	int leidos, escritos, origen, destino;

	if ((origen = open(audioFile, O_RDONLY)) < 0) {
		fprintf(stderr, "Error opening audio file\n");
		exit(1);// Si falla abrir el origen, error
	}
	if ((destino = open(FIFO_FILENAME, O_WRONLY | O_TRUNC, 0666)) < 0) {
		fprintf(stderr, "Error opening FIFO file\n");	// Si falla crear el destino, error
		exit(1);
	}
	if(DEBUG) printf("audio file and FIFO opened correctly\n");

	while ((leidos = read(origen, buf, TAMANO_BUFF_COPIA)) > 0) {		// mientras la lectura de 1024 bytes me devuelva un valor mayor que 0
		if((escritos = write(destino, buf, leidos)) < 0) {
			fprintf(stderr, "Error copying from audio to FIFO\n"); 	// escribo y compruebo la escritura
			exit(1);
		}
	}
	if(DEBUG) printf("Finished copying audio file\n");
	if (leidos < 0 ) {
		fprintf(stderr, "Error reading from audio file\n");	// compruebo que la lectura no haya tenido error al terminar el while
		exit(1);
	}

	close(origen);		// cierro el archivo origen
	close(destino);		// cierro el archivo destino
	if(DEBUG) printf("Audio file and FIFO closed correctly\n");
	return;
}
