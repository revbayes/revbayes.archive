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


template <typename rlType>
class RlVector : public Container {
    
public:
    
    typedef typename rlType::valueType elementType;
    typedef typename std::vector<elementType> valueType;
    typedef typename std::vector<elementType>::iterator iterator;
    typedef typename std::vector<elementType>::const_iterator const_iterator;
    
    RlVector(void);                                                                                                 //!< Default constructor with type RbLanguageObject
    RlVector(const std::vector<elementType> &v);                                                                    //!< Default constructor with type RbLanguageObject
    RlVector(const rlType &v);                                                                                      //!< Default constructor with type RbLanguageObject
    RlVector(size_t n);                                                                                             //!< Default constructor with type RbLanguageObject
    RlVector(size_t n, const elementType &v);                                                                       //!< Default constructor with type RbLanguageObject
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
    
    elementType&                                    operator[](size_t index);                                       //!< subscript operator
    const elementType&                              operator[](size_t index) const;                                 //!< subscript operator (const)
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
    RbPtr<RbObject>                                 getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    const std::vector<elementType>&                 getValue(void) const;                                           //!< Get the stl Vector of elements
    void                                            pop_back(void);                                                 //!< Drop element at back
    void                                            pop_front(void);                                                //!< Drop element from front
    void                                            push_back(const rlType &x);                                     //!< Append element to end
//    void                                            push_back(const valueType &x);                 //!< Append element to end
    void                                            push_front(const rlType &x);                                    //!< Append element to end
//    void                                            push_front(const valueType &x);                //!< Append element to end
    void                                            resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                                            setElement(const size_t index, RbObject *elem);                 //!< Set element with type conversion
    void                                            sort(void);                                                     //!< sort the AbstractVector
    size_t                                          size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                                            unique(void);                                                   //!< removes consecutive duplicates

protected:
        
    // We store internally pointers to our objects. This is necessary because elements can be also of the derived type and we need to be able to make proper copies of the Vector and all its elements
    std::vector<elementType>                        elements;

private:
    
    MemberRules                                     memberRules;
    MethodTable                                     methods;
    
    struct comparator {
        bool operator() (elementType A, elementType B) const { return ( A < B);}
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
template <typename rlType>
RlVector<rlType>::RlVector( void ) : Container( rlType::getClassTypeSpec() ) {
    
}


/** Constructor with dimension (n) and NULL pointers to every object */
template <typename rlType>
RlVector<rlType>::RlVector(size_t n) : Container( rlType::getClassTypeSpec() )  {
    
//    for (size_t i = 0; i < n; i++) {
//        this->push_back( NULL );
//    }
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RlVector<rlType>::RlVector(size_t n, const typename rlType::valueType &x) : Container( rlType::getClassTypeSpec() )  {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( x->getValue() );
    }
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RlVector<rlType>::RlVector(const std::vector<elementType> &x) : Container( rlType::getClassTypeSpec() )  {
    
    for (size_t i = 0; i < x.size(); i++) {
        this->push_back( x[i] );
    }
}



/** Copy Constructor */
template <typename rlType>
RlVector<rlType>::RlVector(const RlVector<rlType> &v) : Container(v), memberRules( v.memberRules ), methods( v.methods ) {
    
    typename std::vector<elementType>::const_iterator it;
    // copy all the elements by deep copy
    for ( it = v.elements.begin(); it != v.elements.end(); it++) {
        elements.push_back( *it );
    }
    
}


/** Destructor. Free the memory of the elements. */
template <typename rlType>
RlVector<rlType>::~RlVector(void) {
    
    // just call clear which will free the memory of the elements
    clear();
}

/** Assignment operator; make sure we get independent elements */
template <typename rlType>
RlVector<rlType>& RlVector<rlType>::operator=( const RlVector<rlType>& x ) {
    
    if ( this != &x ) {
        
        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        Container::operator=( x );
        
        // just call clear which will free the memory of the objects
        clear();
        
        typename std::vector<typename rlType::valueType>::const_iterator i;
        for ( i = x.elements.begin(); i != x.elements.end(); i++ ) {
            elements.push_back( *i );
        }
        
        memberRules     = x.memberRules;
        methods         = x.methods;
    }
    
    return ( *this );
}


/* Subscript operator */
template <typename rlType>
typename rlType::valueType& RlVector<rlType>::operator[]( size_t index ) {
    
    return elements[index];
}


/* Subscript operator */
template <typename rlType>
const typename rlType::valueType& RlVector<rlType>::operator[]( size_t index ) const {
    
    return elements[index];
}


/** Concatenation with operator+ (valueType) */
template <typename rlType>
RlVector<rlType>& RlVector<rlType>::operator+=( const rlType& x ) {
    
    push_back( x.clone() );
    
    return *this;
}


/** Concatenation with operator+ (RlVector) */
template <typename rlType>
RlVector<rlType>& RlVector<rlType>::operator+=( const RlVector<rlType>& x ) {
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        push_back( x[i].clone() );
    
    return *this;
}


/** Equals comparison */
template <typename rlType>
bool RlVector<rlType>::operator==(const RlVector<rlType>& x) const {
    
    if (size() != x.size())
        return false;
    
    for (size_t i=0; i<elements.size(); i++) {
        if (elements[i] != x[i])
            return false;
    }
    
    return Container::operator==( x );
}


/** Not equals comparison */
template <typename rlType>
bool RlVector<rlType>::operator!=(const RlVector<rlType>& x) const {
    
    return !operator==(x);
}


/** Concatenation with operator+ (valueType) */
template <typename rlType>
const RlVector<rlType> RlVector<rlType>::operator+( const rlType& x ) const {
    
    RlVector tempVec = *this;
    
    tempVec.push_back( x.getValue() );
    
    return tempVec;
}


/** Concatenation with operator+ (RlVector) */
template <typename rlType>
const RlVector<rlType> RlVector<rlType>::operator+( const RlVector<rlType>& x ) const {
    
    RlVector<rlType> tempVec = *this;
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i] );
    
    return tempVec;
}


/** Get iterator to the beginning of the Vector. */
template <typename rlType>
typename std::vector<typename rlType::valueType>::iterator RlVector<rlType>::begin( void ) {
    return elements.begin();
}


/** Get iterator to the beginning of the Vector. */
template <typename rlType>
typename std::vector<typename rlType::valueType>::const_iterator RlVector<rlType>::begin( void ) const {
    return elements.begin();
}


/** Convertible to: default implementation */
template <typename rlType>
RbObject* RlVector<rlType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        
        // RbLanguageObject
        if ( type.getElementType() == Integer::getClassTypeSpec() ) {
            RlVector<Integer>* convObject = new RlVector<Integer>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                rlType orgElement = rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement.isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( dynamic_cast<Integer &>( orgElement ) );
                }
                else {
                    convObject->push_back( *static_cast<Integer*>( orgElement.convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
        else if ( type.getElementType() == Natural::getClassTypeSpec() ) {
            RlVector<Natural>* convObject = new RlVector<Natural>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                rlType orgElement = rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement.isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( dynamic_cast<Natural &>( orgElement ) );
                }
                else {
                    convObject->push_back( *static_cast<Natural*>( orgElement.convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
        else if ( type.getElementType() == Probability::getClassTypeSpec() ) {
            RlVector<Probability>* convObject = new RlVector<Probability>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                rlType orgElement = rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement.isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( dynamic_cast<Probability &>( orgElement ) );
                }
                else {
                    convObject->push_back( *static_cast<Probability*>( orgElement.convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
        else if ( type.getElementType() == RbBoolean::getClassTypeSpec() ) {
            RlVector<RbBoolean>* convObject = new RlVector<RbBoolean>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                rlType orgElement = rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement.isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( dynamic_cast<RbBoolean &>( orgElement ) );
                }
                else {
                    convObject->push_back( *static_cast<RbBoolean*>( orgElement.convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
        else if ( type.getElementType() == RbString::getClassTypeSpec() ) {
            RlVector<RbString>* convObject = new RlVector<RbString>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                rlType orgElement = rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement.isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( dynamic_cast<RbString &>( orgElement ) );
                }
                else {
                    convObject->push_back( *static_cast<RbString*>( orgElement.convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
        else if ( type.getElementType() == Real::getClassTypeSpec() ) {
            RlVector<Real>* convObject = new RlVector<Real>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                rlType orgElement = rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement.isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( dynamic_cast<Real &>( orgElement ) );
                }
                else {
                    convObject->push_back( *static_cast<Real*>( orgElement.convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
        else if ( type.getElementType() == RealPos::getClassTypeSpec() ) {
            RlVector<RealPos>* convObject = new RlVector<RealPos>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::vector<elementType>::const_iterator i;
            for ( i = begin(); i != end(); i++) {
                rlType orgElement = rlType( *i );
                // test whether this element is already of the right type
                if ( orgElement.isTypeSpec(type.getElementType()) ) {
                    convObject->push_back( dynamic_cast<RealPos &>( orgElement ) );
                }
                else {
                    convObject->push_back( *static_cast<RealPos *>( orgElement.convertTo(type.getElementType()) ) );
                }
            }
            
            return convObject;
        }
//        else if ( type.getElementType() == Complex::getClassTypeSpec() ) {
//            RlVector<Complex>* convObject = new RlVector<Complex>();
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
//        else if ( type.getElementType() == Move::getClassTypeSpec() ) {
//            RlVector<Move>* convObject = new RlVector<Move>();
//            // insert copies of all objects. clone if they are of the right type, otherwise convert them
//            typename std::vector<elementType>::const_iterator i;
//            for ( i = begin(); i != end(); i++) {
//                rlType orgElement = rlType( *i );
//                // test whether this element is already of the right type
//                if ( orgElement.isTypeSpec(type.getElementType()) ) {
//                    convObject->push_back( dynamic_cast<Move &>( orgElement ) );
//                }
//                else {
//                    convObject->push_back( *static_cast<Move*>( orgElement.convertTo(type.getElementType()) ) );
//                }
//            }
//            
//            return convObject;
//        }
//        else if ( type.getElementType() == Monitor::getClassTypeSpec() ) {
//            RlVector<Monitor>* convObject = new RlVector<Monitor>();
//            // insert copies of all objects. clone if they are of the right type, otherwise convert them
//            typename std::vector<elementType>::const_iterator i;
//            for ( i = begin(); i != end(); i++) {
//                rlType orgElement = rlType( *i );
//                // test whether this element is already of the right type
//                if ( orgElement.isTypeSpec(type.getElementType()) ) {
//                    convObject->push_back( dynamic_cast<Monitor &>( orgElement ) );
//                }
//                else {
//                    convObject->push_back( *static_cast<Monitor*>( orgElement.convertTo(type.getElementType()) ) );
//                }
//            }
//            
//            return convObject;
//        }
        
    }
    
    return Container::convertTo(type);
}



/** Clear contents of value container */
template <typename rlType>
void RlVector<rlType>::clear( void ) {
    
    typename std::vector<elementType>::iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
//        RbObject* theElement = *i;
//        delete theElement;
    }
    
    elements.clear();
}


template <typename rlType>
RlVector<rlType>* RlVector<rlType>::clone() const {
    return new RlVector<rlType>( *this );
}


/** Get iterator to the end of the Vector. */
template <typename rlType>
typename std::vector<typename rlType::valueType>::iterator RlVector<rlType>::end( void ) {
    return elements.end();
}


/** Get iterator to the end of the Vector. */
template <typename rlType>
typename std::vector<typename rlType::valueType>::const_iterator RlVector<rlType>::end( void ) const {
    return elements.end();
}


/** Execute member function. */
template <typename rlType>
RbPtr<RbLanguageObject> RlVector<rlType>::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
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
template <typename rlType>
int RlVector<rlType>::findIndex(const RbObject& x) const {
    
    // get the iterator to the first element
    typename std::vector<typename rlType::valueType>::const_iterator i;
    
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
template <typename rlType>
const std::string& RlVector<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "Vector";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const TypeSpec& RlVector<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
template <typename rlType>
RbPtr<RbObject> RlVector<rlType>::getElement(size_t index) {
    
    return new rlType( elements[index] );
}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
template <typename rlType>
const MemberRules& RlVector<rlType>::getMemberRules(void) const {
    
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
const MethodTable& RlVector<rlType>::getMethods(void) const {
    
    static MethodTable methods;
    static bool methodsSet = false;
    
    if (!methodsSet) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new SimpleMemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        // add method for call "x.sort()" as a function
        ArgumentRules* sortArgRules = new ArgumentRules();
        methods.addFunction("sort",  new SimpleMemberFunction( RbVoid_name, sortArgRules) );
        
        // add method for call "x.unique()" as a function
        ArgumentRules* uniqueArgRules = new ArgumentRules();
        methods.addFunction("unique",  new SimpleMemberFunction( RbVoid_name, uniqueArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &Container::getMethods() );
        
        methodsSet = true;
    }
    
    
    return methods;
}


template <typename rlType>
const std::vector<typename rlType::valueType>& RlVector<rlType>::getValue(void) const {
    return elements;
}



template <typename rlType>
void* RlVector<rlType>::getLeanValue( std::vector<size_t> &lengths ) const {
    
    return const_cast<std::vector<elementType> *>( &elements );
}

/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const TypeSpec& RlVector<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename rlType>
bool RlVector<rlType>::isConvertibleTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        typename std::vector<elementType>::const_iterator i;
        for ( i = begin(); i != end(); i++) {
            rlType orgElement = rlType(*i);
            // test whether this element is already of the right type
            if ( !orgElement.isTypeSpec(type.getElementType()) && !orgElement.isConvertibleTo(type.getElementType()) ) {
                return false;
            }
        }
        
        return true;
    }
    
    return false;
}


/** Print value for user */
template <typename rlType>
size_t RlVector<rlType>::memorySize( void ) const {
    
    return sizeof( std::vector<valueType> );
}


/** Print value for user */
template <typename rlType>
void RlVector<rlType>::printValue( std::ostream& o ) const {
    
    o << "[ ";
    typename std::vector<elementType>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        elementType tmp = *i;
        rlType rlTmp = rlType(tmp);
        rlTmp.printValue(o);
    }
    o <<  " ]";
    
}


/** Pop element off of front of vector, updating length in process */
template <typename rlType>
void RlVector<rlType>::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Pop element off of back of vector, updating length in process */
template <typename rlType>
void RlVector<rlType>::pop_back(void) {
    
    elements.pop_back();
}


/** Push an int onto the back of the vector */
template <typename rlType>
void RlVector<rlType>::push_back( const rlType &x ) {
    
    elements.push_back( x.getValue() );
    
}


///** Push an int onto the back of the vector */
//template <typename rlType>
//void RlVector<rlType>::push_back( const typename rlType::valueType &x ) {
//    
//    elements.push_back( x );
//    
//}


/** Push an int onto the front of the vector */
template <typename rlType>
void RlVector<rlType>::push_front( const rlType &x ) {
    
    elements.insert( elements.begin(), x.getValue() );
}


///** Push an int onto the front of the vector */
//template <typename rlType>
//void RlVector<rlType>::push_front( const typename rlType::valueType &x ) {
//    
//    elements.insert( elements.begin(), x );
//}


/** Resize vector */
template <typename rlType>
void RlVector<rlType>::resize( size_t n ) {
    
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );
    
    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( NULL );
}


/* Set element */
template <typename rlType>
void RlVector<rlType>::setElement(const size_t index, RbObject *elem) {
    if (index >= elements.size()) {
        throw RbException("Cannot set element in Vector outside the current range.");
    }
    
    // remove first the old element at the index
    elements.erase(elements.begin()+index);
  
    throw RbException("Missing implementation of RlVector::setElement()");
//    elements.insert(elements.begin()+index, *elem);
}


/** Get the size of the vector */
template <typename rlType>
size_t RlVector<rlType>::size( void ) const {
    
    return elements.size();
    
}


/* Sort the vector */
template <typename rlType>
void RlVector<rlType>::sort( void ) {
    
    std::sort(elements.begin(), elements.end(), myComparator);
    
}

/* Remove duplicates and resize the vector */
template <typename rlType>
void RlVector<rlType>::unique(void) {
    
    sort();
    std::vector<elementType> uniqueVector;
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