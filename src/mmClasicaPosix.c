/*********************************************************************
Pontificia Universidad Javeriana
Autor: Andrés Barrios
Materia: Sistemas Operativos
Profesor: John Corredor
Fecha: 2 de mayo de 2025
Descripción:
Este programa realiza la multiplicación de matrices cuadradas utilizando
paralelismo con hilos POSIX (pthreads). Cada hilo se encarga de procesar
un subconjunto de filas, y se mide el tiempo total de ejecución.
********************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// Definimos un bloque de memoria global lo suficientemente grande para 3 matrices
#define DATA_SIZE (1024*1024*64*3)

pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC;

// Estructura de parámetros para cada hilo
struct parametros{
	int nH;   // Número total de hilos
	int idH;  // ID del hilo
	int N;    // Tamaño de la matriz
};

// Variables globales para medición de tiempo
struct timeval inicio, fin;

// Inicia la medición de tiempo
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

// Finaliza la medición de tiempo y muestra resultado en microsegundos
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

// Inicializa matrices A y B con valores aleatorios, y C con ceros
void iniMatrix(int SZ){
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = rand() % 10;
		mB[i] = rand() % 10;
		mC[i] = 0;
	}
}

// Imprime una matriz si su tamaño es pequeño (menor a 12)
void impMatrix(int sz, double *matriz){
	if(sz < 12){
    	for(int i = 0; i < sz*sz; i++){
     		if(i%sz==0) printf("\n");
            printf(" %.3f ", matriz[i]);
		}
    	printf("\n>-------------------->\n");
	}
}

// Función ejecutada por cada hilo: multiplica parte de las filas
void *multiMatrix(void *variables){
	struct parametros *data = (struct parametros *)variables;

	int idH = data->idH;
	int nH  = data->nH;
	int N   = data->N;
	int ini = (N/nH)*idH;
	int fin = (N/nH)*(idH+1);

    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i*N);
			pB = mB + j;
            for (int k = 0; k < N; k++, pA++, pB+=N){
				sumaTemp += (*pA * *pB);
			}
			mC[i*N+j] = sumaTemp;
		}
	}

	// Bloqueo simulado (aunque no se necesita aquí)
	pthread_mutex_lock (&MM_mutex);
	pthread_mutex_unlock (&MM_mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	// Verificación de argumentos
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		exit(0);
	}

    int SZ = atoi(argv[1]);       // Tamaño de la matriz
    int n_threads = atoi(argv[2]); // Número de hilos

    pthread_t p[n_threads];
    pthread_attr_t atrMM;

	// Asignar punteros a bloques consecutivos dentro de MEM_CHUNK
	mA = MEM_CHUNK;
	mB = mA + SZ*SZ;
	mC = mB + SZ*SZ;

	// Inicializar matrices
	iniMatrix(SZ);
	impMatrix(SZ, mA);
	impMatrix(SZ, mB);

	InicioMuestra();

	// Configurar atributos y crear hilos
	pthread_mutex_init(&MM_mutex, NULL);
	pthread_attr_init(&atrMM);
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    for (int j=0; j<n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros));
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
        pthread_create(&p[j], &atrMM, multiMatrix, (void *)datos);
	}

    // Esperar a que todos los hilos terminen
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j], NULL);

	FinMuestra();

	// Imprimir la matriz resultado si es pequeña
	impMatrix(SZ, mC);

	// Limpiar
	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit(NULL);
}
