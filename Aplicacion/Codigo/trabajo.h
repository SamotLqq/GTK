#ifndef TRABAJO_H
#define TRABAJO_H

#include <gtk/gtk.h>

typedef struct
{
    guint dia;
    guint mes;
    guint year;
} Fecha;

typedef struct
{
    gchar* id;
    gchar* km;
    gchar* titulo;
    Fecha* fecha;
    gchar* descripcion;
} Trabajo;


// Funciones para trabajos
Trabajo** inicializar_trabajos();
void agregar_trabajo(Trabajo** trabajos, Trabajo* trabajo, int* contadorTrabajosActual, int* contadorTrabajosTotal);
void eliminar_trabajo(Trabajo** trabajos, char* idTrabajo, int* contadorTrabajosActual);
void editar_trabajo(Trabajo** trabajos, Trabajo* nuevosDatos, char* idTrabajo, int* contadorTrabajosActual);
void liberar_trabajos(Trabajo** trabajos, int* contadorTrabajosActual);
void imprimir_trabajos(Trabajo** trabajos, int* contadorTrabajosActual);



#endif