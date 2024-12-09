#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <pango/pango.h>

GtkBuilder *builder;
// Contador global para los trabajos
static int contadorTrabajos = 0;

// renderiza principal.glade con funcionalidades
void renderizar_principal();
// renderiza detalle.glade con funcionalidades
void renderizar_detalle(char* widgetPrevia);
// renderiza listado.glade con funcionalidades
void renderizar_listado();
// renderiza detalleEditar.glade
void renderizar_detalleEditar(char* widgetPrevia, char* widgetBase);
// renderiza trabajo.glade
void renderizar_trabajo (char* widgetBase);
// renderiza trabajoEditar.glade
void renderizar_trabajoEditar (char* widgetBase, char* accion);
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
// manejador de los botones del combobox
void handle_combobox_detalle(GtkComboBox *combobox, gpointer data);
// manejador del boton confriamr de trabajoEditar.glade
void handle_confirmar_te(GtkButton* button, gpointer data);


int main(int argc, char *argv[]) {
    // Inicializar GTK
    gtk_init(&argc, &argv);
    
    // Crear un objeto Builder para cargar la interfaz desde el archivo Glade
    builder = gtk_builder_new();

    // muestra la ventana Principal.glade al ejecutar.
    renderizar_principal();

    // Iniciar el bucle principal de GTK
    gtk_main();

    return 0;
}

void renderizar_principal() {
    gtk_builder_add_from_file(builder, "../interfaces/Principal.glade", NULL);
    // Conexion de botones
    // Buscar
    GObject *p_buscar = gtk_builder_get_object(builder, "p_buscar");
    g_signal_connect(p_buscar, "clicked", G_CALLBACK(handle_buscar_principal), NULL);
    // Ver listado
    GObject *p_listado = gtk_builder_get_object(builder, "p_listado");
    g_signal_connect(p_listado, "clicked", G_CALLBACK(handle_ver_principal), NULL);
    // Agregar
    GObject *p_agregar = gtk_builder_get_object(builder, "p_agregar");
    g_signal_connect(p_agregar, "clicked", G_CALLBACK(handle_agregar_principal), NULL);
    // Agregar Trabajo
    GObject *p_agregar_trabajo = gtk_builder_get_object(builder, "p_agregar_trabajo");
    g_signal_connect(p_agregar_trabajo, "clicked", G_CALLBACK(handle_agregar_trabajo_principal), NULL);
    
    // Obtener la ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "principal");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));
}

void renderizar_detalle(char* widgetPrevia) {
    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Detalle.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "detalle");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones
    // Volver
    GObject *d_volver = gtk_builder_get_object(builder, "d_volver");
    g_signal_connect(d_volver, "clicked", G_CALLBACK(handle_volver_detalle), widgetPrevia);
    // Editar
    GObject *d_editar = gtk_builder_get_object(builder, "d_editar");
    g_signal_connect(d_editar, "clicked", G_CALLBACK(handle_editar_detalle), widgetPrevia);

    // Manejo del combo box
    GObject *d_trabajos = gtk_builder_get_object(builder, "d_trabajos");
    // Crear un modelo ListStore con una columna de tipo texto
    GtkListStore *liststore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    // Agregar datos al ListStore (bd)
    // Agregar filas al ListStore con texto y IDs
    GtkTreeIter iter;
    for (int i = 1; i <= 3; i++) {
        gchar *text = g_strdup_printf("Opción %d", i);
        gchar *id = g_strdup_printf("t_%d", i);

        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter, 0, text, 1, id, -1);

        g_free(text);
        g_free(id);
    }
    // Conectar el ListStore al ComboBox
    gtk_combo_box_set_model(GTK_COMBO_BOX(d_trabajos), GTK_TREE_MODEL(liststore));
    // Agregar un renderer para mostrar texto
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(d_trabajos), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(d_trabajos), renderer, "text", 0, NULL);
    // Agregar el manejador de los botones del combobox
    // Conectar la señal "changed"
    g_signal_connect(d_trabajos, "changed", G_CALLBACK(handle_combobox_detalle), widgetPrevia);


}

void renderizar_listado() {
    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Listado.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "listado");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones
    // Ver (obtenemos el auto que se encuentra en la fila del boton ver elegido y aplicamos handle_buscar)
    
    GObject *lVer1 = gtk_builder_get_object(builder, "l_ver_1");
    g_signal_connect(lVer1, "clicked", G_CALLBACK(handle_ver_listado), NULL);
    GObject *lVer2 = gtk_builder_get_object(builder, "l_ver_2");
    g_signal_connect(lVer2, "clicked", G_CALLBACK(handle_ver_listado), NULL);
    GObject *lVer3 = gtk_builder_get_object(builder, "l_ver_3");
    g_signal_connect(lVer3, "clicked", G_CALLBACK(handle_ver_listado), NULL);
    

    // Editar (obtenemos el auto que se encuentra en la fila del boton editar elegido y aplicamos handle_editar)
    GObject *lEditar1 = gtk_builder_get_object(builder, "l_editar_1");
    g_signal_connect(lEditar1, "clicked", G_CALLBACK(handle_editar_listado), NULL);
    GObject *lEditar2 = gtk_builder_get_object(builder, "l_editar_2");
    g_signal_connect(lEditar2, "clicked", G_CALLBACK(handle_editar_listado), NULL);
    GObject *lEditar3 = gtk_builder_get_object(builder, "l_editar_3");
    g_signal_connect(lEditar3, "clicked", G_CALLBACK(handle_editar_listado), NULL);

    // Eliminar

    // Volver
    GObject *l_volver = gtk_builder_get_object(builder, "l_volver");
    g_signal_connect(l_volver, "clicked", G_CALLBACK(handle_volver_listado), NULL);
}

void renderizar_detalleEditar(char* widgetPrevia, char* widgetBase) {
    // Cargar la segunda interfaz desde el archivo "mi_interfaz2.glade"
    gtk_builder_add_from_file(builder, "../interfaces/DetalleEditar.glade", NULL);

    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "detalle_editar");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));

    // Conexion de botones
    // Volver
    GObject *de_volver = gtk_builder_get_object(builder, "de_volver");
    char** datos = malloc(sizeof(char*)*2);
    datos[0] = widgetPrevia;
    datos[1] = widgetBase; 
    g_signal_connect(de_volver, "clicked", G_CALLBACK(handle_volver_de), datos);

}

void renderizar_trabajo (char* widgetBase) {
    // Cargar la interfaz "trabajo.glade"
    gtk_builder_add_from_file(builder, "../interfaces/Trabajo.glade", NULL);
    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "trabajo");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window)); 

    // Conexion de botones
    // Editar
    GObject *t_editar = gtk_builder_get_object(builder, "t_editar");
    g_signal_connect(t_editar, "clicked", G_CALLBACK(handle_editar_trabajo), widgetBase);
    // Volver
    GObject *t_volver = gtk_builder_get_object(builder, "t_volver");
    g_signal_connect(t_volver, "clicked", G_CALLBACK(handle_volver_trabajo), widgetBase);
}

void renderizar_trabajoEditar (char* widgetBase, char* accion) {
    // Cargar la interfaz "trabajo.glade"
    gtk_builder_add_from_file(builder, "../interfaces/TrabajoEditar.glade", NULL);
    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "trabajo_edit");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window)); 

    // Conexion de botones
    gchar **data = malloc(sizeof(char*)*2);
    data[0] = widgetBase;
    data[1] = accion;
    // confirmar
    GObject *te_confirmar = gtk_builder_get_object(builder, "te_confirmar");
    g_signal_connect(te_confirmar, "clicked", G_CALLBACK(handle_confirmar_te), (gpointer)data);
    // volver
    GObject *te_volver = gtk_builder_get_object(builder, "te_volver");
    g_signal_connect(te_volver, "clicked", G_CALLBACK(handle_volver_te), (gpointer)data);
}

void handle_volver_listado(GtkButton* button, gpointer data){
    renderizar_principal();
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_volver_detalle(GtkButton* button, gpointer data){
    printf("en volver detalle: data = %s\n", (char*)data);
    if (strcmp((char*) data, "principal") == 0) {
        renderizar_principal();
    }
    else {
        renderizar_listado();
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
    free(data);
}

void handle_volver_de(GtkButton* button, gpointer data){
    char** widgets = (char**)data;
    // printf("%s - %s\n", widgets[0], widgets[1]);
    char* widgetPrevia = widgets[0];
    char* widgetBase = widgets[1];

    if (strcmp(widgetPrevia, "detalle") == 0) {
        printf("widget base: %s\n", widgetBase);
        renderizar_detalle(widgetBase);
    }
    else {
        renderizar_listado();
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "detalle_editar");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
    // liberar data
    free(widgetPrevia);
    free(widgets);
}

void handle_volver_trabajo(GtkButton* button, gpointer data) {
    renderizar_detalle((char*)data);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "trabajo");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_volver_te(GtkButton* button, gpointer data) {
    gchar** dataCast = (gchar**)data;
    gchar* widgetBase = dataCast[0];
    gchar* accion = dataCast[1];
    // si es viene de editar renderizamos la ventana anterior sino solo la cerramos.
    if (strcmp(accion, "editar") == 0) {
        renderizar_trabajo(widgetBase);
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "trabajo_edit");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
    free(accion);
    free(dataCast);
}

void handle_ver_listado(GtkButton* button, gpointer data) {
    // Obtener el GtkEntry
    GObject *entry = gtk_builder_get_object(builder, "p_patente_buscar");
    // renderizar el detalle del auto correspondiente cuando este la db hecha.
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "listado");
    renderizar_detalle(widgetPrevia);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));  
}

void handle_ver_principal(GtkButton* button, gpointer data) {
    renderizar_listado();
    // Ocultar o destruir la interfaz anterior.
    GObject *ventana_actual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_buscar_principal(GtkButton* button, gpointer data) {
    // Obtener el GtkEntry
    GObject *entry = gtk_builder_get_object(builder, "p_patente_buscar");
    // Obtener el texto del GtkEntry
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
    // Imprimir el texto (puedes hacer lo que necesites aquí)
    g_print("Texto del GtkEntry: %s\n", entry_text);
    // renderizar el detalle del auto correspondiente cuando este la db hecha.
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "principal");
    renderizar_detalle(widgetPrevia);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));  
}

void handle_agregar_principal(GtkButton* button, gpointer data) {
    // Obtener los GtkEntry
    GObject *entryPatente = gtk_builder_get_object(builder, "p_patente_agregar");
    GObject *entryDueno = gtk_builder_get_object(builder, "p_dueno");
    GObject *entryModelo = gtk_builder_get_object(builder, "p_modelo");
    // Obtener el texto del GtkEntry
    const gchar *entryPatenteText = gtk_entry_get_text(GTK_ENTRY(entryPatente));
    const gchar *entryDuenoText = gtk_entry_get_text(GTK_ENTRY(entryDueno));
    const gchar *entryModeloText = gtk_entry_get_text(GTK_ENTRY(entryModelo));

    // Subirlos a la bd
    g_print("Patente: %s\n", entryPatenteText);
    g_print("Dueno: %s\n", entryDuenoText);
    g_print("Modelo: %s\n", entryModeloText);
}

void handle_agregar_trabajo_principal(GtkButton* button, gpointer data) {
    char* widgetBase = malloc(sizeof(char)*15);
    strcpy(widgetBase, "principal");
    gchar *accion = malloc(sizeof(char)*15);
    strcpy(accion, "agregar");
    renderizar_trabajoEditar(widgetBase, accion);
}

void handle_editar_detalle(GtkButton* button, gpointer data) {
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "detalle");
    renderizar_detalleEditar(widgetPrevia, (char*)data);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventana_actual)); 
}

void handle_editar_listado(GtkButton* button, gpointer data) {
    char* widgetPrevia = malloc(sizeof(char)*10);
    strcpy(widgetPrevia, "detalle");
    char* widgetBase = malloc(sizeof(char)*10);
    strcpy(widgetBase, "listado");
    renderizar_detalleEditar(widgetPrevia, widgetBase);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventana_actual)); 
}

void handle_editar_trabajo(GtkButton* button, gpointer data) {
    gchar *accion = malloc(sizeof(char)*15);
    strcpy(accion, "editar");
    renderizar_trabajoEditar((char*)data, accion);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "trabajo");
    gtk_widget_hide(GTK_WIDGET(ventana_actual)); 
}

void handle_combobox_detalle(GtkComboBox *combobox, gpointer data) {
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
    renderizar_trabajo((char*)data);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_confirmar_te(GtkButton* button, gpointer data) {
    // Obtener argumentos
    gchar** dataCast = (gchar**)data;
    gchar* widgetBase = dataCast[0];
    gchar* accion = dataCast[1];
    printf("WidgetBase: %s\n", widgetBase);
    printf("Accion: %s\n", accion);

    // Obtener los inputs.
    GObject *entryKm = gtk_builder_get_object(builder, "te_km");
    GObject *calendar = gtk_builder_get_object(builder, "te_fecha");
    GObject *detalle = gtk_builder_get_object(builder, "te_detalle");

    // Extraer el texto.
    const gchar *entryKmText = gtk_entry_get_text(GTK_ENTRY(entryKm));
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    GtkTextBuffer *bufferDetalle = gtk_text_view_get_buffer(GTK_TEXT_VIEW(detalle));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(bufferDetalle, &start, &end);
    gchar *textDetalle = gtk_text_buffer_get_text(bufferDetalle, &start, &end, FALSE);

    // subir los datos a la bd
    g_print("KM: %s\n", entryKmText);
    g_print("Fecha: %u-%u-%u\n", day, month + 1, year); 
    g_print("Detalle: %s\n", textDetalle);

    // si la widget base es principal.glade agregamos el trabajo en la interfaz
    if (strcmp(accion, "agregar") == 0) {
        printf("coincidencia con agregar\n");
        // Crear GtkGrid
        GtkWidget *grid = gtk_grid_new();
        // Hacer que las columnas sean homogéneas
        gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
        // Establecer espaciado entre filas y columnas
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

        // Crear un nuevo label con el texto "Trabajo i"
        char label_text[255];
        contadorTrabajos++;
        snprintf(label_text, sizeof(label_text), "<markup><span foreground='black' font='Garuda10' variant='small-caps' font_size='large'>Trabajo %d</span></markup>", contadorTrabajos);
        GtkWidget *label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label), label_text);

        // Crear el Botón "Editar"
        GtkWidget *button_editar = gtk_button_new_with_label("Editar");
        // Crear el Botón "Eliminar"
        GtkWidget *button_eliminar = gtk_button_new_with_label("Eliminar");

        // Agregar el Label a la primera columna del Grid
        gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);  // (grid, widget, columna, fila, ancho, alto)
        // Agregar el Botón "Editar" a la segunda columna del Grid
        gtk_grid_attach(GTK_GRID(grid), button_editar, 1, 0, 1, 1);
        // Agregar el Botón "Eliminar" a la tercer columna del Grid
        gtk_grid_attach(GTK_GRID(grid), button_eliminar, 2, 0, 1, 1);


        // Agregar el GtkGrid al GtkBox
        GObject* box_trabajos = gtk_builder_get_object(builder, "box_trabajos");
        gtk_box_pack_start(GTK_BOX(box_trabajos), grid, FALSE, FALSE, 5);
        
        // Mostrar los widgets recién creados
        gtk_widget_show_all(grid);
    }
    else {
        renderizar_trabajo(widgetBase);
    }

    g_free(textDetalle);
    free(accion);
    free(dataCast);

    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "trabajo_edit");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));

}