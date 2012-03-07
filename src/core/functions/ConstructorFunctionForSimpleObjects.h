/**
 * @file
 * This file contains the declaration of ConstructorFunctionForSimpleObjects, which is used
 * for functions that construct simple objects.
 *
 * @brief Declaration of ConstructorFunctionForSimpleObjects
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef ConstructorFunctionForSimpleObjects_H
#define ConstructorFunctionForSimpleObjects_H

#include "RbFunction.h"
#include "MemberObject.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class DAGNode;

class ConstructorFunctionForSimpleObjects :  public RbFunction {
    
public:
    ConstructorFunctionForSimpleObjects(RbLanguageObject* obj);                                                             //!< Object constructor
    ConstructorFunctionForSimpleObjects(const ConstructorFunctionForSimpleObjects& obj);                                    //!< Copy constructor
    virtual ~ConstructorFunctionForSimpleObjects(void);                                                                     //!< Destructor
    
    // Basic utility functions
    ConstructorFunctionForSimpleObjects*    clone(void) const;                                                              //!< Clone the object
    static const std::string&               getClassName(void);                                                             //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
    const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
    const std::string&                      getTemplateObjectType(void) const { return templateObject->getType(); }         //!< Get the type of the template object
    
protected:
    const RbLanguageObject&                 executeFunction(void);                                                          //!< Execute function
    void                                    setArgumentVariable(const std::string& name, const Variable* var);

    const ArgumentRules*                    argRules;                                                                       //!< Member rules converted to reference rules
    RbLanguageObject*                       templateObject;                                                                 //!< The template object
    RbLanguageObject*                       copyObject;
    
};

#endif

