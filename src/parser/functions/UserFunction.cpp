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
UserFunction::UserFunction( const ArgumentRules&        argRules,
                            const TypeSpec&             retType,
                            std::list<SyntaxElement*>*  stmts,
                            Environment*                defineEnv)
: RbFunction(), argumentRules(argRules), returnType(retType), code(stmts), defineEnvironment(defineEnv) {
    defineEnvironment->retain();
}


/** Copy constructor */
UserFunction::UserFunction(const UserFunction &x) : RbFunction(x), argumentRules(x.argumentRules), returnType(x.returnType) {

    // clone the environment
    defineEnvironment   = x.defineEnvironment->clone();
    defineEnvironment->retain();
    
    // create a new list for the code
    code                = new std::list<SyntaxElement*>();
    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++) {
//        SyntaxElement *element = (*i)->clone();
        SyntaxElement *element = *i;
        code->push_back(element);
        element->retain();
    }
}


/** Destructor */
UserFunction::~UserFunction() {

    for (std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++) {
        (*i)->release();
        if ((*i)-isUnreferenced()) {
            delete (*i);
        }
    }
    
    delete code;

    // defineEnvironment->destroyEnclosure();   //TODO: or something like that
}


/** Brief info on the function */
std::string UserFunction::briefInfo(void) const {

    std::ostringstream o;
    o << "UserFunction: ";
    printValue(o);

    return o.str();
}


/** Clone function */
UserFunction* UserFunction::clone(void) const {

    return new UserFunction(*this);
}


/** Execute function */
RbLanguageObject* UserFunction::execute( void ) {

    // Clear signals
    Signals::getSignals().clearFlags();

    // Set initial return value
    Variable* retValue = NULL;
    
    // retain the arg because they will be destroyed otherwise when we destroy this function environment
    args.retain();

    // Create new variable frame
    Environment* functionEnvironment = new Environment( &args );

    // Execute code
    for ( std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++ ) {
    
        if ( retValue != NULL && retValue->isUnreferenced() )
            delete retValue;

        retValue = (*i)->getContentAsVariable( functionEnvironment );

        if ( Signals::getSignals().isSet( Signals::RETURN ) )
            break;
    }

    // Delete the variable frame of the function
    delete functionEnvironment;

    // Return the return value
    return retValue->getDagNodePtr()->getValuePtr();
}


/** Get argument rules */
const ArgumentRules& UserFunction::getArgumentRules() const {

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& UserFunction::getClass() const {

    static VectorString rbClass = VectorString(UserFunction_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec UserFunction::getReturnType() const {

    return returnType;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& UserFunction::getTypeSpec(void) const {
    return typeSpec;
}


/** Complete info about object */
std::string UserFunction::richInfo() const {

    std::ostringstream o;
    o << "User-defined function:" << std::endl;
    o << "formals     = " << argumentRules.size() << " formal arguments" << std::endl;
    o << "return type = " << returnType << std::endl;
    o << "code        = " << code->size() << " lines of code" << std::endl;
    o << "definition environment:" << std::endl;
    defineEnvironment->printValue(o);

    return o.str();
}

