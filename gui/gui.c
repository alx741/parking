#include <gtk/gtk.h>

static void toggle_lights(GtkWidget *widget, gpointer data)
{
    g_print("faros\n");
}

static void turn_left(GtkWidget *widget, gpointer data)
{
    g_print("Left\n");
}

static void turn_right(GtkWidget *widget, gpointer data)
{
    g_print("Right\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button_left;
    GtkWidget *button_right;
    GtkWidget *button_light;
    GtkWidget *button_box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Parking");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window), button_box);

    button_light = gtk_button_new_with_label("Faros");
    g_signal_connect(button_light, "clicked", G_CALLBACK(toggle_lights), NULL);
    g_signal_connect_swapped(button_light, "clicked", G_CALLBACK(gtk_widget_destroy),
                             window);
    gtk_container_add(GTK_CONTAINER(button_box), button_light);

    button_left = gtk_button_new_with_label("Izquierda");
    g_signal_connect(button_left, "clicked", G_CALLBACK(turn_left), NULL);
    g_signal_connect_swapped(button_left, "clicked", G_CALLBACK(gtk_widget_destroy),
                             window);
    gtk_container_add(GTK_CONTAINER(button_box), button_left);

    button_right = gtk_button_new_with_label("Derecha");
    g_signal_connect(button_right, "clicked", G_CALLBACK(turn_right), NULL);
    g_signal_connect_swapped(button_right, "clicked", G_CALLBACK(gtk_widget_destroy),
                             window);
    gtk_container_add(GTK_CONTAINER(button_box), button_right);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
