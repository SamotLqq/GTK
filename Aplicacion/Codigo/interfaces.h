#ifndef INTERFACES_H
#define INTERFACES_H

#include <gtk/gtk.h>
#include "trabajo.h"

// estructura para argumentos de los handlers
typedef struct 
{
    GtkBuilder *builder;
    Trabajo** trabajos;
    int* contadorTrabajosActual;
    int* contadorTrabajosTotal;
    char* widgetBase;
    char* widgetPrevia;
    gchar* accion;
    gchar* trabajoId;
    GHashTable *tableWidgetsLabel;
} HandlerArgs;

typedef struct 
{
    GtkBuilder* builder;
    Trabajo** trabajos;
    int numeralId;
    char* tituloTrabajo;
    char* widgetBase;
    char* widgetPrevia;
    char* trabajoId;
    char* accion;
    int* contadorTrabajosActual;
    int* contadorTrabajoTotal;
    GHashTable *tableWidgetsLabel;
} RendererArgs;


// renderiza principal.glade con funcionalidades
void renderizar_principal(RendererArgs* args);
// renderiza un trabajo en principal.glade
void renderizar_trabajo_principal(RendererArgs* args);
// renderiza detalle.glade con funcionalidades
void renderizar_detalle(RendererArgs* args);
// renderiza listado.glade con funcionalidades
void renderizar_listado(RendererArgs* args);
// renderiza detalleEditar.glade
void renderizar_detalle_editar(RendererArgs* args);
// renderiza trabajo.glade
void renderizar_trabajo (RendererArgs* args);
// renderiza trabajoEditar.glade
void renderizar_trabajo_editar (RendererArgs* args);
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