#include "trabajo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Funciones para el estado global trabajos.

Trabajo** inicializar_trabajos() {
    Trabajo** trabajos = malloc(sizeof(Trabajo*)*50);
    if (trabajos == NULL) {
        perror("Error al inicializar la lista de trabajos");
        exit(EXIT_FAILURE);
    }
    return trabajos;
}

void agregar_trabajo(Trabajo** trabajos, Trabajo* trabajo, int* contadorTrabajosActual, int* contadorTrabajosTotal) {
    trabajos[*contadorTrabajosActual] = trabajo;
    *contadorTrabajosTotal++;
    *contadorTrabajosActual++;
    imprimir_trabajos(trabajos, contadorTrabajosActual);
}

void eliminar_trabajo(Trabajo** trabajos, char* idTrabajo, int* contadorTrabajosActual) {
    int encontrado = 0;
    for (int i = 0; i < *contadorTrabajosActual; i++) {
        if (encontrado) {
            trabajos[i-1] = trabajos[i];
        }
        else if (strcmp(trabajos[i]->id, idTrabajo) == 0) {
            encontrado = 1;
            free(trabajos[i]->descripcion);
            free(trabajos[i]->fecha);
            free(trabajos[i]->km);
            free(trabajos[i]->titulo);
            free(trabajos[i]->id);
            free(trabajos[i]);
            trabajos[i] = NULL;
        }  
    }
    if (encontrado) {
        *contadorTrabajosActual--;
    }
}

void editar_trabajo(Trabajo** trabajos, Trabajo* nuevosDatos, char* idTrabajo, int* contadorTrabajosActual) {
    for (int i = 0; i < *contadorTrabajosActual; i++) {
        if (strcmp(trabajos[i]->id, idTrabajo) == 0) {
            free(trabajos[i]->descripcion);
            free(trabajos[i]->fecha);
            free(trabajos[i]->id);
            free(trabajos[i]->titulo);
            free(trabajos[i]->km);
            free(trabajos[i]);
            trabajos[i] = nuevosDatos;
        }  
    }
}

void liberar_trabajos(Trabajo** trabajos, int* contadorTrabajosActual) {
    for (int i = 0; i < *contadorTrabajosActual; i++) {
        free(trabajos[i]->descripcion);
        free(trabajos[i]->fecha);
        free(trabajos[i]->km);
        free(trabajos[i]->titulo);
        free(trabajos[i]->id);
        free(trabajos[i]);
    }
}

void imprimir_trabajos(Trabajo** trabajos, int* contadorTrabajosActual) {
    printf("Trabajos:\n");
    for (int i = 0; i < *contadorTrabajosActual; i++) {
        printf("id: %s - ", trabajos[i]->id);
        printf("km: %s - ", trabajos[i]->km);
        printf("titulo: %s - ", trabajos[i]->titulo);
        printf("fecha: %d/%d/%d - ", trabajos[i]->fecha->dia, trabajos[i]->fecha->mes, trabajos[i]->fecha->year);
        printf("descripcion: %s\n", trabajos[i]->descripcion);
    }
}