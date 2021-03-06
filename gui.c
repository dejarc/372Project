/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#include "gui.h"
#include "bit.h"

/* tabs represent widget hierarchy
 */
struct PRIVATE_GUI {
    void (*run)(void);
    void (*step_in)(void);
    void (*step_out)(void);
    void (*open_file)(FILE *);
    void (*asm_print)(char[], word);
    GtkWidget *window;
        GtkWidget *box_vertical;
            GtkWidget *menu_bar;
                GtkWidget *menu_item_file;
                    GtkWidget *menu_file;
                        GtkWidget *menu_item_file_open;
                        GtkWidget *menu_item_file_exit;
            GtkWidget *box_horizontal;
                GtkWidget *button_run;
                GtkWidget *button_step_in;
                GtkWidget *button_step_out;
                GtkWidget *button_go_to_line;
                GtkWidget *label_pc;
                GtkWidget *tree_view_pc;
                    GtkListStore *list_store_pc;
            GtkWidget *scrolled_window_registers;        
                GtkWidget *tree_view_registers;
                    GtkListStore *list_store_registers;
            GtkWidget *scrolled_window_memory;
                GtkWidget *tree_view_memory;
                    GtkListStore *list_store_memory;
};

static void dummy_func() {return;}

/* selects the memory address indicated by the PC
 */
static void go_to_pc(Gui gui) {
    GtkTreeIter iter;
    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gui->list_store_pc), &iter);
    word *pc;
    gtk_tree_model_get(GTK_TREE_MODEL(gui->list_store_pc), &iter, 0, &pc, -1);
    GtkTreePath *path_memory;
    gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(gui->list_store_memory), &iter, NULL, *pc);
    path_memory = gtk_tree_model_get_path(GTK_TREE_MODEL(gui->list_store_memory), &iter);
    gtk_tree_view_set_cursor(GTK_TREE_VIEW(gui->tree_view_memory), path_memory, NULL, FALSE);
}

/* redraws every element of the GUI
 */
static void redraw(Gui gui) {
    gtk_widget_queue_draw(gui->window);
}

/* an instance of (*GCallback)
 */
static void callback_run(GtkWidget *widget, Gui gui) {
    gui->run();
    go_to_pc(gui);
    redraw(gui);
}

/* an instance of (*GCallback)
 */
static void callback_step_in(GtkWidget *widget, Gui gui) {
    gui->step_in();
    go_to_pc(gui);
    redraw(gui);
}

static void callback_step_out(GtkWidget *widget, Gui gui) {
    gui->step_out();
    go_to_pc(gui);
    redraw(gui);
}

/* an instance of (*GtkTreeCellDataFunc)
 */
static void cell_data_func_binary(
        GtkTreeViewColumn *tree_column,
        GtkCellRenderer   *cell,
        GtkTreeModel      *tree_model,
        GtkTreeIter       *iter,
        gpointer          data) {
    word *ptr;
    gtk_tree_model_get(tree_model, iter, 0, &ptr, -1);
    char binary_string[WORD_LEN + 1];
    word mask = 1 << (WORD_LEN - 1);
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        binary_string[i] = !!(*ptr & mask) + '0';
        mask >>= 1;
    }
    g_object_set(G_OBJECT(cell), "text", binary_string, NULL);
}

/* an instance of (*GtkTreeCellDataFunc)
 */
static void cell_data_func_int(
        GtkTreeViewColumn *tree_column,
        GtkCellRenderer   *cell,
        GtkTreeModel      *tree_model,
        GtkTreeIter       *iter,
        gpointer          data) {
    word *ptr;
    gtk_tree_model_get(tree_model, iter, 0, &ptr, -1);
    char int_string[12];
    sprintf(int_string, "%d", *ptr);
    g_object_set(G_OBJECT(cell), "text", int_string, NULL);
}

/* an instance of (*GtkTreeCellDataFunc)
 */
static void cell_data_func_hex(
        GtkTreeViewColumn *tree_column,
        GtkCellRenderer   *cell,
        GtkTreeModel      *tree_model,
        GtkTreeIter       *iter,
        gpointer          data) {
    word *ptr;
    gtk_tree_model_get(tree_model, iter, 0, &ptr, -1);
    char hex_string[WORD_LEN / 4 + 2];
    sprintf(hex_string, "x%08X", *ptr);
    g_object_set(G_OBJECT(cell), "text", hex_string, NULL);
}

/* an instance of (*GtkTreeCellDataFunc)
 */
static void cell_data_func_asm(
        GtkTreeViewColumn *tree_column,
        GtkCellRenderer   *cell,
        GtkTreeModel      *tree_model,
        GtkTreeIter       *iter,
        gpointer          gui) {
    word *ptr;
    gtk_tree_model_get(tree_model, iter, 0, &ptr, -1);
    char buffer[35] = {0};
    ((Gui) gui)->asm_print(buffer, *ptr);
    g_object_set(G_OBJECT(cell), "text", buffer, NULL);
}

/* an instance of (*GCallback)
 */
static void callback_open_file(GtkWidget *widget, Gui gui) {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new(
            "Open File", GTK_WINDOW(gui->window), GTK_FILE_CHOOSER_ACTION_OPEN,
            "Open", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        FILE *file = fopen(filename, "r");
        gui->open_file(file);
        fclose(file);
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
    redraw(gui);
    go_to_pc(gui);
}

/* an instance of (*GCallback)
 */
static void callback_tree_view_edit(
        GtkTreeView       *tree_view,
        GtkTreePath       *path,
        GtkTreeViewColumn *column,
        Gui               gui) {
    GtkTreeModel *tree_model;
    tree_model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(tree_model, &iter, path);
    word *value;
    gtk_tree_model_get(tree_model, &iter, 0, &value, -1);
    GtkWidget *dialog, *spin_button, *content_area;
    spin_button = gtk_spin_button_new_with_range((((double)(word) -1) + 1) / -2, (word) -1, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button), (double) (int) *value);
    dialog = gtk_dialog_new_with_buttons(
            "Edit", GTK_WINDOW(gui->window), GTK_DIALOG_MODAL, 
            "OK", GTK_RESPONSE_ACCEPT,
            "Cancel", GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(dialog), FALSE);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
    gtk_container_add(GTK_CONTAINER(content_area), spin_button);
    gtk_widget_show_all(dialog);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        *value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));
    }
    gtk_widget_destroy(dialog);
    redraw(gui);
}

/* an instance of (*GCallback)
 */
static void callback_go_to_line(GtkWidget *widget, Gui gui) {
    GtkTreeIter iter;
    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gui->list_store_pc), &iter);
    word *pc;
    gtk_tree_model_get(GTK_TREE_MODEL(gui->list_store_pc), &iter, 0, &pc, -1);
    GtkWidget *dialog, *spin_button, *content_area;
    spin_button = gtk_spin_button_new_with_range(0, (word) -1, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button), (double) (int) *pc);
    dialog = gtk_dialog_new_with_buttons(
            "Go-to line...", GTK_WINDOW(gui->window), GTK_DIALOG_MODAL, 
            "OK", GTK_RESPONSE_ACCEPT,
            "Cancel", GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(dialog), FALSE);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
    gtk_container_add(GTK_CONTAINER(content_area), spin_button);
    gtk_widget_show_all(dialog);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        word line = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));
        GtkTreePath *path_memory;
        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(gui->list_store_memory), &iter, NULL, line);
        path_memory = gtk_tree_model_get_path(GTK_TREE_MODEL(gui->list_store_memory), &iter);
        gtk_tree_view_set_cursor(GTK_TREE_VIEW(gui->tree_view_memory), path_memory, NULL, FALSE);
    }
    gtk_widget_destroy(dialog);
    redraw(gui);
}

static void connect_signals(Gui gui) {
    g_signal_connect(G_OBJECT(gui->window),                 "delete_event", 
                     G_CALLBACK(gtk_main_quit),             NULL);
    g_signal_connect(G_OBJECT(gui->menu_item_file_exit),    "activate",
                     G_CALLBACK(gtk_main_quit),             NULL);
    g_signal_connect(G_OBJECT(gui->button_run),             "clicked",
                     G_CALLBACK(callback_run),              gui);
    g_signal_connect(G_OBJECT(gui->button_step_in),         "clicked",
                     G_CALLBACK(callback_step_in),          gui);
    g_signal_connect(G_OBJECT(gui->button_step_out),        "clicked",
                     G_CALLBACK(callback_step_out),         gui);
    g_signal_connect(G_OBJECT(gui->menu_item_file_open),    "activate",
                     G_CALLBACK(callback_open_file),        gui);
    g_signal_connect(G_OBJECT(gui->tree_view_memory),       "row-activated",
                     G_CALLBACK(callback_tree_view_edit),   gui);
    g_signal_connect(G_OBJECT(gui->tree_view_registers),    "row-activated",
                     G_CALLBACK(callback_tree_view_edit),   gui);
    g_signal_connect(G_OBJECT(gui->tree_view_pc),           "row-activated",
                     G_CALLBACK(callback_tree_view_edit),   gui);
    g_signal_connect(G_OBJECT(gui->button_go_to_line),      "clicked",
                     G_CALLBACK(callback_go_to_line),       gui);
}

static void set_up_layout(Gui gui) {
    gtk_window_set_title(GTK_WINDOW(gui->window), "LC-2200 Simulator");
    gtk_window_set_default_size(GTK_WINDOW(gui->window), 500, 750);
    gtk_window_set_icon_from_file(GTK_WINDOW(gui->window), "icon.ico", NULL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(gui->window), FALSE);

    GtkTreeSelection *selection_pc;
    selection_pc = gtk_tree_view_get_selection(GTK_TREE_VIEW(gui->tree_view_pc));
    gtk_tree_selection_set_mode(selection_pc, GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_view_memory), FALSE);    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_view_pc), FALSE);    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_view_registers), FALSE);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(gui->scrolled_window_registers), 216);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(gui->scrolled_window_memory), 500);  

    gtk_box_pack_end(GTK_BOX(gui->box_horizontal), gui->tree_view_pc, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(gui->box_horizontal), gui->label_pc, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_run, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_step_in, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_step_out, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_go_to_line, FALSE, TRUE, 0);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(gui->menu_item_file), gui->menu_file);
    gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_file), gui->menu_item_file_open);
    gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_file), gui->menu_item_file_exit);
    gtk_container_add(GTK_CONTAINER(gui->menu_bar), gui->menu_item_file);
    gtk_container_add(GTK_CONTAINER(gui->scrolled_window_memory), gui->tree_view_memory);
    gtk_container_add(GTK_CONTAINER(gui->scrolled_window_registers), gui->tree_view_registers);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->menu_bar, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->box_horizontal, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->scrolled_window_registers, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->scrolled_window_memory, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(gui->window), gui->box_vertical);

    gtk_widget_show_all(gui->window);
}

Gui gui_ctor() {
    gtk_init(0, NULL);
    Gui gui = malloc(sizeof(struct PRIVATE_GUI));

    gui->run       = &dummy_func;
    gui->open_file = &dummy_func;
    gui->step_in   = &dummy_func;
    gui->step_out  = &dummy_func;
    gui->asm_print = &dummy_func;
    
    gui->button_go_to_line          = gtk_button_new_with_label("Go-to line...");
    gui->window                     = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gui->list_store_pc              = gtk_list_store_new(1, G_TYPE_POINTER);
    gui->tree_view_pc               = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->list_store_pc));
    gui->label_pc                   = gtk_label_new("PC: ");
    gui->menu_bar                   = gtk_menu_bar_new();
    gui->menu_file                  = gtk_menu_new();
    gui->menu_item_file_open        = gtk_menu_item_new_with_label("Open");
    gui->menu_item_file_exit        = gtk_menu_item_new_with_label("Exit");
    gui->button_run                 = gtk_button_new_with_label("Run");
    gui->button_step_in             = gtk_button_new_with_label("Step in");
    gui->button_step_out            = gtk_button_new_with_label("Step out");
    gui->menu_item_file             = gtk_menu_item_new_with_label("File");
    gui->list_store_memory          = gtk_list_store_new(2, G_TYPE_POINTER, G_TYPE_STRING);
    gui->list_store_registers       = gtk_list_store_new(3, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_UINT);
    gui->box_vertical               = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gui->box_horizontal             = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gui->tree_view_memory           = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->list_store_memory));
    gui->tree_view_registers        = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->list_store_registers));
    gui->scrolled_window_memory     = gtk_scrolled_window_new(NULL, NULL);
    gui->scrolled_window_registers  = gtk_scrolled_window_new(NULL, NULL);

    connect_signals(gui);
    set_up_layout(gui);    
    return gui;
}

void gui_connect_memory(Gui gui, word data[], word count) {
	word i;
    for (i = 0; i < count; i++) {
        char address_string[WORD_LEN / 4 + 2];
        sprintf(address_string, "x%08X", i);
        gtk_list_store_insert_with_values(
                gui->list_store_memory, NULL, -1, 
                0, &data[i], 1, address_string, -1);
    }
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
            GTK_TREE_VIEW(gui->tree_view_memory), -1, "", renderer, "text", 1, NULL); 
    gtk_tree_view_insert_column_with_data_func(
            GTK_TREE_VIEW(gui->tree_view_memory),
            -1, "", renderer, &cell_data_func_binary, NULL, NULL);                         
    gtk_tree_view_insert_column_with_data_func(
            GTK_TREE_VIEW(gui->tree_view_memory),
            -1, "", renderer, &cell_data_func_asm, gui, NULL);
}

void gui_connect_registers(Gui gui, word data[], const char *names[], word count) {
	word i;
    for (i = 0; i < count; i++) {
        gtk_list_store_insert_with_values(
                gui->list_store_registers, NULL, -1, 0, &data[i], 1, names[i], 2, i, -1);
    }
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
            GTK_TREE_VIEW(gui->tree_view_registers), -1, "", renderer, "text", 2, NULL);                                        
    gtk_tree_view_insert_column_with_attributes(
            GTK_TREE_VIEW(gui->tree_view_registers), -1, "", renderer, "text", 1, NULL);   
    gtk_tree_view_insert_column_with_data_func(
            GTK_TREE_VIEW(gui->tree_view_registers),
            -1, "", renderer, &cell_data_func_binary, NULL, NULL);                       
    gtk_tree_view_insert_column_with_data_func(
            GTK_TREE_VIEW(gui->tree_view_registers), 
            -1, "", renderer, &cell_data_func_int, NULL, NULL);  
}

void gui_connect_run(Gui gui, void (*run)(void)) {
    gui->run = run;
}

void gui_connect_step_in(Gui gui, void (*step_in)(void)) {
    gui->step_in = step_in;
}

void gui_connect_step_out(Gui gui, void (*step_out)(void)) {
    gui->step_out = step_out;
}

void gui_connect_pc(Gui gui, word *pc) {
    gtk_list_store_insert_with_values(gui->list_store_pc, NULL, -1, 0, pc, -1);
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_data_func(
            GTK_TREE_VIEW(gui->tree_view_pc), -1, "", renderer, &cell_data_func_hex, NULL, NULL);     
}

void gui_connect_open_file(Gui gui, void (*open_file)(FILE *)) {
    gui->open_file = open_file;
}

void gui_connect_asm_print(Gui gui, void (*asm_print)(char[], word)) {
    gui->asm_print = asm_print;
}

/* shows and starts the GUI
 */
void gui_main() {
    gtk_main();
}
