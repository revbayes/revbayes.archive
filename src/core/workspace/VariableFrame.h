/**
 * @file
 * This file contains the declaration of VariableFrame, which is
 * used to hold information about an evaluation or execution
 * frame. Each frame has a pointer to the enclosing (parent)
 * frame. A frame and its parents constitute an evaluation
 * environment. The base environment is the global workspace.
 * It is a special type of frame, which is described in the
 * class Workspace, derived from VariableVariableFrame.
 *
 * A frame essentially consists of a variable table. The workspace
 * also contains a function table, and a class (type) table.
 *
 * @brief Declaration of VariableFrame
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

#ifndef VariableFrame_H 
#define VariableFrame_H

#include "Frame.h"
#include "VariableSlot.h"

#include <map>
#include <string>

class DAGNode;
class VectorInteger;
class RbObject;

typedef std::map<std::string, VariableSlot*> VariableTable;         //!< Typedef for convenience

/**
 * @brief Execution or evaluation variable frame
 *
 * VariableFrame is used to hold information about an evaluation
 * or execution frame. Each frame has a pointer to the enclosing
 * (parent) frame. A frame and its parents together constitute
 * an evaluation environment. The base environment is the global
 * workspace. It is a special type of frame, which is described
 * in the class Workspace, derived from VariableVariableFrame. The global
 * workspace contains the user workspace, which is a workspace
 * holding user-defined types and functions. The user workspace,
 * in turn, contains the nested regular frames, which are created
 * during execution or evaluation of code.
 *
 * A frame essentially consists of a variable table or vector. The
 * workspace also contains a function table, and a class (type) table.
 *
 * A special simplified type of frame is ArgumentVariableFrame, which is used
 * in passing arguments to functions. It contains a vector rather
 * than a table of variables, and its interface is much simpler.
 *
 */
class VariableFrame : public Frame {

    public:
        friend class                SyntaxForCondition;                                                                     //!< Give for condition possibility to modify reference flag
        friend class                SyntaxVariable;                                                                         //!< Give parser direct access to variable slots

                                    VariableFrame(void);                                                                    //!< Constructor of variable frame with NULL parent
                                    VariableFrame(Frame* parentFr);                                                         //!< Constructor of variable frame with parent
                                    VariableFrame(const VariableFrame& x);                                                  //!< Copy constructor: set frame of variable slots
        virtual                    ~VariableFrame(void);                                                                    //!< Destructor to deal with slot pointers in variable table

        // Operators
        VariableFrame&              operator=(const VariableFrame& x);                                                      //!< Assignment operator
        VariableSlot&               operator[](const std::string& name);                                                    //!< Get named variable slot reference
        const VariableSlot&         operator[](const std::string& name) const;                                              //!< Get named variable slot const reference

        // Basic utility functions
        virtual VariableFrame*      clone(void) const { return new VariableFrame(*this); }                                  //!< Clone variable frame
        virtual VariableFrame*      cloneEnvironment(void) const;                                                           //!< Clone environment
        void                        printValue(std::ostream& o) const;                                                      //!< Print table for user
        virtual std::string         richInfo(void) const;                                                                   //!< Complete info to string

        // Regular functions
        virtual void                addVariable(const std::string& name, const TypeSpec& typeSp, DAGNode* variable);        //!< Add variable
        virtual void                addVariableSlot(const std::string& name, const TypeSpec& typeSp);                       //!< Add empty slot
        virtual void                eraseVariable(const std::string& name);                                                 //!< Erase a variable
        bool                        existsVariable(const std::string& name) const;                                          //!< Does variable exist?
        const std::string&          getSlotName(const VariableSlot* slot) const;                                            //!< Get name of a slot

    protected:
        VariableTable               variableTable;                                                                          //!< Variable table
};

#endif
