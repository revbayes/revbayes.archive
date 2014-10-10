#include "RbConstants.h"
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
isHiddenVar( false ),
isReferenceVar( false ),
isVectorVar( false ),
isWorkspaceVar( false ),
min( RbConstants::Integer::max ),
max( 0 )
{
    
}

/** Constructor of filled variable (no type restrictions). */
Variable::Variable(RevObject *v, const std::string &n) :
name( n ),
refCount( 0 ),
revObject( NULL ),
revObjectTypeSpec( RevObject::getClassTypeSpec() ),
isHiddenVar( false ),
isReferenceVar( false ),
isVectorVar( false ),
isWorkspaceVar( false ),
min( RbConstants::Integer::max ),
max( 0 )
{
    setRevObject( v );
}


/** Constructor of reference variable (no type restrictions). */
Variable::Variable(const RevPtr<Variable>& refVar, const std::string &n) :
name( n ),
refCount( 0 ),
revObject( NULL ),
revObjectTypeSpec( RevObject::getClassTypeSpec() ),
isHiddenVar( false ),
isReferenceVar( true ),
isVectorVar( false ),
isWorkspaceVar( false ),
min( RbConstants::Integer::max ),
max( 0 )
{
    
    referencedVariable = refVar;
    
}


/** Copy constructor */
Variable::Variable(const Variable &v) :
name( v.name ),
refCount( 0 ),
revObject( NULL ),
revObjectTypeSpec( v.revObjectTypeSpec ),
isHiddenVar( v.isHiddenVar ),
isReferenceVar( v.isHiddenVar ),
isVectorVar( v.isVectorVar ),
isWorkspaceVar( v.isWorkspaceVar ),
referencedVariable( v.referencedVariable ),
min( v.min ),
max( v.max )
{
    
    if ( v.revObject != NULL )
    {
        setRevObject( v.revObject->clone() );
    }
    else
    {
        revObject = NULL;
    }
    
}


Variable::~Variable( void )
{
#if defined ( DEBUG_MEMORY )
    std::cerr << "Deleting variable '" << name << "' <" << this << ">" << std::endl;
#endif
    
    if ( !isReferenceVar && revObject != NULL )
    {
        delete revObject;
    }
    
}


Variable& Variable::operator=(const Variable &v)
{
    if ( this != &v )
    {
        
        name                = v.name;
        revObjectTypeSpec   = v.revObjectTypeSpec;
        isHiddenVar         = v.isHiddenVar;
        isReferenceVar      = v.isReferenceVar;
        isVectorVar         = v.isVectorVar;
        isWorkspaceVar      = v.isWorkspaceVar;
        referencedVariable  = v.referencedVariable;
        min                 = v.min;
        max                 = v.max;
        
        if ( isReferenceVar )
        {
            revObject = NULL;
        }
        else
        {
            if ( revObject != NULL )
            {
                delete revObject;
                revObject = NULL;
            }
            
            if ( v.revObject != NULL )
            {
                setRevObject( v.revObject->clone() );
            }
            
        }
    }
    
    return *this;
}


/** Resize the index boundaries for this vector variable to include this index. */
void Variable::addIndexBoundary(int idx)
{
    
    if ( min > idx )
    {
        min = idx;
    }
    
    if ( max < idx )
    {
        max = idx;
    }
    
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


int Variable::getMaxIndex( void ) const
{
    return max;
}


int Variable::getMinIndex( void ) const
{
    return min;
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
    if ( isReferenceVar )
    {
        return referencedVariable->getRevObject();
    }
    
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



/**
 * Is the variable or any of its members (upstream DAG nodes) assignable, that is,
 * modifiable by the user? For them to be assignable, they have to be named, otherwise
 * there is no chance for the user to change them.
 */
bool Variable::isAssignable( void ) const
{
    // Check if we are assignable
    if ( name != "" )
        return true;
    
    // Ask our object for assignable upstream variables
    if ( revObject != NULL && revObject->isAssignable() )
        return true;
    
    // No possibility left to modify us
    return false;
}


/** Return the internal flag signalling whether the variable is currently a hidden variable. Hidden variables will not show in the ls() function. */
bool Variable::isHiddenVariable( void ) const
{
    return isHiddenVar;
}


/** Return the internal flag signalling whether the variable is currently a reference variable */
bool Variable::isReferenceVariable( void ) const
{
    return isReferenceVar;
}


/** Return the internal flag signalling whether the variable is currently a vector variable, that is, should be computed by x := v(x[1],...) */
bool Variable::isVectorVariable( void ) const
{
    return isVectorVar;
}


/** Return the internal flag signalling whether the variable is currently a workspace (control) variable */
bool Variable::isWorkspaceVariable( void ) const
{
    if ( isReferenceVar )
    {
        return referencedVariable->isWorkspaceVariable();
    }
    else
    {
        return isWorkspaceVar;
    }
}


/** Make this variable a reference to another variable. Make sure we delete any object we held before. */
void Variable::makeReference(const RevPtr<Variable>& refVar)
{
    if ( !isReferenceVar )
    {
        if ( revObject != NULL )
        {
            delete revObject;
        }
        
        revObject = NULL;
        isReferenceVar = true;
        isWorkspaceVar = false;
    }
    
    referencedVariable = refVar;
}


/* Print value of the variable variable */
void Variable::printValue(std::ostream& o) const
{
    
    if (revObject == NULL)
    {
        o << "NULL";
    }
    else
    {
        revObject->printValue( o );
    }
    
}


/**
 * Replace Rev object and update the DAG in the process.
 * This is a private function so we can assume that the
 * caller knows not to call this function if the variable
 * is in the reference variable state.
 */
void Variable::replaceRevObject( RevObject *newObj )
{
    
    if (revObject != NULL)
    {
        // I need to tell my children that I'm being exchanged
        revObject->replaceVariable( newObj );
        
        delete revObject;
    }
    
    revObject = newObj;
    
    if ( revObject != NULL )
    {
        revObject->setName( name );
    }
    
    try
    {
        if ( revObject != NULL && revObject->getDagNode() != NULL )
        {
            revObject->getDagNode()->setHidden( isHiddenVar );
        }
    }
    catch (RbException e)
    {
        // do nothing
    }
    
}


/**
 * Set whether this variable is a hidden variable. Throw an error if the variable
 * is a reference variable. If so, you need to set the Rev object first, and then set
 * the hidden variable flag.
 */
void Variable::setHiddenVariableState(bool flag)
{
    if ( isReferenceVar )
    {
        throw "A reference variable cannot be made a hidden variable";
    }
    
    isHiddenVar = flag;
    
    if ( revObject != NULL && revObject->getDagNode() != NULL )
    {
        revObject->getDagNode()->setHidden( flag );
    }
}


/**
 * Set whether this variable is a vector variable. Throw an error if the variable
 * is a reference variable. If so, you need to set the Rev object first, and then set
 * the vector variable flag.
 */
void Variable::setVectorVariableState(bool flag)
{
    if ( isReferenceVar )
    {
        throw "A reference variable cannot be made a vector variable";
    }
    
    isVectorVar = flag;
}


/**
 * Check whether this variable is a workspace (control) variable. Throw an error if the variable
 * is a reference variable. If so, you need to set the Rev object first, and then set
 * the workspace (control) variable flag.
 */
void Variable::setWorkspaceVariableState(bool flag)
{
    if ( isReferenceVar )
    {
        throw "A reference variable cannot be made a workspace (control) variable";
    }
    
    isWorkspaceVar = flag;
}


/** Set the name of the variable */
void Variable::setName(std::string const &n)
{
    
    name = n;
    if ( revObject != NULL )
    {
        revObject->setName( n );
    }
    
}


/** Set the variable and update the DAG structure. */
void Variable::setRevObject( RevObject *newValue )
{
    if ( isReferenceVar )
    {
        isReferenceVar = false;
        referencedVariable = NULL;
    }
    
    // Make sure default assignment is not a workspace (control) variable assignment
    isWorkspaceVar = false;
    
    // Replace the Rev object and make sure we update the DAG as necessary
    replaceRevObject( newValue );
    
}


/**
 * We set here the required value type spec. An error is thrown if the
 * current Rev object of the variable, if any, is not of the specified type.
 */
void Variable::setRevObjectTypeSpec(const TypeSpec &ts)
{
    
    const RevObject& theObject = this->getRevObject();
    if ( theObject != RevNullObject::getInstance() )
    {
        
        if ( !theObject.isTypeSpec( ts ) )
        {
            throw RbException( "Existing variable object is not of the required type" );
        }
        
    }
    
    revObjectTypeSpec = ts;
}

