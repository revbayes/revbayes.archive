#ifndef RbGTKGui_H
#define RbGTKGui_H

#include <stdio.h>
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
        void                        addCommand(const std::string &c) { command_history.push_back(c), command_index = command_history.size()-1; }
        void                        executeRevCommand(const std::string &c);
        const std::string&          getNextCommand(void) const { return (command_index >= command_history.size() ? empty_string : command_history[command_index++]); }
        const std::string&          getPreviousCommand(void) const { return command_history[(command_index == 0 ? command_index : command_index--)]; }
        int                         getCommandReturnValue(void) const { return command_return_value; }
        GtkWidget*                  getCommandTextField(void) const { return command_entry; }
        GtkWidget*                  getMainWindow(void) const { return window; }
        StringOutputStream*         getOutputStream(void) const { return rev_output; }
        GtkTextBuffer*              getOutputTextBuffer(void) const { return output_text; }
        GtkTextBuffer*              getScriptTextBuffer(void) const { return script_text; }
        void                        setCommandReturnValue(int v) { command_return_value = v; }
        void                        start(int argc, char *argv[]);                                 //!< This node was changed in the tree
        
    private:
        RbGTKGui(void);
        

        GtkTextBuffer*              output_text;
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
