/****************************************************************************
Pontificia Universidad Javeriana
Autor: Andrés Barrios
Materia: Sistemas Operativos
Profesor: John Corredor
Fecha: 2 de mayo de 2025
Descripción:
Este programa realiza la multiplicación de matrices cuadradas utilizando
paralelismo mediante OpenMP. Cada hilo ejecuta una parte del cálculo de
filas, y se mide el tiempo de ejecución total. Está diseñado para evaluar
el rendimiento en entornos con múltiples núcleos.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

// Variables globales para la medición de tiempo
struct timeval inicio, fin;

// Inicia la captura del tiempo
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

// Finaliza la captura del tiempo y muestra en microsegundos
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

// Imprime matrices pequeñas para verificar (si D < 9)
void impMatrix(size_t *matrix, int D){
	printf("\n");
	if(D < 9){
		for(int i=0; i<D*D; i++){
			if(i%D==0) printf("\n");
			printf("%zu ", matrix[i]);
		}
		printf("\n**-----------------------------**\n");
	}
}

// Inicializa las matrices A y B con valores pseudoaleatorios
void iniMatrix(size_t *m1, size_t *m2, int D){
	for(int i=0; i<D*D; i++, m1++, m2++){
		*m1 = i*2;   // Se puede reemplazar por rand()%10;
		*m2 = i+2;
	}
}

// Multiplica las matrices A y B, resultado en C usando OpenMP
void multiMatrix(size_t *mA, size_t *mB, size_t *mC, int D){
	size_t Suma, *pA, *pB;

	// Inicia región paralela
	#pragma omp parallel
	{
	// Divide el bucle for externo entre hilos
	#pragma omp for
	for(int i=0; i<D; i++){
		for(int j=0; j<D; j++){
			pA = mA+i*D;
			pB = mB+j;
			Suma = 0;
			for(int k=0; k<D; k++, pA++, pB+=D){
				Suma += *pA * *pB;
			}
			mC[i*D+j] = Suma;
		}
	}
	}
}

int main(int argc, char *argv[]){
	// Validar número de argumentos
	if(argc < 3){
		printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
		exit(0);
	}

	// Obtener tamaño de la matriz y número de hilos
	int N = atoi(argv[1]);
	int TH = atoi(argv[2]);

	// Reservar memoria para matrices
	size_t *matrixA  = (size_t *)calloc(N*N, sizeof(size_t));
	size_t *matrixB  = (size_t *)calloc(N*N, sizeof(size_t));
	size_t *matrixC  = (size_t *)calloc(N*N, sizeof(size_t));

	srand(time(NULL));

	// Fijar número de hilos en OpenMP
	omp_set_num_threads(TH);

	// Inicializar matrices de entrada
	iniMatrix(matrixA, matrixB, N);

	// Imprimir matrices si son pequeñas
	impMatrix(matrixA, N);
	impMatrix(matrixB, N);

	// Medición de tiempo y cálculo
	InicioMuestra();
	multiMatrix(matrixA, matrixB, matrixC, N);
	FinMuestra();

	// Mostrar matriz resultado si es pequeña
	impMatrix(matrixC, N);

	// Liberar memoria
	free(matrixA);
	free(matrixB);
	free(matrixC);

	return 0;
}
