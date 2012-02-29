/**
 * @file
 * This file contains the implementation of VectorInteger, a complex type
 * used to hold int vectors.
 *
 * @brief Implementation of VectorInteger
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#include "Integer.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "VectorReal.h"
#include "VectorRealPos.h"

#include <algorithm>
#include <sstream>


/** Default constructor */
VectorInteger::VectorInteger(void) : AbstractVector(Integer::getClassTypeSpec()) {
}


/** Construct vector with one int x */
VectorInteger::VectorInteger(int x) : AbstractVector(Integer::getClassTypeSpec()) {
    
    elements.push_back( x );
}


/** Construct vector with n ints x */
VectorInteger::VectorInteger(size_t n, int x) : AbstractVector(Integer::getClassTypeSpec()) {

    for (size_t i = 0; i < n; i++) {
        elements.push_back( x );
    }
    
}


/** Constructor from int vector */
VectorInteger::VectorInteger(const std::vector<int>& x) : AbstractVector(Integer::getClassTypeSpec()) {

    elements = x;
}


/** Constructor from VectorInteger */
VectorInteger::VectorInteger(const VectorNatural& x) : AbstractVector(Integer::getClassTypeSpec()) {

    for (size_t i=0; i<x.size(); i++) {
        elements.push_back( x[i] );
    }
}


/** Constructor from size_t vector */
VectorInteger::VectorInteger(const std::vector<size_t>& x) : AbstractVector(Integer::getClassTypeSpec()) {

    for (size_t i=0; i<x.size(); i++) {
        elements.push_back( int(x[i]) );
    }
}


/** Subscript operator */
int& VectorInteger::operator[](size_t i) {

    if (i > elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
const int& VectorInteger::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorInteger::operator==(const VectorInteger& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorInteger::operator!=(const VectorInteger& x) const {

    return !operator==(x);
}


void VectorInteger::clear(void) {
    elements.clear();
}


/** Clone function */
VectorInteger* VectorInteger::clone() const {

    return new VectorInteger(*this);
}


/** Can we convert this vector into another object? */
RbObject* VectorInteger::convertTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos::getClassTypeSpec()) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            d.push_back(*it);
        }
        
        return new VectorRealPos(d);
    }
    else if (type == VectorNatural::getClassTypeSpec()) {
        
        return new VectorNatural( getValue() );
    }
    else if (type == VectorReal::getClassTypeSpec()) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            d.push_back(*it);
        }
        
        return new VectorRealPos(d);
    }
    
    return AbstractVector::convertTo(type);
}


/** Get class name of object */
const std::string& VectorInteger::getClassName(void) { 
    
    static std::string rbClassName = "Integer Vector";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& VectorInteger::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( AbstractVector::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& VectorInteger::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const RbObject& VectorInteger::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    const RbObject* n = new Integer(elements[index]);
    
    return *n;
}


RbObject& VectorInteger::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbObject* n = new Integer(elements[index]);
    
    return *n;
}



/* Get method specifications */
const MethodTable& VectorInteger::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( Integer::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &AbstractVector::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/** Export value as STL vector */
std::vector<int> VectorInteger::getValue(void) const {

    return elements;
}


/** Can we convert this vector into another object? */
bool VectorInteger::isConvertibleTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorNatural::getClassTypeSpec()) {
        
        for (std::vector<int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            
            // test whether we can convert this element, otherwise return false
            if (*it < 0) {
                return false;
            }
        }
        
        return true;
    }
    else if (type == VectorReal::getClassTypeSpec() || type == VectorRealPos::getClassTypeSpec()) {
        
        return true;
    }
    
    return AbstractVector::isConvertibleTo(type);
}


void VectorInteger::pop_back(void) {
    elements.pop_back();
}


void VectorInteger::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Push an int onto the back of the vector after checking */
void VectorInteger::push_back( RbObject* x ) {
    
    if ( x->isTypeSpec( Integer::getClassTypeSpec() ) ) {
        elements.push_back(static_cast<Integer*>( x )->getValue());
    } else if ( x->isConvertibleTo(Integer::getClassTypeSpec()) ) {
        elements.push_back(static_cast<Integer*>(x->convertTo(Integer::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Integer::getClassName() + "[] with invalid value" );
    }
}

/** Append element to end of vector, updating length in process */
void VectorInteger::push_back(int x) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorInteger::push_front( RbObject* x ) {
    
    if ( x->isTypeSpec( Integer::getClassTypeSpec() ) ) {
        elements.insert( elements.begin(), static_cast<Integer*>( x )->getValue());
    } else if ( x->isConvertibleTo( Integer::getClassTypeSpec() ) ) {
        elements.insert( elements.begin(), static_cast<Integer*>(x->convertTo(Integer::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Integer::getClassName() + "[] with invalid value" );
    }
}


/** Add element in front of vector, updating length in process */
void VectorInteger::push_front(int x) {
    
    elements.insert(elements.begin(), x);
}


void VectorInteger::resize(size_t n) {
    elements.resize(n);
}


void VectorInteger::setElement(const size_t index, RbLanguageObject* x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( Integer::getClassTypeSpec() ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<Integer*>( x )->getValue());
    } else if ( x->isConvertibleTo(Integer::getClassTypeSpec()) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<Integer*>(x->convertTo(Integer::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Integer::getClassName() + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector */
void VectorInteger::setValue(const std::vector<int>& x) {

    elements = x;
}   


/** Set value of vector using VectorInteger */
void VectorInteger::setValue(const VectorInteger& x) {

    elements = x.elements;
}   


size_t VectorInteger::size(void) const {
    return elements.size();
}


/* Sort the vector */
void VectorInteger::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove duplicates and resizes the vector */
void VectorInteger::unique(void) {

    sort();
    std::vector<int> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i=1 ; i<elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}


