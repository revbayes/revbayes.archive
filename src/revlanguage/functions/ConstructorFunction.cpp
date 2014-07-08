
/**
 * @file
 * This file contains the implementation of ConstructorFunction, which is used
 * for functions that construct member objects.
 *
 * @brief Implementation of ConstructorFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-01 14:55:05 +0200 (Fri, 01 Jun 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: ConstructorFunction.cpp 1603 2012-06-01 12:55:05Z hoehna $
 */

#include "ArgumentRule.h"
#include "ConstructorFunction.h"
#include "RevObject.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
ConstructorFunction::ConstructorFunction( RevObject *obj ) : Function(), templateObject(obj) {
    
    // Hack: we know that we will not own the argRules.
    argRules = &templateObject->getMemberRules();
}


/** Constructor */
ConstructorFunction::ConstructorFunction(const ConstructorFunction& obj) : Function(obj) {
    
    templateObject = obj.templateObject->clone();
    
    // Hack: we know that we will not own the argRules.
    argRules = &templateObject->getMemberRules();
}


ConstructorFunction& ConstructorFunction::operator=(const ConstructorFunction &c) {
    
    if (this != &c) {
        Function::operator=(c);
        
        templateObject = c.templateObject->clone();
        
        // Hack: we know that we will not own the argRules.
        argRules = &templateObject->getMemberRules();
    }
    
    return *this;
}


/** Clone the object */
ConstructorFunction* ConstructorFunction::clone(void) const {
    
    return new ConstructorFunction(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RevPtr<Variable> ConstructorFunction::execute( void ) {
    
    RevObject* copyObject = templateObject->clone();
    
    for ( size_t i = 0; i < args.size(); i++ ) {
        
        if ( args[i].isConstant() ) {
            copyObject->setConstMember( args[i].getLabel(), RevPtr<const Variable>( (Variable*) args[i].getVariable() ) );
        } else {
            copyObject->setMember( args[i].getLabel(), args[i].getReferenceVariable() );
        }
    }
    
    // now call the constructor for the internal object
    copyObject->constructInternalObject();
    
    return new Variable( copyObject );
}


/** Get argument rules */
const ArgumentRules& ConstructorFunction::getArgumentRules(void) const {
    
    return *argRules;
}


/** Get class name of object */
const std::string& ConstructorFunction::getClassName(void) { 
    
    static std::string rbClassName = "ConstructorFunction";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ConstructorFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ConstructorFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& ConstructorFunction::getReturnType(void) const {
    
    return templateObject->getTypeSpec();
}



