#include "interfaces.h"
#include "trabajo.h"
#include "aux.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <pango/pango.h>

void renderizar_principal(RendererArgs* args) {
    GtkBuilder* builder = args->builder;
    GHashTable *tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual; 
    int* contadorTrabajoTotal = args->contadorTrabajoTotal;
    // Agregamos la interfaz principal.glade al builder
    gtk_builder_add_from_file(builder, "../interfaces/Principal.glade", NULL);
    // Renderizar los trabajos actuales en el estado
    for (int i = 0; i < *contadorTrabajosActual; i++) {
        char* trabajoId = trabajos[i]->id;
        int idi = obtener_id_numerico(trabajoId);        
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->builder = builder;
        rendererArgs->numeralId = idi;
        rendererArgs->tituloTrabajo = trabajos[i]->titulo;
        rendererArgs->trabajos = trabajos;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->contadorTrabajoTotal = contadorTrabajoTotal;
        rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
        renderizar_trabajo_principal(rendererArgs);
    }
    // Conexion de botones
    // Buscar
    HandlerArgs* argsPbuscar = malloc(sizeof(HandlerArgs));
    argsPbuscar->builder = builder;
    argsPbuscar->contadorTrabajosActual = contadorTrabajosActual;
    argsPbuscar->contadorTrabajosTotal = contadorTrabajoTotal;
    argsPbuscar->trabajos = trabajos;
    argsPbuscar->tableWidgetsLabel = tableWidgetsLabel;
    GObject *pBuscar = gtk_builder_get_object(builder, "p_buscar");
    g_signal_connect(pBuscar, "clicked", G_CALLBACK(handle_buscar_principal), argsPbuscar);
    // Ver listado
    HandlerArgs* argsPlistado = malloc(sizeof(HandlerArgs));
    argsPlistado->builder = builder;
    argsPlistado->contadorTrabajosActual = contadorTrabajosActual;
    argsPlistado->contadorTrabajosTotal = contadorTrabajoTotal;
    argsPlistado->trabajos = trabajos;
    argsPlistado->tableWidgetsLabel = tableWidgetsLabel;
    GObject *pListado = gtk_builder_get_object(builder, "p_listado");
    g_signal_connect(pListado, "clicked", G_CALLBACK(handle_ver_principal), argsPlistado);
    // Agregar
    HandlerArgs* argsPagregar = malloc(sizeof(HandlerArgs));
    argsPagregar->builder = builder;
    argsPagregar->trabajos = trabajos;
    argsPagregar->contadorTrabajosActual = contadorTrabajosActual;
    argsPagregar->contadorTrabajosTotal = contadorTrabajoTotal;
    GObject *pAgregar = gtk_builder_get_object(builder, "p_agregar");
    g_signal_connect(pAgregar, "clicked", G_CALLBACK(handle_agregar_principal), argsPagregar);
    // Agregar Trabajo
    HandlerArgs* argsPagregarTrabajo = malloc(sizeof(HandlerArgs));
    argsPagregarTrabajo->builder = builder;
    argsPagregarTrabajo->trabajos = trabajos;
    argsPagregarTrabajo->contadorTrabajosActual = contadorTrabajosActual;
    argsPagregarTrabajo->contadorTrabajosTotal = contadorTrabajoTotal;
    argsPagregarTrabajo->tableWidgetsLabel = tableWidgetsLabel;
    GObject *pAgregarTrabajo = gtk_builder_get_object(builder, "p_agregar_trabajo");
    g_signal_connect(pAgregarTrabajo, "clicked", G_CALLBACK(handle_agregar_trabajo_principal), argsPagregarTrabajo);

    // Obtener la ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "principal");
    // ??? completar crear una funcion que llame a gtkmainquit dentro y libere los handlerargs
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));
    // liberar rendererargs
    free(args);
}

void renderizar_trabajo_principal(RendererArgs* args) {
    GtkBuilder* builder = args->builder;
    GHashTable *tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajoTotal;
    int numeralId = args->numeralId;
    char* tituloTrabajo = args->tituloTrabajo;

    char* widgetBase = malloc(sizeof(char)*25);
    strcpy(widgetBase, "principal");
    
    // Crear GtkGrid
    GtkWidget *grid = gtk_grid_new();
    // Hacer que las columnas sean homogéneas
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    // Establecer espaciado entre filas y columnas
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    // asignamos id al grid
    char gridId[25];
    snprintf(gridId, 25, "trabajo_%d", numeralId);
    gtk_widget_set_name(grid, gridId);

    // Crear un nuevo label con el titulo del trabajo
    char labelText[255];
    snprintf(labelText, sizeof(labelText), "<markup><span foreground='black' font='Garuda10' variant='small-caps' font_size='small'>%s</span></markup>", tituloTrabajo);
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), labelText);
    char labelId[25];
    snprintf(labelId, 24, "label_trabajo_%d", numeralId);
    gtk_widget_set_name(label, labelId);
    // Configurar que el GtkLabel tenga un ancho fijo (por ejemplo, 200 píxeles)
    gtk_widget_set_size_request(label, 20, -1);
    // Permitir el salto de línea
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    // Establecer el modo de ajuste de línea para que divida las palabras si es necesario
    gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
    // Asegurarse de que no se recorte el texto
    gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_NONE);
    // Centrar el texto
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    // Agregamos la widget del label a la tabla hash
    g_hash_table_insert(tableWidgetsLabel, g_strdup(labelId), label);

    // Crear el Botón "Editar"
    GtkWidget *buttonEditar = gtk_button_new_with_label("Editar");
    char buttonEditarId[25];
    snprintf(buttonEditarId, 24, "editar_trabajo_%d", numeralId);
    gtk_widget_set_name(buttonEditar, buttonEditarId);
    HandlerArgs* argsEditarTp = malloc(sizeof(HandlerArgs));
    argsEditarTp->builder = builder;
    argsEditarTp->widgetBase = widgetBase;
    argsEditarTp->contadorTrabajosActual = contadorTrabajosActual;
    argsEditarTp->contadorTrabajosTotal = contadorTrabajosTotal;
    argsEditarTp->trabajos = trabajos;
    argsEditarTp->tableWidgetsLabel = tableWidgetsLabel;
    g_signal_connect(buttonEditar, "clicked", G_CALLBACK(handle_editar_trabajo_principal), argsEditarTp);
    // Crear el Botón "Eliminar"
    GtkWidget *buttonEliminar = gtk_button_new_with_label("Eliminar");
    char buttonEliminarId[25];
    snprintf(buttonEliminarId, 24, "eliminar_trabajo_%d", numeralId);
    gtk_widget_set_name(buttonEliminar, buttonEliminarId);
    HandlerArgs* argsEliminarTp = malloc(sizeof(HandlerArgs));
    argsEliminarTp->builder = builder;
    argsEliminarTp->contadorTrabajosActual = contadorTrabajosActual;
    argsEliminarTp->trabajos = trabajos;
    argsEliminarTp->tableWidgetsLabel = tableWidgetsLabel;
    g_signal_connect(buttonEliminar, "clicked", G_CALLBACK(handle_eliminar_trabajo), argsEliminarTp);
    // Agregar un margen de 5 píxeles a la derecha del botón
    gtk_widget_set_margin_end(buttonEliminar, 25);

    // Agregar el Label a la primera columna del Grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);  // (grid, widget, columna, fila, ancho, alto)
    // Agregar el Botón "Editar" a la segunda columna del Grid
    gtk_grid_attach(GTK_GRID(grid), buttonEditar, 1, 0, 1, 1);
    // Agregar el Botón "Eliminar" a la tercer columna del Grid
    gtk_grid_attach(GTK_GRID(grid), buttonEliminar, 2, 0, 1, 1);

    // Agregar el GtkGrid al GtkBox
    GObject* box_trabajos = gtk_builder_get_object(builder, "box_trabajos");//box_trabajos es el id/name de una box en principal.glade
    gtk_box_pack_start(GTK_BOX(box_trabajos), grid, FALSE, FALSE, 5);
    
    // Mostrar los widgets recién creados
    gtk_widget_show_all(grid);

    // liberar rendererargs
    free(args);
}

void renderizar_detalle(RendererArgs* args) {
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajoTotal;
    Trabajo** trabajos = args->trabajos;
    char* widgetBase = args->widgetBase;

    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Detalle.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "detalle");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones
    // Volver
    HandlerArgs* argsVolverDetalle = malloc(sizeof(HandlerArgs));
    argsVolverDetalle->builder = builder;
    argsVolverDetalle->widgetBase = widgetBase;
    argsVolverDetalle->contadorTrabajosActual = contadorTrabajosActual;
    argsVolverDetalle->contadorTrabajosTotal = contadorTrabajosTotal;
    argsVolverDetalle->trabajos = trabajos;
    argsVolverDetalle->tableWidgetsLabel = tableWidgetsLabel;
    GObject *dVolver = gtk_builder_get_object(builder, "d_volver");
    g_signal_connect(dVolver, "clicked", G_CALLBACK(handle_volver_detalle), argsVolverDetalle);
    // Editar
    HandlerArgs* argsEditarDetalle = malloc(sizeof(HandlerArgs));
    argsEditarDetalle->builder = builder;
    argsEditarDetalle->contadorTrabajosActual = contadorTrabajosActual;
    argsEditarDetalle->contadorTrabajosTotal = contadorTrabajosTotal;
    argsEditarDetalle->trabajos = trabajos;
    argsEditarDetalle->widgetBase = widgetBase;
    argsEditarDetalle->tableWidgetsLabel = tableWidgetsLabel;
    GObject *dEditar = gtk_builder_get_object(builder, "d_editar");
    g_signal_connect(dEditar, "clicked", G_CALLBACK(handle_editar_detalle), argsEditarDetalle);

    // Manejo del combo box
    GObject *dTrabajos = gtk_builder_get_object(builder, "d_trabajos");
    // Crear un modelo ListStore con una columna de tipo texto
    GtkListStore *liststore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    // Agregar datos al ListStore (bd)
    // Agregar filas al ListStore con texto y IDs
    GtkTreeIter iter;
    for (int i = 1; i <= 3; i++) {
        gchar *text = g_strdup_printf("Trabajo %d", i);
        gchar *id = g_strdup_printf("t_%d", i);

        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter, 0, text, 1, id, -1);

        g_free(text);
        g_free(id);
    }
    // Conectar el ListStore al ComboBox
    gtk_combo_box_set_model(GTK_COMBO_BOX(dTrabajos), GTK_TREE_MODEL(liststore));
    // Agregar un renderer para mostrar texto
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(dTrabajos), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(dTrabajos), renderer, "text", 0, NULL);
    // Agregar el manejador de los botones del combobox
    // Conectar la señal "changed"
    HandlerArgs* argsComboBoxDetalle = malloc(sizeof(HandlerArgs));
    argsComboBoxDetalle->builder = builder;
    argsComboBoxDetalle->trabajos = trabajos;
    argsComboBoxDetalle->contadorTrabajosActual = contadorTrabajosActual;
    argsComboBoxDetalle->contadorTrabajosTotal = contadorTrabajosTotal;
    argsComboBoxDetalle->widgetBase = widgetBase;
    argsComboBoxDetalle->tableWidgetsLabel = tableWidgetsLabel;
    g_signal_connect(dTrabajos, "changed", G_CALLBACK(handle_combobox_detalle), argsComboBoxDetalle);

    // liberamos args
    free(args);
}

void renderizar_listado(RendererArgs* args) {
    GtkBuilder* builder = args->builder;
    Trabajo** trabajos = args->trabajos;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajoTotal;

    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Listado.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "listado");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones
    // Ver (obtenemos el auto que se encuentra en la fila del boton ver elegido y aplicamos handle_buscar)
    HandlerArgs* argsLver1 = malloc(sizeof(HandlerArgs));
    HandlerArgs* argsLver2 = malloc(sizeof(HandlerArgs));
    HandlerArgs* argsLver3 = malloc(sizeof(HandlerArgs));
    argsLver1->builder = builder;
    argsLver1->contadorTrabajosActual = contadorTrabajosActual;
    argsLver1->contadorTrabajosTotal = contadorTrabajosTotal;
    argsLver1->trabajos = trabajos;
    argsLver1->tableWidgetsLabel = tableWidgetsLabel;
    argsLver2->builder = builder;
    argsLver2->contadorTrabajosActual = contadorTrabajosActual;
    argsLver2->contadorTrabajosTotal = contadorTrabajosTotal;
    argsLver2->trabajos = trabajos;
    argsLver2->tableWidgetsLabel = tableWidgetsLabel;
    argsLver3->builder = builder;
    argsLver3->contadorTrabajosActual = contadorTrabajosActual;
    argsLver3->contadorTrabajosTotal = contadorTrabajosTotal;
    argsLver3->trabajos = trabajos;
    argsLver3->tableWidgetsLabel = tableWidgetsLabel;
    GObject *lVer1 = gtk_builder_get_object(builder, "l_ver_1");
    g_signal_connect(lVer1, "clicked", G_CALLBACK(handle_ver_listado), argsLver1);
    GObject *lVer2 = gtk_builder_get_object(builder, "l_ver_2");
    g_signal_connect(lVer2, "clicked", G_CALLBACK(handle_ver_listado), argsLver2);
    GObject *lVer3 = gtk_builder_get_object(builder, "l_ver_3");
    g_signal_connect(lVer3, "clicked", G_CALLBACK(handle_ver_listado), argsLver3);
    

    // Editar (obtenemos el auto que se encuentra en la fila del boton editar elegido y aplicamos handle_editar)
    HandlerArgs* argsLeditar1 = malloc(sizeof(HandlerArgs));
    HandlerArgs* argsLeditar2 = malloc(sizeof(HandlerArgs));
    HandlerArgs* argsLeditar3 = malloc(sizeof(HandlerArgs));
    argsLeditar1->builder = builder;
    argsLeditar1->trabajos = trabajos;
    argsLeditar1->contadorTrabajosActual = contadorTrabajosActual;
    argsLeditar1->contadorTrabajosTotal = contadorTrabajosTotal;
    argsLeditar1->tableWidgetsLabel = tableWidgetsLabel;
    argsLeditar2->builder = builder;
    argsLeditar2->trabajos = trabajos;
    argsLeditar2->contadorTrabajosActual = contadorTrabajosActual;
    argsLeditar2->contadorTrabajosTotal = contadorTrabajosTotal;
    argsLeditar2->tableWidgetsLabel = tableWidgetsLabel;
    argsLeditar3->builder = builder;
    argsLeditar3->trabajos = trabajos;
    argsLeditar3->contadorTrabajosActual = contadorTrabajosActual;
    argsLeditar3->contadorTrabajosTotal = contadorTrabajosTotal;
    argsLeditar1->tableWidgetsLabel = tableWidgetsLabel;
    GObject *lEditar1 = gtk_builder_get_object(builder, "l_editar_1");
    g_signal_connect(lEditar1, "clicked", G_CALLBACK(handle_editar_listado), argsLeditar1);
    GObject *lEditar2 = gtk_builder_get_object(builder, "l_editar_2");
    g_signal_connect(lEditar2, "clicked", G_CALLBACK(handle_editar_listado), argsLeditar2);
    GObject *lEditar3 = gtk_builder_get_object(builder, "l_editar_3");
    g_signal_connect(lEditar3, "clicked", G_CALLBACK(handle_editar_listado), argsLeditar3);

    // Eliminar
    HandlerArgs* argsLeliminar1 = malloc(sizeof(HandlerArgs));
    HandlerArgs* argsLeliminar2 = malloc(sizeof(HandlerArgs));
    HandlerArgs* argsLeliminar3 = malloc(sizeof(HandlerArgs));
    argsLeliminar1->builder = builder;
    argsLeliminar2->builder = builder;
    argsLeliminar3->builder = builder;
    GObject *lEliminar1 = gtk_builder_get_object(builder, "l_eliminar_1");
    g_signal_connect(lEliminar1, "clicked", G_CALLBACK(handle_eliminar_listado), argsLeliminar1);
    GObject *lEliminar2 = gtk_builder_get_object(builder, "l_eliminar_2");
    g_signal_connect(lEliminar2, "clicked", G_CALLBACK(handle_eliminar_listado), argsLeliminar2);
    GObject *lEliminar3 = gtk_builder_get_object(builder, "l_eliminar_3");
    g_signal_connect(lEliminar3, "clicked", G_CALLBACK(handle_eliminar_listado), argsLeliminar3);

    // Volver
    HandlerArgs* argsLvolver = malloc(sizeof(HandlerArgs));
    argsLvolver->builder = builder;
    argsLvolver->trabajos = trabajos;
    argsLvolver->contadorTrabajosActual = contadorTrabajosActual;
    argsLvolver->contadorTrabajosTotal = contadorTrabajosTotal;

    GObject *lVolver = gtk_builder_get_object(builder, "l_volver");
    g_signal_connect(lVolver, "clicked", G_CALLBACK(handle_volver_listado), NULL);

    // liberar rendererargs
    free(args);
}

void renderizar_detalle_editar(RendererArgs* args) {
    GtkBuilder* builder = args->builder;
    Trabajo** trabajos = args->trabajos;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajoTotal;
    char* widgetPrevia = args->widgetPrevia;
    char* widgetBase = args->widgetBase;

    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/DetalleEditar.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "detalle_editar");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones (pasar args)
    // Volver
    HandlerArgs* argVolverDe = malloc(sizeof(HandlerArgs));
    argVolverDe->builder = builder;
    argVolverDe->widgetBase = widgetBase;
    argVolverDe->widgetPrevia = widgetPrevia;
    argVolverDe->contadorTrabajosActual = contadorTrabajosActual;
    argVolverDe->contadorTrabajosTotal = contadorTrabajosTotal;
    argVolverDe->trabajos = trabajos;
    argVolverDe->tableWidgetsLabel = tableWidgetsLabel;
    GObject *deVolver = gtk_builder_get_object(builder, "de_volver");
    g_signal_connect(deVolver, "clicked", G_CALLBACK(handle_volver_de), argVolverDe);
    // Confirmar
    HandlerArgs* argConfirmarDe = malloc(sizeof(HandlerArgs));
    GObject *deConfirmar = gtk_builder_get_object(builder, "de_confirmar");
    g_signal_connect(deConfirmar, "clicked", G_CALLBACK(handle_confirmar_de), argConfirmarDe);

    // liberar Rendererargs
    free(args);

}

void renderizar_trabajo(RendererArgs* args) {
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajoTotal;
    char* widgetBase = args->widgetBase;
    // Cargar la interfaz "trabajo.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Trabajo.glade", NULL);
    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "trabajo");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window)); 

    // Conexion de botones (COMPLETAR)
    // Editar
    HandlerArgs* argsEditarTrabajo = malloc(sizeof(HandlerArgs));
    argsEditarTrabajo->builder = builder;
    argsEditarTrabajo->widgetBase = widgetBase;
    argsEditarTrabajo->contadorTrabajosActual = contadorTrabajosActual;
    argsEditarTrabajo->contadorTrabajosTotal = contadorTrabajosTotal;
    argsEditarTrabajo->trabajos = trabajos;
    argsEditarTrabajo->tableWidgetsLabel = tableWidgetsLabel;
    GObject *tEditar = gtk_builder_get_object(builder, "t_editar");
    g_signal_connect(tEditar, "clicked", G_CALLBACK(handle_editar_trabajo), argsEditarTrabajo);
    // Volver
    HandlerArgs* argsVolverTrabajo = malloc(sizeof(HandlerArgs));
    argsVolverTrabajo->builder = builder;
    argsVolverTrabajo->widgetBase = widgetBase;
    args->contadorTrabajosActual = contadorTrabajosActual;
    args->contadorTrabajoTotal = contadorTrabajosTotal;
    args->trabajos = trabajos;
    args->tableWidgetsLabel = tableWidgetsLabel;
    GObject *tVolver = gtk_builder_get_object(builder, "t_volver");
    g_signal_connect(tVolver, "clicked", G_CALLBACK(handle_volver_trabajo), argsVolverTrabajo);
}

void renderizar_trabajo_editar (RendererArgs* args) {
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajoTotal;
    char* widgetBase = args->widgetBase;
    char* accion = args->accion;
    char* trabajoId = args->trabajoId;

    // Cargar la interfaz "trabajo.glade" y renderizar el trabajoId en la ventana
    gtk_builder_add_from_file(builder, "../interfaces/TrabajoEditar.glade", NULL);
    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "trabajo_edit");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window)); 

    // Conexion de botones (COMPLETAR)
    // confirmar
    HandlerArgs* argsConfirmarTe = malloc(sizeof(HandlerArgs));
    argsConfirmarTe->builder = builder;
    argsConfirmarTe->widgetBase = widgetBase;
    argsConfirmarTe->accion = accion;
    argsConfirmarTe->trabajos = trabajos;
    argsConfirmarTe->contadorTrabajosActual = contadorTrabajosActual;
    argsConfirmarTe->contadorTrabajosTotal = contadorTrabajosTotal;
    argsConfirmarTe->tableWidgetsLabel = tableWidgetsLabel;
    GObject *teConfirmar = gtk_builder_get_object(builder, "te_confirmar");
    g_signal_connect(teConfirmar, "clicked", G_CALLBACK(handle_confirmar_te), argsConfirmarTe);
    // volver
    HandlerArgs* argsVolverTe = malloc(sizeof(HandlerArgs));
    argsVolverTe->builder = builder;
    argsVolverTe->widgetBase = widgetBase;
    argsVolverTe->accion = accion;
    argsVolverTe->contadorTrabajosActual = contadorTrabajosActual;
    argsVolverTe->contadorTrabajosTotal = contadorTrabajosTotal;
    argsVolverTe->trabajos = trabajos;
    argsVolverTe->tableWidgetsLabel = tableWidgetsLabel;
    GObject *teVolver = gtk_builder_get_object(builder, "te_volver");
    g_signal_connect(teVolver, "clicked", G_CALLBACK(handle_volver_te), argsVolverTe);

    // liberamos rendererargs
    free(args);
}

void handle_volver_listado(GtkButton* button, gpointer data){
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    int *contadorTrabajosActual = args->contadorTrabajosActual;
    int *contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo **trabajos = args->trabajos;

    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->trabajos = trabajos;
    rendererArgs->builder = builder;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    renderizar_principal(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handler args
    free(args);
}

void handle_volver_detalle(GtkButton* button, gpointer data){
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int *contadorTrabajosActual = args->contadorTrabajosActual;
    int *contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo **trabajos = args->trabajos;
    char* widgetBase = args->widgetBase;
    if (strcmp(widgetBase, "principal") == 0) {
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->trabajos = trabajos;
        rendererArgs->builder = builder;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
        renderizar_principal(rendererArgs);
    }
    else {
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->builder = builder;
        rendererArgs->trabajos = trabajos;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
        rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
        renderizar_listado(rendererArgs);
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handler args
    free(args);
    free(widgetBase);
}

void handle_volver_de(GtkButton* button, gpointer data){
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    char* widgetPrevia = args->widgetPrevia;
    char* widgetBase = args->widgetBase;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo** trabajos = args->trabajos;

    if (strcmp(widgetPrevia, "detalle") == 0) {
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->widgetBase = widgetBase;
        rendererArgs->builder = builder;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
        rendererArgs->trabajos = trabajos;
        rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
        renderizar_detalle(rendererArgs);
    } else {
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->builder = builder;
        rendererArgs->trabajos = trabajos;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
        rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
        renderizar_listado(rendererArgs);
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle_editar");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handler args
    free(args);
    free(widgetPrevia);
}

void handle_volver_trabajo(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    char* widgetBase = args->widgetBase;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo** trabajos = args->trabajos;
    // renderizamos detalle
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->builder = builder;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_detalle(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "trabajo");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handler args
    free(args);
}

void handle_volver_te(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int *contadorTrabajosActual = args->contadorTrabajosActual;
    int *contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo **trabajos = args->trabajos;
    gchar* widgetBase = args->widgetBase;
    gchar* accion = args->accion;
    // si viene de editar renderizamos la ventana anterior sino solo la cerramos.
    if (strcmp(accion, "editarDb") == 0) {
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->builder = builder;
        rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
        rendererArgs->trabajos = trabajos;
        rendererArgs->widgetBase = widgetBase;
        renderizar_trabajo(rendererArgs);
    }
    // Ocultar o destruir la interfaz actual
    GObject *ventanaActual = gtk_builder_get_object(builder, "trabajo_edit");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberamos handlerargs
    free(args);
    free(accion);
}

void handle_ver_listado(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo** trabajos = args->trabajos;
    // renderizar el detalle del auto correspondiente cuando este la db hecha.
    char* widgetBase = malloc(sizeof(char)*10);
    strcpy(widgetBase, "listado");
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->builder = builder;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_detalle(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handler args
    free(args);
}

void handle_ver_principal(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo** trabajos = args->trabajos;

    // renderizamos listado
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->builder = builder;
    rendererArgs->trabajos = trabajos;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_listado(rendererArgs);
    // Ocultar o destruir la interfaz anterior.
    GObject *ventanaActual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handler args
    free(args);
}

void handle_buscar_principal(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    // Obtener el GtkEntry
    GObject *entry = gtk_builder_get_object(builder, "p_patente_buscar");
    // Obtener el texto del GtkEntry
    const gchar *entryText = gtk_entry_get_text(GTK_ENTRY(entry));
    // Imprimir el texto (puedes hacer lo que necesites aquí)
    g_print("Texto del GtkEntry: %s\n", entryText);
    // renderizar el detalle del auto correspondiente cuando este la db hecha.
    char* widgetBase = malloc(sizeof(char)*25);
    strcpy(widgetBase, "principal");
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->builder = builder;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_detalle(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    // liberar handler args
    free(args);
}

void handle_agregar_principal(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;

    // Obtener los GtkEntry
    GObject *entryPatente = gtk_builder_get_object(builder, "p_patente_agregar");
    GObject *entryDueno = gtk_builder_get_object(builder, "p_dueno");
    GObject *entryModelo = gtk_builder_get_object(builder, "p_modelo");
    // Obtener el texto del GtkEntry
    const gchar *entryPatenteText = gtk_entry_get_text(GTK_ENTRY(entryPatente));
    const gchar *entryDuenoText = gtk_entry_get_text(GTK_ENTRY(entryDueno));
    const gchar *entryModeloText = gtk_entry_get_text(GTK_ENTRY(entryModelo));
    // Subirlos a la bd
    // ???
    // Vaciar la lista de trabajos
    if (*contadorTrabajosActual > 0) {
        // volver a renderizar principal.glade
        GObject* box_trabajos = gtk_builder_get_object(builder, "box_trabajos");//box_trabajos es el id/name de una box en principal.glade
        // Obtener los hijos de la GtkBox
        GList *children = gtk_container_get_children(GTK_CONTAINER(box_trabajos));
        // Comprobar si hay más de un hijo (el primero es el título)
        if (g_list_length(children) > 1) {
            // Iterar sobre los hijos, pero omitir el primero (índice 0)
            for (GList *l = children->next; l != NULL; l = l->next) {
                GtkWidget *child = GTK_WIDGET(l->data);
                // Destruir cada hijo que no sea el primero
                gtk_widget_destroy(child);
            }
        }
        // Liberar la lista de hijos
        g_list_free(children);
        liberar_trabajos(trabajos, contadorTrabajosActual);
        *contadorTrabajosActual = 0;
        *contadorTrabajosTotal = 0;
    }
    // liberar handlerargs
    free(args);
}

void handle_agregar_trabajo_principal(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo** trabajos = args->trabajos;
    char* widgetBase = malloc(sizeof(char)*15);
    strcpy(widgetBase, "principal");
    gchar *accion = malloc(sizeof(char)*15);
    strcpy(accion, "agregar");
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->builder = builder;
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->accion = accion;
    rendererArgs->trabajoId = NULL;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_trabajo_editar(rendererArgs);
}

void handle_editar_detalle(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int *contadorTrabajosActual = args->contadorTrabajosActual;
    int *contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo **trabajos = args->trabajos;
    char* widgetBase = args->widgetBase;
    // renderizamos detalleeditar.glade
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "detalle");
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->builder = builder;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->widgetPrevia = widgetPrevia;
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_detalle_editar(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    // liberar handlerargs
    free(args);
}

void handle_editar_listado(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int *contadorTrabajosActual = args->contadorTrabajosActual;
    int *contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo **trabajos = args->trabajos;
    // renderizamos detalleeditar.glade
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "detalle");
    char* widgetBase = malloc(sizeof(char)*10);
    strcpy(widgetBase, "listado");
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->builder = builder;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->widgetPrevia = widgetPrevia;
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_detalle_editar(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    // liberar handler args
    free(args);
}

void handle_editar_trabajo(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo** trabajos = args->trabajos;
    char* widgetBase = args->widgetBase;
    gchar *accion = malloc(sizeof(char)*15);
    strcpy(accion, "editarDb");
    // obtener trabajoId como argumento y pasarselo a renderizar_te.
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->builder = builder;
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->accion = accion;
    rendererArgs->trabajoId = NULL;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_trabajo_editar(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "trabajo");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    // liberar handler args
    free(args);
}

void handle_editar_trabajo_principal(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    Trabajo** trabajos = args->trabajos;
    char* widgetBase = args->widgetBase;
    gchar *accion = malloc(sizeof(char)*15);
    strcpy(accion, "editarEstado");
    char* editarTrabajoId = (char*)gtk_widget_get_name((GtkWidget*)button);
    int id = obtener_id_numerico(editarTrabajoId);
    gchar *trabajoId = malloc(sizeof(char)*25);
    snprintf(trabajoId, 24, "trabajo_%d", id);

    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->builder = builder;
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->accion = accion;
    rendererArgs->trabajoId = trabajoId;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_trabajo_editar(rendererArgs);
    // liberar handler args
    free(args);
    // ?? quien libera trabajoId
}

void handle_eliminar_trabajo(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable *tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    // Obtenemos el id del trabajo a eliminar
    char* buttonEliminarId = (char*)gtk_widget_get_name((GtkWidget*)button);
    int id = obtener_id_numerico(buttonEliminarId);
    gchar *trabajoId = malloc(sizeof(char)*25);
    snprintf(trabajoId, 24, "trabajo_%d", id);

    // Eliminamos el trabajo de los renderizados hasta el momento
    if (*contadorTrabajosActual > 0) {
        // volver a renderizar principal.glade
        GObject* box_trabajos = gtk_builder_get_object(builder, "box_trabajos");//box_trabajos es el id/name de una box en principal.glade
        // Obtener los hijos de la GtkBox
        GList *children = gtk_container_get_children(GTK_CONTAINER(box_trabajos));
        // Comprobar si hay más de un hijo (el primero es el título)
        if (g_list_length(children) > 1) {
            // Iterar sobre los hijos, para encontrar el que queremos eliminar
            for (GList *l = children->next; l != NULL; l = l->next) {
                GtkWidget *child = GTK_WIDGET(l->data);
                const gchar* idChild = gtk_widget_get_name(child);
                if (strcmp(idChild, trabajoId) == 0) {
                    gtk_widget_destroy(child);
                }
            }
        }
        // Liberar la lista de hijos
        g_list_free(children);
    }
    // Eliminamos el widget del label asociado de la tabla hash
    char labelId[25];
    snprintf(labelId, 24, "label_trabajo_%d", id);
    g_hash_table_remove(tableWidgetsLabel, labelId);
    // Eliminamos el trabajo del estado global de trabajos
    eliminar_trabajo(trabajos, trabajoId, contadorTrabajosActual);
    // liberar handler args
    free(args);
    free(trabajoId); // tal vez se requiera liberar en eliminar_trabajo
}

void handle_combobox_detalle(GtkComboBox *combobox, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable* tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    char* widgetBase = args->widgetBase;

    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *id_value;

    // Obtener el modelo del ComboBox
    model = gtk_combo_box_get_model(combobox);

    // Comprobar si hay una fila seleccionada
    if (gtk_combo_box_get_active_iter(combobox, &iter)) {
        // Obtener el valor de la columna 1 (donde almacenamos los IDs)
        gtk_tree_model_get(model, &iter, 1, &id_value, -1);

        // Mostrar el ID seleccionado en la consola
        g_print("ID seleccionado: %s\n", id_value);

        // Liberar memoria
        g_free(id_value);
    }
    // renderizamos trabajo.glade
    RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
    rendererArgs->builder = builder;
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
    rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
    rendererArgs->trabajos = trabajos;
    rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
    renderizar_trabajo(rendererArgs);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handler args
    free(args);
}

void handle_confirmar_te(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    GtkBuilder* builder = args->builder;
    GHashTable *tableWidgetsLabel = args->tableWidgetsLabel;
    Trabajo** trabajos = args->trabajos;
    int* contadorTrabajosTotal = args->contadorTrabajosTotal;
    int* contadorTrabajosActual = args->contadorTrabajosActual;
    gchar* widgetBase = args->widgetBase;
    gchar* accion = args->accion;

    // Obtener los inputs.
    GObject *entryTitulo = gtk_builder_get_object(builder, "te_titulo");
    GObject *entryKm = gtk_builder_get_object(builder, "te_km");
    GObject *calendar = gtk_builder_get_object(builder, "te_fecha");
    GObject *detalle = gtk_builder_get_object(builder, "te_detalle");
    // Extraer el texto.
    const gchar *titulo = gtk_entry_get_text(GTK_ENTRY(entryTitulo));
    const gchar *km = gtk_entry_get_text(GTK_ENTRY(entryKm));
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    Fecha* fecha = malloc(sizeof(Fecha));
    fecha->dia = day;
    fecha->mes = month;
    fecha->year = year;
    GtkTextBuffer *bufferDetalle = gtk_text_view_get_buffer(GTK_TEXT_VIEW(detalle));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(bufferDetalle, &start, &end);
    gchar *textDetalle = gtk_text_buffer_get_text(bufferDetalle, &start, &end, FALSE);
    // Guardamos el texto en variables duplicadas
    Trabajo* trabajo = malloc(sizeof(Trabajo));
    trabajo->titulo = g_strdup(titulo);
    trabajo->km = g_strdup(km);
    trabajo->descripcion = textDetalle;
    trabajo->fecha = fecha;

    // si la widget base es principal.glade agregamos el trabajo en la interfaz
    if (strcmp(accion, "agregar") == 0) {
        char *trabajoId = malloc(sizeof(char)*15);
        snprintf(trabajoId, 15, "trabajo_%d", *contadorTrabajosTotal+1);
        trabajo->id = trabajoId;
        // agregamos el trabajo al estado global
        agregar_trabajo(trabajos, trabajo, contadorTrabajosActual, contadorTrabajosTotal);
        // Mostrar el trabajo agregado en principal.glade
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->builder = builder;
        rendererArgs->numeralId = *contadorTrabajosTotal;
        rendererArgs->tituloTrabajo = trabajo->titulo;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->trabajos = trabajos;
        rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
        rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
        renderizar_trabajo_principal(rendererArgs);
    }
    else if (strcmp(accion, "editarDb") == 0){
        RendererArgs* rendererArgs = malloc(sizeof(RendererArgs));
        rendererArgs->builder = builder;
        rendererArgs->widgetBase = widgetBase;
        rendererArgs->contadorTrabajosActual = contadorTrabajosActual;
        rendererArgs->contadorTrabajoTotal = contadorTrabajosTotal;
        rendererArgs->trabajos = trabajos;
        rendererArgs->tableWidgetsLabel = tableWidgetsLabel;
        renderizar_trabajo(rendererArgs);
        // Editar trabajo en db
        free(trabajo->km);
        free(trabajo->titulo);
        free(trabajo->fecha);
        free(trabajo->descripcion);
        free(trabajo);
        printf("falta pasar id del trabajo y del duenio y luego modificar trabajo relacionado a duenio en db\n");
    }
    else { // editarEstado
        // Editar los datos al estado
        gchar* trabajoId = args->trabajoId;
        trabajo->id = trabajoId;
        editar_trabajo(trabajos, trabajo, trabajoId, contadorTrabajosActual);
        // editar el titulo en la widget label
        int id = obtener_id_numerico(trabajoId);
        char labelId[25];
        snprintf(labelId, 24, "label_trabajo_%d", id);
        GtkWidget *label = GTK_WIDGET(g_hash_table_lookup(tableWidgetsLabel, labelId));
        char labelText[255];
        snprintf(labelText, 254, "<markup><span foreground='black' font='Garuda10' variant='small-caps' font_size='small'>%s</span></markup>", trabajo->titulo);
        gtk_label_set_markup(GTK_LABEL(label), labelText);
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "trabajo_edit");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    // liberar handlerArgs
    free(args);
    free(accion);
}

void handle_confirmar_de(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    printf("accedi desde confirmar en detalleditar.glade\n");
    free(args);
}

void handle_eliminar_listado(GtkButton* button, gpointer data) {
    HandlerArgs* args = (HandlerArgs*)data;
    printf("accedi desde eliminar de listado.glade");
    free(args);
}
