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

#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "TypeSpec.h"

#include <string>

namespace RevLanguage {

    class Variable {
    public:
        Variable(const TypeSpec& ts); //!< Constructor of filled variable
        Variable(RbLanguageObject *val, const std::string &n = ""); //!< Constructor of filled, unnamed variable
        Variable(const Variable &v); //!< Copy constructor
        virtual ~Variable(void);

        Variable& operator=(const Variable &v); //!< Assignment operator

        // Regular functions
        Variable* clone(void) const; //!< Clone variable
        const std::string& getName(void) const; //!< Get the name of the variable
        const RbLanguageObject& getValue(void) const; //!< Get the value of the variable
        RbLanguageObject& getValue(void); //!< Get the value of the variable (non-const to return non-const value)
        const TypeSpec& getValueTypeSpec(void) const; //!< Get the required value type spec
        void printValue(std::ostream& o) const; //!< Print value of variable
        void setValue(RbLanguageObject *newVar); //!< Set a variable with a value
        void setName(const std::string &n); //!< Set the name of this variable
        void setValueTypeSpec(const TypeSpec& ts); //!< set the required value type spec

        // smart pointer functions
        size_t decrementReferenceCount(void) const; //!< Decrement the reference count for reference counting in smart pointers
        size_t getReferenceCount(void) const; //!< Get the reference count for reference counting in smart pointers
        void incrementReferenceCount(void) const; //!< Increment the reference count for reference counting in smart pointers

    private:
        // Help functions
        void replaceValue(RbLanguageObject *newVariable); //!< Replace the old DAG node with the new one and set the children and parent

        // Member variables
        std::string name;
        mutable size_t refCount;
        RbLanguageObject* value; //!< Pointer to the variable (reference or not)
        TypeSpec valueTypeSpec;
    };

}


#endif
