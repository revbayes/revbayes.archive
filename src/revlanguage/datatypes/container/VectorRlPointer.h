/**
 * @file
 * This file contains the declaration of Vector of pointers. It is used to store pointers of objects instead of the objects themselves.
 * Pointers intead of objects in necessary if working with base classes.
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

#ifndef VectorRlPointer_H
#define VectorRlPointer_H

#include "TypedWorkspaceObjectContainer.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    template <typename rlType>
    class VectorRlPointer : public TypedWorkspaceObjectContainer<std::vector<rlType *> > {
        
    public:
        
//        typedef typename rlType elementType;
//        typedef typename std::vector<elementType*> valueType;
        
        VectorRlPointer(void);                                                                                                   //!< Default constructor
        VectorRlPointer(const rlType &v);                                                                                     //!< Constructor with values
        
        // overloaded operators
        rlType&                                         operator[](size_t index);                                       //!< subscript operator
        const rlType&                                   operator[](size_t index) const;                                 //!< subscript operator (const)
        
        // Basic utility functions 
        VectorRlPointer*                                clone(void) const;                                              //!< Clone object
        RevObject*                               convertTo(const TypeSpec& type) const;                          //!< Convert to type
        static const std::string&                       getClassName(void);                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
        virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                    //!< Is this object convertible to the asked one?
        
        // Member object function
        RevObject*                               executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
//        const MemberRules&                              getMemberRules(void) const;                                     //!< Get member rules
        const MethodTable&                              getMethods(void) const;                                         //!< Get methods
        
        // Container functions
        RevObject*                               getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
        void                                            push_back(rlType *x);                                           //!< Append element to end
        void                                            push_front(rlType *x);                                          //!< Append element to end
        
    private:
        
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
RevLanguage::VectorRlPointer<rlType>::VectorRlPointer( void ) : TypedWorkspaceObjectContainer<std::vector<rlType *> >( rlType::getClassTypeSpec() ) {
    
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RevLanguage::VectorRlPointer<rlType>::VectorRlPointer(const rlType &v) : TypedWorkspaceObjectContainer<std::vector<rlType *> >( rlType::getClassTypeSpec(), v )  {
    
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
rlType& RevLanguage::VectorRlPointer<rlType>::operator[]( size_t index ) {
    
    return *this->value[index];
}


/* Subscript operator */
template <typename rlType>
const rlType& RevLanguage::VectorRlPointer<rlType>::operator[]( size_t index ) const {
    
    return *this->value[index];
}


/** Convertible to: default implementation */
template <typename rlType>
RevLanguage::RevObject* RevLanguage::VectorRlPointer<rlType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
    }
    
    throw RbException("Cannot convert pointer vectors.");
    
    //    return this->TypedContainer<std::vector<typename rlType::valueType *> >::convertTo(type);
    return NULL;
}


template <typename rlType>
RevLanguage::VectorRlPointer<rlType>* RevLanguage::VectorRlPointer<rlType>::clone() const {
    return new VectorRlPointer<rlType>( *this );
}


/* Map calls to member methods */
template <typename rlType>
RevLanguage::RevObject* RevLanguage::VectorRlPointer<rlType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if ( name == "[]") {
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getRevObject() );
        
        if (this->size() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
        
        RevObject* element = getElement( size_t(index.getValue()) - 1);
        return element;
    } 
    
    return Container::executeMethod( name, args );
}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::VectorRlPointer<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "VectorRlPointer";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::VectorRlPointer<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
template <typename rlType>
RevLanguage::RevObject* RevLanguage::VectorRlPointer<rlType>::getElement(size_t index) {
    
    return this->value[index]->clone();
}


/** Get the methods for this vector class */
/* Get method specifications */
template <typename rlType>
const RevLanguage::MethodTable& RevLanguage::VectorRlPointer<rlType>::getMethods(void) const {
    
    static MethodTable methods;
    static bool methodsSet = false;
    
    if (!methodsSet) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &TypedWorkspaceObjectContainer<std::vector<rlType *> >::getMethods() );
        
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::VectorRlPointer<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename rlType>
bool RevLanguage::VectorRlPointer<rlType>::isConvertibleTo(const TypeSpec &type) const {
    
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
void RevLanguage::VectorRlPointer<rlType>::push_back( rlType *x ) {
    
    std::vector<rlType *>& val = this->value;
    val.push_back( x );
    
}


/** Push an int onto the front of the vector */
template <typename rlType>
void RevLanguage::VectorRlPointer<rlType>::push_front( rlType *x ) {
    
    this->value->getValue().insert( this->value.begin(), x );
}


#endif
