#ifndef WorkspaceVector_H
#define WorkspaceVector_H

#include "RbVector.h"
#include "WorkspaceContainer.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    /**
     * @brief WorkspaceVector: templated vector of workspace object elements
     *
     * WorkspaceVector is a vector specialization of WorkspaceContainer. It is templated on the
     * Rev type of the elements of the vector, which are expected to be workspace objects
     * (inheriting from WorkspaceObject).
     *
     * The class gives direct access to a vector of pointers to the internal value objects
     * through the getVectorRbPointer method. It also gives access to a vector of pointers to
     * the Rev objects through the getVectorRlPointer method. Both types of vectors can be used
     * to construct a WorkspaceVector instance.
     *
     * For Rev objects that are not model objects or workspace objects, see RevObjectVector.
     */
    template <typename rlType>
    class WorkspaceVector : public WorkspaceContainer {
        
    public:
        typedef typename rlType::valueType          elementType;
        typedef RevBayesCore::RbVector<elementType> vectorRbPtr;
        typedef std::vector<rlType*>                vectorRlPtr;
        
                                                    WorkspaceVector(void);                                              //!< Default constructor
                                                    WorkspaceVector(const vectorRbPtr& v);                              //!< Constructor from rb value pointers
                                                    WorkspaceVector(const vectorRlPtr& v);                              //!< Constructor from rl value pointers
        
        // STL-like vector functions provided here
        rlType&                                     operator[](size_t index);                                           //!< Subscript operator
        const rlType&                               operator[](size_t index) const;                                     //!< Subscript operator (const)
        void                                        pop_back(void);                                                     //!< Drop element at back
        void                                        pop_front(void);                                                    //!< Drop element from front
        void                                        push_back(rlType *x);                                               //!< Append element to end
        void                                        push_front(rlType *x);                                              //!< Append element to end

        // Basic utility functions you have to override
        virtual WorkspaceVector<rlType>*            clone(void) const;                                                  //!< Clone object
        static const std::string&                   getClassType(void);                                                 //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                             //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get the object type spec of the instance

        // Basic utility function provided here
        void                                        printValue(std::ostream& o) const;                                  //!< Print value for user
        
        // Container functions provided here
        RevPtr<Variable>                            findOrCreateElement(const std::vector<size_t>& oneOffsetIndices);   //!< Find or create element variable
        size_t                                      getDim(void) const;                                                 //!< Get the dimensions
        RevPtr<Variable>                            getElement(const std::vector<size_t>& oneOffsetIndices);            //!< Get element variable
        virtual void                                setElements(std::vector<RevObject*> elems, const std::vector<size_t>& lengths); //!< Set elements from Rev objects
        
        // WorkspaceVector functions
        vectorRbPtr                                 getVectorRbPointer(void) const;                                     //!< Translate to vector of rb pointers
        vectorRlPtr                                 getVectorRlPointer(void) const;                                     //!< Translate to vector of rl pointers
        
    private:

    };

}


#include <algorithm>

using namespace RevLanguage;


/**
 * Default constructor. Construct empty vector.
 * We do not have to set the element type because that is
 * taken care of in getClassType.
 */
template <typename rlType>
WorkspaceVector<rlType>::WorkspaceVector( void ) :
    WorkspaceContainer()
{
}


/**
 * Construct from vector of pointers to internal value
 * objects. We assume here that we are responsible for
 * managing the associated memory. Here we need to make
 * copies of the original objects, so we delete those
 * after we are done.
 */
template <typename rlType>
WorkspaceVector<rlType>::WorkspaceVector( const vectorRbPtr& v ) :
    WorkspaceContainer()
{
    for ( typename vectorRbPtr::const_iterator it = v.begin(); it != v.end(); ++it )
    {
        Variable* newVar = new Variable( new rlType( *( *it ) ) );
        newVar->setRevObjectTypeSpec( rlType::getClassTypeSpec() );
        elements.push_back( newVar );
        delete ( *it );
    }
}


/**
 * Construct from vector of pointers to Rev language
 * objects. We assume here that we are responsible for
 * managing the associated memory. The pointers are
 * embedded inside the elements vector, and the associated
 * memory is managed as part of managing memory for the
 * elements vector.
 */
template <typename rlType>
WorkspaceVector<rlType>::WorkspaceVector( const vectorRlPtr& v ) :
    WorkspaceContainer()
{
    for ( typename vectorRlPtr::const_iterator it = v.begin(); it != v.end(); ++it )
    {
        Variable* newVar = new Variable( *it );
        newVar->setRevObjectTypeSpec( rlType::getClassTypeSpec() );
        elements.push_back( newVar );
    }
}


/**
 * Subscript operator, provided for convenience. Note that
 * there is no problem to give out non-const references
 * to our Rev object elements.
 */
template <typename rlType>
rlType& WorkspaceVector<rlType>::operator[]( size_t index )
{
    return static_cast<rlType&>( this->elements[ index ]->getRevObject() );
}


/**
 * Subscript operator, provided for convenience. This is
 * the const version.
 */
template <typename rlType>
const rlType& WorkspaceVector<rlType>::operator[]( size_t index ) const
{
    return static_cast<rlType&>( this->elements[ index ]->getRevObject() );
}


/** Return a type-safe clone of the object */
template <typename rlType>
WorkspaceVector<rlType>* WorkspaceVector<rlType>::clone() const
{
    return new WorkspaceVector<rlType>( *this );
}


/**
 * Find or create a variable for parser assignment.
 */
template <typename rlType>
RevPtr<Variable> WorkspaceVector<rlType>::findOrCreateElement(const std::vector<size_t>& oneOffsetIndices)
{
    // Check the indices first
    if ( oneOffsetIndices.size() == 0 || oneOffsetIndices[0] == 0 )
        throw RbException( "Illegal attempt to assign to multiple vector elements" );

    // Split indices into myIndex and elementIndices
    size_t myIndex = oneOffsetIndices[0];
    std::vector<size_t> elementIndices;
    for ( size_t i = 1; i < oneOffsetIndices.size(); ++i )
        elementIndices.push_back( oneOffsetIndices[i] );

    if ( elementIndices.size() == 0 )
    {
        // We want to assign to an element in the vector
        
        // Resize if myIndex is out of range. We need to use NULL variables in case we have
        // abstract Rev language objects.
        for ( size_t it = elements.size(); it < myIndex; ++it )
            elements.push_back( new Variable( NULL ) );
    
        // Return the assignable element
        return elements[ myIndex - 1 ];
    }
    else
    {
        // We want to assign to a subelement
        
        // Check if the index is out of range
        if ( myIndex > elements.size() )
            throw RbException( "Index out of range" );

        // Return the assignable subelement
        return elements[ myIndex - 1 ]->getRevObject().getElement( elementIndices );
    }
}


/**
 * Get Rev type of object. This is the Rev object element type followed by
 * a single set of square brackets. This provides a nice and convenient way
 * of specifying generic types of vectors for all Rev object types.
 */
template <typename rlType>
const std::string& WorkspaceVector<rlType>::getClassType(void)
{
    static std::string revType = rlType::getClassType() + "[]";
    
	return revType;
}


/**
 * Get class type spec describing type of object. Note that we have to
 * use the special version of the TypeSpec constructor because we derive
 * directly from WorkspaceContainer and have the special "rlType[]" type
 * specification.
 */
template <typename rlType>
const TypeSpec& WorkspaceVector<rlType>::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &WorkspaceContainer::getClassTypeSpec(), &rlType::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get the dimension of the workspace container (1 for vector) */
template <typename rlType>
size_t WorkspaceVector<rlType>::getDim(void) const
{
    return 1;
}


/**
 * Get a specific element in the vector. This throws an error if the 
 * index is out of range. Any superfluous indices are passed onto the
 * element at that index.
 *
 * If the indices passed in are empty or have a zero in the first 
 * position, the entire vector is returned.
 *
 * An error is thrown if the index is out of range.
 */
template<typename rlType>
RevPtr<Variable> WorkspaceVector<rlType>::getElement( const std::vector<size_t>& oneOffsetIndices )
{
    // Check first if we want the entire vector
    if ( oneOffsetIndices.size() == 0 || oneOffsetIndices[0] == 0 )
    {
        if ( oneOffsetIndices.size() > 1 )
            throw RbException( "Slicing across Rev objects not allowed" );

        RevPtr<Variable> retVar = new Variable( this->clone() );
        return retVar;
    }

    // We want a single element

    // Split indices into myIndex and elementIndices
    size_t myIndex = oneOffsetIndices[0];
    std::vector<size_t> elementIndices;
    for ( size_t i = 1; i < oneOffsetIndices.size(); ++i )
        elementIndices.push_back( oneOffsetIndices[i] );
    
    // Check that myIndex is in range
    if ( myIndex > elements.size() )
        throw RbException( "Index out of range" );
    
    // Return an element or subelement
    if ( elementIndices.size() == 0 )
        return elements[ myIndex - 1 ];
    else
        return elements[ myIndex - 1 ]->getRevObject().getElement( elementIndices );
}


/**
 * This function simply sets the content of the vector using the provided
 * elements. We do type checking and also check the lengths argument for
 * correctness
 */
template<typename rlType>
void WorkspaceVector<rlType>::setElements(std::vector<RevObject*> elems, const std::vector<size_t>& lengths)
{
    // Check lengths specification
    if ( lengths.size() != 1 || lengths[0] != elems.size() )
        throw RbException( "Incorrect lengths specification when setting WorkspaceVector elements" );
    
    // Check types of objects
    for ( std::vector<RevObject*>::iterator it = elems.begin(); it != elems.end(); ++it )
    {
        if ( !(*it)->isTypeSpec( rlType::getClassTypeSpec() ) )
            throw RbException( "Illegal attempt to set WorkSpace vector of '" + rlType::getClassType() + "' elements with an '" + (*it)->getType() + "' object" );
    }
    
    // Now simply replace the elements, making sure we guard them using the type requirement of Variable
    elements.clear();
    for ( std::vector<RevObject*>::iterator it = elems.begin(); it != elems.end(); ++it )
    {
        Variable* newVar = new Variable( *it );
        newVar->setRevObjectTypeSpec( rlType::getClassTypeSpec() );
        elements.push_back( newVar );
    }
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. (Testing not to below) */
template <typename rlType>
const TypeSpec& WorkspaceVector<rlType>::getTypeSpec(void) const
{
    //    static TypeSpec typeSpec = getClassTypeSpec();
    
    //    return typeSpec;
    
    return getClassTypeSpec();  // This should do the trick; there should be a separate version of the function for each template type
}


/**
 * Here we return a vector of pointers to the Rev object elements. We hope
 * that the caller does not delete the element values but it does not
 * matter if they change them.
 *
 * @todo We might want to change this function so that it gives out references
 *       instead of pointers to the Rev objects
 */
template<typename rlType>
std::vector<rlType*> WorkspaceVector<rlType>::getVectorRlPointer( void ) const
{
    vectorRlPtr theVector;
    
    std::vector< RevPtr<Variable> >::const_iterator it;
    for ( it = elements.begin(); it != elements.end(); ++it )
        theVector.push_back( &( static_cast<rlType&>( (*it)->getRevObject() ) ) );
    
    return theVector;
}


/**
 * Here we return a vector of pointers to the elementType values of the
 * Rev object elements. We hope that the caller does not delete the element
 * values but it does not matter if they change them except that the Rev
 * object has lost control then of what ranges of values are acceptable.
 * This is mainly provided here for backwards compatibility.
 *
 * @todo This function should probably be removed
 */
template<typename rlType>
RevBayesCore::RbVector<typename rlType::valueType> WorkspaceVector<rlType>::getVectorRbPointer( void ) const
{
    vectorRbPtr theVector;
    
    std::vector< RevPtr<Variable> >::const_iterator it;
    for ( it = elements.begin(); it != elements.end(); ++it )
        theVector.push_back( &( static_cast<rlType&>( (*it)->getRevObject() ).getValue() ) );
    
    return theVector;
}


/**
 * Drop an element from the back of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::pop_back( void )
{
    elements.pop_back();
}


/**
 * Drop an element from the front of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::pop_front( void )
{
    elements.erase( elements.begin() );
}

/**
 * Push a Rev object element onto the back of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::push_back( rlType* x )
{
    // Make sure our variable guards the type of element
    Variable* newVar = new Variable( x );
    newVar->setRevObjectTypeSpec( rlType::getClassTypeSpec() );

    // Push it onto the back of the elements vector
    elements.push_back( newVar );
}


/**
 * Push a Rev object element onto the front of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::push_front( rlType* x )
{
    // Make sure our variable guards the type of element
    Variable* newVar = new Variable( x );
    newVar->setRevObjectTypeSpec( rlType::getClassTypeSpec() );
    
    // Push it onto the front of the elements vector
    elements.insert( elements.begin(), newVar );
}


/**
 * Here we print the value of the vector. Because some workspace objects
 * require a lot of space to print their value, we opt for a format that should
 * work well for such cases.
 */
template<typename rlType>
void WorkspaceVector<rlType>::printValue( std::ostream& o ) const
{
    o << std::endl;
    o << getClassType() << " vector with " << size() << " values" << std::endl;
    o << std::endl;
    
    for ( size_t i = 0; i < elements.size(); ++i )
    {
        o << "[" << i + 1 << "]" << std::endl;
        elements[i]->getRevObject().printValue( o );
        o << std::endl;
    }
}


#endif
