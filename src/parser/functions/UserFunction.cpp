/**
 * @file
 * This file contains the implementation of UserFunction, which is
 * used to hold user-defined functions.
 *
 * @brief Implementation of UserFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-21, version 1.0
 *
 * $Id$
 */

#include "ValueRule.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Signals.h"
#include "SyntaxElement.h"
#include "TypeSpec.h"
#include "UserFunction.h"
#include "VectorString.h"

#include <list>
#include <sstream>


// Definition of the static type spec member
const TypeSpec UserFunction::typeSpec(UserFunction_name);

/** Basic constructor */
UserFunction::UserFunction( const ArgumentRules*  argRules,
                            const TypeSpec&             retType,
                            std::list<SyntaxElement*>*  stmts,
                            Environment*                defineEnv)
: RbFunction(), argumentRules(argRules), returnType(retType), code(stmts), defineEnvironment(defineEnv) {
    
}


/** Copy constructor */
UserFunction::UserFunction(const UserFunction &x) : RbFunction(x), argumentRules( x.argumentRules->clone() ), returnType( x.returnType ), code( NULL ), defineEnvironment( NULL ) {

    // clone the environment
    defineEnvironment   = x.defineEnvironment->clone();
    
    // create a new list for the code
    code = new std::list<SyntaxElement*>();
    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++) {
        SyntaxElement* element = (*i)->clone();
        code->push_back(element);
    }
}


UserFunction& UserFunction::operator=(const UserFunction &f) {
    
    if ( this != &f ) {
        delete defineEnvironment;
        // clone the environment
        defineEnvironment   = f.defineEnvironment->clone();
        
        
        delete argumentRules;
        argumentRules = f.argumentRules->clone();
        
        for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++) {
            SyntaxElement* theSyntaxElement = *it;
            delete theSyntaxElement;
        }
        
        delete code;
        // create a new list for the code
        code = new std::list<SyntaxElement*>();
        for (std::list<SyntaxElement*>::const_iterator i=f.code->begin(); i!=f.code->end(); i++) {
            //        SyntaxElement *element = (*i)->clone();
            SyntaxElement* element = (*i)->clone();
            code->push_back(element);
        }
    }
    
    return *this;
}


/** Destructor */
UserFunction::~UserFunction() {

    // defineEnvironment->destroyEnclosure();   //TODO: or something like that

    delete defineEnvironment;
    delete argumentRules;
    
    for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++) {
        SyntaxElement* theSyntaxElement = *it;
        delete theSyntaxElement;
    }
    
    delete code;
}


/** Clone function */
UserFunction* UserFunction::clone(void) const {

    return new UserFunction(*this);
}


/** Execute function */
const RbLanguageObject& UserFunction::executeFunction( void ) {

    // Clear signals
    Signals::getSignals().clearFlags();

    // Set initial return value
    Variable* retValue = NULL;

    // Create new variable frame
    Environment functionEnvironment = Environment( args );

    // Execute code
    for ( std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++ ) {

        retValue = (*i)->evaluateContent( functionEnvironment );

        if ( Signals::getSignals().isSet( Signals::RETURN ) )
            break;
    }

    // Return the return value
    return retValue->getDagNode()->getValue();
}


/** Get argument rules */
const ArgumentRules& UserFunction::getArgumentRules() const {

    return *argumentRules;
}


/** Get class vector describing type of object */
const VectorString& UserFunction::getClass() const {

    static VectorString rbClass = VectorString(UserFunction_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& UserFunction::getReturnType() const {

    return returnType;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& UserFunction::getTypeSpec(void) const {
    return typeSpec;
}


/** Complete info about object */
void UserFunction::printValue(std::ostream &o) const {

    o << "User-defined function:" << std::endl;
    o << "formals     = " << argumentRules->size() << " formal arguments" << std::endl;
    o << "return type = " << returnType << std::endl;
    o << "code        = " << code->size() << " lines of code" << std::endl;
    o << "definition environment:" << std::endl;
    defineEnvironment->printValue(o);

}

