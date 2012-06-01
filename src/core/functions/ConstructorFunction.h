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

class ConstructorFunction :  public RbFunction {

    public:
                                                ConstructorFunction(const RbPtr<MemberObject> &obj);                            //!< Object constructor
                                                ConstructorFunction(const ConstructorFunction& obj);                            //!< Copy constructor
    
        // overloaded operators
        ConstructorFunction&                    operator=(const ConstructorFunction& c);

        // Basic utility functions
        ConstructorFunction*                    clone(void) const;                                                              //!< Clone the object
        static const std::string&               getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object

        // Regular functions
        RlValue<RbLanguageObject>               execute(void);                                                                  //!< Execute the function. This is the function one has to overwrite for single return values.
        const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value

	protected:

        const ArgumentRules*                    argRules;                                                                       //!< Member rules converted to reference rules
        RbPtr<MemberObject>                     templateObject;                                                                 //!< The template object
        RbPtr<MemberObject>                     copyObject;
    
};

#endif

