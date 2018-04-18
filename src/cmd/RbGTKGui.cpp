#include "RbGTKGui.h"

#include "Parser.h"
#include "RbFileManager.h"
#include "RlUserInterface.h"
#include "StringUtilities.h"
#include "Workspace.h"

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <fstream>
#include <stdio.h>
#include <string>


using namespace RevBayesGTK;


static gint button_press (GtkWidget *, GdkEvent *);
//static void menuitem_response (gchar *);
static void menuitem_font_response(gchar *);
static void menuitem_load_response(gchar *);
static void menuitem_run_selected_response(gchar *);
static void menuitem_save_response(gchar *);
static void menuitem_set_wd_response(gchar *);
static void menuitem_source_response(gchar *);


gboolean command_on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    
    RevBayesGTK::RbGTKGui& gui_instance = RevBayesGTK::RbGTKGui::globalInstanceGUI();

    int result = 0;
    std::string command_line = "";
    std::string line = "";
    std::string tmp = "";
    size_t bsz = 0;
    char * buffer = NULL;
    std::string output = "";
    
    bool action_return_value = FALSE;
    
    switch (event->keyval)
    {
        
            
        case GDK_Return:
            
            result = gui_instance.getCommandReturnValue();
            
            const gchar *entry_text;
            entry_text = gtk_entry_get_text(GTK_ENTRY(gui_instance.getCommandTextField()) );

            line = std::string(entry_text);

            if (result == 0 || result == 2)
            {
                command_line = line;
            }
            else if (result == 1)
            {
                command_line += ";" + line;
            }

            bsz = command_line.size();

            buffer = new char[bsz+1];
            buffer[bsz] = 0;
            for (int i = 0; i < bsz; i++)
            {
                buffer[i] = command_line[i];
            }

            tmp = std::string( buffer );

            result = RevLanguage::Parser::getParser().processCommand(tmp, &RevLanguage::Workspace::userWorkspace());

//            gtk_text_buffer_set_text(gui_instance.getOutputTextView(), entry_text, -1);
            
//            gint tmp_pos = GTK_ENTRY (entry)->text_length;
            //    gtk_editable_insert_text (GTK_EDITABLE (entry), " world", -1, &tmp_pos);
//            gtk_text_buffer_set_text(gui_instance.getOutputTextView(), entry_text, -1, &tmp_pos);
            GtkTextIter end;
            gtk_text_buffer_get_end_iter(gui_instance.getOutputTextBuffer(), &end);
            gtk_text_buffer_insert(gui_instance.getOutputTextBuffer(), &end, ( result == 0 || result == 2 ? "> " : "+ ") , -1);
            gtk_text_buffer_get_end_iter(gui_instance.getOutputTextBuffer(), &end);
            gtk_text_buffer_insert(gui_instance.getOutputTextBuffer(), &end, entry_text, -1);
            gtk_text_buffer_get_end_iter(gui_instance.getOutputTextBuffer(), &end);
            gtk_text_buffer_insert(gui_instance.getOutputTextBuffer(), &end, "\n", -1);

            gtk_entry_set_text(GTK_ENTRY(gui_instance.getCommandTextField()), "");
            
            output = gui_instance.getOutputStream()->getOutputString();
            gui_instance.getOutputStream()->emptyOutputString();
            if ( output != "" )
            {
                gtk_text_buffer_get_end_iter(gui_instance.getOutputTextBuffer(), &end);
                gtk_text_buffer_insert(gui_instance.getOutputTextBuffer(), &end, output.c_str(), -1);
            }
            
            gui_instance.addCommand(line);

            break;
//        case GDK_Tab:
//            printf("keyval TAB\n");
//            break;
        case GDK_Up: //GDK_arrow_up:
//            printf("keyval Arrow Up\n");
            gtk_entry_set_text(GTK_ENTRY(gui_instance.getCommandTextField()), gui_instance.getPreviousCommand().c_str() );
            gtk_widget_grab_focus(gui_instance.getCommandTextField());
            action_return_value = TRUE;
            break;
        case GDK_Down:
//            printf("keyval Arrow Down\n");
            gtk_entry_set_text(GTK_ENTRY(gui_instance.getCommandTextField()), gui_instance.getNextCommand().c_str() );
            gtk_widget_grab_focus(gui_instance.getCommandTextField());
            action_return_value = TRUE;
            break;
            
        default:
            action_return_value = FALSE;
    }
    
    return action_return_value;
}


RbGTKGui::RbGTKGui( void ) :
    rl_interpreter( false ),
    command_return_value( 0 ),
    command_index( 0 ),
    empty_string( "" )
{
    
    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    
    /* initialize environment */
    rev_output = new StringOutputStream();
    RevLanguage::UserInterface::userInterface().setOutputStream( rev_output );
    rl_interpreter.startRevLanguageEnvironment(sourceFiles);

    
}


RbGTKGui::~RbGTKGui( void )
{
    
}



void RbGTKGui::executeRevCommand(const std::string &command)
{
    
    std::vector<std::string> lines;
    StringUtilities::stringSplit(command, "\n", lines);

    int result = 0;
    for (size_t i=0; i<lines.size(); ++i)
    {
        std::string current_command = lines[i];
        
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(output_text, &end);
        gtk_text_buffer_insert(output_text, &end, ( result == 0 || result == 2 ? "> " : "+ ") , -1);
        gtk_text_buffer_get_end_iter(output_text, &end);
        gtk_text_buffer_insert(output_text, &end, current_command.c_str(), -1);
        gtk_text_buffer_get_end_iter(output_text, &end);
        gtk_text_buffer_insert(output_text, &end, "\n", -1);
        
        result = RevLanguage::Parser::getParser().processCommand(current_command, &RevLanguage::Workspace::userWorkspace());
    
        const std::string &output = rev_output->getOutputString();
        if ( output != "" )
        {
            gtk_text_buffer_get_end_iter(output_text, &end);
            gtk_text_buffer_insert(output_text, &end, output.c_str(), -1);
        }
        rev_output->emptyOutputString();
    
//        gui_instance.addCommand(line);

    }
    
}


void RbGTKGui::addCommand(const std::string &c)
{
    command_history.push_back(c);
    command_index = command_history.size()-1;
}


const std::string& RbGTKGui::getNextCommand(void) const
{
    if ( command_history.size() > 0 )
    {
        return (command_index >= command_history.size() ? empty_string : command_history[command_index++]);
    }
    
    return empty_string;
}


const std::string& RbGTKGui::getPreviousCommand(void) const
{
    if ( command_history.size() > 0 )
    {
        return command_history[(command_index == 0 ? command_index : command_index--)];
    }
    
    return empty_string;
}


void RbGTKGui::start( int argc, char *argv[] )
{
    
    GtkWidget *table;
    
    GtkWidget       *output_frame;
    GtkWidget       *script_frame;
    GtkWidget       *command_frame;
    
    gtk_init (&argc, &argv);
    
    /* create a new window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_title (GTK_WINDOW (window), "RevBayes");

    
    g_signal_connect (window, "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect_swapped (window, "delete-event",
                              G_CALLBACK (gtk_widget_destroy),
                              window);
    
    
    GtkWidget *menubar          = gtk_menu_bar_new();
    GtkWidget *file_sub_menu    = gtk_menu_new();
    GtkWidget *edit_sub_menu    = gtk_menu_new();
    GtkWidget *run_sub_menu     = gtk_menu_new();
    
    GtkWidget *file_menu    = gtk_menu_item_new_with_label("File");
    GtkWidget *load_menu    = gtk_menu_item_new_with_label("Load script");
    GtkWidget *save_menu    = gtk_menu_item_new_with_label("Save script");
    GtkWidget *source_menu  = gtk_menu_item_new_with_label("Source script");
    GtkWidget *wd_menu      = gtk_menu_item_new_with_label("Set working directory");
    GtkWidget *quit_menu    = gtk_menu_item_new_with_label("Quit");
    
    GtkWidget *edit_menu            = gtk_menu_item_new_with_label("Edit");
    GtkWidget *script_font_menu     = gtk_menu_item_new_with_label("Select script font");
    GtkWidget *output_font_menu     = gtk_menu_item_new_with_label("Select output font");

    GtkWidget *run_menu             = gtk_menu_item_new_with_label("Run");
    GtkWidget *run_selected_menu    = gtk_menu_item_new_with_label("Run selected");

    
    gtk_signal_connect_object (GTK_OBJECT (source_menu), "activate",
                               GTK_SIGNAL_FUNC (menuitem_source_response),
                               (gpointer) "source");
    gtk_signal_connect_object (GTK_OBJECT (wd_menu), "activate",
                               GTK_SIGNAL_FUNC (menuitem_set_wd_response),
                               (gpointer) "set_wd");
    gtk_signal_connect_object (GTK_OBJECT (load_menu), "activate",
                               GTK_SIGNAL_FUNC (menuitem_load_response),
                               (gpointer) "load");
    gtk_signal_connect_object (GTK_OBJECT (save_menu), "activate",
                               GTK_SIGNAL_FUNC (menuitem_save_response),
                               (gpointer) "save");
    gtk_signal_connect_object (GTK_OBJECT (output_font_menu), "activate",
                               GTK_SIGNAL_FUNC (menuitem_font_response),
                               (gpointer) "output");
    gtk_signal_connect_object (GTK_OBJECT (script_font_menu), "activate",
                               GTK_SIGNAL_FUNC (menuitem_font_response),
                               (gpointer) "script");
    gtk_signal_connect_object (GTK_OBJECT (run_selected_menu), "activate",
                               GTK_SIGNAL_FUNC (menuitem_run_selected_response),
                               (gpointer) "run.selected");
    
    g_signal_connect(G_OBJECT(quit_menu), "activate",
                     G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu), file_sub_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_sub_menu), wd_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_sub_menu), load_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_sub_menu), source_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_sub_menu), save_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_sub_menu), quit_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_menu), edit_sub_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_sub_menu), script_font_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_sub_menu), output_font_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit_menu);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(run_menu), run_sub_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(run_sub_menu), run_selected_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), run_menu);
//    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), run_selected_menu);
    
    
    
    
    
    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    
    
    /* create the script view */
    /* We need */
    /*    a text entry */
    /*    a frame */
    
    script_view = gtk_text_view_new();
    script_text = gtk_text_view_get_buffer(GTK_TEXT_VIEW(script_view));
    gtk_widget_modify_font(script_view, pango_font_description_from_string ("Monospace 20"));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(script_view), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(script_view), TRUE);
    
    GtkWidget* script_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(script_scrolled_window) ,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    script_frame = gtk_frame_new("Script");
    
    gtk_container_add(GTK_CONTAINER(script_scrolled_window), script_view);
    gtk_container_add(GTK_CONTAINER(script_frame), script_scrolled_window);
    
    gtk_widget_show(script_scrolled_window);
    gtk_widget_show(script_frame);
    gtk_widget_show(script_view);
    
    
    //    gtk_widget_set_size_request(script_frame, 300, 200);

    
    /* create the output view */
    /* We need */
    /*    a text view */
    /*    a text buffer */
    /*    a frame */
    output_view = gtk_text_view_new();
    gtk_widget_modify_font (output_view, pango_font_description_from_string ("Monospace 20"));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(output_view), FALSE);

    output_text = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_view));
    gtk_text_buffer_set_text(output_text, "", -1);
    
    //    out_std_view = gtk_vscrollbar_new( NULL );
    GtkWidget* output_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(output_scrolled_window) ,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    output_frame = gtk_frame_new ("Output");
    
    gtk_container_add(GTK_CONTAINER(output_scrolled_window), output_view);
    gtk_container_add(GTK_CONTAINER(output_frame), output_scrolled_window);
    
    gtk_widget_show(output_scrolled_window);
    gtk_widget_show(output_frame);
    gtk_widget_show(output_view);
    
//    gtk_widget_set_size_request(output_frame, 800, 400);
    
    /* create the command view */
    /* We need */
    /*    a text entry */
    /*    a frame */
    command_frame = gtk_frame_new ("Command");
    
    command_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY (command_entry), 200);
    
    g_signal_connect(G_OBJECT(command_entry), "key_press_event", G_CALLBACK (command_on_key_press), NULL);
    gtk_entry_set_text(GTK_ENTRY(command_entry), "");
    
    gtk_container_add(GTK_CONTAINER(command_frame), command_entry);
    gtk_widget_show(command_frame);
    gtk_widget_show(command_entry);
    
//    gtk_container_add(GTK_CONTAINER(window), table);
    
    GtkWidget *panel_script_output = gtk_vpaned_new();
    gtk_paned_add1 ( (GtkPaned*)panel_script_output, script_frame);
    gtk_paned_add2 ( (GtkPaned*)panel_script_output, output_frame);
    
    
//    table = gtk_table_new(2, 2, FALSE);
    table = gtk_vbox_new(FALSE, 5);
//    gtk_box_set_spacing ( (GtkBox*)table, 10);
//    gtk_container_add(GTK_CONTAINER(window), table);
    
    gtk_box_pack_start(GTK_BOX(table), panel_script_output, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(table), command_frame, FALSE, TRUE, 5);

    
//    gtk_table_attach(GTK_TABLE(table), panel_script_output, 0, 1, 0, 1,
//                     GTK_FILL, GTK_FILL, 5, 5);
//    gtk_table_attach(GTK_TABLE(table), command_frame, 0, 1, 1, 2,
//                     GTK_FILL, GTK_FILL, 5, 5);
    //    gtk_table_attach(GTK_TABLE(table), script_frame, 0, 1, 0, 1,
    //                     GTK_FILL, GTK_FILL, 5, 5);
    //    gtk_table_attach(GTK_TABLE(table), output_frame, 0, 1, 1, 2,
    //                     GTK_EXPAND, GTK_EXPAND, 5, 5);
    
    
    gtk_container_add(GTK_CONTAINER(vbox), table);
    gtk_widget_show_all(window);
    gtk_widget_show(window);
    
    
    
    // print the welcome to RevBayes message
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(output_text, &end);
    gtk_text_buffer_insert(output_text, &end, rev_output->getOutputString().c_str(), -1);
    
    // emtpy the current stream
    rev_output->emptyOutputString();

    
    gtk_main();

}


static void menuitem_font_response( gchar *caller )
{
    RevBayesGTK::RbGTKGui& gui_instance = RevBayesGTK::RbGTKGui::globalInstanceGUI();
    
    GtkWidget *dialog;
    dialog = gtk_font_selection_dialog_new("Select font");
//    dialog = gtk_font_chooser_dialog_new("Select font", GTK_WINDOW(gui_instance.getMainWindow()));
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
    {
        char *fontname;
        
        fontname = gtk_font_selection_dialog_get_font_name ( (GtkFontSelectionDialog*) dialog);
//        fontname = gtk_font_chooser_get_font ( (GtkFontChooser*)dialog);

        if ( std::string(caller) == "script" )
        {
            gtk_widget_modify_font(gui_instance.getScriptView(), pango_font_description_from_string (fontname));
        }
        else if ( std::string(caller) == "output" )
        {
            gtk_widget_modify_font(gui_instance.getOutputView(), pango_font_description_from_string (fontname));
        }
    }
    
    gtk_widget_destroy (dialog);
}


/* Print a string when a menu item is selected */
static void menuitem_run_selected_response( gchar *string )
{
    RevBayesGTK::RbGTKGui& gui_instance = RevBayesGTK::RbGTKGui::globalInstanceGUI();
    
    GtkTextIter selection_start;
    GtkTextIter selection_end;

    gtk_text_buffer_get_selection_bounds (gui_instance.getScriptTextBuffer(),
                                          &selection_start,
                                          &selection_end);
    
    const gchar *entry_text;
    entry_text = gtk_text_buffer_get_text (gui_instance.getScriptTextBuffer(),
                              &selection_start,
                              &selection_end,
                              FALSE);
    std::string command = std::string( entry_text );
    
    gui_instance.executeRevCommand(command);
}

static void menuitem_load_response( gchar *string )
{
    RevBayesGTK::RbGTKGui& gui_instance = RevBayesGTK::RbGTKGui::globalInstanceGUI();
    
    GtkWidget *dialog;
    
    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          GTK_WINDOW(gui_instance.getMainWindow()),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        
        
        // open file
        std::ifstream read_stream;
        RevBayesCore::RbFileManager f = RevBayesCore::RbFileManager( std::string(filename) );
        if ( f.openFile(read_stream) == false )
        {
//            throw RbException( "Could not open file " + filename );
            std::cerr << "Could not open file with name '" << std::string(filename) << "'." << std::endl;
        }
        
        // read file
        std::string read_line = "";
        GtkTextIter end;
//        while ( f.safeGetline(read_stream,read_line) )
        while ( read_stream.good() )
        {
            f.safeGetline( read_stream, read_line );
            
            gtk_text_buffer_get_end_iter(gui_instance.getScriptTextBuffer(), &end);
            gtk_text_buffer_insert(gui_instance.getScriptTextBuffer(), &end, read_line.c_str(), -1);
            gtk_text_buffer_get_end_iter(gui_instance.getScriptTextBuffer(), &end);
            gtk_text_buffer_insert(gui_instance.getScriptTextBuffer(), &end, "\n", -1);
            
//            gtk_entry_set_text(GTK_ENTRY(gui_instance.getCommandTextField()), "");
            
        };
        
        f.closeFile( read_stream );
        
        g_free (filename);
        
    }
    
    gtk_widget_destroy (dialog);
}


static void menuitem_set_wd_response( gchar *string )
{
    RevBayesGTK::RbGTKGui& gui_instance = RevBayesGTK::RbGTKGui::globalInstanceGUI();
    
    GtkWidget *dialog;
    
    dialog = gtk_file_chooser_dialog_new ("Select working directory",
                                          GTK_WINDOW(gui_instance.getMainWindow()),
                                          GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        
        RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager( std::string(filename) );
        std::cerr << "Full file name:\t\t" << fm.getFullFileName() << std::endl;
        
        std::string fn = std::string(filename)
#       if defined (RB_WIN)
        RevBayesCore::StringUtilities::replaceSubstring(fn, "\\", "\\\\");
#       endif
        std::cerr << "Tmp file name:\t\t" << fn << std::endl;

        std::string command = "setwd(\"" + fn + "\");";
        gui_instance.executeRevCommand(command);
        
    }
    
    gtk_widget_destroy (dialog);
}

static void menuitem_source_response( gchar *string )
{
    RevBayesGTK::RbGTKGui& gui_instance = RevBayesGTK::RbGTKGui::globalInstanceGUI();
    
    GtkWidget *dialog;
    
    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          GTK_WINDOW(gui_instance.getMainWindow()),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

        std::string command = "source(\"" + std::string(filename) + "\");";
        gui_instance.executeRevCommand(command);
        
    }
    
    gtk_widget_destroy (dialog);
}

static void menuitem_save_response( gchar *string )
{
    RevBayesGTK::RbGTKGui& gui_instance = RevBayesGTK::RbGTKGui::globalInstanceGUI();
    
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Save Script",
                                          GTK_WINDOW(gui_instance.getMainWindow()),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

//    if (user_edited_a_new_document)
//    {
//        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), default_folder_for_saving);
//        gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled document");
//    }
//    else
//        gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_for_existing_document);
    
    
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        printf ("%s\n", filename);
        
        GtkTextIter it_start;
        GtkTextIter it_end;
        gtk_text_buffer_get_start_iter(gui_instance.getScriptTextBuffer(), &it_start);
        gtk_text_buffer_get_end_iter(gui_instance.getScriptTextBuffer(), &it_end);
        
        const gchar *entry_text;
        entry_text = gtk_text_buffer_get_text (gui_instance.getScriptTextBuffer(),
                                               &it_start,
                                               &it_end,
                                               FALSE);
        
//        RbFileManager fm = RbFileManager(filename);
        std::fstream out_stream;
        out_stream.open( filename, std::fstream::out);
        out_stream << entry_text;
        out_stream.close();
        
        g_free (filename);

    }
    
    gtk_widget_destroy (dialog);
}

