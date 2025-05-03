#####################################################################
# Pontificia Universidad Javeriana
# Autor: Andrés Barrios
# Materia: Sistemas Operativos
# Profesor: John Corredor
# Fecha: 2 de mayo de 2025
# Descripción:
# Este Makefile compila tres versiones del programa de multiplicación
# de matrices: usando procesos (fork), hilos POSIX (pthreads) y OpenMP.
# Los ejecutables se guardan en la carpeta bin/.
#####################################################################

# Compilador
CC = gcc

# Carpeta de fuentes y destino
SRC = src
BIN = bin

# Flags generales
CFLAGS = -O2
OMP_FLAGS = -fopenmp
POSIX_FLAGS = -lpthread

# Regla principal: compila todo
all: $(BIN)/fork $(BIN)/posix $(BIN)/openmp

# Compila la versión fork
$(BIN)/fork: $(SRC)/mmClasicaFork.c
	$(CC) $(CFLAGS) $< -o $@

# Compila la versión POSIX (hilos pthread)
$(BIN)/posix: $(SRC)/mmClasicaPosix.c
	$(CC) $(CFLAGS) $(POSIX_FLAGS) $< -o $@

# Compila la versión OpenMP
$(BIN)/openmp: $(SRC)/mmClasicaOpenMP.c
	$(CC) $(CFLAGS) $(OMP_FLAGS) $< -o $@

# Limpia los ejecutables generados
clean:
	rm -f $(BIN)/*
