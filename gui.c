#include <gtk/gtk.h>
#include "gui.h"
#include "bit.h"
#include <stdlib.h>
#include <stdio.h>

static word dummy = 0;

struct PRIVATE_GUI {
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
				GtkWidget *label;
				GtkWidget *entry;
					GtkEntryBuffer *entry_buffer;
			GtkWidget *tree_view_registers;
				GtkListStore *list_store_registers;
			GtkWidget *scrolled_window;
				GtkWidget *tree_view_memory;
					GtkListStore *list_store_memory;
					
};

void cell_data_func_mem_address(GtkTreeViewColumn *tree_column,
								GtkCellRenderer *cell,
								GtkTreeModel *tree_model,
								GtkTreeIter *iter,
								gpointer data) {
	word address;
	gtk_tree_model_get(tree_model, iter, 0, &address, -1);
	char address_string[WORD_LEN / 4 + 2];
	sprintf(address_string, "x%08X", address);
	g_object_set(G_OBJECT(cell), "text", address_string, NULL);
}

void cell_data_func_mem_binary(GtkTreeViewColumn *tree_column,
								GtkCellRenderer *cell,
								GtkTreeModel *tree_model,
								GtkTreeIter *iter,
								gpointer data) {
	word *ptr;
	gtk_tree_model_get(tree_model, iter, 1, &ptr, -1);
	char binary_string[WORD_LEN + 1];
	word mask = 1 << (WORD_LEN - 1);
	for (int i = 0; i < WORD_LEN; i++) {
		binary_string[i] = !!(*ptr & mask) + '0';
		mask >>= 1;
	}
	g_object_set(G_OBJECT(cell), "text", binary_string, NULL);
}

void cell_data_func_mem_asm(GtkTreeViewColumn *tree_column,
							GtkCellRenderer *cell,
							GtkTreeModel *tree_model,
							GtkTreeIter *iter,
							gpointer data) {
	gpointer ptr;
	gtk_tree_model_get(tree_model, iter, 1, &ptr, -1);
	g_object_set(G_OBJECT(cell), "text", "ADD R1, R2, R3", NULL);
}

Gui gui_ctor(word memory_addresses) {
	gtk_init(0, NULL);
	Gui gui = malloc(sizeof(struct PRIVATE_GUI));
	
	gui->window					 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gui->label 					 = gtk_label_new("PC: ");
	gui->entry_buffer			 = gtk_entry_buffer_new("x3000", -1);
	gui->entry					 = gtk_entry_new_with_buffer(gui->entry_buffer);
	gui->menu_bar				 = gtk_menu_bar_new();
	gui->menu_file				 = gtk_menu_new();
	gui->menu_execute			 = gtk_menu_new();
	gui->menu_item_file_open	 = gtk_menu_item_new_with_label("Open");
	gui->menu_item_file_exit	 = gtk_menu_item_new_with_label("Exit");
	gui->menu_item_execute_run	 = gtk_menu_item_new_with_label("Run");
	gui->menu_item_execute_step	 = gtk_menu_item_new_with_label("Step");
	gui->menu_item_file	   		 = gtk_menu_item_new_with_label("File");
	gui->menu_item_help	   		 = gtk_menu_item_new_with_label("Help");
	gui->menu_item_execute		 = gtk_menu_item_new_with_label("Execute");
	gui->list_store_memory	 	 = gtk_list_store_new(2, G_TYPE_UINT, G_TYPE_POINTER);
	gui->list_store_registers	 = gtk_list_store_new(1, G_TYPE_UINT);
	gui->box_vertical 		     = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gui->box_horizontal		  	 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gui->tree_view_memory     	 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->list_store_memory));
	gui->tree_view_registers   	 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui->list_store_registers));
	gui->scrolled_window		 = gtk_scrolled_window_new(NULL, NULL);
	gui->button_1			 	 = gtk_button_new_with_label("Button 1");
	gui->button_2			 	 = gtk_button_new_with_label("Button 2");
	
	
	g_signal_connect(G_OBJECT(gui->window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(gui->menu_item_file_exit), "activate", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title(GTK_WINDOW(gui->window), "LC-2200 Simulator");
	gtk_window_set_default_size(GTK_WINDOW(gui->window), 500, 750);
	//gtk_window_set_icon(GTK_WINDOW(window), );
	gtk_window_set_has_resize_grip(GTK_WINDOW(gui->window), FALSE);
	
	for (word i = 0; i < memory_addresses; i++) {
		gtk_list_store_insert_with_values(gui->list_store_memory, NULL, -1, 0, i, 1, &dummy, -1);
	}
	for (int i = 0; i < 8; i++) {
		gtk_list_store_insert_with_values(gui->list_store_registers, NULL, -1, 0, i * 100000, -1);
	}
	
	GtkCellRenderer *renderer;
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(gui->tree_view_memory),
												-1, "", renderer, 
												*cell_data_func_mem_address,
												NULL, NULL);
												
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(gui->tree_view_memory),
												-1, "", renderer, 
												*cell_data_func_mem_binary,
												NULL, NULL);
												
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(gui->tree_view_memory),
												-1, "", renderer, 
												*cell_data_func_mem_asm,
												NULL, NULL);
	
	gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(gui->scrolled_window), 500);
    
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(gui->tree_view_registers), -1,
												"", renderer, "text", 0, NULL);											
												
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_view_memory), FALSE);	
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gui->tree_view_registers), FALSE);		
	
	gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_1, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gui->box_horizontal), gui->button_2, FALSE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(gui->box_horizontal), gui->entry, FALSE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(gui->box_horizontal), gui->label, FALSE, TRUE, 0);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(gui->menu_item_file), gui->menu_file);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(gui->menu_item_execute), gui->menu_execute);
	gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_file), gui->menu_item_file_open);
	gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_file), gui->menu_item_file_exit);
	gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_execute), gui->menu_item_execute_run);
	gtk_menu_shell_append(GTK_MENU_SHELL(gui->menu_execute), gui->menu_item_execute_step);
	gtk_container_add(GTK_CONTAINER(gui->menu_bar), gui->menu_item_file);
	gtk_container_add(GTK_CONTAINER(gui->menu_bar), gui->menu_item_execute);
	gtk_container_add(GTK_CONTAINER(gui->menu_bar), gui->menu_item_help);
	gtk_container_add(GTK_CONTAINER(gui->scrolled_window), gui->tree_view_memory);
	gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->menu_bar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gui->box_vertical),gui-> box_horizontal, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->tree_view_registers, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gui->box_vertical), gui->scrolled_window, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(gui->window), gui->box_vertical);
	gtk_widget_show_all(gui->window);
	return gui;
}

void gui_set_mem(Gui gui, word address, word *data) {
	GtkTreeIter iter;
	gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(gui->list_store_memory), &iter, NULL, address);
	gtk_list_store_set(gui->list_store_memory, &iter, 1, data, -1);
}


void gui_main() {
	gtk_main();
}