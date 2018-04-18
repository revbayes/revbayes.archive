#ifndef RbGTKGui_H
#define RbGTKGui_H

#include <stdio.h>
#include <string>
#include <vector>
#include <gtk/gtk.h>

#include "RevLanguageMain.h"
#include "RlStringOutputStream.h"

namespace RevBayesGTK {
    
    class RbGTKGui {
        
    public:
        // constructors and destructors
        virtual                    ~RbGTKGui(void);
        
        static RbGTKGui&            globalInstanceGUI(void)
        {
            static RbGTKGui gui = RbGTKGui();
            return gui;
        }

        
        // public methods
        void                        addCommand(const std::string &c);
        void                        executeRevCommand(const std::string &c);
        const std::string&          getNextCommand(void) const;
        const std::string&          getPreviousCommand(void) const;
        int                         getCommandReturnValue(void) const { return command_return_value; }
        GtkWidget*                  getCommandTextField(void) const { return command_entry; }
        GtkWidget*                  getMainWindow(void) const { return window; }
        StringOutputStream*         getOutputStream(void) const { return rev_output; }
        GtkTextBuffer*              getOutputTextBuffer(void) const { return output_text; }
        GtkWidget*                  getOutputView(void) const { return output_view; }
        GtkTextBuffer*              getScriptTextBuffer(void) const { return script_text; }
        GtkWidget*                  getScriptView(void) const { return script_view; }
        void                        setCommandReturnValue(int v) { command_return_value = v; }
        void                        start(int argc, char *argv[]);                                 //!< This node was changed in the tree
        
    private:
        RbGTKGui(void);
        

        GtkTextBuffer*              output_text;
        GtkWidget*                  output_view;
        GtkWidget*                  command_entry;
        GtkTextBuffer*              script_text;
        GtkWidget*                  script_view;
        GtkWidget*                  window;

        RevLanguageMain             rl_interpreter;
        StringOutputStream*         rev_output;
        int                         command_return_value;
        std::vector<std::string>    command_history;
        mutable size_t              command_index;
        std::string                 empty_string;
    };
    
}

#endif
