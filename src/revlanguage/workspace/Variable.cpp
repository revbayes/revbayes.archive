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

/** Constructor of empty variable with specified type. */
Variable::Variable( const TypeSpec& ts, const std::string& n ) :
    name( n ),
    refCount( 0 ),
    revObject( NULL ),
    revObjectTypeSpec( ts ),
    isReferenceVariable( false ),
    isControlVariable( false )
{
    
}

/** Constructor of filled variable (no type restrictions). */
Variable::Variable(RevObject *v, const std::string &n) : 
    name( n ),
    refCount( 0 ),
    revObject( NULL ),
    revObjectTypeSpec( RevObject::getClassTypeSpec() ),
    isReferenceVariable( false ),
    isControlVariable( false )
{
    setRevObject( v );
}


/** Constructor of reference variable (no type restrictions). */
Variable::Variable(const RevPtr<Variable>& refVar, const std::string &n) :
    name( n ),
    refCount( 0 ),
    revObject( NULL ),
    revObjectTypeSpec( RevObject::getClassTypeSpec() ),
    isReferenceVariable( true ),
    isControlVariable( false )
{
    
    referencedVariable = refVar;
    
}


/** Copy constructor */
Variable::Variable(const Variable &v) : 
    name( v.name ), 
    refCount( 0 ),
    revObject( NULL ),
    revObjectTypeSpec( v.revObjectTypeSpec ),
    isReferenceVariable( v.isReferenceVariable ),
    isControlVariable( v.isControlVariable ),
    referencedVariable( v.referencedVariable )
{
    if ( v.revObject != NULL )
        setRevObject( v.revObject->clone() );
    else
        revObject = NULL;
}


Variable::~Variable( void )
{
    if ( !isReferenceVariable && revObject != NULL )
        delete revObject;
}


Variable& Variable::operator=(const Variable &v)
{
    if ( this != &v )
    {
        
        name                = v.name;
        revObjectTypeSpec   = v.revObjectTypeSpec;
        isReferenceVariable = v.isReferenceVariable;
        isControlVariable   = v.isControlVariable;
        referencedVariable  = v.referencedVariable;
        
        if ( isReferenceVariable )
            revObject = NULL;
        else
        {
            if ( revObject != NULL )
            {
                delete revObject;
                revObject = NULL;
            }
        
            if ( v.revObject != NULL )
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
size_t Variable::getReferenceCount(void) const
{
    return refCount;
}


/* Get the value of the variable */
RevObject& Variable::getRevObject(void) const
{
    if ( isReferenceVariable )
        return referencedVariable->getRevObject();
    
    if (revObject == NULL)
    {
        return RevNullObject::getInstance();
    }
    
    return *revObject;
}


/**
 * Get the required type specs for values stored inside this variable.
 * We return our own type specification even if we reference another
 * variable. By reassignment, we receive the new value, so it is
 * more important what our type spec is.
 */
const TypeSpec& Variable::getRevObjectTypeSpec(void) const
{
    return revObjectTypeSpec;
}


/* Increment the reference count for this instance. */
void Variable::incrementReferenceCount( void ) const
{
    refCount++;
}


/** Return the internal flag signalling whether the variable is currently a control variable */
bool Variable::isControlVar(void) const
{
    if ( isReferenceVariable )
        return referencedVariable->isControlVar();
    else
        return isControlVariable;
}


/** Return the internal flag signalling whether the variable is currently a reference variable */
bool Variable::isReferenceVar(void) const
{
    return isReferenceVariable;
}


/** Make this variable a reference to another variable. Make sure we delete any object we held before. */
void Variable::makeReference(const RevPtr<Variable>& refVar)
{
    if ( !isReferenceVariable )
    {
        if ( revObject != NULL )
            delete revObject;
        
        revObject = NULL;
        isReferenceVariable = true;
        isControlVariable = false;
    }
    
    referencedVariable = refVar;
}


/* Print value of the variable variable */
void Variable::printValue(std::ostream& o) const
{
    if (revObject == NULL)
        o << "NULL";
    else
        revObject->printValue( o );
}


/**
 * Replace Rev object and update the DAG in the process.
 * This is a private function so we can assume that the
 * caller knows not to call this function if the variable
 * is in the reference variable state.
 */
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


/**
 * Check whether this variable is a control variable. Throw an error if the variable
 * is a reference variable. If so, you need to set the Rev object first, and then set
 * the control variable flag.
 */
void Variable::setControlVarState(bool flag)
{
    if ( isReferenceVariable )
        throw "A reference variable cannot be made a control variable";
    
    isControlVariable = flag;
}


/** Set the name of the variable */
void Variable::setName(std::string const &n) {
    
    name = n;
    if ( revObject != NULL )
        revObject->setName( n );
}


/** Set the variable and update the DAG structure. */
void Variable::setRevObject( RevObject *newValue )
{
    if ( isReferenceVariable )
    {
        isReferenceVariable = false;
        referencedVariable = NULL;
    }
    
    // Make sure default assignment is not a control variable assignment
    isControlVariable = false;

    // Replace the Rev object and make sure we update the DAG as necessary
    replaceRevObject( newValue );
    
}


/**
 * We set here the required value type spec. An error is thrown if the
 * current Rev object of the variable, if any, is not of the specified type.
 */
void Variable::setRevObjectTypeSpec(const TypeSpec &ts) {
    
    const RevObject& theObject = this->getRevObject();
    if ( theObject != RevNullObject::getInstance() )
    {
        if ( !theObject.isTypeSpec( ts ) )
            throw RbException( "Existing variable object is not of the required type" );
    }
    
    revObjectTypeSpec = ts;
}

