#include <gtk/gtk.h>

// Estructura para almacenar referencias a las ventanas
typedef struct {
    GtkWidget *ventana_principal;
    GtkWidget *ventana_detalle;

} Aplicacion;

// Función para manejar el clic en "Ver Motos"
void handler_buscar(GtkWidget *widget, gpointer user_data) {
    // Obtener el puntero a la estructura de la aplicación
    Aplicacion *app = (Aplicacion *)user_data;

    // Ocultar la ventana principal
    gtk_widget_hide(app->ventana_principal);

    // Mostrar la ventana de motos
    gtk_widget_show_all(app->ventana_detalle);
}
int main(int argc, char *argv[]) {
    // Inicializar GTK
    gtk_init(&argc, &argv);

    // Crear la aplicación y cargar la interfaz principal
    Aplicacion app;
    app.ventana_principal = GTK_WIDGET(gtk_builder_new_from_file("../../Interfaces Glade/Principal.glade"));
    app.ventana_detalle = GTK_WIDGET(gtk_builder_new_from_file("../../Interfaces Glade/Detalle.glade"));

    // Conectar señales
    gtk_builder_connect_signals(GTK_BUILDER(app.ventana_principal), &app);
    gtk_builder_connect_signals(GTK_BUILDER(app.ventana_detalle), &app);

    // Mostrar la ventana principal
    gtk_widget_show_all(app.ventana_principal);

    // Ejecutar el bucle principal de GTK
    gtk_main();

    return 0;
}