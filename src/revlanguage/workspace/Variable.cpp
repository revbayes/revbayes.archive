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
#include "RbUtil.h"
#include "RbOptions.h"
#include "RevNullObject.h"
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
    revObject( NULL ),
    revObjectTypeSpec( ts )
{
    
}

/** Constructor of filled variable. */
Variable::Variable(RevObject *v, const std::string &n) : 
    name( n ),
    refCount( 0 ),
    revObject( NULL ),
    revObjectTypeSpec( RevObject::getClassTypeSpec() )
{
    
    setRevObject( v );
    
}


/** Constructor of filled variable. */
Variable::Variable(const Variable &v) : 
    name( v.name ), 
    refCount( 0 ),
    revObject( NULL ),
    revObjectTypeSpec( v.revObjectTypeSpec )
{
    
    if ( v.revObject != NULL )
    {
        setRevObject( v.revObject->clone() );
    }
        
    
}


Variable::~Variable( void )
{
    delete revObject;
}


Variable& Variable::operator=(const Variable &v)
{
    
    if ( this != &v )
    {
        
        name = v.name;
        revObjectTypeSpec = v.revObjectTypeSpec;
        
        delete revObject;
        revObject = NULL;
        
        if ( v.revObject != NULL )
        {
            setRevObject( v.revObject->clone() );
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
const RevObject& Variable::getRevObject(void) const {

    if (revObject == NULL)
    {
        return RevNullObject::getInstance();
    }
    
    return *revObject;
}

/** Get the value of the variable */
RevObject& Variable::getRevObject(void) {
    
    if (revObject == NULL)
    {
        return RevNullObject::getInstance();
    }
    
    return *revObject;
}


/** Get the required type specs for values stored inside this variable */
const TypeSpec& Variable::getRevObjectTypeSpec(void) const {
    
    return revObjectTypeSpec;
}


/* Increment the reference count for this instance. */
void Variable::incrementReferenceCount( void ) const {
    
    refCount++;
    
}



/* Print value of the variable variable */
void Variable::printValue(std::ostream& o) const {
    
    if (revObject == NULL)
        o << "NULL";
    else
        revObject->printValue( o );
    
}


/** Set variable */
void Variable::setRevObject( RevObject *newValue ) {
    
    // change the old variable with the new variable in the parent and children
    replaceRevObject( newValue );
    
}


void Variable::setName(std::string const &n) {
    
    name = n;

}


/** Replace DAG node, only keep the children */
void Variable::replaceRevObject( RevObject *newObj ) {
    
    if (revObject != NULL)
    {
        
        // I need to tell my children that I'm being exchanged
        revObject->replaceVariable( newObj );
        
        delete revObject;

    }
    
    revObject = newObj;
    
    if ( revObject != NULL )
        revObject->setName( name );
    
}


/** We set here the required value type spec. */
void Variable::setRevObjectTypeSpec(const TypeSpec &ts) {
    
    revObjectTypeSpec = ts;
    
}


