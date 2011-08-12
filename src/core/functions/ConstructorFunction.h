/**
 * @file
 * This file contains the declaration of ConstructorFunction, which is used
 * for functions that construct member objects.
 *
 * @brief Declaration of ConstructorFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef ConstructorFunction_H
#define ConstructorFunction_H

#include "RbFunction.h"
#include "MemberObject.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

class ConstructorFunction :  public RbFunction {

    public:
                                    ConstructorFunction(MemberObject* obj);                                         //!< Object constructor

        // Basic utility functions
        ConstructorFunction*        clone(void) const;                                                              //!< Clone the object
    	const VectorString&         getClass(void) const;                                                           //!< Get class vector

        // Regular functions
        bool                        addAsChildOfArguments(void) { return false; }                                   //!< We do not wish that a constructor functions is added as a child of the arguments.
        RbLanguageObject*           execute(void);                                                                  //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                                      //!< Get type of return value
        const std::string&          getTemplateObjectType(void) const { return templateObject->getType(); }         //!< Get the type of the template object

	protected:
        ArgumentRules               argRules;                                                                       //!< Member rules converted to reference rules
        MemberObject*               templateObject;                                                                 //!< The template object
};

#endif

