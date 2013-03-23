#include <ncurses.h> 
#include <string.h>

/*
 * TODO:
 * - Incluir un mutex en el acceso a actualizarPantalla (para que no haya 2 llamadas
 * concurrentes a actualizarPantalla() )
 */

char trenSeleccionado = 0;

void actualizarPantalla() {
	if(trenSeleccionado == 0) {
		mvaddch(5, 2, '*');
		mvaddch(6, 2, ' ');
	} else {
		mvaddch(6, 2, '*');
		mvaddch(5, 2, ' ');
	}
	/*
	 * Incluir aquí el código que actualiza la velocidad y posición
	 */
	refresh();			/* Print it on to the real screen */
}

void imprimirInterfazInicial() {
	mvprintw(2, 36, "CONTROL DE TRENES");
	mvprintw(4, 3, "Tren");
	mvprintw(4, 18, "Sector");
	mvprintw(4, 33, "Velocidad");
	mvprintw(5, 3, "DIESEL");
	mvprintw(5, 18, "2");
	mvprintw(5, 33, "3");
	mvprintw(6, 3, "VAPOR");
	mvprintw(6, 18, "2");
	mvprintw(6, 33, "3");
	mvprintw(8, 3, "Cambio via");	// Ojo: por alguna razón meter vocales acentuadas descuadra las coordenadas
	mvprintw(8, 18, "Estado");
	mvprintw(9, 18, "0");
	mvprintw(20, 3, "(0-9) Ajustar vel. tren selec. - (C) Cambio de vía - (T) Cambiar tren selec.");
}

int main() {	
	initscr();			/* Start curses mode */
	noecho();
	curs_set(0);
	imprimirInterfazInicial();
	actualizarPantalla();
	char in;
	while(1) {
		in = getch();
		if(in >= '0' && in <= '9') {
			// Llamar a la función que actualiza la velocidad
			// ...
		} else if(in == 'c') {
			// Llamar a la función que cambia la vía
			// cambiarVia();
			actualizarPantalla();
		} else if(in == 't') {
			if(trenSeleccionado == 1) {
				trenSeleccionado = 0;
			} else {
				trenSeleccionado = 1;
			}
		}
		actualizarPantalla();
	}
	endwin();			/* End curses mode		  */
	
	return 0;
}