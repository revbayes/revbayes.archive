#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RevNullObject.h"
#include "TypeSpec.h"
#include "RevVariable.h"

#include <cassert>
#include <string>
#include <sstream>

using namespace RevLanguage;

/** Constructor of empty RevVariable with specified type. */
RevVariable::RevVariable( const TypeSpec& ts, const std::string& n ) :
    name( n ),
    refCount( 0 ),
    revObject( NULL ),
    requiredTypeSpec( ts ),
    isElementVar( false ),
    isHiddenVar( false ),
    isReferenceVar( false ),
    isVectorVar( false ),
    isWorkspaceVar( false ),
    elementIndices(),
    referencedVariable( NULL )
{
    
}

/** Constructor of filled RevVariable (no type restrictions). */
RevVariable::RevVariable(RevObject *v, const std::string &n) :
    name( n ),
    refCount( 0 ),
    revObject( NULL ),
    requiredTypeSpec( RevObject::getClassTypeSpec() ),
    isElementVar( false ),
    isHiddenVar( false ),
    isReferenceVar( false ),
    isVectorVar( false ),
    isWorkspaceVar( false ),
    elementIndices(),
    referencedVariable( NULL )
{
    replaceRevObject( v );
}


/** Constructor of reference RevVariable (no type restrictions). */
RevVariable::RevVariable(const RevPtr<RevVariable>& refVar, const std::string &n) :
    name( n ),
    refCount( 0 ),
    revObject( NULL ),
    requiredTypeSpec( RevObject::getClassTypeSpec() ),
    isElementVar( false ),
    isHiddenVar( false ),
    isReferenceVar( true ),
    isVectorVar( false ),
    isWorkspaceVar( false ),
    elementIndices(),
    referencedVariable( refVar )
{
    
}


/** Copy constructor */
RevVariable::RevVariable(const RevVariable &v) :
    name( v.name ),
    refCount( 0 ),
    revObject( NULL ),
    requiredTypeSpec( v.requiredTypeSpec ),
    isElementVar( v.isElementVar ),
    isHiddenVar( v.isHiddenVar ),
    isReferenceVar( v.isHiddenVar ),
    isVectorVar( v.isVectorVar ),
    isWorkspaceVar( v.isWorkspaceVar ),
    elementIndices( v.elementIndices ),
    referencedVariable( v.referencedVariable )
{
    
    if ( v.revObject != NULL )
    {
        replaceRevObject( v.revObject->clone() );
    }
    else
    {
        revObject = NULL;
    }
    
}


RevVariable::~RevVariable( void )
{
#if defined ( DEBUG_MEMORY )
    std::cerr << "Deleting RevVariable '" << name << "' <" << this << ">" << std::endl;
#endif
    
    if ( !isReferenceVar && revObject != NULL )
    {
        delete revObject;
    }
    
}


RevVariable& RevVariable::operator=(const RevVariable &v)
{
    if ( this != &v )
    {
        
        name                = v.name;
        requiredTypeSpec    = v.requiredTypeSpec;
        isElementVar        = v.isElementVar;
        isHiddenVar         = v.isHiddenVar;
        isReferenceVar      = v.isReferenceVar;
        isVectorVar         = v.isVectorVar;
        isWorkspaceVar      = v.isWorkspaceVar;
        elementIndices      = v.elementIndices;
        referencedVariable  = v.referencedVariable;
        
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
                replaceRevObject( v.revObject->clone() );
            }
            
        }
    }
    
    return *this;
}


/** Resize the vector to include this index. */
void RevVariable::addIndex(int idx)
{
    
    elementIndices.insert( idx );
    
}


/* Clone RevVariable and RevVariable */
RevVariable* RevVariable::clone( void ) const
{
    
    return new RevVariable( *this );
}


/* Decrement the reference count. */
size_t RevVariable::decrementReferenceCount( void ) const
{
    
    refCount--;
    
    return refCount;
}


const std::set<int>& RevVariable::getElementIndices( void ) const
{
    return elementIndices;
}


const std::string& RevVariable::getName( void ) const
{
    
    return name;
}



/* Get the reference count for this instance. */
size_t RevVariable::getReferenceCount(void) const
{
    return refCount;
}


/* Get the value of the RevVariable */
RevObject& RevVariable::getRevObject(void) const
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
 * Get the required type specs for values stored inside this RevVariable.
 * We return our own type specification even if we reference another
 * RevVariable. By reassignment, we receive the new value, so it is
 * more important what our type spec is.
 */
const TypeSpec& RevVariable::getRequiredTypeSpec(void) const
{
    return requiredTypeSpec;
}


/* Increment the reference count for this instance. */
void RevVariable::incrementReferenceCount( void ) const
{
    refCount++;
}



/**
 * Is the RevVariable or any of its members (upstream DAG nodes) assignable, that is,
 * modifiable by the user? For them to be assignable, they have to be named, otherwise
 * there is no chance for the user to change them.
 */
bool RevVariable::isAssignable( void ) const
{
    // Check if we are assignable
    if ( name != "" )
        return true;
    
    // Ask our object for assignable upstream RevVariables
    if ( revObject != NULL && revObject->isAssignable() )
        return true;
    
    // No possibility left to modify us
    return false;
}


/** 
 * Return the internal flag signalling whether the RevVariable is an element of a vector, e.g., x[1] would be.
 */
bool RevVariable::isElementVariable( void ) const
{
    return isElementVar;
}


/** Return the internal flag signalling whether the RevVariable is currently a hidden RevVariable. Hidden RevVariables will not show in the ls() function. */
bool RevVariable::isHiddenVariable( void ) const
{
    return isHiddenVar;
}


/** Return the internal flag signalling whether the RevVariable is currently a reference RevVariable */
bool RevVariable::isReferenceVariable( void ) const
{
    return isReferenceVar;
}


/** Return the internal flag signalling whether the RevVariable is currently a vector RevVariable, that is, should be computed by x := v(x[1],...) */
bool RevVariable::isVectorVariable( void ) const
{
    return isVectorVar;
}


/** Return the internal flag signalling whether the RevVariable is currently a workspace (control) RevVariable */
bool RevVariable::isWorkspaceVariable( void ) const
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


/** Make this RevVariable a reference to another RevVariable. Make sure we delete any object we held before. */
void RevVariable::makeReference(const RevPtr<RevVariable>& refVar)
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


/* Print value of the RevVariable RevVariable */
void RevVariable::printValue(std::ostream& o) const
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


/** Replace the RevVariable and update the DAG structure. */
void RevVariable::replaceRevObject( RevObject *newValue )
{
    if ( isReferenceVar )
    {
        isReferenceVar = false;
        referencedVariable = NULL;
    }
    
    // Make sure default assignment is not a workspace (control) RevVariable assignment
    isWorkspaceVar = false;
    
    if ( revObject != NULL )
    {
        
        if ( revObject->isModelObject() && revObject->getDagNode() != NULL )
        {
            revObject->getDagNode()->replace( newValue->getDagNode() );
        }
        
    }
    
    delete revObject;
    revObject = newValue;
    
    if ( revObject != NULL )
    {
        revObject->setName( name );
    }
    
    try
    {
        if ( revObject != NULL && revObject->isModelObject() == true && revObject->getDagNode() != NULL )
        {
            revObject->getDagNode()->setHidden( isHiddenVar );
            revObject->getDagNode()->setElementVariable( isElementVar );
        }
    }
    catch (RbException e)
    {
        // do nothing
    }
    
}




/**
 * Set whether this RevVariable is an element of a vector RevVariable.
 * All element RevVariable are also hidden.
 * Throw an error if the RevVariable is a reference RevVariable. 
 * If so, you need to set the Rev object first, and then set the hidden RevVariable flag.
 */
void RevVariable::setElementVariableState(bool flag)
{
    if ( isReferenceVar )
    {
        throw "A reference RevVariable cannot be made a hidden RevVariable";
    }
    
    isElementVar = flag;
    
    try {
        
        if ( revObject != NULL && revObject->getDagNode() != NULL )
        {
            revObject->getDagNode()->setElementVariable( flag );
        }
        
    }
    catch (RbException e)
    {
        // do nothing
    }
}


/**
 * Set whether this RevVariable is a hidden RevVariable. Throw an error if the RevVariable
 * is a reference RevVariable. If so, you need to set the Rev object first, and then set
 * the hidden RevVariable flag.
 */
void RevVariable::setHiddenVariableState(bool flag)
{
    if ( isReferenceVar )
    {
        throw "A reference RevVariable cannot be made a hidden RevVariable";
    }
    
    isHiddenVar = flag;
    
    try {
        
        if ( revObject != NULL && revObject->getDagNode() != NULL )
        {
            revObject->getDagNode()->setHidden( flag );
        }
        
    }
    catch (RbException e)
    {
        // do nothing
    }

}


/**
 * Set whether this RevVariable is a vector RevVariable. Throw an error if the RevVariable
 * is a reference RevVariable. If so, you need to set the Rev object first, and then set
 * the vector RevVariable flag.
 */
void RevVariable::setVectorVariableState(bool flag)
{
    if ( isReferenceVar )
    {
        throw "A reference RevVariable cannot be made a vector RevVariable";
    }
    
    isVectorVar = flag;
}


/**
 * Check whether this RevVariable is a workspace (control) RevVariable. Throw an error if the RevVariable
 * is a reference RevVariable. If so, you need to set the Rev object first, and then set
 * the workspace (control) RevVariable flag.
 */
void RevVariable::setWorkspaceVariableState(bool flag)
{
    if ( isReferenceVar )
    {
        throw "A reference RevVariable cannot be made a workspace (control) RevVariable";
    }
    
    isWorkspaceVar = flag;
}


/** Set the name of the RevVariable */
void RevVariable::setName(std::string const &n)
{
    
    name = n;
    if ( revObject != NULL )
    {
        revObject->setName( n );
    }
    
}



/**
 * We set here the required value type spec. An error is thrown if the
 * current Rev object of the variable, if any, is not of the specified type.
 */
void RevVariable::setRequiredTypeSpec(const TypeSpec &ts)
{
    
    const RevObject& theObject = this->getRevObject();
    if ( theObject != RevNullObject::getInstance() )
    {
        
        if ( !theObject.isType( ts ) )
        {
            throw RbException( "Existing RevVariable object is not of the required type" );
        }
        
    }
    
    requiredTypeSpec = ts;
}

