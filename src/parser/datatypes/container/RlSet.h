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
 * @date Last modified: $Date: 2012-06-12 10:15:46 +0200 (Tue, 12 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id: RlSet.h 1623 2012-06-12 08:15:46Z hoehna $
 */

#ifndef RlSet_H
#define RlSet_H

#include "Container.h"
#include "MethodTable.h"
#include "SimpleMemberFunction.h"

#include <iostream>
#include <set>


template <typename rlType>
class RlSet : public Container {
    
public:
    
    typedef typename rlType::valueType elementType;
    typedef typename std::set<elementType> valueType;
    typedef typename std::set<elementType>::iterator iterator;
    typedef typename std::set<elementType>::const_iterator const_iterator;
    
    RlSet(void);                                                                                                    //!< Default constructor with type RbLanguageObject
    RlSet(const std::vector<elementType> &v);                                                                       //!< Default constructor with type RbLanguageObject
    RlSet(const rlType &v);                                                                                         //!< Default constructor with type RbLanguageObject
    RlSet(size_t n);                                                                                                //!< Default constructor with type RbLanguageObject
    RlSet(size_t n, const elementType &v);                                                                          //!< Default constructor with type RbLanguageObject
    RlSet(const RlSet& v);                                                                                          //!< Copy Constructor
    
    virtual                                        ~RlSet(void);                                                    //!< Virtual destructor 
    
    // Basic utility functions 
    RlSet*                                          clone(void) const;                                              //!< Clone object
    RbObject*                                       convertTo(const TypeSpec& type) const;                          //!< Convert to type
    static const std::string&                       getClassName(void);                                             //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
    virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                    //!< Is this object convertible to the asked one?
    virtual void                                    printValue(std::ostream& o) const;                              //!< Print value for user
    
    elementType&                                    operator[](size_t index);                                       //!< subscript operator
    const elementType&                              operator[](size_t index) const;                                 //!< subscript operator (const)
    RlSet&                                          operator=(const RlSet& x);                                      //!< Assignment operator
    RlSet&                                          operator+=(const rlType& x);                                    //!< Concatenate
    RlSet&                                          operator+=(const RlSet& x);                                     //!< Concatenate
    const RlSet                                     operator+(const rlType& x) const;                               //!< Concatenate
    const RlSet                                     operator+(const RlSet& x) const;                                //!< Concatenate
    bool                                            operator==(const RlSet& x) const;                               //!< Equality
    bool                                            operator!=(const RlSet& x) const;                               //!< Inequality
    
    // Overwritten RbObject functions
    virtual RbValue<void*>                          getLeanValue(void) const;                                       //!< Transform the object into a basic element pointer for fast access.
    virtual size_t                                  memorySize(void) const;                                         //!< Get the size
    
    
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
    size_t                                          size(void) const;                                               //!< get the number of elements in the AbstractVector
    
protected:
    
    // We store internally pointers to our objects. This is necessary because elements can be also of the derived type and we need to be able to make proper copies of the Vector and all its elements
    std::set<elementType>                           elements;
    
private:
    
    MemberRules                                     memberRules;
    MethodTable                                     methods;
    
    struct comparator {
        bool operator() (elementType A, elementType B) const { return ( A < B);}
    } myComparator;
};



#include "ArgumentRule.h"
#include "Complex.h"
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
RlSet<rlType>::RlSet( void ) : Container( rlType::getClassTypeSpec() ) {
    
}


/** Constructor with dimension (n) and NULL pointers to every object */
template <typename rlType>
RlSet<rlType>::RlSet(size_t n) : Container( rlType::getClassTypeSpec() )  {
    
    //    for (size_t i = 0; i < n; i++) {
    //        this->push_back( NULL );
    //    }
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RlSet<rlType>::RlSet(size_t n, const typename rlType::valueType &x) : Container( rlType::getClassTypeSpec() )  {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( x->getValue() );
    }
}


/** Constructor with dimension (n) and copys of x for every object */
template <typename rlType>
RlSet<rlType>::RlSet(const std::vector<elementType> &x) : Container( rlType::getClassTypeSpec() )  {
    
    for (size_t i = 0; i < x.size(); i++) {
        this->push_back( x[i] );
    }
}



/** Copy Constructor */
template <typename rlType>
RlSet<rlType>::RlSet(const RlSet<rlType> &v) : Container(v), memberRules( v.memberRules ), methods( v.methods ) {
    
    // copy all the elements by deep copy
    elements = v.elements;
}


/** Destructor. Free the memory of the elements. */
template <typename rlType>
RlSet<rlType>::~RlSet(void) {
    
    // just call clear which will free the memory of the elements
    clear();
}

/** Assignment operator; make sure we get independent elements */
template <typename rlType>
RlSet<rlType>& RlSet<rlType>::operator=( const RlSet<rlType>& x ) {
    
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
typename rlType::valueType& RlSet<rlType>::operator[]( size_t index ) {
    
    return elements[index];
}


/* Subscript operator */
template <typename rlType>
const typename rlType::valueType& RlSet<rlType>::operator[]( size_t index ) const {
    
    return elements[index];
}


/** Concatenation with operator+ (valueType) */
template <typename rlType>
RlSet<rlType>& RlSet<rlType>::operator+=( const rlType& x ) {
    
    push_back( x.clone() );
    
    return *this;
}


/** Concatenation with operator+ (RlSet) */
template <typename rlType>
RlSet<rlType>& RlSet<rlType>::operator+=( const RlSet<rlType>& x ) {
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        push_back( x[i].clone() );
    
    return *this;
}


/** Equals comparison */
template <typename rlType>
bool RlSet<rlType>::operator==(const RlSet<rlType>& x) const {
    
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
bool RlSet<rlType>::operator!=(const RlSet<rlType>& x) const {
    
    return !operator==(x);
}


/** Concatenation with operator+ (valueType) */
template <typename rlType>
const RlSet<rlType> RlSet<rlType>::operator+( const rlType& x ) const {
    
    RlSet tempVec = *this;
    
    tempVec.push_back( x.getValue() );
    
    return tempVec;
}


/** Concatenation with operator+ (RlSet) */
template <typename rlType>
const RlSet<rlType> RlSet<rlType>::operator+( const RlSet<rlType>& x ) const {
    
    RlSet<rlType> tempVec = *this;
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i] );
    
    return tempVec;
}


/** Get iterator to the beginning of the Vector. */
template <typename rlType>
typename std::set<typename rlType::valueType>::iterator RlSet<rlType>::begin( void ) {
    return elements.begin();
}


/** Get iterator to the beginning of the Vector. */
template <typename rlType>
typename std::set<typename rlType::valueType>::const_iterator RlSet<rlType>::begin( void ) const {
    return elements.begin();
}



/** Convertible to: default implementation */
template <typename rlType>
RbObject* RlSet<rlType>::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        
        // RbLanguageObject
        if ( type.getElementType() == Integer::getClassTypeSpec() ) {
            RlSet<Integer>* convObject = new RlSet<Integer>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::set<elementType>::const_iterator i;
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
            RlSet<Natural>* convObject = new RlSet<Natural>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::set<elementType>::const_iterator i;
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
            RlSet<Probability>* convObject = new RlSet<Probability>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::set<elementType>::const_iterator i;
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
            RlSet<RbBoolean>* convObject = new RlSet<RbBoolean>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::set<elementType>::const_iterator i;
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
            RlSet<RbString>* convObject = new RlSet<RbString>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::set<elementType>::const_iterator i;
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
            RlSet<Real>* convObject = new RlSet<Real>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::set<elementType>::const_iterator i;
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
            RlSet<RealPos>* convObject = new RlSet<RealPos>();
            // insert copies of all objects. clone if they are of the right type, otherwise convert them
            typename std::set<elementType>::const_iterator i;
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
        //            RlSet<Complex>* convObject = new RlSet<Complex>();
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
        //            RlSet<Move>* convObject = new RlSet<Move>();
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
        //            RlSet<Monitor>* convObject = new RlSet<Monitor>();
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
void RlSet<rlType>::clear( void ) {
    
    typename std::set<elementType>::iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
        //        RbObject* theElement = *i;
        //        delete theElement;
    }
    
    elements.clear();
}


template <typename rlType>
RlSet<rlType>* RlSet<rlType>::clone() const {
    return new RlSet<rlType>( *this );
}


/** Get iterator to the end of the Vector. */
template <typename rlType>
typename std::set<typename rlType::valueType>::iterator RlSet<rlType>::end( void ) {
    return elements.end();
}


/** Get iterator to the end of the Vector. */
template <typename rlType>
typename std::set<typename rlType::valueType>::const_iterator RlSet<rlType>::end( void ) const {
    return elements.end();
}


/** Execute member function. */
template <typename rlType>
RbPtr<RbLanguageObject> RlSet<rlType>::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
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
int RlSet<rlType>::findIndex(const RbObject& x) const {
    
    // get the iterator to the first element
    typename std::set<typename rlType::valueType>::const_iterator i;
    
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
const std::string& RlSet<rlType>::getClassName(void) { 
    
    static std::string rbClassName = "Set";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename rlType>
const TypeSpec& RlSet<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( rlType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
template <typename rlType>
RbPtr<RbObject> RlSet<rlType>::getElement(size_t index) {
    
    typename std::set<typename rlType::valueType>::iterator i;
    i = elements.begin();
    for ( size_t j = 0; j < index; ++j, ++i);
    
    return new rlType( *i );
}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
template <typename rlType>
const MemberRules& RlSet<rlType>::getMemberRules(void) const {
    
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
const MethodTable& RlSet<rlType>::getMethods(void) const {
    
    static MethodTable methods;
    static bool methodsSet = false;
    
    if (!methodsSet) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new SimpleMemberFunction( rlType::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &Container::getMethods() );
        
        methodsSet = true;
    }
    
    
    return methods;
}


template <typename rlType>
const std::vector<typename rlType::valueType>& RlSet<rlType>::getValue(void) const {
    return elements;
}



template <typename rlType>
RbValue<void*> RlSet<rlType>::getLeanValue( void ) const {
    RbValue<void*> tmp;
    tmp.value = const_cast<std::set<elementType> *>( &elements );
    return tmp;
}

/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename rlType>
const TypeSpec& RlSet<rlType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/* Is convertible to: default implementation */
template <typename rlType>
bool RlSet<rlType>::isConvertibleTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the possible base element types
        typename std::set<elementType>::const_iterator i;
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
size_t RlSet<rlType>::memorySize( void ) const {
    
    return sizeof( std::vector<valueType> );
}


/** Print value for user */
template <typename rlType>
void RlSet<rlType>::printValue( std::ostream& o ) const {
    
    o << "[ ";
    typename std::set<elementType>::const_iterator i;
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
void RlSet<rlType>::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Pop element off of back of vector, updating length in process */
template <typename rlType>
void RlSet<rlType>::pop_back(void) {
    
    elements.pop_back();
}


/** Push an int onto the back of the vector */
template <typename rlType>
void RlSet<rlType>::push_back( const rlType &x ) {
    
    elements.insert( x.getValue() );
    
}


///** Push an int onto the back of the vector */
//template <typename rlType>
//void RlSet<rlType>::push_back( const typename rlType::valueType &x ) {
//    
//    elements.push_back( x );
//    
//}


/** Push an int onto the front of the vector */
template <typename rlType>
void RlSet<rlType>::push_front( const rlType &x ) {
    
    elements.insert( elements.begin(), x.getValue() );
}


///** Push an int onto the front of the vector */
//template <typename rlType>
//void RlSet<rlType>::push_front( const typename rlType::valueType &x ) {
//    
//    elements.insert( elements.begin(), x );
//}


/** Resize vector */
template <typename rlType>
void RlSet<rlType>::resize( size_t n ) {
    
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );
    
    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( NULL );
}


/* Set element */
template <typename rlType>
void RlSet<rlType>::setElement(const size_t index, RbObject *elem) {
    if (index >= elements.size()) {
        throw RbException("Cannot set element in Set outside the current range.");
    }
    
    // remove first the old element at the index
    elements.erase(elements.begin()+index);
    
    throw RbException("Missing implementation of RlSet::setElement()");
    //    elements.insert(elements.begin()+index, *elem);
}


/** Get the size of the vector */
template <typename rlType>
size_t RlSet<rlType>::size( void ) const {
    
    return elements.size();
    
}



#endif