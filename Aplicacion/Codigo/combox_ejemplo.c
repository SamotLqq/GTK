#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Crear ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crear el GtkComboBox
    GtkWidget *combobox = gtk_combo_box_new();
    gtk_container_add(GTK_CONTAINER(window), combobox);

    // Crear un modelo ListStore con una columna de tipo texto
    GtkListStore *liststore = gtk_list_store_new(1, G_TYPE_STRING);

    // Agregar datos al ListStore
    GtkTreeIter iter;
    gtk_list_store_append(liststore, &iter); // Nueva fila
    gtk_list_store_set(liststore, &iter, 0, "Opción 1", -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter, 0, "Opción 2", -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter, 0, "Opción 3", -1);

    // Conectar el ListStore al ComboBox
    gtk_combo_box_set_model(GTK_COMBO_BOX(combobox), GTK_TREE_MODEL(liststore));

    // Agregar un renderer para mostrar texto
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox), renderer, "text", 0, NULL);

    // Mostrar todo
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
