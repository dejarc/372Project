#include <gtk/gtk.h>
#include "gui.h"
#include "bit.h"
#include <stdlib.h>
#include <stdio.h>

static const word DUMMY_WORD_0 = 0;

static const char *REG_NAMES[] = {"$zero", "$at", "$v0", "$a0", 
                                  "$a1", "$a2", "$t0", "$t1", 
                                  "$t2", "$s0", "$s1", "$s2",
                                  "$k0", "$sp", "$fp", "$ra"};
                                  
struct PRIVATE_GUI {
    void (*run)();
    void (*step)();
    GtkWidget *window;
        GtkWidget *box_vertical;
            GtkWidget *menu_bar;
                GtkWidget *menu_item_file;
                    GtkWidget *menu_file;
                        GtkWidget *menu_item_file_open;
                        GtkWidget *menu_item_file_exit;
                GtkWidget *menu_item_execute;
                    GtkWidget *menu_execute;
                        GtkWidget *menu_item_execute_run;
                        GtkWidget *menu_item_execute_step;
                GtkWidget *menu_item_help;
            GtkWidget *box_horizontal;
                GtkWidget *button_1;
                GtkWidget *button_2;
                GtkWidget *label_pc;
                GtkWidget *entry_pc;
                    GtkEntryBuffer *entry_buffer_pc;
            GtkWidget *scrolled_window_registers;        
                GtkWidget *tree_view_registers;
                    GtkListStore *list_store_registers;
            GtkWidget *scrolled_window_memory;
                GtkWidget *tree_view_memory;
                    GtkListStore *list_store_memory;
};

void callback_generic(GtkWidget *widget, void (**data)()) {
    (*data)();
}

void cell_data_func_binary(GtkTreeViewColumn    *tree_column,
                           GtkCellRenderer      *cell,
                           GtkTreeModel         *tree_model,
                           GtkTreeIter          *iter,
                           gpointer             column_num) {
    word *ptr;
    gtk_tree_model_get(tree_model, iter, column_num, &ptr, -1);
    char binary_string[WORD_LEN + 1];
    word mask = 1 << (WORD_LEN - 1);
    for (int i = 0; i < WORD_LEN; i++) {
        binary_string[i] = !!(*ptr & mask) + '0';
        mask >>= 1;
    }
    g_object_set(G_OBJECT(cell), "text", binary_string, NULL);
}

void cell_data_func_reg_int(GtkTreeViewColumn       *tree_column,
                            GtkCellRenderer         *cell,
                            GtkTreeModel            *tree_model,
                            GtkTreeIter             *iter,
                            gpointer                data) {
    word *ptr;
    gtk_tree_model_get(tree_model, iter, 2, &ptr, -1);
    char int_string[12];
    sprintf(int_string, "%d", *ptr);
    g_object_set(G_OBJECT(cell), "text", int_string, NULL);
}

void cell_data_func_mem_asm(GtkTreeViewColumn   *tree_column,
                            GtkCellRenderer     *cell,
                            GtkTreeModel        *tree_model,
                            GtkTreeIter         *iter,
                            gpointer            data) {
    gpointer ptr;
    gtk_tree_model_get(tree_model, iter, 1, &ptr, -1);
    g_object_set(G_OBJECT(cell), "text", "ADD R1, R2, R3", NULL);
}

void connect_signals(Gui gui) {
    g_signal_connect(G_OBJECT(gui->window), "delete_event", 
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(gui->menu_item_file_exit), "activate",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(gui->menu_item_execute_run), "activate",
                     G_CALLBACK(callback_generic), &gui->run);
    g_signal_connect(G_OBJECT(gui->menu_item_execute_step), "activate",
                     G_CALLBACK(callback_generic), &gui->step);
}

void set_up_layout(Gui gui) {
    gtk_window_set_title(GTK_WINDOW(gui->window), "LC-2200 Simulator");
    gtk_window_set_default_size(GTK_WINDOW(gui->window), 500, 750);
    //gtk_window_set_icon(GTK_WINDOW(window), );
    gtk_window_set_has_resize_grip(GTK_WINDOW(gui->window), FALSE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_view_memory), FALSE);    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_view_registers), FALSE);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(gui->scrolled_window_registers), 216);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(gui->scrolled_window_memory), 500);
    
    gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_2, FALSE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(gui->box_horizontal), gui->entry_pc, FALSE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(gui->box_horizontal), gui->label_pc, FALSE, TRUE, 0);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(gui->menu_item_file), gui->menu_file);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(gui->menu_item_execute), gui->menu_execute);
    gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_file), gui->menu_item_file_open);
    gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_file), gui->menu_item_file_exit);
    gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_execute), gui->menu_item_execute_run);
    gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_execute), gui->menu_item_execute_step);
    gtk_container_add(GTK_CONTAINER(gui->menu_bar), gui->menu_item_file);
    gtk_container_add(GTK_CONTAINER(gui->menu_bar), gui->menu_item_execute);
    gtk_container_add(GTK_CONTAINER(gui->menu_bar), gui->menu_item_help);
    gtk_container_add(GTK_CONTAINER(gui->scrolled_window_memory), gui->tree_view_memory);
    gtk_container_add(GTK_CONTAINER(gui->scrolled_window_registers), gui->tree_view_registers);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->menu_bar, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui-> box_horizontal, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->scrolled_window_registers, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->scrolled_window_memory, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(gui->window), gui->box_vertical);
    gtk_widget_show_all(gui->window);
}

void set_up_memory(Gui gui, word memory_addresses) {
    for (word i = 0; i < memory_addresses; i++) {
        char address_string[WORD_LEN / 4 + 2];
        sprintf(address_string, "x%08X", i);
        gtk_list_store_insert_with_values(gui->list_store_memory, NULL, -1, 
                                          0, address_string,
                                          1, &DUMMY_WORD_0, 
                                          -1);
    }
    
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_text_new();
    
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(gui->tree_view_memory), -1,
                                                "", renderer, "text", 0, NULL);                
                                                
    gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(gui->tree_view_memory),
                                                -1, "", renderer, 
                                                &cell_data_func_binary,
                                                (gpointer) 1, NULL);
                                                
    gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(gui->tree_view_memory),
                                                -1, "", renderer, 
                                                &cell_data_func_mem_asm,
                                                NULL, NULL);
}

void set_up_registers(Gui gui) {
    for (int i = 0; i < 16; i++) {
        gtk_list_store_insert_with_values(gui->list_store_registers, NULL, -1,
                                          0, i, 
                                          1, REG_NAMES[i], 
                                          2, &DUMMY_WORD_0,
                                          -1);
    }
    
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_text_new();
    
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(gui->tree_view_registers), -1,
                                                "", renderer, "text", 0, NULL);    
                                                
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(gui->tree_view_registers), -1,
                                                "", renderer, "text", 1, NULL);   
                                                
    gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(gui->tree_view_registers),
                                                -1, "", renderer, 
                                                &cell_data_func_binary,
                                                (gpointer) 2, NULL);           
                                                
    gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(gui->tree_view_registers),
                                                -1, "", renderer, 
                                                &cell_data_func_reg_int,
                                                NULL, NULL);    
}

Gui gui_ctor(word memory_addresses) {
    gtk_init(0, NULL);
    Gui gui = malloc(sizeof(struct PRIVATE_GUI));
    
    gui->window                     = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gui->label_pc                   = gtk_label_new("PC: ");
    gui->entry_buffer_pc            = gtk_entry_buffer_new("x3000", -1);
    gui->entry_pc                   = gtk_entry_new_with_buffer(gui->entry_buffer_pc);
    gui->menu_bar                   = gtk_menu_bar_new();
    gui->menu_file                  = gtk_menu_new();
    gui->menu_execute               = gtk_menu_new();
    gui->menu_item_file_open        = gtk_menu_item_new_with_label("Open");
    gui->menu_item_file_exit        = gtk_menu_item_new_with_label("Exit");
    gui->menu_item_execute_run      = gtk_menu_item_new_with_label("Run");
    gui->menu_item_execute_step     = gtk_menu_item_new_with_label("Step");
    gui->menu_item_file             = gtk_menu_item_new_with_label("File");
    gui->menu_item_help             = gtk_menu_item_new_with_label("Help");
    gui->menu_item_execute          = gtk_menu_item_new_with_label("Execute");
    gui->list_store_memory          = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
    gui->list_store_registers       = gtk_list_store_new(3, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_POINTER);
    gui->box_vertical               = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gui->box_horizontal             = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gui->tree_view_memory           = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->list_store_memory));
    gui->tree_view_registers        = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->list_store_registers));
    gui->scrolled_window_memory     = gtk_scrolled_window_new(NULL, NULL);
    gui->scrolled_window_registers  = gtk_scrolled_window_new(NULL, NULL);
    gui->button_1                   = gtk_button_new_with_label("Button 1");
    gui->button_2                   = gtk_button_new_with_label("Button 2");
    
    connect_signals(gui);
    set_up_memory(gui, memory_addresses);
    set_up_registers(gui);
    set_up_layout(gui);
    
    return gui;
}

void gui_connect_memory_address(Gui gui, word address, word *data) {
    GtkTreeIter iter;
    gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(gui->list_store_memory), &iter, NULL, address);
    gtk_list_store_set(gui->list_store_memory, &iter, 1, data, -1);
}

void gui_connect_register(Gui gui, int num, word *data) {
    GtkTreeIter iter;
    gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(gui->list_store_registers), &iter, NULL, num);
    gtk_list_store_set(gui->list_store_registers, &iter, 2, data, -1);
}

void gui_connect_run(Gui gui, void (*run)()) {
    gui->run = run;
}

void gui_connect_step(Gui gui, void (*step)()) {
    gui->step = step;
}

void gui_main() {
    gtk_main();
}