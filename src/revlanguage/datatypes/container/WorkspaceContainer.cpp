#include "WorkspaceContainer.h"

#include "RevPtr.h"
#include "TypeSpec.h"
#include "Variable.h"

#include <vector>

using namespace RevLanguage;


/**
 * Default constructor: construct empty workspace container.
 */
WorkspaceContainer::WorkspaceContainer( void ) :
    Container()
{
}


/**
 * Copy constructor. We make an independent copy of the elements
 * here.
 */
WorkspaceContainer::WorkspaceContainer( const WorkspaceContainer& c) :
    Container( c )
{
    for ( std::vector< RevPtr<Variable> >::const_iterator it = c.elements.begin(); it != c.elements.end(); ++it )
        elements.push_back( (*it)->clone() );
}


/**
 * Destructor does not have to worry about the elements, because they are smart pointers
 */
WorkspaceContainer::~WorkspaceContainer()
{
}


/**
 * Assignment operator. We make sure we have an independent clone of the
 * elements here.
 */
WorkspaceContainer& WorkspaceContainer::operator=( const WorkspaceContainer& c )
{
    if ( this != &c )
    {
        // Free the memory
        elements.clear();
        
        // Create my own copy
        for ( std::vector< RevPtr<Variable> >::const_iterator it = c.elements.begin(); it != c.elements.end(); ++it )
            elements.push_back( (*it)->clone() );
    }
    
    return *this;
}


/** Clear the contents of the container */
void WorkspaceContainer::clear( void )
{
    elements.clear();
}


/** Get class name of object */
const std::string& WorkspaceContainer::getClassType(void)
{
    static std::string revType = "WorkspaceContainer";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& WorkspaceContainer::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Container::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Return the size of the container */
size_t WorkspaceContainer::size( void ) const
{
    return elements.size();
}

