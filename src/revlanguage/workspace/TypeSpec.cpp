#include "RbException.h"
#include "TypeSpec.h"

// #include "ModelVector.h"
// #include "RlString.h"

using namespace RevLanguage;


/**
 * Constructor from type and parent type spec. This constructor
 * is appropriate for all classes except those that derive
 * immediately from ModelContainer or WorkspaceContainer.
 */
TypeSpec::TypeSpec( const std::string& objType, const TypeSpec* par ) :
    parent( par ),
    type( objType )
{
    dim = getDimFromClass();
    element = getElementTypeSpecFromClass();
}


/**
 * Constructor from type, parent type spec, and element type spec.
 * This constructor is used by types that derive directly from ModelContainer
 * or WorkspaceContainer.
 */
TypeSpec::TypeSpec( const std::string& objType, const TypeSpec* par, const TypeSpec* elemType ) :
    element( elemType ),
    parent( par ),
    type( objType )
{
    dim = getDimFromClass();
}


/**
 * Equals comparison. This assumes that the Rev type
 * names are unique, which should be safe. Rev does
 * not support overloaded types.
 */
bool TypeSpec::operator==( const TypeSpec& x ) const
{
    return type == x.type;
}


/**
 * Not equals comparison. Simply rely on negating
 * the equals comparison, as usual.
 */
bool TypeSpec::operator!=( const TypeSpec& x ) const
{
    return type == x.type;
}


/**
 * Get the dimension of the type by climbing the ladder to see whether
 * we are derived from ModelContainer or WorkspaceContainer, and then
 * simply count the number of "[" characters in the type string.
 */
size_t TypeSpec::getDimFromClass( void ) const
{
    // If we reached the end, dim is 0
    if ( parent == NULL )
        return 0;
    
    // Get parent type
    std::string parentType;
    if ( parent != NULL )
        parentType = parent->getType();
    
    // Check if we now have the answer
    if ( parentType == "ModelContainer" || parentType == "WorkspaceContainer" )
    {
        size_t numDims = 0;
        for ( size_t i = 0; i < type.size(); ++i )
        {
            if ( type[i] == '[' )
                numDims++;
        }
        return numDims;
    }

    // If not, continue climbing up our derivation tree
    return parent->getDim();
}


/**
 * Get dim. Simply return the dimension of the type, which
 * we computed in the constructor.
 */
const size_t TypeSpec::getDim( void ) const
{
    return dim;
}


/**
 * Get element type. Simply use the element type spec
 * that we computed in the constructor.
 */
const std::string& TypeSpec::getElementType( void ) const
{
    if ( element == NULL)
        throw RbException( "Illegal attempt to get element type for type without elements" );
    
    return element->getType();
}


/**
 * Get element type spec. Simply return the element type spec
 * that we computed in the constructor.
 */
const TypeSpec& TypeSpec::getElementTypeSpec( void ) const
{
    if ( element == NULL)
        throw RbException( "Illegal attempt to get element type spec for type without elements" );
    
    return *element;
}


/**
 * Get the element type by climbing the ladder to see whether we are
 * derived from ModelContainer or WorkspaceContainer. Then simply skip
 * the trailing square brackets to get the element type.
 */
const TypeSpec* TypeSpec::getElementTypeSpecFromClass(void) const
{
    // If we reached the end there is no element
    if ( parent == NULL )
        return NULL;
    
    // Get parent type
    std::string parentType;
    if ( parent != NULL )
        parentType = parent->getType();
    
    // Check if we now have the answer
    if ( parentType == "ModelContainer" || parentType == "WorkspaceContainer" )
        return this->element;
    
    // If not, continue climbing up our derivation tree
    return parent->getElementTypeSpecFromClass();
}


/**
 * Get parent type string. Simply return the type string of the
 * parent type spec but throw an error if the parent is NULL.
 */
const std::string& TypeSpec::getParentType( void ) const
{
    if ( parent == NULL)
        throw RbException( "Illegal attempt to get parent type for type without parent" );
    
    return parent->getType();
}


/**
 * Get parent type spec. Simply return the parent type spec
 * but throw an error if the parent is NULL.
 */
const TypeSpec& TypeSpec::getParentTypeSpec( void ) const
{
    if ( parent == NULL)
        throw RbException( "Illegal attempt to get parent type spec for type without parent" );
    
    return *parent;
}


/**
 * Get total dim. A container may have as elements a type that
 * is derived from a generic container. In that case, the total
 * dimension of the Rev object includes both the container and
 * the element dimensions. The way that Rev objects are currently
 * created by the parser guarantees that there is no recursion;
 * that is, we cannot have generic containers containing generic
 * containers. However, by calling getTotalDim instead of getDim
 * for the element type spec, we support possible future recursive
 * generic containers.
 */
const size_t TypeSpec::getTotalDim( void ) const
{
    if ( element != NULL )
        return dim + element->getTotalDim();
    else
        return dim;
}


/**
 * Get the type string. Simply return the
 * type member variable.
 */
const std::string& TypeSpec::getType(void) const
{
    return type;
}


/**
 * Test whether we are the same type or derived from the same
 * type as the argument type spec. We do this by a preorder
 * traversal of the tree.
 */
bool TypeSpec::isDerivedOf( const TypeSpec& x ) const
{
    // First we test whether this type is the same as the argument type
    if ( operator==( x ) )
        return true;
    
    // We reached the end
    if ( parent == NULL )
        return false;

    // If not, continue climbing up our derivation tree
    return parent->isDerivedOf(x);
}


#if 0
/**
 * Make an RlString object reflecting the type.
 * We provide this for the Rev "type" function.
 */
RlString* TypeSpec::makeRevType( void ) const
{
    return new RlString( type );
}


/**
 * Make a ModelVector<RlString> object reflecting the class
 * vector. We provide this for the Rev "class" function.
 */
ModelVector<RlString>* TypeSpec::makeRevClass( void ) const
{
    return new ModelVector<RlString>( type );
}
#endif


/**
 * Output as string. Simply print the type to keep the
 * output as short as possible.
 */
std::ostream& operator<<(std::ostream& o, const TypeSpec& x)
{
    o << x.getType();
    return o;
}



