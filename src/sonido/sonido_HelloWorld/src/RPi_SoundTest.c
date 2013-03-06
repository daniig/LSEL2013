#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "No audio file specified.\nUse: thisExec <audioFile>\n");
		exit(1);
	} else {
		execlp("aplay", "aplay", "-q", argv[1], NULL);
	}
	exit(0);
}
