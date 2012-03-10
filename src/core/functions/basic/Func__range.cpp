/**
 * @file
 * This file contains the implementation of Func__range, which is
 * the function used to create models.
 *
 * @brief Implementation of Func__range
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "Func__range.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"



/** default constructor */
Func__range::Func__range( void ) : RbFunction( ) {
    first   = NULL;
    last    = NULL;
}

/** default constructor */
Func__range::Func__range( const Func__range& f ) : RbFunction( f ) {
    
    first  = f.first;
    if ( first != NULL ) {
        first->incrementReferenceCount();
    }
    last = f.last;
    if ( last != NULL ) {
        last->incrementReferenceCount();
    }
}

/** destructor */
Func__range::~Func__range( void ) {
    
    if ( first != NULL && first->decrementReferenceCount() == 0 ) {
        delete first;
    }
    
    if ( last != NULL && last->decrementReferenceCount() == 0 ) {
        delete last;
    }
}


/** Overloaded assignment operator */
Func__range& Func__range::operator=( Func__range const &f ) {
    
    if ( this != &f ) {
        // call the base class assignment operator
        RbFunction::operator=( f );
        
        // free the memory first
        if ( first != NULL && first->decrementReferenceCount() == 0 ) {
            delete first;
        }
        
        if ( last != NULL && last->decrementReferenceCount() == 0 ) {
            delete last;
        }
        
        // reassign the member variables
        first  = f.first;
        if ( first != NULL ) {
            first->incrementReferenceCount();
        }
        
        last = f.last;
        if ( last != NULL ) {
            last->incrementReferenceCount();
        }
    }
    
    return *this;
}


/** Clone object */
Func__range* Func__range::clone( void ) const {

    return new Func__range( *this );
}


/** Execute function */
const RbLanguageObject& Func__range::executeFunction( void ) {

    int f = static_cast<const Integer&>( first->getValue() ).getValue();
    int l = static_cast<const Integer&>( last->getValue() ).getValue();

    range.clear();
    if (f < l) {
        for ( int i = f; i <= l; i++ )
            range.push_back( new Integer(i) );
    }
    else {
        for ( int i = f; i >= l; i-- )
            range.push_back( new Integer(i) );
    }
    
    return range;
}


/** Get argument rules */
const ArgumentRules& Func__range::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back( new ValueRule( "first", Integer::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "last",  Integer::getClassTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func__range::getClassName(void) { 
    
    static std::string rbClassName = "Range function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func__range::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func__range::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func__range::getReturnType( void ) const {

    static TypeSpec returnTypeSpec = RbVector<Integer>::getClassTypeSpec();
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func__range::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "first" ) {
        // free the memory of the old variable
        // Variable uses reference counting so we need to free the memory manually
        if ( first != NULL && first->decrementReferenceCount() == 0 ) {
            delete first;
        }
        
        // set my variable to the new variable
        first = var;
        
        // increment the reference count for the variable
        if (first != NULL ) {
            first->incrementReferenceCount();
        }
    }
    else if ( name == "last" ) {
        // free the memory of the old variable
        // Variable uses reference counting so we need to free the memory manually
        if ( last != NULL && last->decrementReferenceCount() == 0 ) {
            delete last;
        }
        
        // set my variable to the new variable
        last = var;
        
        // increment the reference count for the variable
        if (last != NULL ) {
            last->incrementReferenceCount();
        }
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

