/**
 * @file
 * This file contains the implementation of some functions in
 * ParserFunction, the interface fnd abstract base class for RevBayes
 * functions.
 *
 * @brief Partial implementation of ParserFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: ParserFunction.cpp 1378 2012-04-17 00:27:08Z hoehna $
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "ParserFunction.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "Workspace.h"
#include "InferenceFunction.h"

#include <sstream>


/** Basic constructor. */
ParserFunction::ParserFunction(InferenceFunction *f, const std::string &fn, const ArgumentRules &ar, RbLanguageObject *rv, bool t ) : RbFunction(), function( f ), functionName( fn), argRules( ar ), returnValue( rv ), throwsError( t ), typeSpec( getClassName() + " (" + fn + ")", new TypeSpec( RbFunction::getClassTypeSpec() )) {
    
    argsProcessed = false;
}

/** Copy constructor. */
ParserFunction::ParserFunction(const ParserFunction &x) : RbFunction(x), function( x.function->clone() ), functionName( x.functionName ), argRules( x.argRules ), throwsError( x.throwsError ), returnValue( x.returnValue ), typeSpec( x.typeSpec ) {
    
}


/** Destructor. We need to free the arguments here. */
ParserFunction::~ParserFunction(void) {
    delete function;
}


/* Clone the function */
ParserFunction* ParserFunction::clone(void) const {
    return new ParserFunction(*this);
}


/** Debug info about object */
std::string ParserFunction::debugInfo(void) const {
    
    std::ostringstream o;
    o << getTypeSpec() << ": " << std::endl;
    
    if (argsProcessed)
        o << "Arguments processed; there are " << args.size() << " values." << std::endl;
    else
        o << "Arguments not processed; there are " << args.size() << " slots in the frame." << std::endl;
    
    for ( size_t i = 0;  i < args.size(); i++ ) {
        o << " args[" << i << "] = " << args[i].getVariable().getValue() << std::endl;
    }
    
    return o.str();
}

/* 
 * Execute the Function. 
 *
 */
const RbLanguageObject& ParserFunction::executeFunction( const std::vector<const RbObject*> &args ) {
    
    // converting the arguments into atomic data types
    std::vector<RbValue<void*> > newArgs;
    for (std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); ++i) {
        RbValue<void*> arg;
        arg.value = (*i)->getLeanValue(arg.lengths);
        newArgs.push_back( arg );
    }
    
    // add te return value
    RbLanguageObject* retVal = returnValue->clone();
    RbValue<void*> arg;
    arg.value = retVal->getLeanValue(arg.lengths);
    newArgs.push_back( arg );
    
    // set the current parameter values of the function
    function->setArguments( newArgs );
    
    function->execute();
    
    return *retVal;
}


/** Get the argument rules for the function */
const ArgumentRules& ParserFunction::getArgumentRules( void ) const {
    return argRules;
}


/** Get class name of object */
const std::string& ParserFunction::getClassName(void) { 
    
    static std::string rbClassName = "Parser Function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ParserFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbInternal::getClassTypeSpec() ) );
    
	return rbClass; 
}


InferenceFunction* ParserFunction::getLeanFunction( void ) const {
    return function;
}


/* Get the type spec for this function */
const TypeSpec& ParserFunction::getTypeSpec(void) const {
    return typeSpec;
}

/** Get the return type of the function */
const TypeSpec& ParserFunction::getReturnType( void ) const {
    return returnValue->getTypeSpec();
}


/** Print value for user */
void ParserFunction::printValue(std::ostream& o) const {
    
    const ArgumentRules& argRules = getArgumentRules();
    
    o << getReturnType() << " " << functionName << " (";
    for (size_t i=0; i<argRules.size(); i++) {
        if (i != 0)
            o << ", ";
        argRules[i].printValue(o);
    }
    o << ")";
}





