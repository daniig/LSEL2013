#include <stdio.h>

// No estoy seguro de getchar

int trenSeleccionado = 1;

void mostrarDatos() {
  printf("Velocidad: ... (datos obtenidos de la monitorización)\n");
}

void disminuirVelocidad() {
  printf("Nueva velocidad tren %i: ...\n", trenSeleccionado);
}

void aumentarVelocidad() {
  printf("Nueva velocidad tren %i: ...\n", trenSeleccionado);
}

void cambiarVia() {
  printf("Nueva posición vía: ...\n");
}

int main(int argc, char **argv) {
  
  printf("INTERFAZ DE CONTROL DE TRENES\n");
  printf("(M)onitorizar - (1) (2) Seleccionar tren - (+)(-) Ajustar velocidad - Ajustar (c)ambio de vía\n");
  
  char in[64];
  while(1) {
    // Esperamos a recibir un carácter válido
    printf(">>");
    scanf("%s", in);
    switch (in[0]) {
      case 'm':
      case 'M':
	mostrarDatos();
	break;
      case '1':
	trenSeleccionado = 1;
	printf("Seleccionado tren 1\n");
	break;
      case '2':
	trenSeleccionado = 2;
	printf("Seleccionado tren 2\n");
	break;
      case '+':
	aumentarVelocidad();
	break;
      case '-':
	disminuirVelocidad();
	break;
      case 'c':
      case 'C':
	cambiarVia();
	break;
      default:
	printf("Comando no reconocido\n");
	break;
    }    
  }
  
}

