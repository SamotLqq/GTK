#include <gtk/gtk.h>
#include <stdio.h>

GtkBuilder *builder;
// renderiza principal.glade con funcionalidades
void renderizar_principal();
// renderiza detalle.glade con funcionalidades
void renderizar_detalle();
// renderiza listado.glade con funcionalidades
void renderizar_listado();
// manejador del boton volver a principal.glade desde listado.glade
void handle_volver_principal_listado(GtkButton* button, gpointer data);
// manejador del boton volver a principal.glade desde detalle.glade
void handle_volver_principal_detalle(GtkButton* button, gpointer data);
// manejador del boton volver a detalle.glade desde detalleEditar.glade
void handle_volver_d_de(GtkButton* button, gpointer data);
// manejador del boton ver de principal.glade
void handle_ver(GtkButton* button, gpointer data);
// manejador del boton buscar de principal.glade
void handle_buscar(GtkButton* button, gpointer data);
// manejador del boton editar de detalle.glade
void handle_editar(GtkButton* button, gpointer data);




int main(int argc, char *argv[]) {
    // Inicializar GTK
    gtk_init(&argc, &argv);
    // Crear un objeto Builder para cargar la interfaz desde el archivo Glade
    builder = gtk_builder_new();


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
    g_signal_connect(p_buscar, "clicked", G_CALLBACK(handle_buscar), NULL);
    // Ver listado
    GObject *p_listado = gtk_builder_get_object(builder, "p_listado");
    g_signal_connect(p_listado, "clicked", G_CALLBACK(handle_ver), NULL);
    // Obtener la ventana principal y conectarla al evento de destrucción
    GObject *window = gtk_builder_get_object(builder, "principal");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar la ventana principal
    gtk_widget_show_all(GTK_WIDGET(window));
}

void renderizar_detalle() {
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
    g_signal_connect(d_volver, "clicked", G_CALLBACK(handle_volver_principal_detalle), NULL);
    // Editar
    GObject *d_editar = gtk_builder_get_object(builder, "d_editar");
    g_signal_connect(d_editar, "clicked", G_CALLBACK(handle_editar), NULL);

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
    // Volver
    GObject *l_volver = gtk_builder_get_object(builder, "l_volver");
    g_signal_connect(l_volver, "clicked", G_CALLBACK(handle_volver_principal_listado), NULL);
}

void renderizar_detalleEditar() {
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
    g_signal_connect(de_volver, "clicked", G_CALLBACK(handle_volver_d_de), NULL);

}

void handle_volver_principal_listado(GtkButton* button, gpointer data){
    renderizar_principal();
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "listado");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_volver_principal_detalle(GtkButton* button, gpointer data){
    renderizar_principal();
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_volver_d_de(GtkButton* button, gpointer data){
    renderizar_detalle();
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "detalle_editar");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_ver(GtkButton* button, gpointer data) {
    renderizar_listado();
    // Ocultar o destruir la interfaz anterior.
    GObject *ventana_actual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));
}

void handle_buscar(GtkButton* button, gpointer data) {
    // Obtener el GtkEntry
    GObject *entry = gtk_builder_get_object(builder, "p_patente_buscar");
    // Obtener el texto del GtkEntry
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
    // Imprimir el texto (puedes hacer lo que necesites aquí)
    g_print("Texto del GtkEntry: %s\n", entry_text);


    renderizar_detalle();
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "principal");
    gtk_widget_hide(GTK_WIDGET(ventana_actual));  
}

void handle_editar(GtkButton* button, gpointer data) {
    renderizar_detalleEditar();
    // Ocultar o destruir la interfaz anterior
    GObject *ventana_actual = gtk_builder_get_object(builder, "detalle");
    gtk_widget_hide(GTK_WIDGET(ventana_actual)); 
}