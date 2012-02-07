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

const std::string ConstructorFunction_name = "constructor function";

class ConstructorFunction :  public RbFunction {

    public:
                                                ConstructorFunction(MemberObject* obj);                                         //!< Object constructor
    virtual                                    ~ConstructorFunction();

        // Basic utility functions
        ConstructorFunction*                    clone(void) const;                                                              //!< Clone the object
        const VectorString&                     getClass(void) const;                                                           //!< Get class vector
        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object

        // Regular functions
        const ArgumentRules*                    getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
        const std::string&                      getTemplateObjectType(void) const { return templateObject->getType(); }         //!< Get the type of the template object

	protected:
        RbLanguageObject*                       executeFunction(void);                                                          //!< Execute function
        const ArgumentRules*                    argRules;                                                                       //!< Member rules converted to reference rules
        MemberObject*                           templateObject;                                                                 //!< The template object
    
    private:
        static const TypeSpec                   typeSpec;
};

#endif

