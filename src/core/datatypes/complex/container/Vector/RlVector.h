/**
 * @file
 * This file contains the declaration of Vector, a container type
 * that acts as a base class for all constant Vectors.
 *
 * This class is a wrapper for the stl-class Vector and we provide additional RevBayes functionality
 * (e.g. getTypeSpec and getClassTypeSpec for argument checking)
 *
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id$
 */

#ifndef Vector_H
#define Vector_H

#include "Container.h"
#include "MethodTable.h"
#include "SimpleMemberFunction.h"

#include <iostream>
#include <vector>


template <typename valueType, typename rlType>
class RlVector : public Container {
    
public:
    
    typedef typename std::vector<valueType>::iterator iterator;
    typedef typename std::vector<valueType>::const_iterator const_iterator;
    
    RlVector(void);                                                                                                 //!< Default constructor with type RbLanguageObject
    RlVector(size_t n);                                                                                             //!< Default constructor with type RbLanguageObject
    RlVector(size_t n, const valueType &v);                                                                         //!< Default constructor with type RbLanguageObject
    RlVector(const RlVector& v);                                                                                    //!< Copy Constructor
    
    virtual                                        ~RlVector(void);                                                 //!< Virtual destructor 
    
    // Basic utility functions 
    RlVector*                                       clone(void) const;                                              //!< Clone object
    RbObject*                                       convertTo(const TypeSpec& type) const;                          //!< Convert to type
    static const std::string&                       getClassName(void);                                             //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
    virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                    //!< Is this object convertible to the asked one?
    virtual void                                    printValue(std::ostream& o) const;                              //!< Print value for user
    
    valueType&                                      operator[](size_t index);                                       //!< subscript operator
    const valueType&                                operator[](size_t index) const;                                 //!< subscript operator (const)
    RlVector&                                       operator=(const RlVector& x);                                   //!< Assignment operator
    RlVector&                                       operator+=(const rlType& x);                                    //!< Concatenate
    RlVector&                                       operator+=(const RlVector& x);                                  //!< Concatenate
    const RlVector                                  operator+(const rlType& x) const;                               //!< Concatenate
    const RlVector                                  operator+(const RlVector& x) const;                             //!< Concatenate
    bool                                            operator==(const RlVector& x) const;                            //!< Equality
    bool                                            operator!=(const RlVector& x) const;                            //!< Inequality
    
    // Overwritten RbObject functions
    virtual void*                                   getLeanValue(std::vector<size_t> &length) const;                //!< Transform the object into a basic element pointer for fast access.
    virtual size_t                                  memorySize() const;                                             //!< Get the size

    
    // Member object function
    RbPtr<RbLanguageObject>                         executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);         //!< Override to map member methods to internal functions
    const MemberRules&                              getMemberRules(void) const;                                     //!< Get member rules
    const MethodTable&                              getMethods(void) const;                                         //!< Get methods
    
    // Vector functions
    iterator                                        begin(void);                                                    //!< Iterator to the beginning of the Vector
    const_iterator                                  begin(void) const;                                              //!< Const-iterator to the beginning of the Vector
    void                                            clear(void);                                                    //!< Clear
    iterator                                        end(void);                                                      //!< Iterator to the end of the Vector
    const_iterator                                  end(void) const;                                                //!< Const-iterator to the end of the Vector
    int                                             findIndex(const RbObject& x) const;                             //!< Find the index if the element being equal to that one
    RbObject                                        getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    const std::vector<valueType>&                   getValue(void) const;                                           //!< Get the stl Vector of elements
    void                                            pop_back(void);                                                 //!< Drop element at back
    void                                            pop_front(void);                                                //!< Drop element from front
    void                                            push_back(const rlType &x);                                     //!< Append element to end
    void                                            push_back(const valueType &x);                                  //!< Append element to end
    void                                            push_front(const rlType &x);                                    //!< Append element to end
    void                                            push_front(const valueType &x);                                 //!< Append element to end
    void                                            resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                                            setElement(const size_t index, RbObject *elem);                 //!< Set element with type conversion
    void                                            sort(void);                                                     //!< sort the AbstractVector
    size_t                                          size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                                            unique(void);                                                   //!< removes consecutive duplicates

protected:
        
    // We store internally pointers to our objects. This is necessary because elements can be also of the derived type and we need to be able to make proper copies of the Vector and all its elements
    std::vector<valueType>                          elements;

private:
    
    MemberRules                                     memberRules;
    MethodTable                                     methods;
    TypeSpec                                        typeSpec;
    
    struct comparator {
        bool operator() (valueType A, valueType B) const { return ( A < B);}
    } myComparator;
};



#include "Complex.h"
#include "ConstArgumentRule.h"
#include "Ellipsis.h"
#include "MethodTable.h"
#include "Monitor.h"
#include "Move.h"
#include "Probability.h"
#include "RbBoolean.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbString.h"
#include "RbUtil.h"
#include "SimpleMemberFunction.h"
#include "TypeSpec.h"

#include <algorithm>

/** Vector type of elements */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>::RlVector( void ) : Container( rlType::getClassTypeSpec() ), typeSpec(getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec()) ) {
    
    // set the member rules
    memberRules.push_back( new ConstArgumentRule( "x"  , elementType ) );
    memberRules.push_back( new Ellipsis( elementType ) );
    
    // set the methods
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ConstArgumentRule( "index" , Natural::getClassTypeSpec() ) );
    methods.addFunction("[]",  new SimpleMemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
    
    // add method for call "x.sort()" as a function
    ArgumentRules* sortArgRules = new ArgumentRules();
    methods.addFunction("sort",  new SimpleMemberFunction( RbVoid_name, sortArgRules) );
    
    // add method for call "x.unique()" as a function
    ArgumentRules* uniqueArgRules = new ArgumentRules();
    methods.addFunction("unique",  new SimpleMemberFunction( RbVoid_name, uniqueArgRules) );
    
    // necessary call for proper inheritance
    methods.setParentTable( &Container::getMethods() );
}


/** Constructor with dimension (n) and NULL pointers to every object */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>::RlVector(size_t n) : Container( rlType::getClassTypeSpec() ), typeSpec(getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) )  {
    
    // set the member rules
    memberRules.push_back( new ConstArgumentRule( "x"  , elementType ) );
    memberRules.push_back( new Ellipsis( elementType ) );
    
    // set the methods
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ConstArgumentRule( "index" , Natural::getClassTypeSpec() ) );
    methods.addFunction("[]",  new SimpleMemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
    
    // add method for call "x.sort()" as a function
    ArgumentRules* sortArgRules = new ArgumentRules();
    methods.addFunction("sort",  new SimpleMemberFunction( RbVoid_name, sortArgRules) );
    
    // add method for call "x.unique()" as a function
    ArgumentRules* uniqueArgRules = new ArgumentRules();
    methods.addFunction("unique",  new SimpleMemberFunction( RbVoid_name, uniqueArgRules) );
    
    // necessary call for proper inheritance
    methods.setParentTable( &Container::getMethods() );
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( NULL );
    }
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>::RlVector(size_t n, const valueType &x) : Container( rlType::getClassTypeSpec() ), typeSpec(getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( elementType) )  {
    
    // set the member rules
    memberRules.push_back( new ConstArgumentRule( "x"  , elementType ) );
    memberRules.push_back( new Ellipsis( elementType ) );
    
    // set the methods
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ConstArgumentRule( "index" , Natural::getClassTypeSpec() ) );
    methods.addFunction("[]",  new SimpleMemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
    
    // add method for call "x.sort()" as a function
    ArgumentRules* sortArgRules = new ArgumentRules();
    methods.addFunction("sort",  new SimpleMemberFunction( RbVoid_name, sortArgRules) );
    
    // add method for call "x.unique()" as a function
    ArgumentRules* uniqueArgRules = new ArgumentRules();
    methods.addFunction("unique",  new SimpleMemberFunction( RbVoid_name, uniqueArgRules) );
    
    // necessary call for proper inheritance
    methods.setParentTable( &Container::getMethods() );
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( x->clone() );
    }
}



/** Copy Constructor */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>::RlVector(const RlVector<valueType, rlType> &v) : Container(v), memberRules( v.memberRules ), methods( v.methods ), typeSpec( v.typeSpec ) {
    
    typename std::vector<valueType>::const_iterator it;
    // copy all the elements by deep copy
    for ( it = v.elements.begin(); it != v.elements.end(); it++) {
        elements.push_back( *it );
    }
    
}


/** Destructor. Free the memory of the elements. */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>::~RlVector(void) {
    
    // just call clear which will free the memory of the elements
    clear();
}

/** Assignment operator; make sure we get independent elements */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>& RlVector<valueType, rlType>::operator=( const RlVector<valueType, rlType>& x ) {
    
    if ( this != &x ) {
        
        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        Container::operator=( x );
        
        // just call clear which will free the memory of the objects
        clear();
        
        std::vector<RbObject *>::const_iterator i;
        for ( i = x.elements.begin(); i != x.elements.end(); i++ ) {
            
            RbObject *element = NULL;
            if ( *i != NULL) {
                element = (*i)->clone();
            }
            elements.push_back( element );
        }
        
        memberRules     = x.memberRules;
        methods         = x.methods;
        typeSpec        = x.typeSpec;
    }
    
    return ( *this );
}


/* Subscript operator */
template <typename valueType, typename rlType>
valueType& RlVector<valueType, rlType>::operator[]( size_t index ) {
    
    return elements[index];
}


/* Subscript operator */
template <typename valueType, typename rlType>
const valueType& RlVector<valueType, rlType>::operator[]( size_t index ) const {
    
    return elements[index];
}


/** Concatenation with operator+ (valueType) */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>& RlVector<valueType, rlType>::operator+=( const rlType& x ) {
    
    push_back( x.clone() );
    
    return *this;
}


/** Concatenation with operator+ (RlVector) */
template <typename valueType, typename rlType>
RlVector<valueType, rlType>& RlVector<valueType, rlType>::operator+=( const RlVector<valueType, rlType>& x ) {
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        push_back( x[i].clone() );
    
    return *this;
}


/** Equals comparison */
template <typename valueType, typename rlType>
bool RlVector<valueType, rlType>::operator==(const RlVector<valueType, rlType>& x) const {
    
    if (size() != x.size())
        return false;
    
    for (size_t i=0; i<elements.size(); i++) {
        if (elements[i] != x[i])
            return false;
    }
    
    return Container::operator==( x );
}


/** Not equals comparison */
template <typename valueType, typename rlType>
bool RlVector<valueType, rlType>::operator!=(const RlVector<valueType, rlType>& x) const {
    
    return !operator==(x);
}


/** Concatenation with operator+ (valueType) */
template <typename valueType, typename rlType>
const RlVector<valueType, rlType> RlVector<valueType, rlType>::operator+( const rlType& x ) const {
    
    RlVector tempVec = *this;
    
    tempVec.push_back( x.getValue() );
    
    return tempVec;
}


/** Concatenation with operator+ (RlVector) */
template <typename valueType, typename rlType>
const RlVector<valueType, rlType> RlVector<valueType, rlType>::operator+( const RlVector<valueType, rlType>& x ) const {
    
    RlVector<valueType, rlType> tempVec = *this;
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i] );
    
    return tempVec;
}


/** Get iterator to the beginning of the Vector. */
template <typename valueType, typename rlType>
typename std::vector<valueType>::iterator RlVector<valueType, rlType>::begin( void ) {
    return elements.begin();
}


/** Get iterator to the beginning of the Vector. */
template <typename valueType, typename rlType>
typename std::vector<valueType>::const_iterator RlVector<valueType, rlType>::begin( void ) const {
    return elements.begin();
}


/** Convertible to: default implementation */
template <typename valueType, typename rlType>
RbObject* RlVector<valueType, rlType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        RlVector<valueType, rlType> *convObject = new RlVector<valueType, rlType>();
        
        //  convert all objects. clone if they are of the right type, otherwise convert them
        throw RbException("Missing implementation in RlVector::convert()");
//        typename std::vector<valueType>::const_iterator i;
//        for ( i = begin(); i != end(); i++) {
//            RbObject* orgElement = *i;
//            // test whether this element is already of the right type
//            if ( orgElement->isTypeSpec(type.getElementType()) ) {
//                convObject->push_back( orgElement->clone() );
//            }
//            else {
//                convObject->push_back( orgElement->convertTo(type.getElementType() ) );
//            }
//        }
        
        return convObject;
        
    }
    
    return Container::convertTo(type);
}



/** Clear contents of value container */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::clear( void ) {
    
    typename std::vector<valueType>::iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
//        RbObject* theElement = *i;
//        delete theElement;
    }
    
    elements.clear();
}


template <typename valueType, typename rlType>
RlVector<valueType, rlType>* RlVector<valueType, rlType>::clone() const {
    return new RlVector<valueType, rlType>( *this );
}


/** Get iterator to the end of the Vector. */
template <typename valueType, typename rlType>
typename std::vector<valueType>::iterator RlVector<valueType, rlType>::end( void ) {
    return elements.end();
}


/** Get iterator to the end of the Vector. */
template <typename valueType, typename rlType>
typename std::vector<valueType>::const_iterator RlVector<valueType, rlType>::end( void ) const {
    return elements.end();
}


/** Execute member function. */
template <typename valueType, typename rlType>
RbPtr<RbLanguageObject> RlVector<valueType, rlType>::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if ( name == "sort" ) {
        sort();
        
        return NULL;
    }
    else if ( name == "unique" ) {
        unique();
        
        return NULL;
    }
    
    return Container::executeSimpleMethod(name, args);
}



/**
 * Find the index of the given element.
 * We rely on overloaded operator== in the element classes to check for matches.
 * 
 * \param x the element we are looking for. 
 * \return The index or -1 if we didn't find it.
 */
template <typename valueType, typename rlType>
int RlVector<valueType, rlType>::findIndex(const RbObject& x) const {
    
    // get the iterator to the first element
    typename std::vector<valueType>::const_iterator i;
    
    // initialize the index
    int index = 0;
    for ( i = elements.begin(); i != elements.end(); i++, index++) {
        const RbObject& element = *(*i);
        
        // test if the object matches
        // note that we rely on the implemented operator==
        if ( element == x ) {
            return index;
        }
    }
    
    return -1;
}


/* Get class name of object */
template <typename valueType, typename rlType>
const std::string& RlVector<valueType, rlType>::getClassName(void) { 
    
    static std::string rbClassName = "Vector";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename valueType, typename rlType>
const TypeSpec& RlVector<valueType, rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
template <typename valueType, typename rlType>
RbObject RlVector<valueType, rlType>::getElement(size_t index) {
    
    return rlType( elements[index] );
}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
template <typename valueType, typename rlType>
const MemberRules& RlVector<valueType, rlType>::getMemberRules(void) const {
    
    return memberRules;
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename valueType, typename rlType>
const MethodTable& RlVector<valueType, rlType>::getMethods(void) const {
    
    return methods;
}


template <typename valueType, typename rlType>
const std::vector<valueType>& RlVector<valueType, rlType>::getValue(void) const {
    return elements;
}



template <typename valueType, typename rlType>
void* RlVector<valueType, rlType>::getLeanValue( std::vector<size_t> &lengths ) const {
    
    return const_cast<std::vector<valueType> *>( &elements );
}

/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename valueType, typename rlType>
const TypeSpec& RlVector<valueType, rlType>::getTypeSpec(void) const {
    
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename valueType, typename rlType>
bool RlVector<valueType, rlType>::isConvertibleTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
//    if ( type.getBaseType() == getClassName() ) {
//        
//        // work through all the elements
//        typename std::vector<valueType>::const_iterator i;
//        for ( i = begin(); i != end(); i++) {
//            rlType orgElement( *i );
//            // test whether this element is already of the right type
//            if ( !orgElement.isTypeSpec(type.getElementType()) && !orgElement.isConvertibleTo(type.getElementType()) ) {
//                return false;
//            }
//        }
//        
//        return true;
//    }
    
    return false;
}


/** Print value for user */
template <typename valueType, typename rlType>
size_t RlVector<valueType, rlType>::memorySize( void ) const {
    
    return sizeof( std::vector<valueType> );
}


/** Print value for user */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::printValue( std::ostream& o ) const {
    
    o << "[ ";
    typename std::vector<valueType>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        rlType(*i).printValue(o);
    }
    o <<  " ]";
    
}


/** Pop element off of front of vector, updating length in process */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Pop element off of back of vector, updating length in process */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::pop_back(void) {
    
    elements.pop_back();
}


/** Push an int onto the back of the vector */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::push_back( const rlType &x ) {
    
    elements.push_back( x.getValue() );
    
}


/** Push an int onto the back of the vector */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::push_back( const valueType &x ) {
    
    elements.push_back( x );
    
}


/** Push an int onto the front of the vector */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::push_front( const rlType &x ) {
    
    elements.insert( elements.begin(), x.getValue() );
}


/** Push an int onto the front of the vector */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::push_front( const valueType &x ) {
    
    elements.insert( elements.begin(), x );
}


/** Resize vector */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::resize( size_t n ) {
    
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );
    
    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( NULL );
}


/* Set element */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::setElement(const size_t index, RbObject *elem) {
    if (index >= elements.size()) {
        throw RbException("Cannot set element in Vector outside the current range.");
    }
    
    // remove first the old element at the index
    RbObject* oldElement = elements[index];
    delete oldElement;
    elements.erase(elements.begin()+index);
    
    elements.insert(elements.begin()+index, elem);
}


/** Get the size of the vector */
template <typename valueType, typename rlType>
size_t RlVector<valueType, rlType>::size( void ) const {
    
    return elements.size();
    
}


/* Sort the vector */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::sort( void ) {
    
    std::sort(elements.begin(), elements.end(), myComparator);
    
}

/* Remove duplicates and resize the vector */
template <typename valueType, typename rlType>
void RlVector<valueType, rlType>::unique(void) {
    
    sort();
    std::vector<valueType> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if ( elements[i] != elements[i-1] )
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}



#endif