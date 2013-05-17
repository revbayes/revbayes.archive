/**
 * @file
 * This file contains the implementation of Variable, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Implementation of Variable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "TypeSpec.h"
#include "Variable.h"

#include <cassert>
#include <string>
#include <sstream>

using namespace RevLanguage;

/** Constructor of filled variable. */
Variable::Variable(const TypeSpec& ts) : valueTypeSpec( ts ), value( NULL ) {
    
}

/** Constructor of filled variable. */
Variable::Variable(RbLanguageObject *v, const std::string &n) : valueTypeSpec( RbLanguageObject::getClassTypeSpec() ), name( n ), value( NULL ) {
    
    setValue( v );
    
}


/* Clone variable and variable */
Variable* Variable::clone( void ) const {
    
    return new Variable( *this );
}


/* Decrement the reference count. */
size_t Variable::decrementReferenceCount( void ) const {
    
    refCount--;
    
    return refCount;
}


const std::string& Variable::getName( void ) const {
    
    return name;
}



/* Get the reference count for this instance. */
size_t Variable::getReferenceCount(void) const {
    
    return refCount;
}


/* Get the value of the variable */
const RbLanguageObject& Variable::getValue(void) const {

    if (value == NULL) 
    {
        return RbNullObject::getInstance();
    }
    
    return *value;
}

/** Get the value of the variable */
RbLanguageObject& Variable::getValue(void) {
    
    if (value == NULL) 
    {
        return RbNullObject::getInstance();
    }
    
    return *value;
}


/** Get the required type specs for values stored inside this variable */
const TypeSpec& Variable::getValueTypeSpec(void) const {
    
    return valueTypeSpec;
}


/* Increment the reference count for this instance. */
void Variable::incrementReferenceCount( void ) const {
    
    refCount++;
    
}



/* Print value of the variable variable */
void Variable::printValue(std::ostream& o) const {
    
    if (value == NULL)
        o << "NULL";
    else
        value->printValue( o );
    
}


/** Set variable */
void Variable::setValue( RbLanguageObject *newValue ) {
    
    // change the old variable with the new variable in the parent and children
    replaceValue( newValue );
    
}


void Variable::setName(std::string const &n) {
    
    name = n;

}


/** Replace DAG node, only keep the children */
void Variable::replaceValue( RbLanguageObject *newValue ) {
    
    if (value != NULL) 
    {
        
        // I need to tell my children that I'm being exchanged
        value->replaceVariable( newValue );
        
        delete value;
        value = newValue;

    }
    
    value = newValue;
    
    if ( value != NULL )
        value->setName( name );
    
}


/** We set here the required value type spec. */
void Variable::setValueTypeSpec(const TypeSpec &ts) {
    
    valueTypeSpec = ts;
    
}


