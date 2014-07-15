#ifndef ModelVector_H
#define ModelVector_H

#include "ModelContainer.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    /**
     * @brief ModelVector: templated class for Rev vectors of model objects
     *
     * We can rely on the template class specialization of ModelContainer for
     * vectors to provide most of the functionality we need.
     *
     * The class is based on a value type of std::vector<rlType>. This means
     * that it cannot be used for abstract Rev classes. For those, use
     * ModelVectorAbstractElement instead.
     */
    template <typename rlType>
    class ModelVector : public ModelContainer< rlType, 1, std::vector<typename rlType::valueType> > {
        
    public:
        
        typedef typename rlType::valueType          elementType;
        typedef typename std::vector<elementType>   valueType;
        
                                                    ModelVector(void);                                          //!< Default constructor
                                                    ModelVector(const valueType& v);                            //!< Constructor from vector of values
                                                    ModelVector(RevBayesCore::TypedDagNode<valueType>* n);      //!< Constructor from value node

        // STL-like vector functions provided here
        elementType                                 operator[](size_t index) const;                             //!< Subscript operator to internal value of Rev element, not allowing assignment
        void                                        pop_back(void);                                             //!< Drop element from back
        void                                        pop_front(void);                                            //!< Drop element from front
        void                                        push_back(const rlType& x);                                 //!< Push Rev object element onto back
        void                                        push_back(const elementType& x);                            //!< Push internal value of Rev object element onto back
        void                                        push_front(const rlType& x);                                //!< Push Rev object element onto front
        void                                        push_front(const elementType& x);                           //!< Push internal value of Rev object element onto front
        
        // Basic utility functions you have to override
        virtual ModelVector<rlType>*                clone(void) const;                                          //!< Clone object
        static const std::string&                   getClassType(void);                                         //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                     //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const;                                    //!< Get the object type spec of the instance

        // Type conversion functions
        RevObject*                                  convertTo(const TypeSpec& type) const;                      //!< Convert to requested type
        virtual bool                                isConvertibleTo(const TypeSpec& type) const;                //!< Is this object convertible to the requested type?

        // Member object functions
        RevPtr<Variable>                            executeMethod(std::string const &name, const std::vector<Argument> &args);  //!< Execute member methods
        
        // Container functions implemented here. Override findOrCreateElement and getElement to protect from assignment
        virtual RevPtr<Variable>                    findOrCreateElement(const std::vector<size_t>& oneOffsetIndices);               //!< Find or create element variable
        virtual RevPtr<Variable>                    getElement(const std::vector<size_t>& oneOffsetIndices);                        //!< Get element variable
        void                                        setElements(std::vector<RevObject*> elems, const std::vector<size_t>& lengths); //!< Set elements from Rev objects

        // ModelVector functions: override if you do not want to support these in-place algorithms
        virtual void                                sort(void);                                                 //!< Sort vector
        virtual void                                unique(void);                                               //!< Remove consecutive duplicates

    protected:

        // Container help function
        RevPtr<Variable>                            getElementFromValue(const std::vector<size_t>& oneOffsetIndices);   //!< Get element from value
        
    private:
        struct comparator {
            bool operator() (elementType A, elementType B) const { return ( A < B);}
        } myComparator;
    };
    
}


#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RevPtr.h"
#include "TypeSpec.h"
#include "Variable.h"
#include "Workspace.h"

#include <algorithm>

using namespace RevLanguage;


/**
 * Default constructor: construct an empty vector. We do not need to store the element type
 * because this can be accessed from our Rev type specification.
 */
template <typename rlType>
ModelVector<rlType>::ModelVector( void ) :
    ModelContainer< rlType, 1, valueType >()
{
}


/**
 * Constructor from a valueType object, that is, from an STL vector containing instances
 * of the internal value type of the Rev object elements of the model vector.
 */
template <typename rlType>
ModelVector<rlType>::ModelVector( const valueType &v ) :
    ModelContainer< rlType, 1, valueType >( v )
{
}


/**
 * Constructor from a DAG node appropriate for the model vector
 * object.
 */
template <typename rlType>
ModelVector<rlType>::ModelVector( RevBayesCore::TypedDagNode<valueType> *n ) :
    ModelContainer< rlType, 1, valueType >( n )
{
}


/**
 * Subscript operator, provided for convenience. Note that we should
 * not allow the user to assign to our elements, so we need to give
 * out a copy and not a reference to the element. This makes it
 * possible to implement this as a const function returning a non-const
 * variable. The return of a copy is ensured by not specifying a reference
 * (&) on the return type.
 */
template <typename rlType>
typename rlType::valueType ModelVector<rlType>::operator[]( size_t index ) const
{
    return this->getValue()[ index ];
}


/**
 * Convert to object of another type. Here we use the setElements function
 * of the Container base class to do generic type conversion in all cases
 * where the elements are individually convertible to the desired element
 * type. This is not done automatically for us because of the templating.
 * A vector of RealPos, for example, does not inherit from a vector of Real,
 * which means that a vector of RealPos is not a specialized vector of Real
 * in the C++ sense (or in the Rev sense).
 */
template <typename rlType>
RevObject* ModelVector<rlType>::convertTo(const TypeSpec &type) const
{
    // First check that we are not asked to convert to our own type
    if ( type == getClassTypeSpec() )
        return this->clone();
    
    // test whether we want to convert to another generic model vector
    if ( type.getDim() == 1 && type.getParentType() == getClassTypeSpec().getParentType() )
    {
        // We are both model vectors. Rely on generic code to cover all allowed conversions
        
        // First generate an empty model vector of the desired type
        Container* theConvertedContainer = Workspace::userWorkspace().makeNewEmptyContainer( type.getElementType(), type.getDim() );
        
        // Now generate the vector of elements
        std::vector<RevObject*> theConvertedObjects;
        
        for ( typename valueType::const_iterator i = this->getValue().begin(); i != this->getValue().end(); ++i )
        {
            RevObject* orgElement = new rlType( *i );
            theConvertedObjects.push_back( orgElement->convertTo( *type.getElementTypeSpec() ) );
        }
        
        // Set the elements of the converted container, which assumes ownership of the objects
        std::vector<size_t> lengths;
        lengths.push_back( this->size() );
        theConvertedContainer->setElements( theConvertedObjects, lengths );
        
        // Now return the converted container object
        return theConvertedContainer;
    }
    
    // Call the base class if all else fails. This will eventually throw an error if the type conversion is not supported.
    return this->ModelContainer< rlType, 1, std::vector<typename rlType::valueType> >::convertTo( type );
}


/** Get a type-safe clone of the object */
template <typename rlType>
ModelVector<rlType>* ModelVector<rlType>::clone() const
{
    return new ModelVector<rlType>( *this );
}


/**
 * Map calls to member methods. This deals with the stochastic variable methods.
 */
template <typename rlType>
RevPtr<Variable> ModelVector<rlType>::executeMethod( std::string const &name, const std::vector<Argument> &args )
{
    if ( name == "sort" )
    {
        sort();
        
        return NULL;
    }
    else if ( name == "unique" )
    {
        unique();
        
        return NULL;
    }
    
    return ModelContainer<rlType, 1, std::vector< typename rlType::valueType> >::executeMethod( name, args );
}


/**
 * Find an element for assignment. If we are a stochastic node or
 * deterministic node, we ask the user first if they want to break up
 * the previous association of the variable with a single dynamic DAG
 * node and make it a composite of several individual DAG nodes instead.
 * If we are a constant node, we simply go ahead quietly with the
 * replacement.
 */
template<typename rlType>
RevPtr<Variable> ModelVector<rlType>::findOrCreateElement( const std::vector<size_t>& oneOffsetIndices )
{
    // First retrieve the variable elements vector, if possible
    ContainerNode<rlType, valueType>* theContainerNode = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    if ( theContainerNode == NULL )
    {
        if ( dynamic_cast< RevBayesCore::ConstantNode<valueType>* >( this->dagNode ) == NULL )
        {
            bool answer = UserInterface::userInterface().ask( "Do you want to convert the container to a composite value" );
            
            if ( answer == true )
                this->makeCompositeValue();
            else
                throw RbException( "Assignment to elements of a homogeneous container not allowed" );
        }
        else
            this->makeCompositeValue();
        
        theContainerNode = static_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    }
    
    // Check the indices first
    if ( oneOffsetIndices.size() == 0 || oneOffsetIndices[0] == 0 )
        throw RbException( "Assignment to multiple vector elements not supported (yet)" );

    // Split indices into myIndex and elementIndices (one-offset!)
    size_t myIndex = oneOffsetIndices[0];
    std::vector<size_t> elementIndices;
    for ( size_t i = 1; i < oneOffsetIndices.size(); ++i )
        elementIndices.push_back( oneOffsetIndices[i] );
    
    if ( elementIndices.size() == 0 )
    {
        // We want to assign to an element in the vector
        
        // Resize if myIndex is out of range.
        for ( size_t it = this->size(); it < myIndex; ++it )
            theContainerNode->push_back( new rlType() );
        
        // Return the assignable element
        return theContainerNode->getElement( myIndex - 1 );
    }
    else
    {
        // We want to assign to a subelement
        
        // Check if the index is out of range
        if ( myIndex > this->size() )
            throw RbException( "Index out of range" );
        
        // Return the assignable subelement
        return theContainerNode->getElement( myIndex - 1 )->getRevObject().getElement( elementIndices );
    }
}


/**
 * Get class name of object. This is the Rev object element type followed by
 * a single set of square brackets. This provides a nice and convenient way
 * of specifying generic types of vectors for all Rev object types.
 */
template <typename rlType>
const std::string& ModelVector<rlType>::getClassType(void)
{
    static std::string rbClassName = rlType::getClassType() + "[]";
    
	return rbClassName;
}


/**
 * Get class type spec describing type of object. Note that we have to
 * use the special version of the TypeSpec constructor because we derive
 * directly from ModelContainer and have the special "rlType[]" type
 * specification.
 */
template <typename rlType>
const RevLanguage::TypeSpec& ModelVector<rlType>::getClassTypeSpec(void)
{
    static TypeSpec rbClass = TypeSpec( getClassType(), &ModelContainer<rlType, 1, valueType>::getClassTypeSpec(), &rlType::getClassTypeSpec() );

	return rbClass;
}


/**
 * Get an element. We support reference assignment by giving out a smart pointer to the actual
 * element if we are a composite node. We do that by retrieving the elements from our container
 * node, and then returning the appropriate element or elements if a slice is requested.
 *
 * If we are a constant node, we could transform ourselves to a composite conatiner node first
 * and then get the elements from it. However, we opt not to do that here, to keep the vector
 * simple if possible.
 *
 * If we are a simple dynamic node, or a constant node, we simply give out a new temporary variable,
 * a clone of the original element. In this way, we can guard ourselves from attempts by others to
 * modify the element in cases where that would be inappropriate.
 *
 * You can still assign to elements of simple vector nodes through calling the createOrFindElement
 * function. It will ask the user if they want to convert a dynamic node to a composite node. See
 * that function for more information.
 *
 * Note that we use oneOffsetIndices, so substract one before using them to retrieve elements
 * from the internal elements vector. A zero in the first position of the index means that we
 * want to return the entire vector.
 */
template<typename rlType>
RevPtr<Variable> ModelVector<rlType>::getElement( const std::vector<size_t>& oneOffsetIndices )
{
    // First check if we want to return a slice
    if ( oneOffsetIndices.size() == 0 || oneOffsetIndices[0] == 0 )
    {
        if ( oneOffsetIndices.size() > 1 )
            throw RbException( "Slicing across Rev objects not supported" );
        return new Variable( this->clone() );
    }
    
    // We want a single element: first check if we have or can get element variables
    ContainerNode<rlType, valueType>* theContainerNode = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    
    // We need to retrieve the element from the value vector if we do not have a container node
    if ( theContainerNode == NULL )
        return getElementFromValue( oneOffsetIndices );
    
    // We are a composite vector with a container node. We retrieve the element from its elements vector
    
    // Split indices into myIndex and elementIndices
    size_t myIndex = oneOffsetIndices[0];
    std::vector<size_t> elementIndices;
    for ( size_t i = 1; i < oneOffsetIndices.size(); ++i )
        elementIndices.push_back( oneOffsetIndices[i] );
    
    // Check that myIndex is in range
    if ( myIndex > this->size() )
        throw RbException( "Index out of range" );
    
    // Return an element or subelement
    if ( elementIndices.size() == 0 )
        return theContainerNode->getElement( myIndex - 1 );
    else
        return theContainerNode->getElement( myIndex - 1 )->getRevObject().getElement( elementIndices );
}


/**
 * Get element(s) from value vector. Note that we use the language default of
 * one-offset indices, so remember to subtract one before accessing internal
 * objects.
 *
 * Note that a zero or missing first index means that we want the entire vector.
 */
template <typename rlType>
RevPtr<Variable> ModelVector<rlType>::getElementFromValue( const std::vector<size_t>& oneOffsetIndices )
{
    RevPtr<Variable> retVariable;

    if ( oneOffsetIndices.size() == 0 || oneOffsetIndices[0] == 0 )
    {
        ModelVector<rlType>* newVector = this->clone();
        newVector->makeConstantValue();
        
        retVariable = new Variable( newVector );
    }
    else
        retVariable = new Variable( new rlType( this->getValue()[ oneOffsetIndices[0] - 1 ] ) );
    
    return retVariable;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. (Testing not to below) */
template <typename rlType>
const RevLanguage::TypeSpec& ModelVector<rlType>::getTypeSpec(void) const
{
//    static TypeSpec typeSpec = getClassTypeSpec();

//    return typeSpec;
    
    return getClassTypeSpec();  // This should do the trick; there should be a separate version of the function for each template type
}


/**
 * In this function we check whether this type is convertible to some other
 * Rev object type. Here we focus entirely on supporting conversion to
 * other generic vectors with compatible elements. This is not done automatically
 * because of the templating: a vector of RealPos does not inherit from a vector
 * of Real, for example.
 */
template <typename rlType>
bool ModelVector<rlType>::isConvertibleTo( const TypeSpec& type ) const
{
    if ( type.getDim() == 1 && type.getParentType() == getClassTypeSpec().getParentType() )
    {
        // We want to convert to another generic model vector

        // Simply check whether our elements can convert to the desired element type
        typename std::vector<elementType>::const_iterator i;
        for ( i = this->getValue().begin(); i != this->getValue().end(); ++i )
        {
            rlType orgElement = rlType(*i);
            
            // Test whether this element is already of the desired element type or can be converted to it
            if ( !orgElement.isTypeSpec( *type.getElementTypeSpec() ) && !orgElement.isConvertibleTo( *type.getElementTypeSpec() ) )
                return false;
        }
        
        return true;
    }
    
    return Container::isConvertibleTo( type );
}


/**
 * Drop an element from the back of the vector. We do not allow this if we are a
 * dynamic variable but throw an error in that case.
 */
template <typename rlType>
void ModelVector<rlType>::pop_back( void )
{
    if ( dynamic_cast< RevBayesCore::DynamicNode<valueType>* >( this->dagNode ) != NULL )
        throw RbException( "Cannot pop element from dynamic vector variable" );
    
    this->makeCompositeValue();
    static_cast< ContainerNode<rlType, valueType>* >( this->dagNode )->pop_back();
}


/**
 * Drop an element from the front of the vector. We do not allow this if we are a
 * dynamic variable but throw an error in that case.
 */
template <typename rlType>
void ModelVector<rlType>::pop_front( void )
{
    if ( dynamic_cast< RevBayesCore::DynamicNode<valueType>* >( this->dagNode ) != NULL )
        throw RbException( "Cannot pop element from dynamic vector variable" );
    
    this->makeCompositeValue();
    static_cast< ContainerNode<rlType, valueType>* >( this->dagNode )->pop_back();
}


/**
 * Push a Rev object element onto the back of the vector. We do not allow this if we are a
 * dynamic variable but throw an error in that case.
 */
template <typename rlType>
void ModelVector<rlType>::push_back( const rlType& x )
{
    RevBayesCore::ConstantNode<valueType>*  theConstantNode     = dynamic_cast< RevBayesCore::ConstantNode<valueType>* >( this->dagNode );
    ContainerNode<rlType, valueType>*       theContainerNode    = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );

    if ( theConstantNode != NULL )
    {
        theConstantNode->getValue().push_back( x.getValue() );
    }
    else if ( theContainerNode != NULL )
    {
        theContainerNode->push_back( x.clone() );
    }
    else
        throw RbException( "Cannot push element onto simple dynamic vector variable" );
}


/**
 * Push the internal value of a Rev object element onto the back of the vector. We do not
 * allow this if we are a dynamic variable but throw an error in that case.
 */
template <typename rlType>
void ModelVector<rlType>::push_back( const elementType& x )
{
    RevBayesCore::ConstantNode<valueType>*  theConstantNode     = dynamic_cast< RevBayesCore::ConstantNode<valueType>* >( this->dagNode );
    ContainerNode<rlType, valueType>*       theContainerNode    = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    
    if ( theConstantNode != NULL )
    {
        theConstantNode->getValue().push_back( x );
    }
    else if ( theContainerNode != NULL )
    {
        theContainerNode->push_back( new rlType( x ) );
    }
    else
        throw RbException( "Cannot push element onto simple dynamic vector variable" );
}


/**
 * Push a Rev object element onto the front of the vector. We do not allow this if we are a
 * simple dynamic variable but throw an error in that case.
 */
template <typename rlType>
void ModelVector<rlType>::push_front( const rlType& x )
{
    RevBayesCore::ConstantNode<valueType>*  theConstantNode     = dynamic_cast< RevBayesCore::ConstantNode<valueType>* >( this->dagNode );
    ContainerNode<rlType, valueType>*       theContainerNode    = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    
    if ( theConstantNode != NULL )
    {
        theConstantNode->getValue().insert( theConstantNode->getValue().begin(), x.getValue() );
    }
    else if ( theContainerNode != NULL )
    {
        theContainerNode->push_front( x.clone() );
    }
    else
        throw RbException( "Cannot push element onto simple dynamic vector variable" );
}


/**
 * Push the internal value of a Rev object element onto the front of the vector. We do not
 * allow this if we are a dynamic variable but throw an error in that case.
 */
template <typename rlType>
void ModelVector<rlType>::push_front( const elementType& x )
{
    RevBayesCore::ConstantNode<valueType>*  theConstantNode     = dynamic_cast< RevBayesCore::ConstantNode<valueType>* >( this->dagNode );
    ContainerNode<rlType, valueType>*       theContainerNode    = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    
    if ( theConstantNode != NULL )
    {
        theConstantNode->getValue().insert( theConstantNode->getValue().begin(), x );
    }
    else if ( theContainerNode != NULL )
    {
        theContainerNode->push_front( new rlType( x ) );
    }
    else
        throw RbException( "Cannot push element onto simple dynamic vector variable" );
}


/**
 * Set elements from a vector of Rev objects. We assume that we want
 * a simple constant container. This function will typically be called
 * by the type conversion code.
 */
template <typename rlType>
void ModelVector<rlType>::setElements( std::vector<RevObject*> elems, const std::vector<size_t>& lengths )
{
    valueType* newVal = new valueType();
    for ( std::vector<RevObject*>::iterator it = elems.begin(); it != elems.end(); ++it )
        (*newVal).push_back( static_cast< ModelObject<typename rlType::valueType>* >( (*it) )->getValue() );
    
    this->setDagNode( new RevBayesCore::ConstantNode<valueType>( "", newVal ) );
}


/**
 * Sort the vector. This is only possible if we are a composite value vector,
 * so the code below will not work.
 *
 * @todo Implement this properly
 */
template <typename rlType>
void ModelVector<rlType>::sort( void )
{
//    std::sort( this->begin(), this->end(), myComparator );

    throw RbException( "Missing implementation of sort() in ModelVector." );
}

                                       
/**
 * Remove duplicates and resize the vector. This is a mutable member function,
 * meaning that it modifies this instance. This means that it can only be
 * performed if we are a composite value vector.
 *
 * @todo Implement this properly
 */
template <typename rlType>
void ModelVector<rlType>::unique(void) {
    
#if 0
    sort();
    valueType uniqueVector;
    uniqueVector.push_back (this->value->getValue()[0]);
    for (size_t i = 1 ; i<this->size() ; i++)
    {
        if ( this->value->getValue()[i] != this->value->getValue()[i-1] )
            uniqueVector.push_back(this->value->getValue()[i]);
    }
    
    this->clear();

    this->setValues(uniqueVector);  // not available; impossible if not composite
#endif
    
    throw RbException( "Missing implementation in unique() in ModelVector." );

    return;
}


#endif
