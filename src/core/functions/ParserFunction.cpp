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
ParserFunction::ParserFunction(InferenceFunction *f, const std::string &fn, const ArgumentRules &ar, RbLanguageObject *rv, bool t ) : RbFunction(), function( f ), functionName( fn), argRules( ar ), returnValue( rv ), throwsError( t ) {
    
    argsProcessed = false;
}

/** Copy constructor. */
ParserFunction::ParserFunction(const ParserFunction &x) : RbFunction(x), function( x.function->clone() ), functionName( x.functionName ), argRules( x.argRules ), throwsError( x.throwsError ), returnValue( x.returnValue ) {
    
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
 * This is the default implementation which calls executeFunction and wraps the return value 
 * (which is of type RbLanguageObject) into a ConstantNode.
 *
 * If you do not wish to wrap the return value into a constant node, then you need to overwrite this function.
 */
const RbLanguageObject& ParserFunction::execute(void) {
    
    
    std::vector<const RbObject*> newArgs;
    for (std::vector<Argument>::iterator i = args.begin(); i != args.end(); ++i) {
        newArgs.push_back( &i->getVariable().getValue() );
    }
    return execute( newArgs );
}


/* 
 * Execute the Function. 
 *
 * This is the default implementation which calls executeFunction and wraps the return value 
 * (which is of type RbLanguageObject) into a ConstantNode.
 *
 * If you do not wish to wrap the return value into a constant node, then you need to overwrite this function.
 */
const RbLanguageObject& ParserFunction::execute( const std::vector<const RbObject*> &args ) {
    
    // check each argument if it is a vector and hence the function needs repeated evaluation
    bool repeatedExecution = false;
    size_t size = 0;
    for (std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); ++i) {
        if ( (*i)->isTypeSpec( Container::getClassTypeSpec() ) ) {
            repeatedExecution = true;
            size = static_cast<const Container*>( *i )->size();
            break;
        }
    }
    
    RbLanguageObject* retVal;
    if ( repeatedExecution ) {
        RbVector<RbLanguageObject>* retValVector = new RbVector<RbLanguageObject>();
        for ( size_t j = 0; j < size; ++j) {
            std::vector<const RbObject*> newArgs;
            for (std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); ++i) {
                if ( (*i)->isTypeSpec( Container::getClassTypeSpec() ) ) {
                    newArgs.push_back( &static_cast<const Container*>( (*i) )->getElement(j) );
                }
                else {
                    newArgs.push_back( *i );
                    
                }
            }
            // call the execute function now for the single elements
            const RbLanguageObject& singleRetVal = execute(newArgs);
            retValVector->push_back( singleRetVal.clone() );
            // \TODO If the execute functions returns a pointer to the object and the caller owns the object, 
            // then we don't need to copy each time the object.
        }
        
        retVal = retValVector;
    }
    else {
        
        // get the value by executing the internal function
        retVal = executeFunction(args).clone();
    }
    
    return *retVal;
    
}


/* Execute the Function. This is the default implementation which is called by calls execute. This function returns a basic RbLanguageObject
 * that is wraped into a ConstantNode by the calling execute function.
 * If you write your own execute function, you do not need to overwrite this function, otherwise you should.
 */
const RbLanguageObject& ParserFunction::executeFunction( const std::vector<const RbObject*> &args) {
    
    // \TODO: We might want to throw an error!
    //    static RbNullObject nullReference;
    
    std::vector<RbValue<void*> > newArgs;
    for (std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); ++i) {
        RbValue<void*> arg;
        arg.value = (*i)->getValue(arg.lengths);
        newArgs.push_back( arg );
    }
    
    // add te return value
    RbLanguageObject* retVal = returnValue->clone();
    RbValue<void*> arg;
    arg.value = retVal->getValue(arg.lengths);
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


/* Get the type spec for this function */
const TypeSpec& ParserFunction::getTypeSpec(void) const {
    return getClassTypeSpec();
}

/** Get the return type of the function */
const TypeSpec& ParserFunction::getReturnType( void ) const {
    return returnValue->getTypeSpec();
}


/** Print value for user */
void ParserFunction::printValue(std::ostream& o) const {
    
    const ArgumentRules& argRules = getArgumentRules();
    
    o << getReturnType() << " function (";
    for (size_t i=0; i<argRules.size(); i++) {
        if (i != 0)
            o << ", ";
        argRules[i].printValue(o);
    }
    o << ")";
}





