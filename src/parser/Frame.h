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

#include "ObjectSlot.h"

#include <map>
#include <string>

class DAGNode;
class IntVector;
class RbObject;

class Frame {

    public:
                                Frame(void);                    //!< Constructor of frame with NULL parent
                                Frame(Frame* parentFr);         //!< Constructor of frame with parent
        virtual                 ~Frame() {}                     //!< Destroy table of frame, not entire environment

        // Basic utility functions
        virtual Frame*          clone(void) const { return new Frame(*this); }  //!< Clone frame
        virtual Frame*          cloneEnvironment(void) const;                   //!< Clone environment
        virtual void            printValue(std::ostream& o) const;              //!< Print table for user
        virtual std::string     toString(void) const;                           //!< Complete info to string

        // Regular functions
	    void                    addVariable(const std::string& name, RbObject* value);      //!< Add a const variable
	    void                    addVariable(const std::string& name, DAGNode* var);         //!< Add a variable
	    void                    addVariable(const std::string& name, const IntVector& index, DAGNode* var);   //!< Add container variable
	    void                    addVariable(const std::string& name, const std::string& type, int dim=0);   //!< Add declared but empty slot
        void                    eraseVariable(const std::string& name);                     //!< Erase a variable
        bool                    existsVariable(const std::string& name) const;              //!< Does variable exist?
        Frame*                  getParentFrame(void) const { return parentFrame; }          //!< Get parent frame 
        const RbObject*         getValue(const std::string& name) const;                    //!< Get value
        const DAGNode*          getVariable(const std::string& name) const;                 //!< Get variable
        const RbObject*         getValElement(const std::string& name, const IntVector& index) const;   //!< Get val elem
        const DAGNode*          getVarElement(const std::string& name, const IntVector& index) const;   //!< Get var elem
        void                    setValue(const std::string& name, RbObject* val);           //!< Set variable value
        void                    setVariable(const std::string& name, DAGNode* var);         //!< Set variable
        void                    setValElement(const std::string& name, const IntVector& index, RbObject* value);    //!< Set element of an object
        void                    setVarElement(const std::string& name, const IntVector& index, DAGNode* variable);  //!< Set element of a DAG node container
	
    private:
        Frame*                              parentFrame;        //!< Pointer to enclosing frame
        std::map<std::string, ObjectSlot>   variableTable;      //!< Variable table
};

#endif


