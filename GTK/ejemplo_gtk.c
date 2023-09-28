#include <gtk/gtk.h>

// Esta función se llama cuando se hace clic en el botón
void on_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("¡Hola, mundo!\n");
}

int main(int argc, char *argv[])
{
    // Inicializar GTK
    gtk_init(&argc, &argv);

    // Crear una ventana
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Ejemplo de GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crear un botón
    GtkWidget *button = gtk_button_new_with_label("Haz clic");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Agregar el botón a la ventana
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Mostrar todos los elementos
    gtk_widget_show(button);
    gtk_widget_show(box);
    gtk_widget_show(window);

    // Iniciar el bucle principal de GTK
    gtk_main();

    return 0;
}