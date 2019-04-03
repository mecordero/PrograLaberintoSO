
#include <stdio.h>
#include <stdlib.h>


enum Tipo_Celda {PARED, FINAL, LIBRE} tipo;

struct celda {
	char caracter;
	char arriba ;
	char abajo ;
	char derecha;
	char izquierda;
	enum Tipo_Celda tipo;
	//sem_t semaforo;
	};

int main(int argc, char **argv)
{
	FILE * fileLaberinto;
	
	char charActual; //char que lee el archivo
	int posActual; //posicion del arreglo medidaX y medidaY
	
	char numFilas [5]; //string numero de filas
	char numColumnas [5]; //string numero de columnas
	
	int filas = 0; //entero de filas
	int columnas = 0; //entero de columnas
	
	char * fileName = "laberinto.txt";
	
	fileLaberinto = fopen(fileName, "r");
	if(fileLaberinto == NULL){
		printf("Could not open file %s", fileName);
		return 1;	
	}
		
	//obtiene numero de filas
	posActual = 0;
	while ((charActual = fgetc(fileLaberinto)) != 'x'){
		numFilas[posActual++] = charActual;
	}
	numFilas[posActual] = '\0';
		
	//obtiene medida de y
	posActual = 0;
	while ((charActual = fgetc(fileLaberinto)) != '\n'){
		numColumnas[posActual++] = charActual;
	}
	numColumnas[posActual] = '\0';
	
	//convierte strings a enteros
	sscanf(numFilas, "%d", &filas);
	sscanf(numColumnas, "%d", &columnas);
		
	printf("Filas: %d\n", filas);
	printf("Columnas: %d\n", columnas);
		
	struct celda laberinto[filas][columnas];	
	
	for(int i = 0; i < filas; i ++){
		for(int j = 0; j < columnas; j++){
			laberinto[i][j].caracter = '*';
			laberinto[i][j].arriba = '.';
			laberinto[i][j].abajo = '.';
			laberinto[i][j].derecha = '.';
			laberinto[i][j].izquierda= '.';
			tipo = PARED;
			//sem_init(&laberinto[i][j].semaforo, 0, 0);
		}
	}
	
	int i = 0;
	int j = 0;
	
	while ( feof(fileLaberinto) ) {
		while ((charActual = fgetc(fileLaberinto)) != '\n'){
			laberinto[i][j].caracter = charActual;
			switch (charActual) {
				case '/':
					laberinto[i][j].tipo = FINAL;
					break;
				case ' ':
					laberinto[i][j].tipo = LIBRE;
					break;
				default:
					laberinto[i][j].tipo = PARED;
					break;					
			}
			j++;
		}
		j = 0;
		i++;
	}
	
	for(int i = 0; i < filas; i ++){
		for(int j = 0; j < columnas; j++){
			printf("%c\t", laberinto[i][j].caracter);
		}
		printf("\n");
	}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	fclose(fileLaberinto);
	
	return 0;
}