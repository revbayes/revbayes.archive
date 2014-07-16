/**
 * @file
 * This file contains the declaration of Vector of pointers. It is used to store pointers of objects instead of the objects themselves.
 * Pointers intead of objects is necessary if working with base classes.
 *
 * This class is a wrapper for the stl-class Vector and we provide additional RevBayes functionality
 * (e.g. getTypeSpec and getClassTypeSpec for argument checking)
 *
 *
 * @brief Declaration of VectorRbPointer
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-31 17:34:45 +0200 (Fri, 31 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id: Vector.h 1773 2012-08-31 15:34:45Z hoehna $
 */

#ifndef VectorAbstractElement_H
#define VectorAbstractElement_H

#include "RbVector.h"
#include "TypedContainer.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    template <typename rlType>
    class VectorAbstractElement : public TypedContainer< RevBayesCore::RbVector<typename rlType::valueType> > {
        
    public:
        
        typedef typename rlType::valueType                      elementType;
        typedef typename RevBayesCore::RbVector<elementType>    valueType;
        
        VectorAbstractElement(void);                                                                                                   //!< Default constructor
        VectorAbstractElement(const valueType &v);                                                                                     //!< Constructor with values
        
        // overloaded operators
        elementType&                                    operator[](size_t index);                                       //!< subscript operator
        const elementType&                              operator[](size_t index) const;                                 //!< subscript operator (const)
        //        Vector&                                         operator=(const Vector& x);                                     //!< Assignment operator
        //        Vector&                                         operator+=(const rlType& x);                                    //!< Concatenate
        //        Vector&                                         operator+=(const Vector& x);                                    //!< Concatenate
        //        const Vector                                    operator+(const rlType& x) const;                               //!< Concatenate
        //        const Vector                                    operator+(const Vector& x) const;                               //!< Concatenate
        //        bool                                            operator==(const Vector& x) const;                              //!< Equality
        //        bool                                            operator!=(const Vector& x) const;                              //!< Inequality
        
        // Basic utility functions 
        VectorAbstractElement*                          clone(void) const;                                              //!< Clone object
        RevObject*                                      convertTo(const TypeSpec& type) const;                          //!< Convert to type
        static const std::string&                       getClassName(void);                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
        virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                    //!< Is this object convertible to the asked one?
        
        // Member object function
        //        RevObject*                               executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        const MemberRules&                              getMemberRules(void) const;                                     //!< Get member rules
        const MethodTable&                              getMethods(void) const;                                         //!< Get methods
        
        // Container functions
        RevObject*                                      getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
        void                                            push_back(const rlType &x);                                     //!< Append element to end
        void                                            push_back(elementType *x);                                      //!< Append element to end
        void                                            push_front(const rlType &x);                                    //!< Append element to end
        void                                            push_front(elementType *x);                                     //!< Append element to end
        
    private:

    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Monitor.h"
#include "Move.h"
#include "Natural.h"
#include "Probability.h"
#include "RlBoolean.h"
#include "RbException.h"
#include "RlString.h"
#include "RbUtil.h"
#include "RlUtils.h"
#include "TypeSpec.h"

#include <algorithm>

/** Vector type of elements */
template <typename rlType>
RevLanguage::VectorAbstractElement<rlType>::VectorAbstractElement( void ) : TypedContainer< RevBayesCore::RbVector<typename rlType::valueType> >( rlType::getClassTypeSpec() ) {
    
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RevLanguage::VectorAbstractElement<rlType>::VectorAbstractElement(const valueType &v) : TypedContainer< RevBayesCore::RbVector<typename rlType::valueType> >( rlType::getClassTypeSpec(), v )  {
    
}


///** Assignment operator; make sure we get independent elements */
//template <typename rlType>
//RevLanguage::Vector<rlType>& RevLanguage::Vector<rlType>::operator=( const Vector<rlType>& x ) {
//    
//    if ( this != &x ) {
//        
//        // First assign using parent assignment operator. This will test to make sure the containers
//        // are of the same type, and throw an error if they are not. By calling it before we destroy
//        // our own elements, we can make sure that an assignment error leaves us intact, which it should
//        TypedContainer<std::vector<typename rlType::valueType> >::operator=( x );
//        
//        // just call clear which will free the memory of the objects
//        clear();
//    }
//    
//    return ( *this );
//}


/* Subscript operator */
template <typename rlType>
typename rlType::valueType& RevLanguage::VectorAbstractElement<rlType>::operator[]( size_t index ) {
    
    return *this->value->getValue()[index];
}


/* Subscript operator */
template <typename rlType>
const typename rlType::valueType& RevLanguage::VectorAbstractElement<rlType>::operator[]( size_t index ) const {
    
    return *this->value->getValue()[index];
}


///** Concatenation with operator+ (valueType) */
//template <typename rlType>
//RevLanguage::Vector<rlType>& RevLanguage::Vector<rlType>::operator+=( const rlType& x ) {
//    
//    push_back( x.clone() );
//    
//    return *this;
//}
//
//
///** Concatenation with operator+ (Vector) */
//template <typename rlType>
//RevLanguage::Vector<rlType>& RevLanguage::Vector<rlType>::operator+=( const Vector<rlType>& x ) {
//    
//    for ( size_t i = 0; i < x.elements.size(); i++ )
//        push_back( x[i].clone() );
//    
//    return *this;
//}


///** Concatenation with operator+ (valueType) */
//template <typename rlType>
//const RevLanguage::Vector<rlType> RevLanguage::Vector<rlType>::operator+( const rlType& x ) const {
//    
//    Vector tempVec = *this;
//    
//    tempVec.push_back( x.getValue() );
//    
//    return tempVec;
//}
//
//
///** Concatenation with operator+ (Vector) */
//template <typename rlType>
//const RevLanguage::Vector<rlType> RevLanguage::Vector<rlType>::operator+( const Vector<rlType>& x ) const {
//    
//    Vector<rlType> tempVec = *this;
//    
//    for ( size_t i = 0; i < x.elements.size(); i++ )
//        tempVec.push_back( x[i] );
//    
//    return tempVec;
//}


/** Convertible to: default implementation */
template <typename rlType>
RevLanguage::RevObject* RevLanguage::VectorAbstractElement<rlType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
    }
    
    throw RbException("Cannot convert pointer vectors.");
    
//    return this->TypedContainer<std::vector<typename rlType::valueType *> >::convertTo(type);
    return NULL;
}


template <typename rlType>
RevLanguage::VectorAbstractElement<rlType>* RevLanguage::VectorAbstractElement<rlType>::clone() const {
    return new VectorAbstractElement<rlType>( *this );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::VectorAbstractElement<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "VectorAbstractElement";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::VectorAbstractElement<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
template <typename rlType>
RevLanguage::RevObject* RevLanguage::VectorAbstractElement<rlType>::getElement(size_t index) {
    
//    return this->value->getValue()[index];
    throw RbException("Cannot access element in pointer vector.");
    return NULL;
}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
template <typename rlType>
const RevLanguage::MemberRules& RevLanguage::VectorAbstractElement<rlType>::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( ! rulesSet ) {
        // set the member rules
        memberRules.push_back( new ArgumentRule( "x", true, rlType::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis( rlType::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::VectorAbstractElement<rlType>::getMethods(void) const {
    
    static MethodTable methods;
    static bool methodsSet = false;
    
    if (!methodsSet) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
//        methods.setParentTable( &TypedContainer<std::vector<elementType> *>::getMethods() );
        
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::VectorAbstractElement<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename rlType>
bool RevLanguage::VectorAbstractElement<rlType>::isConvertibleTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
//        typename std::vector<elementType *>::const_iterator i;
//        for ( i = this->begin(); i != this->end(); i++) {
//            rlType orgElement = rlType(*i);
//            // test whether this element is already of the right type
//            if ( !orgElement.isTypeSpec(type.getElementType()) && !orgElement.isConvertibleTo(type.getElementType()) ) {
//                return false;
//            }
//        }
//        
//        return true;
    }
    
    return Container::isConvertibleTo( type );
}


///** Pop element off of front of vector, updating length in process */
//template <typename rlType>
//void RevLanguage::Vector<rlType>::pop_front(void) {
//    
//    elements.erase(elements.begin());
//}
//
//
///** Pop element off of back of vector, updating length in process */
//template <typename rlType>
//void RevLanguage::Vector<rlType>::pop_back(void) {
//    
//    elements.pop_back();
//}


/** Push an int onto the back of the vector */
template <typename rlType>
void RevLanguage::VectorAbstractElement<rlType>::push_back( const rlType &x ) {
    
    std::vector<typename rlType::valueType *>& val = this->value;
    const typename rlType::valueType &element = x.getValue();
    val.push_back( element.clone() );
    
}


/** Push an int onto the back of the vector */
template <typename rlType>
void RevLanguage::VectorAbstractElement<rlType>::push_back( typename rlType::valueType *x ) {
    
    this->value->getValue().push_back( x );
    
}


/** Push an int onto the front of the vector */
template <typename rlType>
void RevLanguage::VectorAbstractElement<rlType>::push_front( const rlType &x ) {
    
    this->value->getValue().insert( this->value->getValue().begin(), x.getValue() );
}


/** Push an int onto the front of the vector */
template <typename rlType>
void RevLanguage::VectorAbstractElement<rlType>::push_front( typename rlType::valueType *x ) {
    
    this->value->getValue().insert( this->value->getValue().begin(), x );
}


///** Resize vector */
//template <typename rlType>
//void RevLanguage::Vector<rlType>::resize( size_t n ) {
//    
//    if ( n < elements.size() )
//        throw RbException( "Invalid attempt to shrink vector" );
//    
//    for ( size_t i = elements.size(); i < n; i++ )
//        elements.push_back( NULL );
//}
//
//
///* Set element */
//template <typename rlType>
//void RevLanguage::Vector<rlType>::setElement(const size_t index, RevObject *elem) {
//    if (index >= elements.size()) {
//        throw RbException("Cannot set element in Vector outside the current range.");
//    }
//    
//    // remove first the old element at the index
//    elements.erase(elements.begin()+index);
//  
//    throw RbException("Missing implementation of Vector::setElement()");
////    elements.insert(elements.begin()+index, *elem);
//}

#endif
