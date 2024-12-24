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
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->trabajos = trabajos;
    rendererArgs->builder = builder;
    rendererArgs->contadorTrabajosActual = &contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = &contadorTrabajosTotal;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_principal(rendererArgs);

    // Iniciar el bucle principal de GTK
    gtk_main();

    // Liberamos memoria
    liberar_trabajos(trabajos, &contadorTrabajosActual);
    free(trabajos);
    // liberar hash
    // liberar builder

    return 0;
}