#include <gtk/gtk.h>

int main(int argc, char **argv) {
		
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
			
	gtk_init (&argc, &argv);
	
	enum {
		COLUMN_ADDRESS,
		COLUMN_BINARY,
		COLUMN_HEX,
		COLUMN_ASM,	
		N_COLUMNS
	};
  
	window					 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	label 					 = gtk_label_new("PC: ");
	entry_buffer			 = gtk_entry_buffer_new("x3000", -1);
	entry					 = gtk_entry_new_with_buffer(entry_buffer);
	menu_bar				 = gtk_menu_bar_new();
	menu_file				 = gtk_menu_new();
	menu_execute			 = gtk_menu_new();
	menu_item_file_open	   	 = gtk_menu_item_new_with_label("Open");
	menu_item_file_exit		 = gtk_menu_item_new_with_label("Exit");
	menu_item_execute_run	 = gtk_menu_item_new_with_label("Run");
	menu_item_execute_step	 = gtk_menu_item_new_with_label("Step");
	menu_item_file	   		 = gtk_menu_item_new_with_label("File");
	menu_item_help	   		 = gtk_menu_item_new_with_label("Help");
	menu_item_execute		 = gtk_menu_item_new_with_label("Execute");
	list_store_memory	 	 = gtk_list_store_new(N_COLUMNS, G_TYPE_UINT, G_TYPE_UINT,
												  G_TYPE_UINT, G_TYPE_UINT);
	list_store_registers	 = gtk_list_store_new(1, G_TYPE_UINT);
	box_vertical 		     = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	box_horizontal		  	 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	tree_view_memory     	 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store_memory));
	tree_view_registers   	 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store_registers));
	scrolled_window		 	 = gtk_scrolled_window_new(NULL, NULL);
	button_1			 	 = gtk_button_new_with_label("Button 1");
	button_2			 	 = gtk_button_new_with_label("Button 2");
	
	
	g_signal_connect(window, "delete_event", gtk_main_quit, NULL);
	g_signal_connect(menu_item_file_exit, "activate", gtk_main_quit, NULL);
	gtk_window_set_title(GTK_WINDOW(window), "LC-2200 Simulator");
	gtk_window_set_default_size(GTK_WINDOW(window), 500, 750);
	//gtk_window_set_icon(GTK_WINDOW(window), );
	gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);

	for (int i = 0; i <= 100; i++) {
		gtk_list_store_insert_with_values(list_store_memory, NULL, -1, 
			COLUMN_ADDRESS, i + 1000,
			COLUMN_BINARY, i + 200000000,
			COLUMN_HEX, i + 30000,
			COLUMN_ASM, i + 55555555,
			-1);
	}
	for (int i = 0; i < 8; i++) {
		gtk_list_store_insert_with_values(list_store_registers, NULL, -1, 0, i * 100000, -1);
	}
	
	GtkCellRenderer *renderer;
	renderer = gtk_cell_renderer_text_new();
	for (int i = 0; i < N_COLUMNS; i++) {
		gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view_memory), -1,
												    "", renderer, "text", i, NULL);
	}
	
	
	gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled_window), 500);
    
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view_registers), -1,
												"", renderer, "text", 0, NULL);											
												
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view_memory), FALSE);	
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view_registers), FALSE);		
	
	gtk_box_pack_start(GTK_BOX(box_horizontal), button_1, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box_horizontal), button_2, FALSE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(box_horizontal), entry, FALSE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(box_horizontal), label, FALSE, TRUE, 0);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_file), menu_file);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_execute), menu_execute);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), menu_item_file_open);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), menu_item_file_exit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_execute), menu_item_execute_run);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_execute), menu_item_execute_step);
	gtk_container_add(GTK_CONTAINER(menu_bar), menu_item_file);
	gtk_container_add(GTK_CONTAINER(menu_bar), menu_item_execute);
	gtk_container_add(GTK_CONTAINER(menu_bar), menu_item_help);
	gtk_container_add(GTK_CONTAINER(scrolled_window), tree_view_memory);
	gtk_box_pack_start(GTK_BOX(box_vertical), menu_bar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box_vertical), box_horizontal, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box_vertical), tree_view_registers, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box_vertical), scrolled_window, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), box_vertical);

	gtk_widget_show_all(window);

	gtk_main ();
	return 0;
}