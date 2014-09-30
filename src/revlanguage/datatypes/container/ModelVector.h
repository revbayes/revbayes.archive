#ifndef ModelVector_H
#define ModelVector_H

#include "ModelObject.h"
#include "RbVector.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    /**
     * @brief ModelVector: templated class for Rev vectors of model objects
     */
    template <typename rlType>
    class ModelVector : public ModelObject<RevBayesCore::RbVector<typename rlType::valueType> > {
        
    public:
        
        typedef typename rlType::valueType          elementType;
        typedef typename RevBayesCore::RbVector<elementType>      valueType;
        typedef typename valueType::iterator        iterator;
        typedef typename valueType::const_iterator  const_iterator;
        
                                                    ModelVector(void);                                          //!< Default constructor
                                                    ModelVector(const valueType& v);                            //!< Constructor from vector of values
                                                    ModelVector(RevBayesCore::TypedDagNode<valueType>* n);      //!< Constructor from value node

        // STL-like vector functions provided here
        elementType                                 operator[](size_t index) const;                             //!< Subscript operator to internal value of Rev element, not allowing assignment
//        void                                        pop_back(void);                                             //!< Drop element from back
//        void                                        pop_front(void);                                            //!< Drop element from front
        void                                        push_back(const rlType& x);                                 //!< Push Rev object element onto back
        void                                        push_back(const elementType& x);                            //!< Push internal value of Rev object element onto back
//        void                                        push_front(const rlType& x);                                //!< Push Rev object element onto front
//        void                                        push_front(const elementType& x);                           //!< Push internal value of Rev object element onto front
        
        // Basic utility functions you have to override
        virtual ModelVector<rlType>*                clone(void) const;                                          //!< Clone object
        static const std::string&                   getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                     //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const;                                    //!< Get the object type spec of the instance

        // Basic utility function you do not have to override
        void                                        printValue(std::ostream& o) const;                          //!< Print value for user
 
        // Type conversion functions
        RevObject*                                  convertTo(const TypeSpec& type) const;                      //!< Convert to requested type
        virtual bool                                isConvertibleTo(const TypeSpec& type, bool once) const;     //!< Is this object convertible to the requested type?

        // Member object functions
        virtual RevPtr<Variable>                    executeMethod(std::string const &name, const std::vector<Argument> &args);      //!< Map member methods to internal methods
        virtual const MethodTable&                  getMethods(void) const;                                                         //!< Get member methods
        virtual MethodTable                         makeMethods(void) const;                                                        //!< Make member methods
        
        // Container functions you may want to override to protect from assignment
//        virtual RevPtr<Variable>                    getElement(size_t index);                                              //!< Get element variable (single index)
        
        // ModelVector functions: override if you do not want to support these in-place algorithms
        virtual void                                clear(void);                                                                    //!< Clear the vector
        virtual size_t                              size(void) const;                                                               //!< Size of the vector
        virtual void                                sort(void);                                                                     //!< Sort vector
        virtual void                                unique(void);                                                                   //!< Remove consecutive duplicates

    protected:

        // Helper function
//        RevPtr<Variable>                            getElementFromValue(size_t oneOffsetIndex) const;                                     //!< Get element from value (single index)

    private:
        
        struct comparator {
            bool operator() (elementType A, elementType B) const { return ( A < B ); }
        } myComparator;
    
    };
    
}


#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "MethodTable.h"
#include "Natural.h"
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
    ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >()
{
}


/**
 * Constructor from a valueType object, that is, from an STL vector containing instances
 * of the internal value type of the Rev object elements of the model vector.
 */
template <typename rlType>
ModelVector<rlType>::ModelVector( const valueType &v ) :
    ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >( v.clone() )
{
}


/**
 * Constructor from a DAG node appropriate for the model vector
 * object.
 */
template <typename rlType>
ModelVector<rlType>::ModelVector( RevBayesCore::TypedDagNode<valueType> *n ) :
    ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >( n )
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
 * Clear the vector.
 */
template <typename rlType>
void ModelVector<rlType>::clear( void )
{
    this->dagNode->getValue().clear();
}


/** Get a type-safe clone of the object */
template <typename rlType>
ModelVector<rlType>* ModelVector<rlType>::clone() const
{
    return new ModelVector<rlType>( *this );
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
    
//    // Test whether we want to convert to another generic model vector
//    if ( type.getParentType() == getClassTypeSpec().getParentType() )
//    {
//        // We are both model vectors. Rely on generic code to cover all allowed conversions
//        
//        // Now generate the vector of elements
//        std::vector<RevObject*> theConvertedObjects;
//        
//        for ( typename valueType::const_iterator i = this->getValue().begin(); i != this->getValue().end(); ++i )
//        {
//            RevObject* orgElement = new rlType( *i );
//            if ( orgElement->isTypeSpec( *type.getElementTypeSpec() ) )
//                theConvertedObjects.push_back( orgElement );
//            else
//                theConvertedObjects.push_back( orgElement->convertTo( *type.getElementTypeSpec() ) );
//        }
//        
//        // Set the elements of the converted container, which assumes ownership of the objects
//        std::vector<size_t> lengths;
//        lengths.push_back( this->size() );
//        theConvertedContainer->setElements( theConvertedObjects, lengths );
//        
//        // Now return the converted container object
//        return theConvertedContainer;
//    }
    
    // Call the base class if all else fails. This will eventually throw an error if the type conversion is not supported.
    return this->ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >::convertTo( type );
}


/**
 * Map calls to member methods.
 */
template <typename rlType>
RevPtr<Variable> ModelVector<rlType>::executeMethod( std::string const &name, const std::vector<Argument> &args )
{
    if ( name == "size" )
    {
        // return a new variable with the size of this container
        return RevPtr<Variable>( new Variable( new Natural( size() ), "" ) );
    }
    else if ( name == "sort" )
    {
        // Check whether the DAG node is actually a constant node
        if ( !this->dagNode->isConstant() )
        {
            throw RbException( "Only constant variables can be sorted." );
        }
        sort();
        
        return NULL;
    }
    else if ( name == "unique" )
    {
        // Check whether the DAG node is actually a constant node
        if ( !this->dagNode->isConstant() )
        {
            throw RbException( "Only constant variables can be made unique." );
        }
        unique();
        
        return NULL;
    }
    
    return ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >::executeMethod( name, args );
}


/**
 * Get Rev type of object. This is the Rev object element type followed by
 * a single set of square brackets. This provides a nice and convenient way
 * of specifying generic types of vectors for all Rev object types.
 */
template <typename rlType>
const std::string& ModelVector<rlType>::getClassType(void)
{
    static std::string revType = rlType::getClassType() + "[]";
    
	return revType;
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
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >::getClassTypeSpec(), &rlType::getClassTypeSpec() );

	return revTypeSpec;
}



/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
template <typename rlType>
const MethodTable& ModelVector<rlType>::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = this->makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get the type spec (dynamic version) */
template <typename rlType>
const TypeSpec& ModelVector<rlType>::getTypeSpec(void) const
{
    return getClassTypeSpec();
}


/**
 * In this function we check whether this type is convertible to some other
 * Rev object type. Here we focus entirely on supporting conversion to
 * other generic vectors with compatible elements. This is not done automatically
 * because of the templating: a vector of RealPos does not inherit from a vector
 * of Real, for example.
 */
template <typename rlType>
bool ModelVector<rlType>::isConvertibleTo( const TypeSpec& type, bool once ) const
{
//    if ( type.getParentType() == getClassTypeSpec().getParentType() )
//    {
//        // We want to convert to another generic model vector
//
//        // Simply check whether our elements can convert to the desired element type
//        typename std::vector<elementType>::const_iterator i;
//        for ( i = this->getValue().begin(); i != this->getValue().end(); ++i )
//        {
//            rlType orgElement = rlType(*i);
//            
//            // Test whether this element is already of the desired element type or can be converted to it
//            if ( !orgElement.isTypeSpec( *type.getElementTypeSpec() ) && !orgElement.isConvertibleTo( *type.getElementTypeSpec(), once ) )
//            {
//                return false;
//            }
//        }
//        
//        return true;
//    }
    
    return ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >::isConvertibleTo( type, once );
}


/** Make methods for this class */
template <typename rlType>
MethodTable ModelVector<rlType>::makeMethods(void) const
{
    MethodTable methods = MethodTable();
    
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size", new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );
    
    ArgumentRules* sortArgRules = new ArgumentRules();
    methods.addFunction("sort", new MemberProcedure( RlUtils::Void, sortArgRules) );
    
    ArgumentRules* uniqueArgRules = new ArgumentRules();
    methods.addFunction("unique", new MemberProcedure( RlUtils::Void, uniqueArgRules) );
    
    // Insert inherited methods
    methods.insertInheritedMethods( ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >::makeMethods() );
    
    return methods;
}


/**
 * Add an element to the end of the vector.
 */
template <typename rlType>
void ModelVector<rlType>::push_back(const elementType &x)
{
    return this->dagNode->getValue().push_back( x );
}


/**
 * Add an element to the end of the vector.
 */
template <typename rlType>
void ModelVector<rlType>::push_back(const rlType &x)
{
    return this->dagNode->getValue().push_back( x.getValue() );
}


/**
 * Print the value of the vector, respecting the formatting of the
 * model object elements. We do this by retrieving the elements, one
 * by one, and printing them using their own printValue implementation.
 * Among other things, this takes care of proper formatting.
 *
 * We make a perfectly safe const cast here, since we only utilize the
 * const printValue function of the element.
 */
template <typename rlType>
void ModelVector<rlType>::printValue( std::ostream& o ) const
{
    
    size_t lineLength = 75;
    
    std::ostringstream s, t;
    s << "[ ";
    size_t curLength = 2;
    this->getDagNode()->printValue( o, "" );
//    for ( size_t i = 1; i <= this->size(); ++i )
//    {
//        RevPtr<Variable> elem = const_cast< ModelVector<rlType>* >(this)->getElement( i );
//        elem->getRevObject().printValue( t );
//
//        if ( i != this->size() )
//            t << ", ";
//        if ( curLength + t.str().size() > lineLength )
//        {
//            s << std::endl << "  ";
//            curLength = 2;
//        }
//        s << t.str();
//        curLength += t.str().size();
//        t.str("");
//    }
    if ( curLength + 2 > lineLength )
        s << std::endl << "]";
    else
        s << " ]";
    o << s.str();
}


/**
 * Size of the vector.
 */
template <typename rlType>
size_t ModelVector<rlType>::size( void ) const
{
    return this->dagNode->getValue().size();
}


/**
 * Sort the vector.
 */
template <typename rlType>
void ModelVector<rlType>::sort( void )
{
    this->dagNode->getValue().sort();
}

                                       
/**
 * Remove duplicates and resize the vector. This is a mutable member function,
 * meaning that it modifies this instance.
 *
 */
template <typename rlType>
void ModelVector<rlType>::unique(void)
{
    
    sort();
    valueType uniqueVector;
    uniqueVector.push_back (this->dagNode->getValue()[0]);
    for (size_t i = 1 ; i<this->size() ; i++)
    {
        if ( this->dagNode->getValue()[i] != this->dagNode->getValue()[i-1] )
        {
            uniqueVector.push_back(this->dagNode->getValue()[i]);
        }
    }
    
    this->clear();

    this->dagNode->getValue() = uniqueVector;
    
}


#endif
