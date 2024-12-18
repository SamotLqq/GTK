#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <pango/pango.h>

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
    Fecha* fecha;
    gchar* descripcion;
} Trabajo;
// Contador global para los trabajos agregados contando eliminados
static int contadorTrabajosTotal = 0;
// Contador global para los trabajos actuales
static int contadorTrabajosActual = 0;
// Lista de trabajos por agregar
static Trabajo** trabajos = NULL;
// Funciones para trabajos
void inicializar_trabajos();
void agregar_trabajo(Trabajo* trabajo);
void eliminar_trabajo(char* idTrabajo);
void editar_trabajo(char* idTrabajo, Trabajo* nuevosDatos);
void liberar_trabajos();
void imprimir_trabajos();

// Funciones especificas

// obtenemos el ultimo caracter de una cadena con el formato "nombre_[idnum]", -1 error.
int obtener_id_numerico(char* idCompleto) {
    int idNumerico;

    if (sscanf(idCompleto, "editar_trabajo_%d", &idNumerico) == 1) {
        printf("El número extraído es: %d\n", idNumerico);
        return idNumerico;
    } else if (sscanf(idCompleto, "eliminar_trabajo_%d", &idNumerico) == 1) {
        printf("El número extraído es: %d\n", idNumerico);
        return idNumerico;
    } else if (sscanf(idCompleto, "trabajo_%d", &idNumerico) == 1) {
        printf("El número extraído es: %d\n", idNumerico);
        return idNumerico;
    } else {
        printf("Formato inválido.\n");
        return -1;
    }
}


// almacena las interfaces en tiempo de ejecucion
GtkBuilder *builder;
// renderiza principal.glade con funcionalidades
void renderizar_principal();
// renderiza un trabajo en principal.glade
void renderizar_trabajo_principal(int numeralId);
// renderiza detalle.glade con funcionalidades
void renderizar_detalle(char* widgetBase);
// renderiza listado.glade con funcionalidades
void renderizar_listado();
// renderiza detalleEditar.glade
void renderizar_detalleEditar(char* widgetPrevia, char* widgetBase);
// renderiza trabajo.glade
void renderizar_trabajo (char* widgetBase);
// renderiza trabajoEditar.glade
void renderizar_trabajoEditar (char* widgetBase, char* accion, char* trabajoId);
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
// manejador de los botones del combobox
void handle_combobox_detalle(GtkComboBox *combobox, gpointer data);
// manejador del boton confriamr de trabajoEditar.glade
void handle_confirmar_te(GtkButton* button, gpointer data);

int main(int argc, char *argv[]) {
    // inicializar trabajos
    inicializar_trabajos();

    // Inicializar GTK
    gtk_init(&argc, &argv);
    
    // Crear un objeto Builder para cargar la interfaz desde el archivo Glade
    builder = gtk_builder_new();

    // muestra la ventana Principal.glade al ejecutar.
    renderizar_principal();

    // Iniciar el bucle principal de GTK
    gtk_main();

    // Liberamos trabajos
    free(trabajos);

    return 0;
}

void renderizar_principal() {
    gtk_builder_add_from_file(builder, "../interfaces/Principal.glade", NULL);
    // Renderizar los trabajos actuales en el estado
    for (int i = 0; i < contadorTrabajosActual; i++) {
        char* trabajoId = trabajos[i]->id;
        int idi = obtener_id_numerico(trabajoId);        
        renderizar_trabajo_principal(idi);
    }
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

void renderizar_trabajo_principal(int numeralId) {
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

    // Crear un nuevo label con el texto "Trabajo i"
    char label_text[255];
    snprintf(label_text, sizeof(label_text), "<markup><span foreground='black' font='Garuda10' variant='small-caps' font_size='large'>Trabajo %d</span></markup>", numeralId);
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), label_text);
    char labelId[25];
    snprintf(labelId, 24, "label_trabajo_%d", numeralId);
    gtk_widget_set_name(label, labelId);

    // Crear el Botón "Editar"
    GtkWidget *button_editar = gtk_button_new_with_label("Editar");
    char buttonEditarId[25];
    snprintf(buttonEditarId, 24, "editar_trabajo_%d", numeralId);
    gtk_widget_set_name(button_editar, buttonEditarId);
    g_signal_connect(button_editar, "clicked", G_CALLBACK(handle_editar_trabajo_principal), NULL);
    // Crear el Botón "Eliminar"
    GtkWidget *button_eliminar = gtk_button_new_with_label("Eliminar");
    char buttonEliminarId[25];
    snprintf(buttonEliminarId, 24, "eliminar_trabajo_%d", numeralId);
    gtk_widget_set_name(button_eliminar, buttonEliminarId);
    g_signal_connect(button_eliminar, "clicked", G_CALLBACK(handle_eliminar_trabajo), NULL);


    // Agregar el Label a la primera columna del Grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);  // (grid, widget, columna, fila, ancho, alto)
    // Agregar el Botón "Editar" a la segunda columna del Grid
    gtk_grid_attach(GTK_GRID(grid), button_editar, 1, 0, 1, 1);
    // Agregar el Botón "Eliminar" a la tercer columna del Grid
    gtk_grid_attach(GTK_GRID(grid), button_eliminar, 2, 0, 1, 1);


    // Agregar el GtkGrid al GtkBox
    GObject* box_trabajos = gtk_builder_get_object(builder, "box_trabajos");//box_trabajos es el id/name de una box en principal.glade
    gtk_box_pack_start(GTK_BOX(box_trabajos), grid, FALSE, FALSE, 5);
    
    // Mostrar los widgets recién creados
    gtk_widget_show_all(grid);
}

void renderizar_detalle(char* widgetBase) {
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
    g_signal_connect(d_volver, "clicked", G_CALLBACK(handle_volver_detalle), widgetBase);
    // Editar
    GObject *d_editar = gtk_builder_get_object(builder, "d_editar");
    g_signal_connect(d_editar, "clicked", G_CALLBACK(handle_editar_detalle), widgetBase);

    // Manejo del combo box
    GObject *d_trabajos = gtk_builder_get_object(builder, "d_trabajos");
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
    gtk_combo_box_set_model(GTK_COMBO_BOX(d_trabajos), GTK_TREE_MODEL(liststore));
    // Agregar un renderer para mostrar texto
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(d_trabajos), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(d_trabajos), renderer, "text", 0, NULL);
    // Agregar el manejador de los botones del combobox
    // Conectar la señal "changed"
    g_signal_connect(d_trabajos, "changed", G_CALLBACK(handle_combobox_detalle), widgetBase);


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

void renderizar_trabajoEditar (char* widgetBase, char* accion, char* trabajoId) {
    // Cargar la interfaz "trabajo.glade"
    gtk_builder_add_from_file(builder, "../interfaces/TrabajoEditar.glade", NULL);
    // Obtener la nueva ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "trabajo_edit");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la nueva ventana principal
    gtk_widget_show_all(GTK_WIDGET(window)); 

    // Conexion de botones
    gchar **data = malloc(sizeof(char*)*3);
    data[0] = widgetBase;
    data[1] = accion;
    data[2] = trabajoId;
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
    gchar* trabajoId = dataCast[2];
    // si viene de editar renderizamos la ventana anterior sino solo la cerramos.
    if (strcmp(accion, "editarDb") == 0) {
        renderizar_trabajo(widgetBase);
    }
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "trabajo_edit");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
    free(accion);
    free(dataCast);
    if (trabajoId) free(trabajoId);
}

void handle_ver_listado(GtkButton* button, gpointer data) {
    // Obtener el GtkEntry
    GObject *entry = gtk_builder_get_object(builder, "p_patente_buscar");
    // renderizar el detalle del auto correspondiente cuando este la db hecha.
    char* widgetBase = malloc(sizeof(char)*10);
    strcpy(widgetBase, "listado");
    renderizar_detalle(widgetBase);
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
    char* widgetBase = malloc(sizeof(char)*10);
    strcpy(widgetBase, "principal");
    renderizar_detalle(widgetBase);
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
    imprimir_trabajos();
    // Vaciar la lista de trabajos
    if (contadorTrabajosActual > 0) {
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
        liberar_trabajos();
        contadorTrabajosActual = 0;
        contadorTrabajosTotal = 0;
    }
}

void handle_agregar_trabajo_principal(GtkButton* button, gpointer data) {
    char* widgetBase = malloc(sizeof(char)*15);
    strcpy(widgetBase, "principal");
    gchar *accion = malloc(sizeof(char)*15);
    strcpy(accion, "agregar");
    renderizar_trabajoEditar(widgetBase, accion, NULL);
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
    strcpy(accion, "editarDb");
    // obtener trabajoId como argumento y pasarselo a renderizar_te.
    renderizar_trabajoEditar((char*)data, accion, NULL);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "trabajo");
    gtk_widget_hide(GTK_WIDGET(ventana_actual)); 
}

void handle_editar_trabajo_principal(GtkButton* button, gpointer data) {
    gchar *accion = malloc(sizeof(char)*15);
    strcpy(accion, "editarEstado");
    char* editarTrabajoId = (char*)gtk_widget_get_name((GtkWidget*)button);
    printf("id de boton editar clickeado: %s\n", editarTrabajoId);
    int id = obtener_id_numerico(editarTrabajoId);
    printf("id numerico: %d\n", id);
    gchar *trabajoId = malloc(sizeof(char)*25);
    snprintf(trabajoId, 24, "trabajo_%d", id);
    renderizar_trabajoEditar((char*)data, accion, trabajoId);
}

void handle_eliminar_trabajo(GtkButton* button, gpointer data) {
    // Obtenemos el id del trabajo a eliminar
    char* buttonEliminarId = (char*)gtk_widget_get_name((GtkWidget*)button);
    int id = obtener_id_numerico(buttonEliminarId);
    gchar *trabajoId = malloc(sizeof(char)*25);
    snprintf(trabajoId, 24, "trabajo_%d", id);
    printf("id de boton eliminar clickeado: %s\n", trabajoId);

    // Eliminamos el trabajo de los renderizados hasta el momento
    if (contadorTrabajosActual > 0) {
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
                    printf("id del child: %s\n", idChild);
                    gtk_widget_destroy(child);
                }
            }
        }
        // Liberar la lista de hijos
        g_list_free(children);
    }
    // Eliminamos el trabajo del estado global de trabajos
    eliminar_trabajo(trabajoId);
    // Liberamos la memoria ocupada por el id usado como argumento
    free(trabajoId);
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

    // Obtener los inputs.
    GObject *entryKm = gtk_builder_get_object(builder, "te_km");
    GObject *calendar = gtk_builder_get_object(builder, "te_fecha");
    GObject *detalle = gtk_builder_get_object(builder, "te_detalle");

    // Extraer el texto.
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
    trabajo->km = g_strdup(km);
    trabajo->descripcion = textDetalle;
    trabajo->fecha = fecha;

    // si la widget base es principal.glade agregamos el trabajo en la interfaz
    if (strcmp(accion, "agregar") == 0) {
        char *trabajoId = malloc(sizeof(char)*15);
        snprintf(trabajoId, 15, "trabajo_%d", contadorTrabajosTotal+1);
        trabajo->id = trabajoId;
        // agregamos el trabajo al estado global
        agregar_trabajo(trabajo);
        // Mostrar el trabajo agregado en principal.glade
        renderizar_trabajo_principal(contadorTrabajosTotal);
    }
    else if (strcmp(accion, "editarDb") == 0){
        renderizar_trabajo(widgetBase);
        // Editar trabajo en db
        free(trabajo->km);
        free(trabajo->fecha);
        free(trabajo->descripcion);
        free(trabajo);
        printf("falta pasar id del trabajo y del duenio y luego modificar trabajo relacionado a duenio en db\n");
    }
    else { // editarEstado
        // Editar los datos al estado
        gchar* trabajoId = dataCast[2];
        printf("editando trabajo con id: %s en estado\n", trabajoId);
        trabajo->id = trabajoId;
        editar_trabajo(trabajoId, trabajo);
    }
    free(accion);
    free(dataCast);
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "trabajo_edit");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

// Funciones para el estado global trabajos.

void inicializar_trabajos() {
    trabajos = malloc(sizeof(Trabajo*)*50);
    if (trabajos == NULL) {
        perror("Error al inicializar la lista de trabajos");
        exit(EXIT_FAILURE);
    }
}

void agregar_trabajo(Trabajo* trabajo) {
    trabajos[contadorTrabajosActual] = trabajo;
    contadorTrabajosTotal++;
    contadorTrabajosActual++;
    imprimir_trabajos();
}

void eliminar_trabajo(char* idTrabajo) {
    int encontrado = 0;
    for (int i = 0; i < contadorTrabajosActual; i++) {
        if (encontrado) {
            trabajos[i-1] = trabajos[i];
        }
        else if (strcmp(trabajos[i]->id, idTrabajo) == 0) {
            encontrado = 1;
            free(trabajos[i]->descripcion);
            free(trabajos[i]->fecha);
            free(trabajos[i]->km);
            free(trabajos[i]->id);
            free(trabajos[i]);
            trabajos[i] = NULL;
        }  
    }
    if (encontrado) {
        contadorTrabajosActual--;
    }
}

void editar_trabajo(char* idTrabajo, Trabajo* nuevosDatos) {
    for (int i = 0; i < contadorTrabajosActual; i++) {
        if (strcmp(trabajos[i]->id, idTrabajo) == 0) {
            free(trabajos[i]->descripcion);
            free(trabajos[i]);
            trabajos[i] = nuevosDatos;
        }  
    }
}

void liberar_trabajos() {
    for (int i = 0; i < contadorTrabajosActual; i++) {
        free(trabajos[i]->descripcion);
        free(trabajos[i]->fecha);
        free(trabajos[i]->km);
        free(trabajos[i]->id);
        free(trabajos[i]);
    }
}

void imprimir_trabajos() {
    printf("Trabajos:\n");
    for (int i = 0; i < contadorTrabajosActual; i++) {
        printf("id: %s - ", trabajos[i]->id);
        printf("km: %s - ", trabajos[i]->km);
        printf("fecha: %d/%d/%d - ", trabajos[i]->fecha->dia, trabajos[i]->fecha->mes, trabajos[i]->fecha->year);
        printf("descripcion: %s\n", trabajos[i]->descripcion);
    }
}