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
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef Frame_H 
#define Frame_H

#include "VariableSlot.h"

#include <map>
#include <string>

class DAGNode;
class VectorInteger;
class RbObject;

typedef std::map<std::string, VariableSlot> VariableTable;

/**
 * @brief Execution or evaluation frame
 *
 * Frame is used to hold information about an evaluation or
 * execution frame. Each frame has a pointer to the enclosing
 * (parent) frame. A frame and its parents together constitute
 * an evaluation environment. The base environment is the global
 * workspace. It is a special type of frame, which is described
 * in the class Workspace, derived from Frame. The global work-
 * space contains the user workspace, which is a workspace hold-
 * ing user-defined types and functions. The user workspace, in
 * turn, contains the nested regular frames, which are created
 * during execution or evaluation of code.
 *
 * A frame essentially consists of a variable table. The workspace
 * also contains a function table, and a class (type) table.
 *
 */
class Frame {

    public:
                                    Frame(void);                                                                            //!< Constructor of frame with NULL parent
                                    Frame(Frame* parentFr);                                                                 //!< Constructor of frame with parent
                                    Frame(const Frame& x);                                                                  //!< Copy constructor: set frame of variable slots

        // Operators
        bool                        operator==(const Frame& x) const;                                                       //!< Equals comparison
        bool                        operator!=(const Frame& x) const { return !operator==(x); }                             //!< Not equals comparison

        // Basic utility functions
        virtual Frame*              clone(void) const { return new Frame(*this); }                                          //!< Clone frame
        virtual Frame*              cloneEnvironment(void) const;                                                           //!< Clone environment
        virtual void                printValue(std::ostream& o) const;                                                      //!< Print table for user
        virtual std::string         toString(void) const;                                                                   //!< Complete info to string

        // Regular functions
	    void                        addReference(const std::string& name, DAGNode* ref);                                    //!< Add a reference
	    void                        addReference(const std::string& name, const std::string& type, int dim=0);              //!< Add empty reference slot
	    void                        addVariable(const std::string& name, DAGNode* value);                                   //!< Add a simple variable
	    void                        addVariable(const std::string& name, const VectorInteger& index, DAGNode* elemValue);   //!< Add a plate variable
	    void                        addVariable(const std::string& name, const std::string& type, int dim=0);               //!< Add empty variable slot
        void                        eraseVariable(const std::string& name);                                                 //!< Erase a variable
        bool                        existsVariable(const std::string& name) const;                                          //!< Does variable exist?
        Frame*                      getParentFrame(void) const { return parentFrame; }                                      //!< Get parent frame
        DAGNode*                    getReference(const std::string& name);                                                  //!< Get reference
        const std::string&          getSlotName(const VariableSlot* slot) const;                                            //!< Get name of a slot
        const RbObject*             getValue(const std::string& name) const;                                                //!< Get value
        const DAGNode*              getVariable(const std::string& name) const;                                             //!< Get variable
        VariableSlot&               getVariableSlot(const std::string& name);                                               //!< Get variable slot
        const VariableSlot&         getVariableSlot(const std::string& name) const;                                         //!< Get variable slot (const)
        const VariableTable&        getVariableTable(void) const { return variableTable; }                                  //!< Return variable table
        void                        setValue(const std::string& name, RbObject* newVal);                                    //!< Set value
        void                        setVariable(const std::string& name, DAGNode* newVar);                                  //!< Set variable
        void                        swapReference(DAGNode* oldRef, DAGNode* newRef);                                        //!< Swap a reference variable

    protected:
        Frame*                      parentFrame;                                                                            //!< Pointer to enclosing frame
        VariableTable               variableTable;                                                                          //!< Variable table
};

#endif
