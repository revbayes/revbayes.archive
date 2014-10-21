#ifndef WorkspaceVector_H
#define WorkspaceVector_H

#include "RlContainer.h"
#include "RbVector.h"
#include "WorkspaceObject.h"

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
     * The class gives access to an RbVector of clones of the internal value objects through
     * the getVectorRbPointer() method. The workspace vector can be set using either an
     * RbVector object or a vector of RevObject element pointers (vectorRlPtr). In the latter
     * case, we assume ownership of the elements; in the former, we generate independent clones
     * of the elements.
     *
     * For Rev objects that are not model objects or workspace objects, see RevObjectVector.
     */
    template <typename rlType>
    class WorkspaceVector : public WorkspaceObject<RevBayesCore::RbVector<rlType> >, public Container {
        
    public:
        typedef typename rlType::valueType          elementType;
        typedef RevBayesCore::RbVector<elementType> vectorRbPtr;
        typedef std::vector<rlType*>                vectorRlPtr;
        
                                                    WorkspaceVector(void);                                              //!< Default constructor
                                                    WorkspaceVector(const RevBayesCore::RbVector<rlType>& v);           //!< Constructor from rb value pointers
                                                    WorkspaceVector(const vectorRbPtr& v);                              //!< Constructor from rb value pointers
                                                    WorkspaceVector(const vectorRlPtr& v);                              //!< Constructor from rl value pointers
        
        // STL-like vector functions provided here
        rlType&                                     operator[](size_t index);                                           //!< Subscript operator
        const rlType&                               operator[](size_t index) const;                                     //!< Subscript operator (const)
        void                                        pop_back(void);                                                     //!< Drop element at back
        void                                        pop_front(void);                                                    //!< Drop element from front
        void                                        push_back(const rlType &x);                                         //!< Append element to end
        void                                        push_back(const RevObject &x);                                      //!< Append element to end
        void                                        push_front(const rlType &x);                                        //!< Append element to end
        size_t                                      size(void) const;                                                   //!< The size of the vector

        // Basic utility functions you have to override
        virtual WorkspaceVector<rlType>*            clone(void) const;                                                  //!< Clone object
        static const std::string&                   getClassType(void);                                                 //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                             //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get the object type spec of the instance        
        
        // Basic utility function provided here
        void                                        printValue(std::ostream& o) const;                                  //!< Print value for user
        
        // Container functions provided here
        virtual rlType*                             getElement(size_t idx) const;                                                   //!< Get element variable (single index)
        
        // WorkspaceVector functions
        vectorRbPtr                                 getVectorRbPointer(void) const;                                     //!< Generate vector of rb pointers
        
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
    WorkspaceObject<RevBayesCore::RbVector< rlType > >()
{
    this->value = new RevBayesCore::RbVector< rlType >();
    
}


/**
 * Construct from vector of pointers to internal value
 * objects. We assume here that we are responsible for
 * managing the associated memory. Here we need to make
 * copies of the original objects, so we delete those
 * after we are done.
 */
template <typename rlType>
WorkspaceVector<rlType>::WorkspaceVector( const RevBayesCore::RbVector<rlType>& v ) : WorkspaceObject<RevBayesCore::RbVector<rlType> >()
{
    // just to be safe
    delete this->value;
    
    this->value = v.clone();
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
    WorkspaceObject<RevBayesCore::RbVector<typename rlType::valueType> >()
{
    *this->value = v;
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
WorkspaceVector<rlType>::WorkspaceVector( const vectorRlPtr& v ) : WorkspaceObject<RevBayesCore::RbVector< rlType > >()
{

//    for ( typename vectorRlPtr::const_iterator it = v.begin(); it != v.end(); ++it )
//    {
//        this->value->push_back( **it );
//    }
    
}


/**
 * Subscript operator, provided for convenience. Note that
 * there is no problem to give out non-const references
 * to our Rev object elements.
 */
template <typename rlType>
rlType& WorkspaceVector<rlType>::operator[]( size_t index )
{
    return (*this->value)[ index ];
}


/**
 * Subscript operator, provided for convenience. This is
 * the const version.
 */
template <typename rlType>
const rlType& WorkspaceVector<rlType>::operator[]( size_t index ) const
{
    return (*this->value)[ index ];
}


/** Return a type-safe clone of the object */
template <typename rlType>
WorkspaceVector<rlType>* WorkspaceVector<rlType>::clone() const
{
    return new WorkspaceVector<rlType>( *this );
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
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &WorkspaceObject<RevBayesCore::RbVector<typename rlType::valueType> >::getClassTypeSpec(), &rlType::getClassTypeSpec() );
    
	return revTypeSpec;
}


template <typename rlType>
rlType* WorkspaceVector<rlType>::getElement(size_t idx) const
{
    const rlType &element = this->operator[](idx);
    return element.clone();
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
 * Here we return a vector of clones of the elementType values of the
 * Rev object elements. The clones are put in an RbVector, which is
 * assumed to take on ownership of those element value clones.
 */
template<typename rlType>
RevBayesCore::RbVector<typename rlType::valueType> WorkspaceVector<rlType>::getVectorRbPointer( void ) const
{
    vectorRbPtr theVector;
    
    RevBayesCore::RbIterator< rlType > it;
    for ( it = this->value->begin(); it != this->value->end(); ++it )
    {
        theVector.push_back( it->getValue() );
    }
    
    return theVector;
}


/**
 * Drop an element from the back of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::pop_back( void )
{
    this->value->pop_back();
}


/**
 * Drop an element from the front of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::pop_front( void )
{
    this->value->erase( this->value->begin() );
}

/**
 * Push a Rev object element onto the back of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::push_back( const rlType &x )
{

    // Push it onto the back of the elements vector
    this->value->push_back( x );
}


/**
 * Push a Rev object element onto the back of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::push_back( const RevObject &x )
{
    
    // cast the object
    const rlType *x_converted = dynamic_cast< const rlType* >( &x );
    
    if ( x_converted == NULL )
    {
        throw RbException("Could not append an element of type " + x.getType() + " to a vector of type " + this->getType() );
    }
    
    // Push it onto the back of the elements vector
    this->push_back( *x_converted );
}


/**
 * Push a Rev object element onto the front of the vector.
 */
template <typename rlType>
void WorkspaceVector<rlType>::push_front( const rlType &x )
{
    
    // Push it onto the front of the elements vector
    this->value->insert( this->value->begin(), x );
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

    std::stringstream s;
    if ( size() == 1 )
        s << getClassType() << " vector with 1 value";
    else
        s << getClassType() << " vector with " << size() << " values";
    o << s.str() << std::endl;
    
    for ( size_t i = 0; i < s.str().size(); ++i )
        o << "=";
    o << std::endl << std::endl;

    for ( size_t i = 0; i < this->value->size(); ++i )
    {
        o << "[" << i + 1 << "]" << std::endl;
            (*this->value)[i].printValue(o);
        o << std::endl << std::endl;
    }

    o << std::endl;
}




/**
 * Get the of the vector.
 */
template <typename rlType>
size_t WorkspaceVector<rlType>::size( void ) const
{
    return this->value->size();
}


#endif
