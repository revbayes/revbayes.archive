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

#include "DAGNode.h"
#include "IntVector.h"
#include "ObjectSlot.h"
#include "RbObject.h"

#include <map>
#include <string>


/** Frame: class used to hold local variable table */
class Frame {

    public:
            Frame(Frame* parentFr=NULL);        //!< Constructor from parent frame

        // Basic utility function
        virtual void    print(std::ostream& o) const;   //!< Print table

        // Regular functions
	    bool        addVariable(const std::string& name, DAGNode* variable);   //!< Add a variable
	    bool        addVariable(const std::string& name, const IntVector& index, DAGNode* variable);   //!< Add a container variable
	    bool        addVariable(const std::string& name, const std::string& type, int dim);   //!< Add declared but empty slot
        bool        eraseVariable(const std::string& name);                     //!< Erase a variable
        bool        existsVariable(const std::string& name) const;              //!< Does variable exist?
        std::string getDeclaredType(const std::string& name);                   //!< Get declared type of variable
        RbObject*   getVariable(const std::string& name);                       //!< Get a variable
	
    protected:
        Frame*                                  parentFrame;        //!< Pointer to enclosing frame
	    std::map<const std::string, ObjectSlot> variableTable;      //!< Variable table
};

#endif

