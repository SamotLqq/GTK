#ifndef INTERFACES_H
#define INTERFACES_H

#include <gtk/gtk.h>
#include "trabajo.h"



typedef struct
{
    GtkBuilder* builder;
    GHashTable *tableWidgetsLabel;
    Trabajo** trabajos;
    int* contadorTrabajosTotal;
    int* contadorTrabajosActual;
} Contexto;

typedef struct
{
    Contexto* ctx;
    char* tituloTrabajo;
    char* widgetBase;
    char* widgetPrevia;
    char* trabajoId;
    char* accion;
} Args;

typedef struct 
{
    Args** arr;
    int cant;
} ArgsArray;


// Devuelve una nueva instancia de Args con los argumentos inicializados en NULL.
Args* inicializar_args();
// Devuelve una instancia de ArgsArray con lugar para 5 arr y contador en 0.
ArgsArray* inicializar_args_arr();
// Agrega un elemento al arreglo de argumentos al final
void agregar_args_arr(ArgsArray* arr, Args* args);
// Agrega un elemento al arreglo de argumento al principio
void agg_arr_inicio(ArgsArray* arr, Args* args);



// recibe como argumento un puntero a una estructura de tipo ArgsArray
void destruccion_widget(ArgsArray* arr);

// renderiza principal.glade con funcionalidades
void renderizar_principal(ArgsArray* arr);
// renderiza un trabajo en principal.glade
void renderizar_trabajo_principal(ArgsArray* arr);
// renderiza detalle.glade con funcionalidades
void renderizar_detalle(ArgsArray* arr);
// renderiza listado.glade con funcionalidades
void renderizar_listado(ArgsArray* arr);
// renderiza detalleEditar.glade
void renderizar_detalle_editar(ArgsArray* arr);
// renderiza trabajo.glade
void renderizar_trabajo (ArgsArray* arr);
// renderiza trabajoEditar.glade
void renderizar_trabajo_editar (ArgsArray* arr);
// manejador del boton volver a principal.glade desde listado.glade
void handle_volver_listado(GtkButton* button, gpointer data);
// manejador del boton volver desde detalle.glade
void handle_volver_detalle(GtkButton* button, gpointer data);
// manejador del boton volver desde detalleEditar.glade
void handle_volver_de(GtkButton* button, gpointer data);
// manejador del boton volver de trabajo.glade
void handle_volver_trabajo(GtkButton* button, gpointer data);
// manejador del boton volver de trabajoEditar.glade
void handle_volver_te(GtkButton* button, gpointer data);
// manejador del boton editar de detalle.glade
void handle_editar_detalle(GtkButton* button, gpointer data);
// manejador del boton editar de listado.glade
void handle_editar_listado(GtkButton* button, gpointer data);
// manejador del boton editar de trabajo.glade
void handle_editar_trabajo(GtkButton* button, gpointer data);
// manejador del boton editar de principal.glade
void handle_editar_trabajo_principal(GtkButton* button, gpointer data);
// manejador del boton ver de principal.glade
void handle_ver_principal(GtkButton* button, gpointer data);
// manejador del boton ver de listado.glade
void handle_ver_listado(GtkButton* button, gpointer data);
// manejador del boton buscar de principal.glade
void handle_buscar_principal(GtkButton* button, gpointer data);
// manejador del boton buscar de principal.glade
void handle_agregar_principal(GtkButton* button, gpointer data);
// manejador del boton agregar trabajo en principal.glade
void handle_agregar_trabajo_principal(GtkButton* button, gpointer data);
// manejador del boton eliminar trabajo en principal.glade
void handle_eliminar_trabajo(GtkButton* button, gpointer data);
// manejador del boton eliminar de listado.glade
void handle_eliminar_listado(GtkButton* button, gpointer data);
// manejador de los botones del combobox
void handle_combobox_detalle(GtkComboBox *combobox, gpointer data);
// manejador del boton confriamr de trabajoEditar.glade
void handle_confirmar_te(GtkButton* button, gpointer data);
// manejador del boton confriamr de detalleEditar.glade
void handle_confirmar_de(GtkButton* button, gpointer data);

#endif