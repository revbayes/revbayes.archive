#include <stdio.h>
#include <stdlib.h>

#include "RbGTKGui.h"


//static void enter_callback( GtkWidget *widget,
//                           GtkWidget *entry )
//{
//    const gchar *entry_text;
//    entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
//    printf ("Entry contents: %s\n", entry_text);
//
//}


int main( int argc, char *argv[] )
{
    
    RevBayesGTK::RbGTKGui& gui = RevBayesGTK::RbGTKGui::globalInstanceGUI();
    gui.start(argc, argv);
    
    return 0;
}
