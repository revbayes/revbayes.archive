#ifndef WorkspaceVector_H
#define WorkspaceVector_H

#include "RlContainer.h"
#include "RbVector.h"
#include "WorkspaceToCoreWrapperObject.h"

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
    class WorkspaceVector : public WorkspaceToCoreWrapperObject<RevBayesCore::RbVector<rlType> >, public Container {
        
    public:
//        typedef typename rlType::valueType          elementType;
//        typedef RevBayesCore::RbVector<elementType> vectorRbPtr;
        typedef std::vector<rlType*>                vectorRlPtr;
        
                                                    WorkspaceVector(void);                                              //!< Default constructor
                                                    WorkspaceVector(const RevBayesCore::RbVector<rlType>& v);           //!< Constructor from rb value pointers
//                                                    WorkspaceVector(const vectorRbPtr& v);                              //!< Constructor from rb value pointers
                                                    WorkspaceVector(const vectorRlPtr& v);                              //!< Constructor from rl value pointers
        
        // STL-like vector functions provided here
        rlType&                                     operator[](size_t index);                                           //!< Subscript operator
        const rlType&                               operator[](size_t index) const;                                     //!< Subscript operator (const)
        void                                        pop_back(void);                                                     //!< Drop element at back
        void                                        pop_front(void);                                                    //!< Drop element from front
        void                                        push_back(const RevObject &x);                                      //!< Append element to end
        void                                        push_front(const rlType &x);                                        //!< Append element to end
        size_t                                      size(void) const;                                                   //!< The size of the vector

        // Basic utility functions you have to override
        virtual WorkspaceVector<rlType>*            clone(void) const;                                                  //!< Clone object
        static const std::string&                   getClassType(void);                                                 //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                             //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get the object type spec of the instance        
        
        // Container functions provided here
        virtual rlType*                             getElement(size_t idx) const;                                                   //!< Get element variable (single index)
        
        // Type conversion functions
//        RevObject*                                  convertTo(const TypeSpec& type) const;                      //!< Convert to requested type
//        virtual double                              isConvertibleTo(const TypeSpec& type, bool once) const;     //!< Is this object convertible to the requested type?

        // WorkspaceVector functions
//        vectorRbPtr                                 getVectorRbPointer(void) const;                                     //!< Generate vector of rb pointers
        
    protected:
        // Basic utility function provided here
        void                                        printValue(std::ostream& o) const;                                  //!< Print value for user

        
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
    WorkspaceToCoreWrapperObject<RevBayesCore::RbVector< rlType > >()
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
WorkspaceVector<rlType>::WorkspaceVector( const RevBayesCore::RbVector<rlType>& v ) : WorkspaceToCoreWrapperObject<RevBayesCore::RbVector<rlType> >()
{
    // just to be safe
    delete this->value;
    
    this->value = v.clone();
}


///**
// * Construct from vector of pointers to internal value
// * objects. We assume here that we are responsible for
// * managing the associated memory. Here we need to make
// * copies of the original objects, so we delete those
// * after we are done.
// */
//template <typename rlType>
//WorkspaceVector<rlType>::WorkspaceVector( const vectorRbPtr& v ) :
//    WorkspaceToCoreWrapperObject<RevBayesCore::RbVector<typename rlType::valueType> >()
//{
//    *this->value = v;
//}


/**
 * Construct from vector of pointers to Rev language
 * objects. We assume here that we are responsible for
 * managing the associated memory. The pointers are
 * embedded inside the elements vector, and the associated
 * memory is managed as part of managing memory for the
 * elements vector.
 */
template <typename rlType>
WorkspaceVector<rlType>::WorkspaceVector( const vectorRlPtr& v ) : WorkspaceToCoreWrapperObject<RevBayesCore::RbVector< rlType > >()
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
 * Convert to object of another type. Here we use the setElements function
 * of the Container base class to do generic type conversion in all cases
 * where the elements are individually convertible to the desired element
 * type. This is not done automatically for us because of the templating.
 * A vector of RealPos, for example, does not inherit from a vector of Real,
 * which means that a vector of RealPos is not a specialized vector of Real
 * in the C++ sense (or in the Rev sense).
 */
//template <typename rlType>
//RevObject* WorkspaceVector<rlType>::convertTo(const TypeSpec &type) const
//{
//    
//    // First check that we are not asked to convert to our own type
//    if ( type == getClassTypeSpec() )
//    {
//        return this->clone();
//    }
//    
//    // Test whether we want to convert to another generic model vector
//    if ( type.getParentType() == getClassTypeSpec().getParentType() )
//    {
//        // We are both model vectors. Rely on generic code to cover all allowed conversions
//        
//        // First generate an empty model vector of the desired type
//        RevObject *emptyContainer = Workspace::userWorkspace().makeNewDefaultObject( type.getType() );
//        Container *theConvertedContainer = dynamic_cast<Container*>( emptyContainer );
//        
//        // test if the cast succeeded
//        if (theConvertedContainer == NULL)
//        {
//            throw RbException("Could not convert a container of type " + this->getClassType() + " to a container of type " + type.getType() );
//        }
//        
//        for ( typename RevBayesCore::RbConstIterator<elementType> i = this->getValue().begin(); i != this->getValue().end(); ++i )
//        {
//            
//            rlType orgElement = rlType( *i );
//            if ( orgElement.isType( *type.getElementTypeSpec() ) )
//            {
//                theConvertedContainer->push_back( orgElement );
//            }
//            else
//            {
//                RevObject *convObj = orgElement.convertTo( *type.getElementTypeSpec() );
//                theConvertedContainer->push_back( *convObj );
//                delete convObj;
//            }
//            
//        }
//        
//        // Now return the converted container object
//        return emptyContainer;
//    }
//    
//    // Call the base class if all else fails. This will eventually throw an error if the type conversion is not supported.
//    return this->ModelObject<RevBayesCore::RbVector<typename rlType::valueType> >::convertTo( type );
//}


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
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &WorkspaceToCoreWrapperObject<RevBayesCore::RbVector<rlType> >::getClassTypeSpec(), &rlType::getClassTypeSpec() );
    
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


///**
// * Here we return a vector of clones of the elementType values of the
// * Rev object elements. The clones are put in an RbVector, which is
// * assumed to take on ownership of those element value clones.
// */
//template<typename rlType>
//RevBayesCore::RbVector<typename rlType::valueType> WorkspaceVector<rlType>::getVectorRbPointer( void ) const
//{
//    vectorRbPtr theVector;
//    
//    RevBayesCore::RbIterator< rlType > it;
//    for ( it = this->value->begin(); it != this->value->end(); ++it )
//    {
//        theVector.push_back( it->getValue() );
//    }
//    
//    return theVector;
//}


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
void WorkspaceVector<rlType>::push_back( const RevObject &x )
{
    
    // cast the object
    const rlType *x_converted = dynamic_cast< const rlType* >( &x );
    
    if ( x_converted == NULL )
    {
        throw RbException("Could not append an element of type " + x.getType() + " to a vector of type " + this->getType() );
    }
    
    // Push it onto the back of the elements vector
    this->value->push_back( *x_converted );
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
    {
        s << getClassType() << " vector with 1 value";
    }
    else
    {
        s << getClassType() << " vector with " << size() << " values";
    }
    o << s.str() << std::endl;
    
    for ( size_t i = 0; i < s.str().size(); ++i )
    {
        o << "=";
    }
    o << std::endl << std::endl;

    for ( size_t i = 0; i < this->value->size(); ++i )
    {
        o << "[" << i + 1 << "]" << std::endl;
        RevObject& tmp = (*this->value)[i];
        tmp.printValue(o,true);
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
