#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <pango/pango.h>
#include "trabajo.h"
#include "interfaces.h"
#include "aux.h"

int main(int argc, char *argv[]) {

    // Inicializar trabajos
    Trabajo** trabajos = inicializar_trabajos();
    // Contador global para los trabajos agregados contando eliminados
    int contadorTrabajosTotal = 0;
    // Contador global para los trabajos actuales
    int contadorTrabajosActual = 0;
    // Inicializamos tabla hash para guardar los widgets de los labels
    GHashTable *tableWidgetsLabel = g_hash_table_new(g_str_hash, g_str_equal);

    // Inicializar GTK
    gtk_init(&argc, &argv);
    
    // Crear un objeto Builder para cargar la interfaz desde el archivo Glade
    GtkBuilder *builder = gtk_builder_new();

    // muestra la ventana Principal.glade al ejecutar.
    Args* args = inicializar_args();
    Contexto* ctx = malloc(sizeof(Contexto));
    ctx->tableWidgetsLabel = tableWidgetsLabel;
    ctx->builder = builder;
    ctx->trabajos = trabajos;
    ctx->contadorTrabajosActual = &contadorTrabajosActual;
    ctx->contadorTrabajosTotal = &contadorTrabajosTotal;
    args->ctx = ctx;
    renderizar_principal(args);

    // Iniciar el bucle principal de GTK
    gtk_main();

    // printf("entre\n"); // verificaca si se ejecuta lo que esta debajo del bucle principal.
    
    // Liberamos memoria
    // liberar args, ctx y contenido.
    // liberar trabajos
    // liberar hash
    // g_object_unref(builder); // libera builder

    return 0;
}