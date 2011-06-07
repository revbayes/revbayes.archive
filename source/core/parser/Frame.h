/**
 * @file
 * This file contains the declaration of Frame, which is
 * the abstract base class for classes that are used to hold
 * information about an evaluation or execution frame.
 * Each frame has a pointer to the enclosing (parent) frame.
 * A frame and its parents constitute an evaluation environment.
 * The base environment is the global workspace. It is a
 * special type of frame, which is described in the class
 * Workspace, derived from Frame through VariableFrame.
 *
 * A frame essentially consists of a table (map) or vector of
 * variables, which may or may not be labeled. The workspace
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

#include <map>
#include <string>

class DAGNode;
class RbObject;
class VariableSlot;


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
        // Operators
        virtual VariableSlot&           operator[](const std::string& name) = 0;                                                //!< Get named variable slot reference
        virtual const VariableSlot&     operator[](const std::string& name) const = 0;                                          //!< Get named variable slot const reference

        // Basic utility functions
        virtual Frame*                  clone(void) const = 0;                                                                  //!< Clone frame
        virtual Frame*                  cloneEnvironment(void) const = 0;                                                       //!< Clone environment
        virtual void                    printValue(std::ostream& o) const = 0;                                                  //!< Print table for user
        virtual std::string             richInfo(void) const = 0;                                                               //!< Complete info to string

        // Frame functions you have to override
        virtual bool                    existsVariable(const std::string& name) const = 0;                                      //!< Does variable exist?
        virtual const std::string&      getSlotName(const VariableSlot* slot) const = 0;                                        //!< Get name of a slot

        // Frame functions you do not have to override
        Frame*                          getParentFrame(void) { return parentFrame; }                                            //!< Get parent frame ptr
        DAGNode*                        getReference(const std::string& name) const;                                            //!< Convenient alternative for [name]->getValue()
        const RbObject*                 getValue(const std::string& name) const;                                                //!< Convenient alternative for [name]->getValue()
        const DAGNode*                  getVariable(const std::string& name) const;                                             //!< Convenient alternative for [name]->getValue()

    protected:
                                        Frame(void);                                                                            //!< Constructor of frame with NULL parent
                                        Frame(Frame* parentFr);                                                                 //!< Constructor of frame with parent

        Frame*                          parentFrame;                                                                            //!< Pointer to enclosing frame
};

#endif
