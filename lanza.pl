#!/usr/bin/perl
#####################################################################
# Pontificia Universidad Javeriana
# Autor: Andrés Barrios
# Materia: Sistemas Operativos
# Profesor: John Corredor
# Fecha: 2 de mayo de 2025
# Descripción:
# Script en Perl que ejecuta automáticamente una batería de pruebas
# para el programa de multiplicación de matrices (OpenMP, POSIX o Fork).
# Ejecuta combinaciones de tamaños de matriz y número de hilos y
# guarda los resultados de tiempo en archivos .dat.
#####################################################################

use strict;
use warnings;

# Obtener ruta actual del proyecto
my $Path = `pwd`;
chomp($Path);

# Ruta al ejecutable que deseas probar
# Puedes cambiar entre: bin/fork, bin/posix o bin/openmp
my $Nombre_Ejecutable = "$Path/bin/openmp";

# Lista de tamaños de matriz a probar (NxN)
my @Size_Matriz = ("256", "512", "1024");

# Lista de números de hilos/procesos a usar
my @Num_Hilos   = (1, 2, 4, 8);

# Número de repeticiones por combinación (mínimo 30)
my $Repeticiones = 30;

# Bucle principal de experimentación
foreach my $size (@Size_Matriz){
	foreach my $hilo (@Num_Hilos) {

		# Definir nombre del archivo de salida para esa combinación
		my $file = "$Path/data/MM-${size}-H${hilo}.dat";

		# Abrir archivo en modo escritura (append)
		open(my $OUT, ">>", $file) or die "No se puede abrir $file: $!";

		print "Ejecutando: $Nombre_Ejecutable $size $hilo x$Repeticiones veces...\n";

		# Ejecutar la prueba múltiples veces
		for (my $i = 0; $i < $Repeticiones; $i++) {
			my $cmd = "$Nombre_Ejecutable $size $hilo";
			my $resultado = `$cmd`;
			print $OUT $resultado;
		}

		close($OUT);
	}
}
