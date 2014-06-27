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
Variable::Variable(const TypeSpec& ts) : 
    name(""),
    refCount( 0 ),
    value( NULL ), 
    valueTypeSpec( ts )
{
    
}

/** Constructor of filled variable. */
Variable::Variable(RevObject *v, const std::string &n) : 
name( n ), 
refCount( 0 ),
value( NULL ),
valueTypeSpec( RevObject::getClassTypeSpec() )
{
    
    setValue( v );
    
}


/** Constructor of filled variable. */
Variable::Variable(const Variable &v) : 
    name( v.name ), 
    refCount( 0 ),
    value( NULL ),
    valueTypeSpec( v.valueTypeSpec )
{
    
    if ( v.value != NULL )
    {
        setValue( v.value->clone() );
    }
        
    
}


Variable::~Variable( void )
{
    delete value;
}


Variable& Variable::operator=(const Variable &v)
{
    
    if ( this != &v )
    {
        
        name = v.name;
        valueTypeSpec = v.valueTypeSpec;
        
        delete value;
        value = NULL;
        
        if ( v.value != NULL )
        {
            setValue( v.value->clone() );
        }
        
    }
    
    return *this;
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
const RevObject& Variable::getValue(void) const {

    if (value == NULL) 
    {
        return RbNullObject::getInstance();
    }
    
    return *value;
}

/** Get the value of the variable */
RevObject& Variable::getValue(void) {
    
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
void Variable::setValue( RevObject *newValue ) {
    
    // change the old variable with the new variable in the parent and children
    replaceValue( newValue );
    
}


void Variable::setName(std::string const &n) {
    
    name = n;

}


/** Replace DAG node, only keep the children */
void Variable::replaceValue( RevObject *newValue ) {
    
    if (value != NULL) 
    {
        
        // I need to tell my children that I'm being exchanged
        value->replaceVariable( newValue );
        
        delete value;

    }
    
    value = newValue;
    
    if ( value != NULL )
        value->setName( name );
    
}


/** We set here the required value type spec. */
void Variable::setValueTypeSpec(const TypeSpec &ts) {
    
    valueTypeSpec = ts;
    
}


