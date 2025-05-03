/****************************************************************************
Pontificia Universidad Javeriana
Autor: Andrés Barrios
Materia: Sistemas Operativos
Profesor: John Corredor
Fecha: 2 de mayo de 2025
Descripción:
Este programa realiza la multiplicación de matrices cuadradas usando
procesos (modelo fork). Cada proceso hijo calcula un subconjunto de filas,
y al final se imprime el tiempo de ejecución total.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

// Variables globales para medir tiempo de ejecución
struct timeval inicio, fin;

// Inicia la medición de tiempo
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

// Finaliza la medición de tiempo y muestra en microsegundos
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

// Multiplica porciones de la matriz asignadas al proceso
void multiMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF) {
	double Suma, *pA, *pB;
    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {
			Suma = 0;
			pA = mA+i*D;
			pB = mB+j;
            for (int k = 0; k < D; k++, pA++, pB+=D) {
				Suma += *pA * *pB;
            }
			mC[i*D+j] = Suma;
        }
    }
}

// Imprime matrices pequeñas para verificación visual
void impMatrix(double *matrix, int D) {
	if (D < 9) {
    	printf("\nImpresión...\n");
    	for (int i = 0; i < D*D; i++, matrix++) {
			if(i%D==0) printf("\n");
            	printf(" %f ", *matrix);
        }
        printf("\n ");
    }
}

// Llena las matrices A y B con números aleatorios
void iniMatrix(double *mA, double *mB, int D){
	for (int i = 0; i < D*D; i++, mA++, mB++){
            *mA = rand() % 10;
            *mB = rand() % 10;
        }
}

int main(int argc, char *argv[]) {
	// Validación de argumentos
	if (argc < 3) {
		printf("\n \t\tUse: $./nom_ejecutable Size Hilos \n");
		exit(0);
	}

	// Lectura de argumentos: tamaño y número de procesos
	int N      = atoi(argv[1]);
	int num_P  = atoi(argv[2]);

	// Reservar memoria para matrices
    double *matA = (double *) calloc(N*N, sizeof(double));
    double *matB = (double *) calloc(N*N, sizeof(double));
    double *matC = (double *) calloc(N*N, sizeof(double));

    srand(time(0)); // Semilla para generación aleatoria

    iniMatrix(matA, matB, N); // Inicializa A y B
    impMatrix(matA, N);       // Imprime si es pequeña
    impMatrix(matB, N);

    int rows_per_process = N / num_P;

	// Iniciar medición de tiempo
	InicioMuestra();

	// Crear procesos hijos
    for (int i = 0; i < num_P; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Proceso hijo
            int start_row = i * rows_per_process;
            int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;

			// Calcula su porción de la matriz
			multiMatrix(matA, matB, matC, N, start_row, end_row);

			if(N < 9){
            	printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), start_row, end_row-1);
            	for (int r = start_row; r < end_row; r++) {
                	for (int c = 0; c < N; c++) {
                    	printf(" %f ", matC[N*r+c]);
                	}
                	printf("\n");
            	}
			}
            exit(0); // El hijo termina
        } else if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
    }

    // Proceso padre espera a todos los hijos
    for (int i = 0; i < num_P; i++) {
        wait(NULL);
    }

  	// Finaliza medición y muestra tiempo
	FinMuestra();

	// Liberar memoria
	free(matA);
	free(matB);
	free(matC);

    return 0;
}
