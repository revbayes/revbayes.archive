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

#include "TypedContainer.h"

#include <iostream>
#include <vector>

namespace RevLanguage {

    template <typename rlType>
    class Vector : public TypedContainer<std::vector<typename rlType::valueType> > {
    
    public:
    
        typedef typename rlType::valueType elementType;
        typedef typename std::vector<elementType> valueType;
    
        Vector(void);                                                                                                   //!< Default constructor
        Vector(const valueType &v);                                                                                     //!< Constructor with values
        Vector(RevBayesCore::TypedDagNode<valueType> *v);                                                               //!< Constructor with value node
            
        // overloaded operators
        elementType&                                    operator[](size_t index);                                       //!< subscript operator
        const elementType&                              operator[](size_t index) const;                                 //!< subscript operator (const)
        
        // Basic utility functions 
        Vector*                                         clone(void) const;                                              //!< Clone object
        RbLanguageObject*                               convertTo(const TypeSpec& type) const;                          //!< Convert to type
        static const std::string&                       getClassName(void);                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
        virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                    //!< Is this object convertible to the asked one?
        
        // Member object function
//        RbLanguageObject*                               executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        const MemberRules&                              getMemberRules(void) const;                                     //!< Get member rules
        const MethodTable&                              getMethods(void) const;                                         //!< Get methods
            
        // Container functions
        RbLanguageObject*                               getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
        void                                            push_back(const rlType &x);                                     //!< Append element to end
        void                                            push_back(const elementType &x);                                //!< Append element to end
        void                                            push_front(const rlType &x);                                    //!< Append element to end
        void                                            push_front(const elementType &x);                               //!< Append element to end
        void                                            sort(void);                                                     //!< sort the AbstractVector
        void                                            unique(void);                                                   //!< removes consecutive duplicates

    private:
        struct comparator {
            bool operator() (elementType A, elementType B) const { return ( A < B);}
        } myComparator;
    };
    
//    // Global functions using the class
//    template<class rlType>
//    std::ostream&                       operator<<(std::ostream& o, const std::vector<typename rlType::valueType>& x);                                //!< Overloaded output operator


}


#include "ArgumentRule.h"
#include "Complex.h"
#include "DeterministicNode.h"
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
#include "VectorIndexOperator.h"

#include <algorithm>

/** Vector type of elements */
template <typename rlType>
RevLanguage::Vector<rlType>::Vector( void ) : TypedContainer<std::vector<typename rlType::valueType> >( rlType::getClassTypeSpec() ) {

}


/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RevLanguage::Vector<rlType>::Vector(const valueType &v) : TypedContainer<std::vector<typename rlType::valueType> >( rlType::getClassTypeSpec(), v )  {
    
}



/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RevLanguage::Vector<rlType>::Vector(RevBayesCore::TypedDagNode<valueType> *v) : TypedContainer<std::vector<typename rlType::valueType> >( rlType::getClassTypeSpec(), v )  {

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
typename rlType::valueType& RevLanguage::Vector<rlType>::operator[]( size_t index ) {
    
    return this->value->getValue()[index];
}

/* Subscript operator */
template <typename rlType>
const typename rlType::valueType& RevLanguage::Vector<rlType>::operator[]( size_t index ) const {
    
    return this->value->getValue()[index];
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
RevLanguage::RbLanguageObject* RevLanguage::Vector<rlType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        
        // RbLanguageObject
        if ( type.getElementType() == Integer::getClassTypeSpec() ) {
            std::vector<int> elements;
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = this->begin(); i != this->end(); i++) {
                RbLanguageObject *orgElement = new rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    Integer *itg = static_cast<Integer *>( orgElement );
                    elements.push_back( itg->getValue() );
                }
                else {
                    Integer *convElement = static_cast<Integer*>( orgElement->convertTo(type.getElementType()) );
                    elements.push_back( convElement->getValue() );
                    delete convElement;
                }
                delete orgElement;
            }
            
            Vector<Integer>* convObject = new Vector<Integer>( elements );
            return convObject;
        }       
		else if ( type.getElementType() == Natural::getClassTypeSpec() ) {
            std::vector<int> elements;
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = this->begin(); i != this->end(); i++) {
                RbLanguageObject *orgElement = new rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    Natural *nat = static_cast<Natural *>( orgElement );
                    elements.push_back( nat->getValue() );
                }
                else {
                    Natural *convElement = static_cast<Natural*>( orgElement->convertTo(type.getElementType()) );
                    elements.push_back( convElement->getValue() );
                    delete convElement;
                }
                delete orgElement;
            }
            
            Vector<Natural>* convObject = new Vector<Natural>( elements );
            return convObject;
        }
        else if ( type.getElementType() == Probability::getClassTypeSpec() ) {
            std::vector<double> elements;
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = this->begin(); i != this->end(); i++) {
                RbLanguageObject *orgElement = new rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    Probability *p = static_cast<Probability *>( orgElement );
                    elements.push_back( p->getValue() );
                }
                else {
                    Probability *convElement = static_cast<Probability*>( orgElement->convertTo(type.getElementType()) );
                    elements.push_back( convElement->getValue() );
                    delete convElement;
                }
                delete orgElement;
            }
            
            Vector<Probability>* convObject = new Vector<Probability>( elements );
            return convObject;
        }
        else if ( type.getElementType() == RlBoolean::getClassTypeSpec() ) {
            std::vector<bool> elements;
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = this->begin(); i != this->end(); i++) {
                RbLanguageObject *orgElement = new rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    RlBoolean *b = static_cast<RlBoolean *>( orgElement );
                    elements.push_back( b->getValue() );
                }
                else {
                    RlBoolean *convElement = static_cast<RlBoolean*>( orgElement->convertTo(type.getElementType()) );
                    elements.push_back( convElement->getValue() );
                    delete convElement;
                }
                delete orgElement;
            }
            
            Vector<RlBoolean>* convObject = new Vector<RlBoolean>( elements );
            return convObject;
        }
        else if ( type.getElementType() == RlString::getClassTypeSpec() ) {
            std::vector<std::string> elements;
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = this->begin(); i != this->end(); i++) {
                RbLanguageObject *orgElement = new rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    RlString *s = static_cast<RlString *>( orgElement );
                    elements.push_back( s->getValue() );
                }
                else {
                    RlString *convElement = static_cast<RlString*>( orgElement->convertTo(type.getElementType()) );
                    elements.push_back( convElement->getValue() );
                    delete convElement;
                }
                delete orgElement;
            }
            
            Vector<RlString>* convObject = new Vector<RlString>( elements );
            return convObject;
        }
        else if ( type.getElementType() == Real::getClassTypeSpec() ) {
            std::vector<double> elements;
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = this->begin(); i != this->end(); i++) {
                RbLanguageObject *orgElement = new rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    Real *r = static_cast<Real *>( orgElement );
                    elements.push_back( r->getValue() );
                }
                else {
                    Real *convElement = static_cast<Real*>( orgElement->convertTo(type.getElementType()) );
                    elements.push_back( convElement->getValue() );
                    delete convElement;
                }
                delete orgElement;
            }
            
            Vector<Real>* convObject = new Vector<Real>( elements );
            return convObject;
        }
        else if ( type.getElementType() == RealPos::getClassTypeSpec() ) {
            std::vector<double> elements;
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = this->begin(); i != this->end(); i++) {
                RbLanguageObject *orgElement = new rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement->isTypeSpec(type.getElementType()) ) {
                    RealPos *rp = static_cast<RealPos *>( orgElement );
                    elements.push_back( rp->getValue() );
                }
                else {
                    RealPos *convElement = static_cast<RealPos*>( orgElement->convertTo(type.getElementType()) );
                    elements.push_back( convElement->getValue() );
                    delete convElement;
                }
                delete orgElement;
            }
            
            Vector<RealPos>* convObject = new Vector<RealPos>( elements );
            return convObject;
        }
//        else if ( type.getElementType() == Complex::getClassTypeSpec() ) {
//            Vector<Complex>* convObject = new Vector<Complex>();
//            // insert copies of all objects. clone if they are of the right type, otherwise convert them
//            typename std::vector<elementType>::const_iterator i;
//            for ( i = begin(); i != end(); i++) {
//                rlType orgElement = rlType( *i );
//                // test whether this element is already of the right type
//                if ( orgElement.isTypeSpec(type.getElementType()) ) {
//                    convObject->push_back( dynamic_cast<Complex &>( orgElement ) );
//                }
//                else {
//                    convObject->push_back( * static_cast<Complex*>( orgElement.convertTo(type.getElementType() ) ) );
//                }
//            }
//            
//            return convObject;
//        }
        
    }
    
    return this->TypedContainer<std::vector<typename rlType::valueType> >::convertTo(type);
//    return NULL;
}


template <typename rlType>
RevLanguage::Vector<rlType>* RevLanguage::Vector<rlType>::clone() const {
    return new Vector<rlType>( *this );
}

//
///* Map calls to member methods */
//template <typename rlType>
//RevLanguage::RbLanguageObject* RevLanguage::Vector<rlType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
//    
//    if ( name == "[]") {
//        // get the member with give index
//        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getValue() );
//        
//        
//        RevBayesCore::VectorIndexOperator<elementType>* f = new RevBayesCore::VectorIndexOperator<elementType>( this->value, index.getValueNode() );
//        RevBayesCore::DeterministicNode<elementType> *detNode = new RevBayesCore::DeterministicNode<elementType>("", f);
//        
//        rlType* value = new rlType( detNode );
//    } 
//    
//    return TypedContainer<std::vector<elementType> >::executeMethod( name, args );
//}


/* Get class name of object */
template <typename rlType>
const std::string& RevLanguage::Vector<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "Vector";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::Vector<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
template <typename rlType>
RevLanguage::RbLanguageObject* RevLanguage::Vector<rlType>::getElement(size_t index) {
    
    return new rlType( this->value->getValue()[index] );
}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
template <typename rlType>
const RevLanguage::MemberRules& RevLanguage::Vector<rlType>::getMemberRules(void) const {
    
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
const RevLanguage::MethodTable& RevLanguage::Vector<rlType>::getMethods(void) const {
    
    static MethodTable methods;
    static bool methodsSet = false;
    
    if (!methodsSet) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("clamp", new MemberFunction( RlUtils::Void, clampArgRules) );
        
        ArgumentRules* setValueArgRules = new ArgumentRules();
        setValueArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("setValue", new MemberFunction( RlUtils::Void, setValueArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &TypedContainer<std::vector<elementType> >::getMethods() );
        
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::Vector<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename rlType>
bool RevLanguage::Vector<rlType>::isConvertibleTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        typename std::vector<elementType>::const_iterator i;
        for ( i = this->begin(); i != this->end(); i++) {
            rlType orgElement = rlType(*i);
            // test whether this element is already of the right type
            if ( !orgElement.isTypeSpec(type.getElementType()) && !orgElement.isConvertibleTo(type.getElementType()) ) {
                return false;
            }
        }
        
        return true;
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
void RevLanguage::Vector<rlType>::push_back( const rlType &x ) {
    
    std::vector<typename rlType::valueType>& val = this->value->getValue();
    const typename rlType::valueType &element = x.getValue();
    val.push_back( element );
    
}


/** Push an int onto the back of the vector */
template <typename rlType>
void RevLanguage::Vector<rlType>::push_back( const typename rlType::valueType &x ) {
    
    this->value->getValue().push_back( x );
    
}


/** Push an int onto the front of the vector */
template <typename rlType>
void RevLanguage::Vector<rlType>::push_front( const rlType &x ) {
    
    this->value->getValue().insert( this->value->getValue().begin(), x.getValue() );
}


/** Push an int onto the front of the vector */
template <typename rlType>
void RevLanguage::Vector<rlType>::push_front( const typename rlType::valueType &x ) {
    
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
//void RevLanguage::Vector<rlType>::setElement(const size_t index, RbLanguageObject *elem) {
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

/* Sort the vector */
template <typename rlType>
void RevLanguage::Vector<rlType>::sort( void ) {
    
    std::sort( this->begin(), this->end(), myComparator);
    
}

/* Remove duplicates and resize the vector */
template <typename rbType>
void RevLanguage::Vector<rbType>::unique(void) {
    
    sort();
    valueType uniqueVector;
    uniqueVector.push_back (this->value->getValue()[0]);
    for (size_t i = 1 ; i<this->size() ; i++) {
        if ( this->value->getValue()[i] != this->value->getValue()[i-1] )
            uniqueVector.push_back(this->value->getValue()[i]);
    }
    
    this->clear();
//    this->setValues(uniqueVector);
    throw RbException("Missing implementation in unique() in Vector.");
    return;
    
}



//template <typename rlType>
//std::ostream& RevLanguage::operator<<(std::ostream& o, const std::vector<typename rlType::valueType>& x) {
//    o << "(";
//    size_t n = x.size();
//    for (size_t i = 0; i < n; ++i) {
//        if ( i > 0 ) {
//            o << ", ";
//        }
//        o << x[i];
//    }
//    o << ")";
//    
//    return o;
//}

#endif
