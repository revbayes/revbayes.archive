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

#ifndef VectorRbPointer_H
#define VectorRbPointer_H

#include "TypedWorkspaceObjectContainer.h"
#include "RbVector.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    template <typename rlType>
    class VectorRbPointer : public TypedWorkspaceObjectContainer< RevBayesCore::RbVector<typename rlType::valueType> > {
        
    public:
        
        typedef typename rlType::valueType elementType;
        typedef typename RevBayesCore::RbVector<elementType> valueType;
        
        VectorRbPointer(void);                                                                                                   //!< Default constructor
       
        // overloaded operators
        rlType&                                         operator[](size_t index);                                       //!< subscript operator
        const rlType&                                   operator[](size_t index) const;                                 //!< subscript operator (const)
        //        Vector&                                         operator=(const Vector& x);                                     //!< Assignment operator
        //        Vector&                                         operator+=(const rlType& x);                                    //!< Concatenate
        //        Vector&                                         operator+=(const Vector& x);                                    //!< Concatenate
        //        const Vector                                    operator+(const rlType& x) const;                               //!< Concatenate
        //        const Vector                                    operator+(const Vector& x) const;                               //!< Concatenate
        //        bool                                            operator==(const Vector& x) const;                              //!< Equality
        //        bool                                            operator!=(const Vector& x) const;                              //!< Inequality
        
        // Basic utility functions 
        VectorRbPointer*                                clone(void) const;                                              //!< Clone object
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
        rlType*                                         getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
        rlType*                                         getElement(size_t index) const;                                 //!< Get element (const to return const element)
//        RevBayesCore::RbVector<elementType>             getInternalValue() const;                                       //!< Get the vector of internal element
        void                                            push_back(const rlType &x);                                     //!< Append element to the end
        void                                            push_back(rlType *x);                                           //!< Append element to the end
        void                                            push_front(const rlType &x);                                    //!< Append element to the front
        void                                            push_front(rlType *x);                                          //!< Append element to the front
        
    private:
        
        std::vector<rlType*>                            languageVector;
        
    };
    
}


#include "ArgumentRule.h"
#include "Complex.h"
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
RevLanguage::VectorRbPointer<rlType>::VectorRbPointer( void ) : TypedWorkspaceObjectContainer<RevBayesCore::RbVector<typename rlType::valueType> >( rlType::getClassTypeSpec() ) {
    
}


/* Subscript operator */
template <typename rlType>
rlType& RevLanguage::VectorRbPointer<rlType>::operator[]( size_t index ) {
    
    return *this->value[index];
}


/* Subscript operator */
template <typename rlType>
const rlType& RevLanguage::VectorRbPointer<rlType>::operator[]( size_t index ) const {
    
    return *this->languageVector[index];
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
RevLanguage::RevObject* RevLanguage::VectorRbPointer<rlType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
    }
    
    throw RbException("Cannot convert pointer vectors.");
    
    //    return this->TypedContainer<std::vector<typename rlType::valueType *> >::convertTo(type);
    return NULL;
}


template <typename rlType>
RevLanguage::VectorRbPointer<rlType>* RevLanguage::VectorRbPointer<rlType>::clone() const {
    return new VectorRbPointer<rlType>( *this );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::VectorRbPointer<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "VectorRbPointer";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::VectorRbPointer<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
template <typename rlType>
rlType* RevLanguage::VectorRbPointer<rlType>::getElement(size_t index)
{
    
    return this->languageVector[index]->clone();
}


/* Get element */
template <typename rlType>
rlType* RevLanguage::VectorRbPointer<rlType>::getElement(size_t index) const
{
    
    return this->value[index]->clone();
}


///* Get element */
//template <typename rlType>
//RevBayesCore::RbVector<typename rlType::valueType> RevLanguage::VectorRbPointer<rlType>::getInternalValue(void) const
//{
//    
//    return coreVector;
//}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
template <typename rlType>
const RevLanguage::MemberRules& RevLanguage::VectorRbPointer<rlType>::getMemberRules(void) const {
    
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
const RevLanguage::MethodTable& RevLanguage::VectorRbPointer<rlType>::getMethods(void) const {
    
    static MethodTable methods;
    static bool methodsSet = false;
    
    if (!methodsSet)
    {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &TypedWorkspaceObjectContainer<RevBayesCore::RbVector<typename rlType::valueType> >::getMethods() );
        
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::VectorRbPointer<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename rlType>
bool RevLanguage::VectorRbPointer<rlType>::isConvertibleTo(const TypeSpec &type) const {
    
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
void RevLanguage::VectorRbPointer<rlType>::push_back( const rlType &x )
{
    
    RevBayesCore::RbVector<typename rlType::valueType>& val = this->value;
    val.push_back( x.getValue() );
    
    languageVector.push_back( x.clone() );
    
}


/** Push an int onto the back of the vector */
template <typename rlType>
void RevLanguage::VectorRbPointer<rlType>::push_back( rlType *x )
{
    
    RevBayesCore::RbVector<typename rlType::valueType>& val = this->value;
    val.push_back( x->getValue() );
    
    languageVector.push_back( x );
    
}


/** Push an rlType onto the front of the vector */
template <typename rlType>
void RevLanguage::VectorRbPointer<rlType>::push_front( const rlType &x ) {
    
    this->value->insert( this->value->begin(), x.getValue() );
    this->languageVector.insert( this->languageVector.begin(), x.clone() );
}


/** Push an rlType onto the front of the vector */
template <typename rlType>
void RevLanguage::VectorRbPointer<rlType>::push_front( rlType *x ) {
    
    this->value->insert( this->value->begin(), x->getValue() );
    this->languageVector.insert( this->languageVector.begin(), x );
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
