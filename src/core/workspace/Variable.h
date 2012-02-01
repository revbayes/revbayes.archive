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
 * b := expf(a)
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

const std::string Variable_name = "Variable";

class Variable : public RbInternal {
    
public:
    Variable(const std::string &n, DAGNode* var);                                           //!< Constructor of filled variable
    Variable(DAGNode* var);                                                                 //!< Constructor of filled, unnamed variable
    Variable(const std::string &n);                                                         //!< Constructor with name only
    Variable(const Variable& x);                                                            //!< Copy value or reference correctly
    virtual                ~Variable(void);                                                 //!< Manage variable (DAGNode) destruction
    
    // Assignment operator
    Variable&                           operator=(const Variable& x);                       //!< Assignment
    
    // Regular functions
    Variable*                           clone(void) const;                                  //!< Clone variable
    const VectorString&                 getClass() const;                                   //!< Get class vector
    const TypeSpec&                     getTypeSpec(void) const;                            //!< Get language type of the object
    const std::string&                  getName(void) const;                                //!< Get name of variable
    const DAGNode*                      getDagNode(void) const;                             //!< Get the variable 
    DAGNode*                            getDagNode(void);                                   //!< Get the variable (non-const to return non-const node)
    const RbLanguageObject*             getValue(void) const;                               //!< Get the value of the variable
    RbLanguageObject*                   getValue(void);                                     //!< Get the value of the variable (non-const to return non-const value)
    void                                setName(const std::string &n);                      //!< Replacing the name of the variable
    void                                printValue(std::ostream& o) const;                  //!< Print value of variable
    void                                setDagNode(DAGNode* newVar);                        //!< Set a variable with a variable
    
private:
    // Help functions
    void                                replaceDagNode(DAGNode* newVariable );              //!< Replace the old DAG node with the new one and set the children and parent
    
    // Member variables
    RbPtr<DAGNode>                      node;                                               //!< Pointer to the variable (reference or not)
    std::string                         name;                                               //!< the name of the variable

    static const TypeSpec               typeSpec;
};

/* Global functions using the class */
std::ostream&           operator<<(std::ostream& o, const Variable& x);                             //!< Overloaded output operator

#endif
