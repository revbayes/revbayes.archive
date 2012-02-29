/**
 * @file
 * This file contains the implementation of VectorString, a complex type
 * used to hold string vectors.
 *
 * @brief Implementation of VectorString
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

#include "MemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <algorithm>
#include <sstream>


/** Construct empty string vector */
VectorString::VectorString(void) : AbstractVector(RbString::getClassTypeSpec()) {
}


/** Construct vector with one string x */
VectorString::VectorString(const std::string& x) : AbstractVector(RbString::getClassTypeSpec()) {

    elements.push_back( x );
}


/** Constructor from std::string vector */
VectorString::VectorString(const std::vector<std::string>& x) : AbstractVector(RbString::getClassTypeSpec()) {

    elements = x;
}


/** Subscript operator */
std::string& VectorString::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
const std::string& VectorString::operator[](size_t i) const {

    if (i > elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorString::operator==(const VectorString& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorString::operator!=(const VectorString& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (RbString) */
VectorString VectorString::operator+( const RbString& x ) const {

    VectorString tempVec = *this;

    tempVec.push_back( x.getValue() );

    return tempVec;
}


/** Concatenation with operator+ (VectorString) */
VectorString VectorString::operator+( const VectorString& x ) const {

    VectorString tempVec = *this;

    for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i] );

    return tempVec;
}


/** Concatenation with operator+ (std::string) */
VectorString VectorString::operator+( const std::string& x ) const {

    VectorString tempVec = *this;

    tempVec.push_back( x );

    return tempVec;
}


void VectorString::clear(void) {
    elements.clear();
}


/** Clone function */
VectorString* VectorString::clone() const {

    return new VectorString(*this);
}




/** Get class name of object */
const std::string& VectorString::getClassName(void) { 
    
    static std::string rbClassName = "String Vector";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& VectorString::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( AbstractVector::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& VectorString::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const RbObject& VectorString::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    const RbObject* n = new RbString(elements[index]);
    
    return *n;
}


RbObject& VectorString::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbObject* n = new RbString(elements[index]);
    
    return *n;
}



/* Get method specifications */
const MethodTable& VectorString::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( RbString::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &AbstractVector::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/** Get STL vector of strings */
std::vector<std::string> VectorString::getValue(void) const {	 

    return elements;	 
}


void VectorString::pop_back(void) {
    elements.pop_back();
}


void VectorString::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Push an int onto the back of the vector after checking */
void VectorString::push_back( RbObject* x ) {
    
    if ( x->isTypeSpec( RbString::getClassTypeSpec() ) ) {
        elements.push_back(static_cast<RbString*>( x )->getValue() );
    } else if ( x->isConvertibleTo(RbString::getClassTypeSpec()) ) {
        elements.push_back(static_cast<RbString*>(x->convertTo(RbString::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RbString::getClassName() + "[] with invalid value" );
    }
}


/** Append string element to end of vector, updating length in process */
void VectorString::push_back(const std::string &x) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorString::push_front( RbObject* x ) {
    
    if ( x->isTypeSpec( RbString::getClassTypeSpec() ) ) {
        elements.insert( elements.begin(), static_cast<RbString*>( x )->getValue());
    } else if ( x->isConvertibleTo(RbString::getClassTypeSpec()) ) {
        elements.insert( elements.begin(), static_cast<RbString*>(x->convertTo(RbString::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RbString::getClassName() + "[] with invalid value" );
    }
}


void VectorString::resize(size_t n) {
    elements.resize(n);
}


void VectorString::setElement(const size_t index, RbLanguageObject* x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( RbString::getClassTypeSpec() ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<RbString*>( x )->getValue());
    } else if ( x->isConvertibleTo(RbString::getClassTypeSpec()) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<RbString*>(x->convertTo(RbString::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RbString::getClassName() + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector */
void VectorString::setValue(const std::vector<std::string>& x) {
    
    elements = x;
}   


/** Set value of vector using VectorString */
void VectorString::setValue(const VectorString& x) {
    
    elements = x.elements;
}   


size_t VectorString::size(void) const {
    return elements.size();
}


/** Sort the vector */
void VectorString::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/** Remove consecutive duplicates and resizes the vector */
void VectorString::unique(void) {
    
    sort();
    std::vector<std::string> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    elements = uniqueVector;
    return;
    
}

