// HOla
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Enum de los distintos tipos de celda
enum Tipo_Celda {PARED, FINAL, LIBRE} tipo;
enum Direccion {ARRIBA, ABAJO, DERECHA, IZQUIERDA} direccion;

//variable del caracter actual
char caracterActual = 48;

//Estructura de una celda del laberinto
struct Celda {
	char caracter;
	char arriba ;
	char abajo ;
	char derecha;
	char izquierda;
	enum Tipo_Celda tipo;
	//sem_t semaforo;
	};
	
//Estructura de los atributos que tiene cada hilo
struct AtributosHilo{
	struct Celda * celdas;
	enum Direccion direccion;
	int totalFilas;
	int totalCols;
	int filaActual;
	int colActual;
	int contadorRecorrido;	
	char caracter;
};	

//Funcion generadora de caracteres
char nuevoChar(){
	
	if(caracterActual == 57){
		caracterActual = 65;
	}else if(caracterActual == 90){
		caracterActual = 97;
	}else
		caracterActual ++;
	
	
	return caracterActual;
}

//Funcion que revisa si una celda es libre
char celdaEsLibre(int col, int fila, struct Celda * celdas, int totalCols, int totalFilas, enum Direccion direccion){
	
	if(col < 0 || fila < 0)
		return 0;
	if(col >= totalCols || fila >= totalFilas)
		return 0;
	struct Celda * celda = &(celdas[fila * totalCols + col]);

	if(celda->tipo == LIBRE){
		if(direccion == ARRIBA && celda->arriba == '.')
			return 1;
		else if(direccion == ABAJO && celda->abajo == '.')
			return 1;
		else if(direccion == IZQUIERDA && celda->izquierda == '.')
			return 1;
		else if(direccion == DERECHA && celda->derecha == '.')
			return 1;
	}
	return 0;
}

//Funcion que ejecuta cada hilo
void* recorrerLaberinto(void* atributosHilo){	
	
	struct AtributosHilo *atributos = (struct AtributosHilo*) atributosHilo;	
	
	//marca la posicion en la que inicia
	atributos->celdas[atributos->filaActual * atributos->totalCols + atributos->colActual ].caracter = atributos->caracter;
	
	
	//se fija si puede crear hilos a los lados
	/*
	if(atributos->direccion == DERECHA || atributos->direccion == IZQUIERDA){
					
		//puede crear hijos arriba o abajo
					
		//si puede crear hacia arriba
		if(celdaEsLibre(atributos->colActual, atributos->filaActual-1, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
			pthread_t hiloHijo;
			struct AtributosHilo atributosHilo = {atributos->celdas, ARRIBA, atributos->totalFilas, atributos->totalCols, 
			atributos->filaActual-1, atributos->colActual, atributos->contadorRecorrido + 1,nuevoChar()};
			pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
			pthread_join(hiloHijo, NULL);
						
		//si puede crear hacia abajo
		}else if(celdaEsLibre(atributos->colActual, atributos->filaActual+1, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
			pthread_t hiloHijo;
			struct AtributosHilo atributosHilo = {atributos->celdas, ABAJO, atributos->totalFilas, atributos->totalCols, 
			atributos->filaActual+1, atributos->colActual, atributos->contadorRecorrido + 1,nuevoChar()};
			pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
			pthread_join(hiloHijo, NULL);
		}
					
					
	}
	else if(atributos->direccion == ARRIBA || atributos->direccion == ABAJO){
		//puede crear hijos derecha o izquierda
					
		//si puede crear hacia derecha
		if(celdaEsLibre(atributos->colActual+1, atributos->filaActual, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
			pthread_t hiloHijo;
			struct AtributosHilo atributosHilo = {atributos->celdas, ARRIBA, atributos->totalFilas, atributos->totalCols, 
			atributos->filaActual, atributos->colActual+1, atributos->contadorRecorrido + 1,nuevoChar()};
			pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
			pthread_join(hiloHijo, NULL);
						
		//si puede crear hacia izquierda
		}else if(celdaEsLibre(atributos->colActual-1, atributos->filaActual, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
			pthread_t hiloHijo;
			struct AtributosHilo atributosHilo = {atributos->celdas, ABAJO, atributos->totalFilas, atributos->totalCols, 
			atributos->filaActual, atributos->colActual-1, atributos->contadorRecorrido + 1,nuevoChar()};
			pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
			pthread_join(hiloHijo, NULL);
		}
	} */
	
	char fin = 0; //es 1 cuando muere o llega a una salida
	while(fin == 0) {				
		int sigFila = atributos->filaActual;
		int sigCol = atributos->colActual;
		
		//calcula la pos. a la que se va a mover
		switch (atributos->direccion) {
			case ABAJO:
				sigFila++;
			break;
			case ARRIBA:
				sigFila--;
			break;
			case DERECHA:
				sigCol++;
			break;
			case IZQUIERDA:
				sigCol--;
			break;
		}
		//busca la celda a la que se va a mover
		struct Celda * celdaSig = &(atributos->celdas[sigFila * atributos->totalCols + sigCol ]);

		//se fija si la siguiente celda es libre
		if(celdaSig->tipo == LIBRE){
			if( (atributos->direccion == ABAJO && celdaSig->abajo == '.') ||
				(atributos->direccion == ARRIBA && celdaSig->arriba == '.')  ||
				(atributos->direccion == DERECHA && celdaSig->derecha == '.') ||
				(atributos->direccion == IZQUIERDA && celdaSig->izquierda == '.') ) {

				// Actualiza la posición actual con la siguiente
				atributos->colActual = sigCol;
				atributos->filaActual = sigFila;
				// Se registra en la celda
				switch (atributos->direccion) {
					case ABAJO:
						celdaSig->abajo = pthread_self();
						break;
					case ARRIBA:
						celdaSig->arriba = pthread_self();
						break;
					case DERECHA:
						celdaSig->derecha = pthread_self();
						break;
					case IZQUIERDA:
						celdaSig->izquierda = pthread_self();
						break;
				}
				celdaSig->caracter = atributos->caracter;	
				
				//se fija si puede crear hilos a los lados
				
				if(atributos->direccion == DERECHA || atributos->direccion == IZQUIERDA){
					
					//puede crear hijos arriba o abajo
					
					//si puede crear hacia arriba
					if(celdaEsLibre(atributos->colActual, atributos->filaActual-1, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
						pthread_t hiloHijo;
						struct AtributosHilo atributosHilo = {atributos->celdas, ARRIBA, atributos->totalFilas, atributos->totalCols, 
							atributos->filaActual-1, atributos->colActual, atributos->contadorRecorrido + 1,nuevoChar()};
						pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
						pthread_join(hiloHijo, NULL);
						
					//si puede crear hacia abajo
					}else if(celdaEsLibre(atributos->colActual, atributos->filaActual+1, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
						pthread_t hiloHijo;
						struct AtributosHilo atributosHilo = {atributos->celdas, ABAJO, atributos->totalFilas, atributos->totalCols, 
							atributos->filaActual+1, atributos->colActual, atributos->contadorRecorrido + 1,nuevoChar()};
						pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
						pthread_join(hiloHijo, NULL);
					}
					
					
				}
				else if(atributos->direccion == ARRIBA || atributos->direccion == ABAJO){
					//puede crear hijos derecha o izquierda
					
					//si puede crear hacia derecha
					if(celdaEsLibre(atributos->colActual+1, atributos->filaActual, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
						pthread_t hiloHijo;
						struct AtributosHilo atributosHilo = {atributos->celdas, ARRIBA, atributos->totalFilas, atributos->totalCols, 
							atributos->filaActual, atributos->colActual+1, atributos->contadorRecorrido + 1,nuevoChar()};
						pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
						pthread_join(hiloHijo, NULL);
						
					//si puede crear hacia izquierda
					}else if(celdaEsLibre(atributos->colActual-1, atributos->filaActual, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion)){
						pthread_t hiloHijo;
						struct AtributosHilo atributosHilo = {atributos->celdas, ABAJO, atributos->totalFilas, atributos->totalCols, 
							atributos->filaActual, atributos->colActual-1, atributos->contadorRecorrido + 1,nuevoChar()};
						pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
	
						pthread_join(hiloHijo, NULL);
					}
					
					
				}
				
			}
		}
		else if(celdaSig->tipo == PARED){
			fin = 1;
		} else fin = 1; //Llegó
	}	
	return NULL;
}

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
	while ((charActual = fgetc(fileLaberinto)) != ' '){
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
		
	struct Celda laberinto[filas][columnas];	
	
	for(int i = 0; i < filas; i ++){
		for(int j = 0; j < columnas; j++){
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
	
	
	while((charActual = fgetc(fileLaberinto)) != EOF){
		if(charActual == '\n'){
			j = 0;
			i++;
			continue;
		}
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
		
	fclose(fileLaberinto);
	
	//TERMINA DE LEER EL ARCHIVO AQUI
	
	
	//imprime el laberinto
	for(int i = 0; i < filas; i ++){
		for(int j = 0; j < columnas; j++){
			printf("%c", laberinto[i][j].caracter);
		}
		printf("\n");
	}
	


	//crea un hilo de ejemplo
	pthread_t hiloInicial;
	struct AtributosHilo atributosHilo = {*laberinto, DERECHA, filas, columnas, 0, 0, 0, nuevoChar()};
	
	if ( laberinto[1][0].tipo == PARED) {
		atributosHilo.direccion = ABAJO;
	}
	
	pthread_create(&hiloInicial, NULL, recorrerLaberinto, &atributosHilo);
	
	pthread_join(hiloInicial, NULL);
			
	
	printf("laberinto\n");
	printf("\n");
	//imprime el laberinto
	for(int i = 0; i < filas; i ++){
		for(int j = 0; j < columnas; j++){
			printf("%c", laberinto[i][j].caracter);
		}
		printf("\n");
	}
	
	
	return 0;
}
