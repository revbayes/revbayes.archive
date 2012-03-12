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

#include <iostream>
#include <vector>


template <typename valueType>
class RbVector : public Container {
    
public:
    
    typedef typename std::vector<valueType*>::iterator iterator;
    typedef typename std::vector<valueType*>::const_iterator const_iterator;
    
    RbVector(void);                                                                                                 //!< Default constructor with type RbLanguageObject
    RbVector(size_t n);                                                                                                 //!< Default constructor with type RbLanguageObject
    RbVector(size_t n, valueType* x);                                                                                                 //!< Default constructor with type RbLanguageObject
    RbVector(const RbVector<valueType>& v);                                                                         //!< Copy Constructor
    
    virtual                                        ~RbVector(void);                                                 //!< Virtual destructor 
    
    // Basic utility functions 
    RbVector*                                       clone(void) const;                                              //!< Clone object
    RbObject*                                       convertTo(const TypeSpec& type) const;                          //!< Convert to type
    static const std::string&                       getClassName(void);                                             //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
    virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                    //!< Is this object convertible to the asked one?
    virtual void                                    printValue(std::ostream& o) const;                              //!< Print value for user
    
    valueType&                                      operator[](size_t i);                                           //!< Index op allowing change
    const valueType&                                operator[](size_t i) const;                                     //!< Const index op
    RbVector<valueType>&                            operator=(const RbVector<valueType>& x);                        //!< Assignment operator
    RbVector<valueType>&                            operator+=(const valueType& x) const;                           //!< Concatenate
    RbVector<valueType>&                            operator+=(const RbVector<valueType>& x) const;                 //!< Concatenate
    const RbVector<valueType>                       operator+(const valueType& x) const;                            //!< Concatenate
    const RbVector<valueType>                       operator+(const RbVector<valueType>& x) const;                  //!< Concatenate
    bool                                            operator==(const RbVector<valueType>& x) const;                 //!< Equality
    bool                                            operator!=(const RbVector<valueType>& x) const;                 //!< Inequality
    
    // Member object function
    const MemberRules&                              getMemberRules(void) const;                                     //!< Get member rules
    const MethodTable&                              getMethods(void) const;                                         //!< Get methods
    void                                            setMemberVariable(const std::string& name, const Variable* var);//!< Vector member variable
    
    // Vector functions
    iterator                                        begin(void);                                                    //!< Iterator to the beginning of the Vector
    const_iterator                                  begin(void) const;                                              //!< Const-iterator to the beginning of the Vector
    void                                            clear(void);                                                    //!< Clear
    iterator                                        end(void);                                                      //!< Iterator to the end of the Vector
    const_iterator                                  end(void) const;                                                //!< Const-iterator to the end of the Vector
    const valueType&                                find(const valueType& x) const;                                 //!< Find the element being equal to that one
    int                                             findIndex(const valueType& x) const;                            //!< Find the index if the element being equal to that one
    const RbObject&                                 getElement(size_t index) const;                                 //!< Get element
    RbObject&                                       getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    const std::vector<valueType* >&                 getValue(void) const;                                           //!< Get the stl Vector of elements
    void                                            pop_back(void);                                                 //!< Drop element at back
    void                                            pop_front(void);                                                //!< Drop element from front
    void                                            push_back(valueType* x);                                        //!< Append element to end
    void                                            push_front(valueType* x);                                       //!< Add element in front
    void                                            resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                                            setElement(const size_t index, valueType* elem);                //!< Set element with type conversion
    void                                            sort(void);                                                     //!< sort the AbstractVector
    size_t                                          size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                                            unique(void);                                                   //!< removes consecutive duplicates

protected:
    
    virtual const RbLanguageObject&                 executeOperationSimple(const std::string& name, const std::vector<Argument>& args);//!< Execute method
    

    // We store internally pointers to our objects. This is necessary because elements can be also of the derived type and we need to be able to make proper copies of the Vector and all its elements
    std::vector<valueType* >                        elements;
};

#include "Ellipsis.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "Vector.h"

/** Vector type of elements */
template <typename valueType>
RbVector<valueType>::RbVector(void) : Container( valueType::getClassTypeSpec() ) {
}


/** Constructor with dimension (n) and NULL pointers to every object */
template <typename valueType>
RbVector<valueType>::RbVector(size_t n) : Container( valueType::getClassTypeSpec() ) {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( NULL );
    }
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename valueType>
RbVector<valueType>::RbVector(size_t n, valueType* x) : Container( valueType::getClassTypeSpec() ) {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( x->clone() );
    }
}



/** Copy Constructor */
template <typename valueType>
RbVector<valueType>::RbVector(const RbVector<valueType> &v) : Container(v) {
    
    typename std::vector<valueType* >::const_iterator it;
    // copy all the elements by deep copy
    for ( it = v.elements.begin(); it != v.elements.end(); it++) {
        // We need to type cast because clone return by default RbObject*
        valueType *copy =  NULL;
        if ( (*it) != NULL )
            copy = static_cast<valueType*>( (*it)->clone() );
        elements.push_back( copy );
    }
    
}


/** Destructor. Free the memory of the elements. */
template <typename valueType>
RbVector<valueType>::~RbVector(void) {
    
    // just call clear which will free the memory of the elements
    clear();
}

/** Assignment operator; make sure we get independent elements */
template <typename valueType>
RbVector<valueType>& RbVector<valueType>::operator=( const RbVector<valueType>& x ) {
    
    if ( this != &x ) {
        
        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        Container::operator=( x );
        
        // just call clear which will free the memory of the objects
        clear();
        
        typename std::vector<valueType*>::const_iterator i;
        for ( i = x.elements.begin(); i != x.elements.end(); i++ ) {
            valueType *element = static_cast<valueType*>( (*i)->clone() );
            elements.push_back( element );
        }
    }
    
    return ( *this );
}


/** Concatenation with operator+ (valueType) */
template <typename valueType>
RbVector<valueType>& RbVector<valueType>::operator+=( const valueType& x ) const {
    
    push_back( x.clone() );
    
    return *this;
}


/** Concatenation with operator+ (RbVector) */
template <typename valueType>
RbVector<valueType>& RbVector<valueType>::operator+=( const RbVector<valueType>& x ) const {
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        push_back( x[i].clone() );
    
    return *this;
}

/** Subscript operator */
template <typename valueType>
valueType& RbVector<valueType>::operator[](size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    
    return *elements[index];
}


/** Subscript const operator */
template <typename valueType>
const valueType& RbVector<valueType>::operator[](size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    return *elements[index];
}


/** Equals comparison */
template <typename valueType>
bool RbVector<valueType>::operator==(const RbVector<valueType>& x) const {
    
    if (size() != x.size())
        return false;
    
    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }
    
    return true;
}


/** Not equals comparison */
template <typename valueType>
bool RbVector<valueType>::operator!=(const RbVector<valueType>& x) const {
    
    return !operator==(x);
}


/** Concatenation with operator+ (valueType) */
template <typename valueType>
const RbVector<valueType> RbVector<valueType>::operator+( const valueType& x ) const {
    
    RbVector<valueType> tempVec = *this;
    
    tempVec.push_back( x.clone() );
    
    return tempVec;
}


/** Concatenation with operator+ (RbVector) */
template <typename valueType>
const RbVector<valueType> RbVector<valueType>::operator+( const RbVector<valueType>& x ) const {
    
    RbVector<valueType> tempVec = *this;
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i].clone() );
    
    return tempVec;
}


/** Get iterator to the beginning of the Vector. */
template <typename valueType>
typename std::vector<valueType*>::iterator RbVector<valueType>::begin( void ) {
    return elements.begin();
}


/** Get iterator to the beginning of the Vector. */
template <typename valueType>
typename std::vector<valueType*>::const_iterator RbVector<valueType>::begin( void ) const {
    return elements.begin();
}


/** Convertible to: default implementation */
template <typename valueType>
RbObject* RbVector<valueType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        
        // RbLanguageObject
        if ( type.getElementType() == RbLanguageObject::getClassTypeSpec() ) {
            RbVector<RbLanguageObject>* convObject = new RbVector<RbLanguageObject>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<valueType*>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                valueType* orgElement = *i;
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( static_cast<RbLanguageObject*>( orgElement->clone() ) );
                }
                else {
                    convObject->push_back( static_cast<RbLanguageObject*>( orgElement->convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
    }
    
    return NULL;
}



/** Clear contents of value container */
template <typename valueType>
void RbVector<valueType>::clear( void ) {
    
    typename std::vector<valueType*>::iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
        valueType* theElement = *i;
        delete theElement;
    }
    
    elements.clear();
}


template <typename valueType>
RbVector<valueType>* RbVector<valueType>::clone() const {
    return new RbVector<valueType>(*this);
}


/**
 * Find the given element.
 * We rely on overloaded operator== in the element classes.
 * 
 * \param x the element we are looking for. 
 * \return The reference to the element we found or RbNullObject
 */
template <typename valueType>
const valueType& RbVector<valueType>::find(const valueType& x) const {
    
    // get the iterator to the first element
    typename std::vector<valueType*>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
        valueType& element = *(*i);
        
        // test if the object matches
        // note that we rely on the implemented operator==
        if ( element == x ) {
            return element;
        }
    }
    
    return RbNullObject::getInstance();
}


/** Get iterator to the end of the Vector. */
template <typename valueType>
typename std::vector<valueType*>::iterator RbVector<valueType>::end( void ) {
    return elements.end();
}


/** Get iterator to the end of the Vector. */
template <typename valueType>
typename std::vector<valueType*>::const_iterator RbVector<valueType>::end( void ) const {
    return elements.end();
}


/** Execute member function. */
template <typename valueType>
const RbLanguageObject& RbVector<valueType>::executeOperationSimple(std::string const &name, const std::vector<Argument> &args) {
    
    if ( name == "sort" ) {
        sort();
        
        return RbNullObject::getInstance();
    }
    
    return Container::executeOperationSimple(name, args);
}



/**
 * Find the index of the given element.
 * We rely on overloaded operator== in the element classes to check for matches.
 * 
 * \param x the element we are looking for. 
 * \return The index or -1 if we didn't find it.
 */
template <typename valueType>
int RbVector<valueType>::findIndex(const valueType& x) const {
    
    // get the iterator to the first element
    typename std::vector<valueType*>::const_iterator i;
    
    // initialize the index
    int index = 0;
    for ( i = elements.begin(); i != elements.end(); i++, index++) {
        const valueType& element = *(*i);
        
        // test if the object matches
        // note that we rely on the implemented operator==
        if ( element == x ) {
            return index;
        }
    }
    
    return -1;
}


/** Get class name of object */
template <typename valueType>
const std::string& RbVector<valueType>::getClassName(void) { 
    
    static std::string rbClassName = "Vector";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename valueType>
const TypeSpec& RbVector<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( valueType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get element */
template <typename valueType>
const RbObject& RbVector<valueType>::getElement(size_t index) const {
    
    return operator[](index);
}


/** Get element */
template <typename valueType>
RbObject& RbVector<valueType>::getElement(size_t index) {
    
    return operator[](index);
}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
template <typename valueType>
const MemberRules& RbVector<valueType>::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesVector = false;
    
    if (!rulesVector) {
        
        memberRules.push_back( new ValueRule( "x"  , valueType::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis( valueType::getClassTypeSpec() ) );
        
        rulesVector = true;
    }
    
    return memberRules;
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename valueType>
const MethodTable& RbVector<valueType>::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( valueType::getClassTypeSpec(), squareBracketArgRules) );
        
        // add method for call "x.sort()" as a function
        ArgumentRules* sortArgRules = new ArgumentRules();
        methods.addFunction("sort",  new MemberFunction( RbVoid_name, sortArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &Container::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


template <typename valueType>
const std::vector<valueType* >& RbVector<valueType>::getValue(void) const {
    return elements;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename valueType>
const TypeSpec& RbVector<valueType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( valueType::getClassTypeSpec() ) );
    
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename valueType>
bool RbVector<valueType>::isConvertibleTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        
        // RbLanguageObject
        if ( type.getElementType() == RbLanguageObject::getClassTypeSpec() ) {
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<valueType*>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                valueType* orgElement = *i;
                // test whether this element is already of the right type
                if ( !orgElement->isTypeSpec(type.getElementType()) && !orgElement->isConvertibleTo(type.getElementType()) ) {
                    return false;
                }
            }
            
            return true;
        }
    }
    
    return false;
}


/** Print value for user */
template <typename valueType>
void RbVector<valueType>::printValue( std::ostream& o ) const {
    
    o << "[ ";
    typename std::vector<valueType*>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        if ( (*i) == NULL )
            o << "NULL";
        else 
            (*i)->printValue(o);
    }
    o <<  " ]";
    
}


/** Pop element off of front of vector, updating length in process */
template <typename valueType>
void RbVector<valueType>::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Pop element off of back of vector, updating length in process */
template <typename valueType>
void RbVector<valueType>::pop_back(void) {
    
    elements.pop_back();
}


/** Push an int onto the back of the vector */
template <typename valueType>
void RbVector<valueType>::push_back( valueType* x ) {
    
    elements.push_back( x );
    
}


/** Push an int onto the front of the vector */
template <typename valueType>
void RbVector<valueType>::push_front( valueType* x ) {
    
    elements.insert( elements.begin(), x );
}


/** Resize vector */
template <typename valueType>
void RbVector<valueType>::resize( size_t n ) {
    
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );
    
    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( NULL );
}


/** Set element */
template <typename valueType>
void RbVector<valueType>::setElement(const size_t index, valueType* elem) {
    if (index >= elements.size()) {
        throw RbException("Cannot set element in Vector outside the current range.");
    }
    
    // remove first the old element at the index
    elements.erase(elements.begin()+index);
    
    elements.insert(elements.begin()+index, elem);
}


/** Vector a member variable */
template <typename valueType>
void RbVector<valueType>::setMemberVariable(const std::string& name, const Variable* var) {
    
    if (name == "x" || name == "" ) { // the ellipsis variables
        valueType* element = static_cast<valueType*>( var->getValue().clone() );
        push_back( element);
    }
    else {
        Container::setMemberVariable(name, var);
    }
}


/** Get the size of the vector */
template <typename valueType>
size_t RbVector<valueType>::size( void ) const {
    
    return elements.size();
    
}


/* Sort the vector */
template <typename valueType>
void RbVector<valueType>::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    
}

/* Remove duplicates and resize the vector */
template <typename valueType>
void RbVector<valueType>::unique(void) {
    
    sort();
    std::vector<valueType* > uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}


#endif