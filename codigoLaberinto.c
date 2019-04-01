

#include <stdio.h>

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
		
	char laberinto[filas][columnas];	
	
	for(int i = 0; i < filas; i ++){
		for(int j = 0; j < columnas; j++){
			laberinto[i][j] = '*';
		}
	}
	
	
	for(int i = 0; i < filas; i ++){
		for(int j = 0; j < columnas; j++){
			printf("%c\t", laberinto[i][j]);
		}
		printf("\n");
	}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	fclose(fileLaberinto);
	
	return 0;
}

