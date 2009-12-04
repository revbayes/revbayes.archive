/**
 * @file
 * This file contains the declaration of Frame, which is
 * used to hold information about an evaluation or execution
 * frame. Each frame has a pointer to the enclosing (parent)
 * frame. A frame and its parents constitute an evaluation
 * environment. The base environment is the global workspace.
 * It is a special type of frame, which is described in the
 * class Workspace, derived from Frame.
 *
 * A frame essentially consists of a variable table. The workspace
 * also contains a function table, and a class (type) table.
 *
 * @brief Declaration of Frame
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef Frame_H 
#define Frame_H

#include <map>
#include <string>

#include "DAGNode.h"

/** Frame: class used to hold local variable table */
class Frame {

    public:
            Frame(Frame* parentFr=NULL);        //!< Constructor from parent frame
	        virtual ~Frame();                   //!< Destructor deletes local objects

        // Basic utility functions
        virtual void                print(std::ostream& o) const;               //!< Print table

        // Regular functions
	    bool        addVariable(const std::string& name, RbObject* variable);   //!< Add a variable
        bool        eraseVariable(const std::string& name);                     //!< Erase a variable
        bool        existsVariable(const std::string& name);                    //!< Does variable exist?
        RbObject*   getVariable(const std::string& name);                       //!< Get a variable
	
    protected:
        Frame*                            parentFrame;          //!< Pointer to enclosing frame
	    std::map<const std::string, RbObject*>  variableTable;  //!< Local variable table
};

#endif

