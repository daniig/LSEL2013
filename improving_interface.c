/* test using alarm */
#include <stdio.h>
#include <unistd.h>
#include <signal.h> // Librería para las alarmas
#include <stdlib.h> // Librería para el clear screen

int c=0;


/* alarm signal handler */
void alarm_func()
{
		system("clear");	// Clear del terminal
		printf("\n >>>>>>> MONITORIZACION <<<<<<<\n");
        printf("\nSteam Train Info: %i", c);
        printf("\nDiesel Train Info: %i", c*3);
        printf("\nTrack Position: %i", c*5);
        printf("\n\n    >>>>>>> CONTROL <<<<<<<\n");
        printf("\n>> ");
        c++;
        fflush(stdout);  // Flush de la salida estandar
        alarm(3);
        return;
}
int main( int argc, char **argv )
{
		char command;
        signal( SIGALRM, alarm_func );  // Definición del manejador para las alarmas
        alarm_func();
        while(1){
        //alarm(3);
        command=getchar();
   		//alarm(0);
        printf("\nHas ejecutado el comando: %c", command );
        if (command=='e') break; // Comando que finaliza la ejecución.
        }
        return 0;
}



