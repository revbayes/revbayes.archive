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

#include "ConstArgumentRule.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Signals.h"
#include "SyntaxElement.h"
#include "TypeSpec.h"
#include "UserFunction.h"


#include <list>
#include <sstream>



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
        // call the base class assignment operator
        RbFunction::operator=(f);
        
        delete defineEnvironment;
        // clone the environment
        defineEnvironment   = f.defineEnvironment->clone();
        
        
        delete argumentRules;
        argumentRules = f.argumentRules->clone();
        
        for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++) {
            SyntaxElement* theSyntaxElement = *it;
            delete theSyntaxElement;
        }
        code->clear();
        
        // create a new list for the code
        for (std::list<SyntaxElement*>::const_iterator i=f.code->begin(); i!=f.code->end(); i++) {
            SyntaxElement* element = (*i)->clone();
            code->push_back(element);
        }
    }
    
    return *this;
}


/** Destructor */
UserFunction::~UserFunction() {

    // defineEnvironment->destroyEnclosure();   // \TODO: or something like that

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
RbPtr<RbLanguageObject> UserFunction::executeFunction( void ) {

    // Clear signals
    Signals::getSignals().clearFlags();

    // Set initial return value
    RbPtr<Variable> retVar = NULL;

    // Create new variable frame
    Environment functionEnvironment = Environment( NULL );
    // \TODO: Check that the arguments can still be accessed
//    Environment functionEnvironment = Environment( args );

    // Execute code
    for ( std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++ ) {

        SyntaxElement* theSyntaxElement = *i;
        retVar = theSyntaxElement->evaluateContent( *defineEnvironment );

        if ( Signals::getSignals().isSet( Signals::RETURN ) )
            break;
    }

    
    // Return the return value
    return RbPtr<RbLanguageObject>( retVar->getValue().clone() );
}


/** Get argument rules */
const ArgumentRules& UserFunction::getArgumentRules() const {

    return *argumentRules;
}


/** Get class name of object */
const std::string& UserFunction::getClassName(void) { 
    
    static std::string rbClassName = "User function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& UserFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& UserFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& UserFunction::getReturnType() const {

    return returnType;
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


/** We catch here the setting of the argument variables to store our parameters. */
void UserFunction::setArgumentVariable(std::string const &name, const Variable* var) {
    
    // We actually just catch the call to the base class which would complain that the argument was not expected.
    // User functions handle their arguments in some unknown different way.
    
    if ( defineEnvironment->existsVariable( name ) ) {
        (*defineEnvironment)[name].setVariable( var->clone() );
    }
    else {
        defineEnvironment->addVariable(name, var->clone() );
    }
}

