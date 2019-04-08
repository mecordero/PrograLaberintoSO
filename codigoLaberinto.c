// HOla
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//Enum de los distintos tipos de celda
enum Tipo_Celda
{
	PARED,
	FINAL,
	LIBRE
} tipo;
enum Direccion
{
	ARRIBA,
	ABAJO,
	DERECHA,
	IZQUIERDA
} direccion;

//variable del caracter actual
char caracterActual = 48;
sem_t charName_sem;
int exito = 0;

//Estructura de una celda del laberinto
struct Celda
{
	char caracter;
	char arriba;
	char abajo;
	char derecha;
	char izquierda;
	enum Tipo_Celda tipo;
	sem_t semaforo;
};

//Estructura de los atributos que tiene cada hilo
struct AtributosHilo
{
	struct Celda *celdas;
	enum Direccion direccion;
	int totalFilas;
	int totalCols;
	int filaActual;
	int colActual;
	int contadorRecorrido;
	char caracter;
};

//Funcion generadora de caracteres
char nuevoChar()
{
	sem_wait(&charName_sem);

	if (caracterActual == 57)
	{
		caracterActual = 65;
	}
	else if (caracterActual == 90)
	{
		caracterActual = 97;
	}
	else
		caracterActual++;

	sem_post(&charName_sem);
	return caracterActual;
}

//Funcion que revisa si una celda es libre
char celdaEsLibre(int col, int fila, struct Celda *celdas, int totalCols, int totalFilas, enum Direccion direccion)
{

	if (col < 0 || fila < 0)
		return 0;
	if (col >= totalCols || fila >= totalFilas)
		return 0;
	struct Celda *celda = &(celdas[fila * totalCols + col]);

	if (celda->tipo == LIBRE)
	{
		if (direccion == ARRIBA && celda->arriba == '.')
		{
			return 1;
		}
		else if (direccion == ABAJO && celda->abajo == '.')
		{
			return 1;
		}
		else if (direccion == IZQUIERDA && celda->izquierda == '.')
		{
			return 1;
		}
		else if (direccion == DERECHA && celda->derecha == '.')
		{
			return 1;
		}
	}
	return 0;
}

//Funcion que imprime laberinto
void imprimirLaberinto(struct Celda *celdas, int filas, int columnas)
{
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			printf("%c", celdas[j + i * columnas].caracter);
		}
		printf("\n");
	}
	printf("\n");
}

//Funcion que ejecuta cada hilo
void *recorrerLaberinto(void *atributosHilo)
{

	struct AtributosHilo *atributos = (struct AtributosHilo *)atributosHilo;

	char fin = 0; //es 1 cuando muere o llega a una salida
	while (fin == 0)
	{
		//imprimirLaberinto(atributos->celdas, atributos->totalFilas, atributos->totalCols);
		struct Celda *celda = &atributos->celdas[atributos->filaActual * atributos->totalCols + atributos->colActual];
		sem_wait(&(celda->semaforo));
		//marca la posicion en la que inicia
		celda->caracter = atributos->caracter;
		
		// Se registra en la celda
		switch (atributos->direccion)
		{
		case ABAJO:
			celda->abajo = '*';
			// printf("abajo es: %c\n", celda->abajo);
			break;
		case ARRIBA:
			celda->arriba = '*';
			// printf("arriba es: %c\n", celda->arriba);
			break;
		case DERECHA:
			celda->derecha = '*';
			// printf("derecha es: %c\n", celda->derecha);
			break;
		case IZQUIERDA:
			celda->izquierda = '*';
			// printf("izquierda es: %c\n", celda->izquierda);
			break;
		}
		
		//aumenta el contador
		atributos->contadorRecorrido++;

		//se fija si puede crear hilos a los lados

		if (atributos->direccion == DERECHA || atributos->direccion == IZQUIERDA)
		{

			//puede crear hijos arriba o abajo

			//si puede crear hacia arriba
			if (celdaEsLibre(atributos->colActual, atributos->filaActual - 1, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion))
			{
				pthread_t hiloHijo;
				char id = nuevoChar();
				struct AtributosHilo atributosHilo = {atributos->celdas, ARRIBA, atributos->totalFilas, atributos->totalCols,
													  atributos->filaActual - 1, atributos->colActual, atributos->contadorRecorrido, id};
				pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);

				pthread_join(hiloHijo, NULL);
			//si puede crear hacia abajo
			}

			if (celdaEsLibre(atributos->colActual, atributos->filaActual + 1, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion))
			{
				pthread_t hiloHijo;
				char id = nuevoChar();
				struct AtributosHilo atributosHilo = {atributos->celdas, ABAJO, atributos->totalFilas, atributos->totalCols,
													  atributos->filaActual + 1, atributos->colActual, atributos->contadorRecorrido, id};
				pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);

				pthread_join(hiloHijo, NULL);
			}
		}
		else if (atributos->direccion == ARRIBA || atributos->direccion == ABAJO)
		{
			//puede crear hijos derecha o izquierda

			//si puede crear hacia derecha
			if (celdaEsLibre(atributos->colActual + 1, atributos->filaActual, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion))
			{
				pthread_t hiloHijo;
				char id = nuevoChar();
				struct AtributosHilo atributosHilo = {atributos->celdas, DERECHA, atributos->totalFilas, atributos->totalCols,
													  atributos->filaActual, atributos->colActual + 1, atributos->contadorRecorrido, id};
				pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);

				pthread_join(hiloHijo, NULL);

				//si puede crear hacia izquierda
			} 
			if (celdaEsLibre(atributos->colActual - 1, atributos->filaActual, atributos->celdas, atributos->totalCols, atributos->totalFilas, atributos->direccion))
			{
				pthread_t hiloHijo;
				char id = nuevoChar();
				struct AtributosHilo atributosHilo = {atributos->celdas, IZQUIERDA, atributos->totalFilas, atributos->totalCols,
													  atributos->filaActual, atributos->colActual - 1, atributos->contadorRecorrido, id};
				pthread_create(&hiloHijo, NULL, recorrerLaberinto, &atributosHilo);
				pthread_join(hiloHijo, NULL);
			}
		}

		int sigFila = atributos->filaActual;
		int sigCol = atributos->colActual;

		//calcula la pos. a la que se va a mover
		switch (atributos->direccion)
		{
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
		struct Celda *celdaSig = &(atributos->celdas[sigFila * atributos->totalCols + sigCol]);

		//se fija si la siguiente celda es libre
		if (celdaSig->tipo == LIBRE)
		{
			if ((atributos->direccion == ABAJO && celdaSig->abajo == '.') ||
				(atributos->direccion == ARRIBA && celdaSig->arriba == '.') ||
				(atributos->direccion == DERECHA && celdaSig->derecha == '.') ||
				(atributos->direccion == IZQUIERDA && celdaSig->izquierda == '.'))
			{
				// Actualiza la posición actual con la siguiente
				atributos->colActual = sigCol;
				atributos->filaActual = sigFila;
			}
		}
		else if (celdaSig->tipo == PARED) // TODO: Falta caso cuando se llega al final del laberinto
		{
			fin = 1;
		}
		else {
			fin = 1; //Llegó
			exito = 0;
			
			printf("El hilo %c llega a una salida, ha reccorrido %d espacios\n\n", atributos->caracter, ++atributos->contadorRecorrido);
			
		}


		sem_post(&celda->semaforo);
		sleep(2);
	}
	return NULL;
}

void * hilo_impr(void * atributosHilo) {
	struct AtributosHilo *atributos = (struct AtributosHilo *)atributosHilo;
	while( exito == 0) {
		sleep(1);
		imprimirLaberinto(atributos->celdas, atributos->totalFilas, atributos->totalCols);
	}

	return NULL;
}


int main(int argc, char **argv)
{
	FILE *fileLaberinto;

	// se inicializa el semáforo para los nombres de los hilos
	sem_init(&charName_sem, 0, 10);

	char charActual; //char que lee el archivo
	int posActual;   //posicion del arreglo medidaX y medidaY

	char numFilas[5];	//string numero de filas
	char numColumnas[5]; //string numero de columnas

	int filas = 0;	//entero de filas
	int columnas = 0; //entero de columnas

	char *fileName = "laberinto.txt";

	fileLaberinto = fopen(fileName, "r");
	if (fileLaberinto == NULL)
	{
		printf("Could not open file %s", fileName);
		return 1;
	}

	//obtiene numero de filas
	posActual = 0;
	while ((charActual = fgetc(fileLaberinto)) != ' ')
	{
		numFilas[posActual++] = charActual;
	}
	numFilas[posActual] = '\0';

	//obtiene medida de y
	posActual = 0;
	while ((charActual = fgetc(fileLaberinto)) != '\n')
	{
		numColumnas[posActual++] = charActual;
	}
	numColumnas[posActual] = '\0';

	//convierte strings a enteros
	sscanf(numFilas, "%d", &filas);
	sscanf(numColumnas, "%d", &columnas);

	printf("Filas: %d\n", filas);
	printf("Columnas: %d\n", columnas);

	struct Celda laberinto[filas][columnas];

	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			laberinto[i][j].arriba = '.';
			laberinto[i][j].abajo = '.';
			laberinto[i][j].derecha = '.';
			laberinto[i][j].izquierda = '.';
			tipo = PARED;
			sem_init(&laberinto[i][j].semaforo, 0, 10);
		}
	}

	int i = 0;
	int j = 0;

	while ((charActual = fgetc(fileLaberinto)) != EOF)
	{
		if (charActual == '\n')
		{
			j = 0;
			i++;
			continue;
		}
		laberinto[i][j].caracter = charActual;
		switch (charActual)
		{
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

	imprimirLaberinto(*laberinto, filas, columnas);
	sleep(1);

	//crea un hilo de ejemplo
	pthread_t hiloInicial;
	pthread_t impresion;
	struct AtributosHilo atributosHilo = {*laberinto, ABAJO, filas, columnas, 0, 0, 0, nuevoChar()};
	if (laberinto[1][0].tipo == PARED)
	{
		atributosHilo.direccion = DERECHA;
	}

	pthread_create(&hiloInicial, NULL, recorrerLaberinto, &atributosHilo);

	pthread_create(&impresion, NULL, hilo_impr, &atributosHilo);

	pthread_join(hiloInicial, NULL);

	return 0;
}
