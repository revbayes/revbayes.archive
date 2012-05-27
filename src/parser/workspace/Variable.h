/**
 * @file
 * This file contains the declaration of Variable, which is
 * used to hold pointers to DAG nodes.
 *
 * A Variable corresponds to a variable instance in the RevLanguage.
 * For instance "a <- 1" will create a variable holding a pointer to 
 * a constant DAG node with value 1. The variables themselves live in 
 * variable slots which have a name to the variable. We could not 
 * have used DAG node pointers directly in the variable slots because
 * of reassignment. If we declared the following sequences of commands:
 * b <- 2
 * c := ln(b)
 * a <- 5
 * b := exp(a)
 * First 'b' is a constant DAG node and later replaced by a deterministic
 * node with the exponential function. Only if 'c' holds a pointer to a 
 * pointer to a DAG node (i.e. a Variable which holds a pointer to a
 * DAG node) can 'c' get the current value of 'b' even after reassignment.
 *
 * @brief Declaration of Variable
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

#ifndef Variable_H 
#define Variable_H

#include "DAGNode.h"
#include "RbInternal.h"
#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "TypeSpec.h"

#include <string>


class Variable : public RbInternal {
    
public:
    Variable(const TypeSpec& ts);                                           //!< Constructor of filled variable
    Variable(const RbPtr<DAGNode> &var);                                                                 //!< Constructor of filled, unnamed variable
    
    // Regular functions
    Variable*                               clone(void) const;                                  //!< Clone variable
    static const std::string&               getClassName(void);                                 //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                             //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                            //!< Get language type of the object
    RbPtr<const DAGNode>                    getDagNode(void) const;                             //!< Get the variable 
    const RbPtr<DAGNode>&                   getDagNode(void);                                   //!< Get the variable (non-const to return non-const node)
    const std::string&                      getName(void) const;                                //!< Get the name of the variable
    RlValue<const RbLanguageObject>         getValue(void) const;                               //!< Get the value of the variable
    const RlValue<RbLanguageObject>&        getValue(void);                                     //!< Get the value of the variable (non-const to return non-const value)
    const TypeSpec&                         getValueTypeSpec(void) const;                       //!< Get the required value type spec
    void                                    printValue(std::ostream& o) const;                  //!< Print value of variable
    void                                    setDagNode(const RbPtr<DAGNode> &newVar);           //!< Set a variable with a variable
    void                                    setValueTypeSpec(const TypeSpec& ts);               //!< set the required value type spec
    
private:
    // Help functions
    void                                    replaceDagNode(const RbPtr<DAGNode> &newVariable);  //!< Replace the old DAG node with the new one and set the children and parent
    
    // Member variables
    RbPtr<DAGNode>                          node;                                               //!< Pointer to the variable (reference or not)
    TypeSpec                                valueTypeSpec;
    
};


#endif
