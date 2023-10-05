#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    // Inicializa GTK
    gtk_init(&argc, &argv);

    // Crea un nuevo GtkBuilder y carga la interfaz desde el archivo XML
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "plantilla.glade", NULL);

    // Crea la ventana principal desde el archivo Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

    // Conecta la señal para cerrar la ventana con la función gtk_main_quit
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Muestra la ventana principal
    gtk_widget_show(window);

    // Entra en el bucle principal de GTK
    gtk_main();

    return 0;
}
