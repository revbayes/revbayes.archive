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
#include "RbObjectWrapper.h"

#include <map>
#include <string>


/** Frame: class used to hold local variable table */
class Frame {

    public:
                                Frame(Frame* parentFr=NULL);    //!< Constructor from parent frame
        virtual                 ~Frame() {}                     //!< Destructor; do not destroy entire environment

        // Basic utility function
        virtual void            print(std::ostream& o) const;   //!< Print table

        // Regular functions
	    void                    addVariable(const std::string& name, RbObject* value);      //!< Add a const variable
	    void                    addVariable(const std::string& name, RbObjectWrapper* var); //!< Add a variable
	    void                    addVariable(const std::string& name, const IntVector& index, DAGNode* var);   //!< Add container variable
	    void                    addVariable(const std::string& name, const std::string& type, int dim=0);   //!< Add declared but empty slot
        void                    eraseVariable(const std::string& name);                     //!< Erase a variable
        bool                    existsVariable(const std::string& name) const;              //!< Does variable exist?
        const std::string&      getDeclaredType(const std::string& name) const;             //!< Get declared type of variable
        int                     getDim(const std::string& name) const;                      //!< Get dim of variable
        const RbObject*         getValue(const std::string& name) const;                    //!< Get value
        const RbObjectWrapper*  getVariable(const std::string& name) const;                 //!< Get a variable
        const RbObjectWrapper*  getVarElement(const std::string& name, const IntVector& index) const;//!< Get var elem
        const RbObject*         getValElement(const std::string& name, const IntVector& index) const;//!< Get val elem
        void                    setVariable(const std::string& name, RbObjectWrapper* var); //!< Set a variable
        void                    setValElement(const std::string& name, const IntVector& index, RbObject* value); //!< Set element of an object
        void                    setVarElement(const std::string& name, const IntVector& index, DAGNode* variable); //!< Set element of a DAG node container
	
    private:
        Frame*                                  parentFrame;        //!< Pointer to enclosing frame
	    std::map<const std::string, ObjectSlot> variableTable;      //!< Variable table
};

#endif


