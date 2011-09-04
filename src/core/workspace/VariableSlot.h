/**
 * @file
 * This file contains the declaration of VariableSlot, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Declaration of VariableSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#ifndef VariableSlot_H 
#define VariableSlot_H

#include "DAGNode.h"
#include "RbInternal.h"
#include "TypeSpec.h"

#include <string>

class Variable;
class DAGNode;
class Frame;
class RbObject;

const std::string VariableSlot_name = "Variable Slot";

class VariableSlot : public RbInternal {
    
public:
    VariableSlot(const std::string &label, const TypeSpec& typeSp, Variable *var);                  //!< Constructor of filled slot
    VariableSlot(const std::string &label, const TypeSpec& typeSp);                                 //!< Constructor of empty slot
    VariableSlot(const std::string &label, Variable* var);                                          //!< Constructor with filled slot
    VariableSlot(const VariableSlot& x);                                                            //!< Copy value or reference correctly
    virtual                ~VariableSlot(void);                                                     //!< Manage variable (DAGNode) destruction
    
    // Assignment operator
    VariableSlot&           operator=(const VariableSlot& x);                                       //!< Assignment
    
    // Regular functions
    VariableSlot*           clone(void) const;                                                      //!< Clone slot
    const VectorString&     getClass() const;                                                       //!< Get class vector
    const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
    const RbLanguageObject* getValue(void) const;                                                   //!< Get the value of the variable (ptr, not copy)
    const DAGNode*          getDagNode(void) const;                                                 //!< Get the variable pointer
    DAGNode*                getDagNodePtr(void) const;                                              //!< Get a reference to the variable
    const std::string&      getLabel(void) const { return label; }                                  //!< Get the label of the slot
    const std::string&      getSlotType(void) const { return varTypeSpec.getType(); }               //!< Type of slot
    const TypeSpec&         getSlotTypeSpec(void) const { return varTypeSpec; }                     //!< Type specification for slot
    Variable*               getVariable(void) const { return variable; }                            //!< Get the argument
    virtual bool            isValidVariable(DAGNode* newVariable ) const;                           //!< Is newVariable valid for the slot?
    void                    printValue(std::ostream& o) const;                                      //!< Print value of slot
    void                    setVariable(Variable *var);                                             //!< Set a slot with a variable
    
private:
    
    // Member variables
    TypeSpec                varTypeSpec;                                                            //!< The type specification for the slot
    Variable               *variable;                                                               //!< the argument living in the slot 
    std::string             label;                                                                  //!< the label for this slot. The label should correspond to the name uner which this slot is stored in variable table. However, the label does not have to correspond to the variable name, e.g. a argument could have the label mean but the name of the variable is mu.

    static const TypeSpec   typeSpec;
};

/* Global functions using the class */
std::ostream&           operator<<(std::ostream& o, const VariableSlot& x);                         //!< Overloaded output operator

#endif
