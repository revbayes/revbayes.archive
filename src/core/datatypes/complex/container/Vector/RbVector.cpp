//
//  RbVector.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 5/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RbVector.h"


#include "Complex.h"
#include "Ellipsis.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Monitor.h"
#include "Move.h"
#include "Probability.h"
#include "RbBoolean.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbString.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"

/** Vector type of elements */
RbVector::RbVector( const TypeSpec &elemType ) : Container( elemType ), typeSpec(getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( elementType) ) {
    
    // set the member rules
    memberRules.push_back( new ValueRule( "x"  , elementType ) );
    memberRules.push_back( new Ellipsis( elementType ) );
    
    // set the methods
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
    methods.addFunction("[]",  new MemberFunction( elemType, squareBracketArgRules) );
    
    // add method for call "x.sort()" as a function
    ArgumentRules* sortArgRules = new ArgumentRules();
    methods.addFunction("sort",  new MemberFunction( RbVoid_name, sortArgRules) );
    
    // add method for call "x.unique()" as a function
    ArgumentRules* uniqueArgRules = new ArgumentRules();
    methods.addFunction("unique",  new MemberFunction( RbVoid_name, uniqueArgRules) );
    
    // necessary call for proper inheritance
    methods.setParentTable( &Container::getMethods() );
}


/** Constructor with dimension (n) and NULL pointers to every object */
RbVector::RbVector(const TypeSpec &elemType, size_t n) : Container( elemType ), typeSpec(getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( elementType) )  {
    
    // set the member rules
    memberRules.push_back( new ValueRule( "x"  , elementType ) );
    memberRules.push_back( new Ellipsis( elementType ) );
    
    // set the methods
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
    methods.addFunction("[]",  new MemberFunction( elemType, squareBracketArgRules) );
    
    // add method for call "x.sort()" as a function
    ArgumentRules* sortArgRules = new ArgumentRules();
    methods.addFunction("sort",  new MemberFunction( RbVoid_name, sortArgRules) );
    
    // add method for call "x.unique()" as a function
    ArgumentRules* uniqueArgRules = new ArgumentRules();
    methods.addFunction("unique",  new MemberFunction( RbVoid_name, uniqueArgRules) );
    
    // necessary call for proper inheritance
    methods.setParentTable( &Container::getMethods() );
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( NULL );
    }
}


/** Constructor with dimension (n) and copys of x for every object */
RbVector::RbVector(const TypeSpec &elemType, size_t n, RbObject* x) : Container( elemType ), typeSpec(getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( elementType) )  {
    
    // set the member rules
    memberRules.push_back( new ValueRule( "x"  , elementType ) );
    memberRules.push_back( new Ellipsis( elementType ) );
    
    // set the methods
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
    methods.addFunction("[]",  new MemberFunction( elemType, squareBracketArgRules) );
    
    // add method for call "x.sort()" as a function
    ArgumentRules* sortArgRules = new ArgumentRules();
    methods.addFunction("sort",  new MemberFunction( RbVoid_name, sortArgRules) );
    
    // add method for call "x.unique()" as a function
    ArgumentRules* uniqueArgRules = new ArgumentRules();
    methods.addFunction("unique",  new MemberFunction( RbVoid_name, uniqueArgRules) );
    
    // necessary call for proper inheritance
    methods.setParentTable( &Container::getMethods() );
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( x->clone() );
    }
}



/** Copy Constructor */
RbVector::RbVector(const RbVector &v) : Container(v), memberRules( v.memberRules ), methods( v.methods ), typeSpec( v.typeSpec ) {
    
    std::vector<RbObject *>::const_iterator it;
    // copy all the elements by deep copy
    for ( it = v.elements.begin(); it != v.elements.end(); it++) {
        // We need to type cast because clone return by default RbObject*
        RbObject *copy =  NULL;
        if ( (*it) != NULL )
            copy = (*it)->clone();
        elements.push_back( copy );
    }
    
}


/** Destructor. Free the memory of the elements. */
RbVector::~RbVector(void) {
    
    // just call clear which will free the memory of the elements
    clear();
}

/** Assignment operator; make sure we get independent elements */
RbVector& RbVector::operator=( const RbVector& x ) {
    
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


/** Concatenation with operator+ (valueType) */
RbVector& RbVector::operator+=( const RbObject& x ) {
    
    push_back( x.clone() );
    
    return *this;
}


/** Concatenation with operator+ (RbVector) */
RbVector& RbVector::operator+=( const RbVector& x ) {
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        push_back( x[i].clone() );
    
    return *this;
}

/** Subscript operator */
RbObject& RbVector::operator[](size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    
    return *elements[index];
}


/** Subscript const operator */
const RbObject& RbVector::operator[](size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    return *elements[index];
}


/** Equals comparison */
bool RbVector::operator==(const RbVector& x) const {
    
    if (size() != x.size())
        return false;
        
    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }
        
    return Container::operator==( x );
}


/** Not equals comparison */
bool RbVector::operator!=(const RbVector& x) const {
    
    return !operator==(x);
}


/** Concatenation with operator+ (valueType) */
const RbVector RbVector::operator+( const RbObject& x ) const {
    
    RbVector tempVec = *this;
    
    tempVec.push_back( x.clone() );
    
    return tempVec;
}


/** Concatenation with operator+ (RbVector) */
const RbVector RbVector::operator+( const RbVector& x ) const {
    
    RbVector tempVec = *this;
    
    for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i].clone() );
    
    return tempVec;
}


/** Get iterator to the beginning of the Vector. */
std::vector<RbObject *>::iterator RbVector::begin( void ) {
    return elements.begin();
}


/** Get iterator to the beginning of the Vector. */
std::vector<RbObject *>::const_iterator RbVector::begin( void ) const {
    return elements.begin();
}


/** Convertible to: default implementation */
RbObject* RbVector::convertTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        RbVector *convObject = new RbVector(type);

        //  convert all objects. clone if they are of the right type, otherwise convert them
        std::vector<RbObject *>::const_iterator i;
        for ( i = begin(); i != end(); i++) {
            RbObject* orgElement = *i;
            // test whether this element is already of the right type
            if ( orgElement->isTypeSpec(type.getElementType()) ) {
                convObject->push_back( orgElement->clone() );
            }
            else {
                convObject->push_back( orgElement->convertTo(type.getElementType() ) );
            }
        }
            
        return convObject;
        
    }
    
    return Container::convertTo(type);
}



/** Clear contents of value container */
void RbVector::clear( void ) {
    
    std::vector<RbObject *>::iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
        RbObject* theElement = *i;
        delete theElement;
    }
    
    elements.clear();
}


RbVector* RbVector::clone() const {
    return new RbVector( *this );
}


/**
 * Find the given element.
 * We rely on overloaded operator== in the element classes.
 * 
 * \param x the element we are looking for. 
 * \return The reference to the element we found or RbNullObject
 */
const RbObject& RbVector::find(const RbObject& x) const {
    
    // get the iterator to the first element
    std::vector<RbObject *>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
        RbObject& element = *(*i);
        
        // test if the object matches
        // note that we rely on the implemented operator==
        if ( element == x ) {
            return element;
        }
    }
    
    return RbNullObject::getInstance();
}


/** Get iterator to the end of the Vector. */
std::vector<RbObject *>::iterator RbVector::end( void ) {
    return elements.end();
}


/** Get iterator to the end of the Vector. */
std::vector<RbObject *>::const_iterator RbVector::end( void ) const {
    return elements.end();
}


/** Execute member function. */
const RbLanguageObject& RbVector::executeOperationSimple(std::string const &name, const std::vector<Argument> &args) {
    
    if ( name == "sort" ) {
        sort();
        
        return RbNullObject::getInstance();
    }
    else if ( name == "unique" ) {
        unique();
        
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
int RbVector::findIndex(const RbObject& x) const {
    
    // get the iterator to the first element
    std::vector<RbObject *>::const_iterator i;
    
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
const std::string& RbVector::getClassName(void) { 
    
    static std::string rbClassName = "Vector";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& RbVector::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get element */
const RbObject& RbVector::getElement(size_t index) const {
    
    return operator[](index);
}


/* Get element */
RbObject& RbVector::getElement(size_t index) {
    
    return this->operator[](index);
}


/** 
 * Get the member rules
 * We expect that a Vector is created by "Vector(x,...)". 
 * All variables are for simplicity just single elements. For more sophisticated constructors (e.g. from a vector of elements)
 * constructor functions should be used.
 */
const MemberRules& RbVector::getMemberRules(void) const {
    
    return memberRules;
}


/** Get the methods for this vector class */
/* Get method specifications */
const MethodTable& RbVector::getMethods(void) const {
    
    return methods;
}


const std::vector<RbObject* >& RbVector::getValue(void) const {
    return elements;
}



void* RbVector::getLeanValue( std::vector<size_t> &lengths ) const {
    
    // add the length for this dimension
    lengths.push_back( elements.size() );
    
    // create the c-style array of elements
    void* data = malloc(memorySize());
    
    // tmp length vector
    std::vector<size_t> tmp_lengths;
    
    // iterate over all elements and copy their values
    std::vector<RbObject *>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++ ) {
        const RbObject *x = *i;
        void* elemVal;
        if ( i == elements.begin() ) {
            elemVal = x->getLeanValue( lengths );
        }
        else {
            elemVal = x->getLeanValue( tmp_lengths );            
        }
        memcpy(data, elemVal,(*i)->memorySize());
        //        *(data) = *(*i)->getValue(lengths);
        char *tmp_ptr = (char*)data;
        tmp_ptr += (*i)->memorySize();
        data = tmp_ptr;
    }
    char *tmp_ptr = (char*)data;
    tmp_ptr -= memorySize();
    data = tmp_ptr;
    
    return data;
}

/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RbVector::getTypeSpec(void) const {
    
    return typeSpec;
}


/* Is convertible to: default implementation */
bool RbVector::isConvertibleTo(const TypeSpec &type) const {
    
    // test whether we want to convert to another Vector
    if ( type.getBaseType() == getClassName() ) {
        
        // work through all the elements
        std::vector<RbObject *>::const_iterator i;
        for ( i = begin(); i != end(); i++) {
            RbObject* orgElement = *i;
            // test whether this element is already of the right type
            if ( !orgElement->isTypeSpec(type.getElementType()) && !orgElement->isConvertibleTo(type.getElementType()) ) {
                return false;
            }
        }
        
        return true;
    }
    
    return false;
}


/** Print value for user */
size_t RbVector::memorySize( void ) const {
    
    size_t memSize = 0;
    std::vector<RbObject *>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++ ) {
        memSize += (*i)->memorySize();
    }
    
    return memSize;
}


/** Print value for user */
void RbVector::printValue( std::ostream& o ) const {
    
    o << "[ ";
    std::vector<RbObject *>::const_iterator i;
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
void RbVector::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Pop element off of back of vector, updating length in process */
void RbVector::pop_back(void) {
    
    elements.pop_back();
}


/** Push an int onto the back of the vector */
void RbVector::push_back( RbObject *x ) {
    
    elements.push_back( x );
    
}


/** Push an int onto the front of the vector */
void RbVector::push_front( RbObject *x ) {
    
    elements.insert( elements.begin(), x );
}


/** Resize vector */
void RbVector::resize( size_t n ) {
    
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );
    
    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( NULL );
}


/** Set element */
void RbVector::setElement(const size_t index, RbObject *elem) {
    if (index >= elements.size()) {
        throw RbException("Cannot set element in Vector outside the current range.");
    }
    
    // remove first the old element at the index
    RbObject* oldElement = elements[index];
    delete oldElement;
    elements.erase(elements.begin()+index);
    
    elements.insert(elements.begin()+index, elem);
}


/** Vector a member variable */
void RbVector::setMemberVariable(const std::string& name, const Variable* var) {
    
    if (name == "x" || name == "" ) { // the ellipsis variables
        RbObject* element = var->getValue().clone();
        push_back( element);
    }
    else {
        Container::setMemberVariable(name, var);
    }
}


/** Get the size of the vector */
size_t RbVector::size( void ) const {
    
    return elements.size();
    
}


/* Sort the vector */
void RbVector::sort( void ) {
    
    std::sort(elements.begin(), elements.end(), myComparator);
    
}

/* Remove duplicates and resize the vector */
void RbVector::unique(void) {
    
    sort();
    std::vector<RbObject* > uniqueVector;
    uniqueVector.push_back (elements[0]->clone());
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if ( *(elements[i]) != *(elements[i-1]) )
            uniqueVector.push_back(elements[i]->clone());
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}
