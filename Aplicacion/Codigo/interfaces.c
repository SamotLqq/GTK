#include "interfaces.h"
#include "trabajo.h"
#include "aux.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <pango/pango.h>

Args* inicializar_args() {
    Args* args = malloc(sizeof(Args));
    if (args == NULL) {
        perror("Error al inicializar args\n");
        exit(EXIT_FAILURE);
    }
    args->ctx = NULL;
    args->accion = NULL;
    args->tituloTrabajo = NULL;
    args->trabajoId = NULL;
    args->widgetBase = NULL;
    args->widgetPrevia = NULL;
    return args;
}

ArgsArray* inicializar_args_arr() {
    ArgsArray* arr = malloc(sizeof(ArgsArray));
    if (arr == NULL) {
        perror("Error al inicializar arr");
        exit(EXIT_FAILURE);
    }
    arr->arr = malloc(sizeof(Args*)*5);
    if (arr->arr == NULL) {
        perror("Error al inicializar arrarr");
        exit(EXIT_FAILURE);
    }
    arr->cant = 0;
    return arr;
}

void agregar_args_arr(ArgsArray* arr, Args* args) {
    if (arr->cant < 5) {
        arr->arr[arr->cant] = args;
        arr->cant = arr->cant + 1;
    }
    else {
        perror("Error al agregar args\n");
        exit(EXIT_FAILURE);
    }
}

void agg_arr_inicio(ArgsArray* arr, Args* args) {
    if (arr->cant < 5) {
        for (int i = arr->cant - 1; i >= 0; i--) {
            arr->arr[i+1] = arr->arr[i];
        }
        arr->arr[0] = args;
    }
    else {
        perror("Error al agregar args\n");
        exit(EXIT_FAILURE);
    }
}

void destruccion_widget(ArgsArray* arr) {
    for (int i = 0; i < arr->cant; i++)
    {
        Args* args = arr->arr[i];
        if (args->accion) free(args->accion);
        if (args->tituloTrabajo) free(args->tituloTrabajo);
        if (args->trabajoId) free(args->trabajoId);
        if (args->widgetBase) free(args->widgetBase);
        if (args->widgetPrevia) free(args->widgetPrevia);
        if (args->ctx) free(args->ctx);
        if (args) free(args);
    }
    free(arr);
}

void renderizar_principal(ArgsArray* arr) {
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    Trabajo** trabajos = args->ctx->trabajos;
    int* contadorTrabajosActual = args->ctx->contadorTrabajosActual; 
    printf("direcciones %p - %p - %p - %p\n", args, builder, trabajos, contadorTrabajosActual);
    // Agregamos la interfaz principal.glade al builder
    gtk_builder_add_from_file(builder, "../interfaces/Principal.glade", NULL);
    // Renderizar los trabajos actuales en el estado
    for (int i = 0; i < *contadorTrabajosActual; i++) {
        Args* rtpArgs = inicializar_args();
        rtpArgs->trabajoId = g_strdup(trabajos[i]->id);
        printf("trabajo id: %s\n", rtpArgs->trabajoId);
        rtpArgs->tituloTrabajo =  g_strdup(trabajos[i]->titulo);
        printf("titulo trabajo: %s\n", rtpArgs->tituloTrabajo);
        rtpArgs->ctx = args->ctx;
        agg_arr_inicio(arr, rtpArgs);
        renderizar_trabajo_principal(arr);
    }
    // Conexion de botones
    Args* handlerArgs = inicializar_args();
    handlerArgs->ctx = args->ctx;
    agg_arr_inicio(arr, handlerArgs);
    // Buscar
    GObject *pBuscar = gtk_builder_get_object(builder, "p_buscar");
    g_signal_connect(pBuscar, "clicked", G_CALLBACK(handle_buscar_principal), arr);
    // Ver listado
    GObject *pListado = gtk_builder_get_object(builder, "p_listado");
    g_signal_connect(pListado, "clicked", G_CALLBACK(handle_ver_principal), arr);
    // Agregar
    GObject *pAgregar = gtk_builder_get_object(builder, "p_agregar");
    g_signal_connect(pAgregar, "clicked", G_CALLBACK(handle_agregar_principal), arr);
    // Agregar Trabajo
    GObject *pAgregarTrabajo = gtk_builder_get_object(builder, "p_agregar_trabajo");
    g_signal_connect(pAgregarTrabajo, "clicked", G_CALLBACK(handle_agregar_trabajo_principal), arr);
    // Obtener la ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "principal");
    // ??? completar crear una funcion que llame a gtkmainquit dentro y libere los handlerargs
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));
}

void renderizar_trabajo_principal(ArgsArray* arr) {
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    GHashTable *tableWidgetsLabel = args->ctx->tableWidgetsLabel;
    int numeralId = obtener_id_numerico(args->trabajoId);
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

    Args* handlerArgs = inicializar_args();
    handlerArgs->ctx = args->ctx;
    handlerArgs->widgetBase = widgetBase;
    agg_arr_inicio(arr, handlerArgs);
    // Crear el Botón "Editar"
    GtkWidget *buttonEditar = gtk_button_new_with_label("Editar");
    char buttonEditarId[25];
    snprintf(buttonEditarId, 24, "editar_trabajo_%d", numeralId);
    gtk_widget_set_name(buttonEditar, buttonEditarId);
    g_signal_connect(buttonEditar, "clicked", G_CALLBACK(handle_editar_trabajo_principal), arr);
    // Crear el Botón "Eliminar"
    GtkWidget *buttonEliminar = gtk_button_new_with_label("Eliminar");
    gtk_widget_set_margin_end(buttonEliminar, 25); // Agregar un margen de 5 píxeles a la derecha del botón
    char buttonEliminarId[25];
    snprintf(buttonEliminarId, 24, "eliminar_trabajo_%d", numeralId);
    gtk_widget_set_name(buttonEliminar, buttonEliminarId);
    g_signal_connect(buttonEliminar, "clicked", G_CALLBACK(handle_eliminar_trabajo), arr);


    // Agregar el Label a la primera columna del Grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);  // (grid, widget, columna, fila, ancho, alto)
    // Agregar el Botón "Editar" a la segunda columna del Grid
    gtk_grid_attach(GTK_GRID(grid), buttonEditar, 1, 0, 1, 1);
    // Agregar el Botón "Eliminar" a la tercer columna del Grid
    gtk_grid_attach(GTK_GRID(grid), buttonEliminar, 2, 0, 1, 1);

    // Agregar el GtkGrid al GtkBox
    GObject* boxTrabajos = gtk_builder_get_object(builder, "box_trabajos");//box_trabajos es el id/name de una box en principal.glade
    gtk_box_pack_start(GTK_BOX(boxTrabajos), grid, FALSE, FALSE, 5);
    
    // Mostrar los widgets recién creados
    gtk_widget_show_all(grid);
}

void renderizar_detalle(ArgsArray* arr) {
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;

    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Detalle.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "detalle");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones
    Args* handlerArgs = inicializar_args();
    handlerArgs->ctx = args->ctx;
    handlerArgs->widgetBase = g_strdup(widgetBase);
    agg_arr_inicio(arr, handlerArgs);
    // Volver
    GObject *dVolver = gtk_builder_get_object(builder, "d_volver");
    g_signal_connect(dVolver, "clicked", G_CALLBACK(handle_volver_detalle), arr);
    // Editar
    GObject *dEditar = gtk_builder_get_object(builder, "d_editar");
    g_signal_connect(dEditar, "clicked", G_CALLBACK(handle_editar_detalle), arr);
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
    g_signal_connect(dTrabajos, "changed", G_CALLBACK(handle_combobox_detalle), arr);
}

void renderizar_listado(ArgsArray* arr) {
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    printf("direcciones: %p - %p en renderizar listado\n", args, builder);


    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Listado.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "listado");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones
    Args* handlerArgs = inicializar_args();
    handlerArgs->ctx = args->ctx;
    agg_arr_inicio(arr, handlerArgs);
    // Ver (obtenemos el auto que se encuentra en la fila del boton ver elegido y aplicamos handle_buscar)
    GObject *lVer1 = gtk_builder_get_object(builder, "l_ver_1");
    g_signal_connect(lVer1, "clicked", G_CALLBACK(handle_ver_listado), arr);
    GObject *lVer2 = gtk_builder_get_object(builder, "l_ver_2");
    g_signal_connect(lVer2, "clicked", G_CALLBACK(handle_ver_listado), arr);
    GObject *lVer3 = gtk_builder_get_object(builder, "l_ver_3");
    g_signal_connect(lVer3, "clicked", G_CALLBACK(handle_ver_listado), arr);
    

    // Editar (obtenemos el auto que se encuentra en la fila del boton editar elegido y aplicamos handle_editar)
    GObject *lEditar1 = gtk_builder_get_object(builder, "l_editar_1");
    g_signal_connect(lEditar1, "clicked", G_CALLBACK(handle_editar_listado), arr);
    GObject *lEditar2 = gtk_builder_get_object(builder, "l_editar_2");
    g_signal_connect(lEditar2, "clicked", G_CALLBACK(handle_editar_listado), arr);
    GObject *lEditar3 = gtk_builder_get_object(builder, "l_editar_3");
    g_signal_connect(lEditar3, "clicked", G_CALLBACK(handle_editar_listado), arr);

    // Eliminar
    GObject *lEliminar1 = gtk_builder_get_object(builder, "l_eliminar_1");
    g_signal_connect(lEliminar1, "clicked", G_CALLBACK(handle_eliminar_listado), arr);
    GObject *lEliminar2 = gtk_builder_get_object(builder, "l_eliminar_2");
    g_signal_connect(lEliminar2, "clicked", G_CALLBACK(handle_eliminar_listado), arr);
    GObject *lEliminar3 = gtk_builder_get_object(builder, "l_eliminar_3");
    g_signal_connect(lEliminar3, "clicked", G_CALLBACK(handle_eliminar_listado), arr);

    // Volver
    GObject *lVolver = gtk_builder_get_object(builder, "l_volver");
    g_signal_connect(lVolver, "clicked", G_CALLBACK(handle_volver_listado), arr);
    printf("sali renderizar listado\n");
}

void renderizar_detalle_editar(ArgsArray* arr) {
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
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
    Args* handlerArgs = inicializar_args();
    handlerArgs->ctx = args->ctx;
    handlerArgs->widgetBase = g_strdup(widgetBase);
    handlerArgs->widgetPrevia = g_strdup(widgetPrevia);
    agg_arr_inicio(arr, handlerArgs);
    // Volver
    GObject *deVolver = gtk_builder_get_object(builder, "de_volver");
    g_signal_connect(deVolver, "clicked", G_CALLBACK(handle_volver_de), arr);
    // Confirmar
    GObject *deConfirmar = gtk_builder_get_object(builder, "de_confirmar");
    g_signal_connect(deConfirmar, "clicked", G_CALLBACK(handle_confirmar_de), arr);
}

void renderizar_trabajo(ArgsArray* arr) {
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;
    // Cargar la interfaz "trabajo.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Trabajo.glade", NULL);
    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "trabajo");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window)); 

    // Conexion de botones (COMPLETAR)
    Args* handlerArgs = inicializar_args();
    handlerArgs->ctx = args->ctx;
    handlerArgs->widgetBase = g_strdup(widgetBase);
    agg_arr_inicio(arr, handlerArgs);
    // Editar
    GObject *tEditar = gtk_builder_get_object(builder, "t_editar");
    g_signal_connect(tEditar, "clicked", G_CALLBACK(handle_editar_trabajo), arr);
    // Volver
    GObject *tVolver = gtk_builder_get_object(builder, "t_volver");
    g_signal_connect(tVolver, "clicked", G_CALLBACK(handle_volver_trabajo), arr);
}

void renderizar_trabajo_editar (ArgsArray* arr) {
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
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
    Args* handlerArgs = inicializar_args();
    handlerArgs->ctx = args->ctx;
    handlerArgs->widgetBase = g_strdup(widgetBase);
    handlerArgs->accion = g_strdup(accion);
    agg_arr_inicio(arr, handlerArgs);
    // confirmar
    GObject *teConfirmar = gtk_builder_get_object(builder, "te_confirmar");
    g_signal_connect(teConfirmar, "clicked", G_CALLBACK(handle_confirmar_te), arr);
    // volver
    GObject *teVolver = gtk_builder_get_object(builder, "te_volver");
    g_signal_connect(teVolver, "clicked", G_CALLBACK(handle_volver_te), arr);
}

void handle_volver_listado(GtkButton* button, gpointer data){
    printf("entre handle volver listado\n");
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    printf("direccion builder en hvl: %p\n", builder);

    ArgsArray* rendererArr = inicializar_args_arr();
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_principal(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
    printf("sali handle volver listado\n");
    
}

void handle_volver_detalle(GtkButton* button, gpointer data){
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;

    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    if (strcmp(widgetBase, "principal") == 0) {
        renderizar_principal(rendererArr);
    }
    else {
        renderizar_listado(rendererArr);
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
}

void handle_volver_de(GtkButton* button, gpointer data){
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetPrevia = args->widgetPrevia;
    char* widgetBase = args->widgetBase;

    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    if (strcmp(widgetPrevia, "detalle") == 0) {
        rendererArgs->widgetBase = g_strdup(widgetBase);
        renderizar_detalle(rendererArr);
    } else {
        renderizar_listado(rendererArr);
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle_editar");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
}

void handle_volver_trabajo(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;
    // renderizamos detalle
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetBase = g_strdup(widgetBase);
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_detalle(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "trabajo");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
}

void handle_volver_te(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;
    char* accion = args->accion;
    // si viene de editar renderizamos la ventana anterior sino solo la cerramos.
    if (strcmp(accion, "editarDb") == 0) {
        Args* rendererArgs = inicializar_args();
        rendererArgs->ctx = args->ctx;
        rendererArgs->widgetBase = g_strdup(widgetBase);
        ArgsArray* rendererArr = inicializar_args_arr();
        agregar_args_arr(rendererArr, rendererArgs);
        renderizar_trabajo(rendererArr);
    }
    // Ocultar o destruir la interfaz actual
    GObject *ventanaActual = gtk_builder_get_object(builder, "trabajo_edit");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
}

void handle_ver_listado(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    // renderizar el detalle del auto correspondiente cuando este la db hecha.
    char* widgetBase = malloc(sizeof(char)*10);
    strcpy(widgetBase, "listado");
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetBase = widgetBase;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_detalle(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
}

void handle_ver_principal(GtkButton* button, gpointer data) {
    printf("entre handle ver principal\n");
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    printf("direccion args en handle_ver_principal: %p\n", args);

    // renderizamos listado
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_listado(rendererArr);
    // Ocultar o destruir la interfaz anterior.
    GObject *ventanaActual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
    printf("sali handle  ver prinicipal\n");
}

void handle_buscar_principal(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    // Obtener el GtkEntry
    GObject *entry = gtk_builder_get_object(builder, "p_patente_buscar");
    // Obtener el texto del GtkEntry
    const gchar *entryText = gtk_entry_get_text(GTK_ENTRY(entry));
    // Realizar busqueda de la patente en la DB ???
    g_print("Texto del GtkEntry: %s\n", entryText);
    // renderizar el detalle del auto correspondiente cuando este la db hecha.
    char* widgetBase = malloc(sizeof(char)*25);
    strcpy(widgetBase, "principal");
    Args* rendererArgs = inicializar_args();
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->ctx = args->ctx;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_detalle(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    destruccion_widget(arr);
}

void handle_agregar_principal(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    Trabajo** trabajos = args->ctx->trabajos;
    int* contadorTrabajosActual = args->ctx->contadorTrabajosActual;
    int* contadorTrabajosTotal = args->ctx->contadorTrabajosTotal;

    // Obtener los GtkEntry
    GObject *entryPatente = gtk_builder_get_object(builder, "p_patente_agregar");
    GObject *entryDueno = gtk_builder_get_object(builder, "p_dueno");
    GObject *entryModelo = gtk_builder_get_object(builder, "p_modelo");
    // Obtener el texto del GtkEntry
    const gchar *entryPatenteText = gtk_entry_get_text(GTK_ENTRY(entryPatente));
    const gchar *entryDuenoText = gtk_entry_get_text(GTK_ENTRY(entryDueno));
    const gchar *entryModeloText = gtk_entry_get_text(GTK_ENTRY(entryModelo));
    // Subirlos a la bd
    printf("Datos obtenidos: %s - %s - %s\n", entryDuenoText, entryModeloText, entryPatenteText);
    // ???
    // Vaciar la lista de trabajos
    if (*contadorTrabajosActual > 0) {
        // volver a renderizar principal.glade
        GObject* boxTrabajos = gtk_builder_get_object(builder, "box_trabajos");//box_trabajos es el id/name de una box en principal.glade
        // Obtener los hijos de la GtkBox
        GList *children = gtk_container_get_children(GTK_CONTAINER(boxTrabajos));
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
}

void handle_agregar_trabajo_principal(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    char* widgetBase = malloc(sizeof(char)*15);
    strcpy(widgetBase, "principal");
    char *accion = malloc(sizeof(char)*15);
    strcpy(accion, "agregar");
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetBase = widgetBase;
    rendererArgs->accion = accion;
    rendererArgs->trabajoId = NULL;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_trabajo_editar(rendererArr);
}

void handle_editar_detalle(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;
    // renderizamos detalleeditar.glade
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "detalle");

    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetPrevia = widgetPrevia;
    rendererArgs->widgetBase = g_strdup(widgetBase);
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_detalle_editar(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    destruccion_widget(arr);
}

void handle_editar_listado(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    // renderizamos detalleeditar.glade
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "detalle");
    char* widgetBase = malloc(sizeof(char)*10);
    strcpy(widgetBase, "listado");
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetPrevia = widgetPrevia;
    rendererArgs->widgetBase = widgetBase;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_detalle_editar(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    destruccion_widget(arr);
}

void handle_editar_trabajo(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;

    char *accion = malloc(sizeof(char)*15);
    strcpy(accion, "editarDb");
    // obtener trabajoId como argumento y pasarselo a renderizar_te.
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetBase = g_strdup(widgetBase);
    rendererArgs->accion = accion;
    rendererArgs->trabajoId = NULL;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_trabajo_editar(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "trabajo");
    gtk_widget_hide(GTK_WIDGET(ventanaActual)); 
    destruccion_widget(arr);

}

void handle_editar_trabajo_principal(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    char* widgetBase = args->widgetBase;

    char *accion = malloc(sizeof(char)*15);
    strcpy(accion, "editarEstado");
    char* editarTrabajoId = (char*)gtk_widget_get_name((GtkWidget*)button);
    int id = obtener_id_numerico(editarTrabajoId);
    char *trabajoId = malloc(sizeof(char)*25);
    snprintf(trabajoId, 24, "trabajo_%d", id);

    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetBase = g_strdup(widgetBase);
    rendererArgs->accion = accion;
    rendererArgs->trabajoId = trabajoId;
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_trabajo_editar(rendererArr);
}

void handle_eliminar_trabajo(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    GHashTable *tableWidgetsLabel = args->ctx->tableWidgetsLabel;
    Trabajo** trabajos = args->ctx->trabajos;
    int* contadorTrabajosActual = args->ctx->contadorTrabajosActual;
    // Obtenemos el id del trabajo a eliminar
    char* buttonEliminarId = (char*)gtk_widget_get_name((GtkWidget*)button);
    int id = obtener_id_numerico(buttonEliminarId);
    char *trabajoId = malloc(sizeof(char)*25);
    snprintf(trabajoId, 24, "trabajo_%d", id);

    // Eliminamos el trabajo de los renderizados hasta el momento
    if (*contadorTrabajosActual > 0) {
        // volver a renderizar principal.glade
        GObject* boxTrabajos = gtk_builder_get_object(builder, "box_trabajos");//box_trabajos es el id/name de una box en principal.glade
        // Obtener los hijos de la GtkBox
        GList *children = gtk_container_get_children(GTK_CONTAINER(boxTrabajos));
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
    free(trabajoId);
}

void handle_combobox_detalle(GtkComboBox *combobox, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
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
    Args* rendererArgs = inicializar_args();
    rendererArgs->ctx = args->ctx;
    rendererArgs->widgetBase = g_strdup(widgetBase);
    ArgsArray* rendererArr = inicializar_args_arr();
    agregar_args_arr(rendererArr, rendererArgs);
    renderizar_trabajo(rendererArr);
    // Ocultar o destruir la interfaz anterior
    GObject *ventanaActual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventanaActual));
    destruccion_widget(arr);
}

void handle_confirmar_te(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    GtkBuilder* builder = args->ctx->builder;
    GHashTable *tableWidgetsLabel = args->ctx->tableWidgetsLabel;
    Trabajo** trabajos = args->ctx->trabajos;
    int* contadorTrabajosTotal = args->ctx->contadorTrabajosTotal;
    int* contadorTrabajosActual = args->ctx->contadorTrabajosActual;
    char* widgetBase = args->widgetBase;
    char* accion = args->accion;

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

    if (strcmp(accion, "agregar") == 0) {
        char *trabajoId = malloc(sizeof(char)*15);
        snprintf(trabajoId, 15, "trabajo_%d", *contadorTrabajosTotal+1);
        trabajo->id = trabajoId;
        // agregamos el trabajo al estado global
        agregar_trabajo(trabajos, trabajo, contadorTrabajosActual, contadorTrabajosTotal);
        // Mostrar el trabajo agregado en principal.glade
        Args* rendererArgs = inicializar_args();
        rendererArgs->ctx = args->ctx;
        rendererArgs->trabajoId = g_strdup(trabajoId);
        rendererArgs->tituloTrabajo = g_strdup(trabajo->titulo);
        ArgsArray* rendererArr = inicializar_args_arr();
        agregar_args_arr(rendererArr, rendererArgs);
        renderizar_trabajo_principal(rendererArr);
    }
    else if (strcmp(accion, "editarDb") == 0){
        Args* rendererArgs = inicializar_args();
        rendererArgs->ctx = args->ctx;
        rendererArgs->widgetBase = g_strdup(widgetBase);
        ArgsArray* rendererArr = inicializar_args_arr();
        agregar_args_arr(rendererArr, rendererArgs);
        renderizar_trabajo(rendererArr);
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
        char* trabajoId = args->trabajoId;
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
    destruccion_widget(arr);
}

void handle_confirmar_de(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    printf("accedi desde confirmar en detalleditar.glade\n");
}

void handle_eliminar_listado(GtkButton* button, gpointer data) {
    ArgsArray* arr = (ArgsArray*)data;
    Args* args = arr->arr[0];
    printf("accedi desde eliminar de listado.glade\n");
}
