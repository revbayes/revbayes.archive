/**
 * @file
 * This file contains the implementation of VectorRealPos,
 * a vector type used to hold positive real numbers.
 *
 * @brief Implementation of VectorRealPos
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

#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <algorithm>
#include <iomanip>


/** Construct empty vector */
VectorRealPos::VectorRealPos( void ) : AbstractVector( RealPos::getClassTypeSpec() ) {
}


/** Construct vector with one positive real x from a double */
VectorRealPos::VectorRealPos( double x ) : AbstractVector( RealPos::getClassTypeSpec() ) {

    if ( x <= 0.0 )
        throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value" );
    
    elements.push_back( x );
}


/** Construct vector with n copies of positive real x from a double */
VectorRealPos::VectorRealPos( size_t n, double x ) : AbstractVector( RealPos::getClassTypeSpec() ) {

    if ( x <= 0.0 )
        throw RbException( "Nonpositive value for " + VectorRealPos::getClassName() );

    for ( size_t i = 0; i < n; i++ ) {
        elements.push_back( x );
    }
}


/** Constructor from double vector */
VectorRealPos::VectorRealPos( const std::vector<double>& x ) : AbstractVector( RealPos::getClassTypeSpec() ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] <= 0.0 )
            throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ ){
        elements.push_back( x[i] );
    }
}


/** Constructor from VectorReal */
VectorRealPos::VectorRealPos( const VectorReal& x ) : AbstractVector( RealPos::getClassTypeSpec() ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] <= 0.0 )
            throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ ) {
        elements.push_back( x[i] );
    }

}


/**
 * Subscript operator. By only implementing a value
 * subscript operator, we can prevent users of the class from
 * modifying a value without passing through the validation
 * checks in this class.
 */
double VectorRealPos::operator[]( size_t i ) const {

    if (i > elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorRealPos::operator==( const VectorRealPos& x ) const {

    if ( size() != x.size() )
        return false;

    for ( size_t i = 0; i < elements.size(); i++) {
        if ( operator[](i) != x[i] )
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorRealPos::operator!=( const VectorRealPos& x ) const {

    return !operator==( x );
}


void VectorRealPos::clear(void) {
    elements.clear();
}


/** Clone function */
VectorRealPos* VectorRealPos::clone( void ) const {

    return new VectorRealPos( *this );
}

/** Can we convert this vector into another object? */
RbObject* VectorRealPos::convertTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorReal::getClassTypeSpec()) {
        
        return new VectorReal( getValue() );
    }
    
    return AbstractVector::convertTo(type);
}


/** Get class name of object */
const std::string& VectorRealPos::getClassName(void) { 
    
    static std::string rbClassName = "Real+ Vector";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& VectorRealPos::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( AbstractVector::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& VectorRealPos::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const RbObject& VectorRealPos::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    const RbObject* n = new RealPos(elements[index]);
    
    return *n;
}


RbObject& VectorRealPos::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbObject* n = new RealPos(elements[index]);
    
    return *n;
}


/** Get value as an STL vector of double */
std::vector<double> VectorRealPos::getValue( void ) const {

    return elements;
}

/** Can we convert this vector into another object? */
bool VectorRealPos::isConvertibleTo(TypeSpec const &type) const {
    
    // test for type conversion
    if ( type == VectorReal::getClassTypeSpec() ) {

        return true;
    }
    
    return AbstractVector::isConvertibleTo(type);
}


void VectorRealPos::pop_back(void) {
    elements.pop_back();
}


void VectorRealPos::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Print value for user */
void VectorRealPos::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    for (std::vector<double>::const_iterator i = elements.begin(); i!= elements.end(); i++) {
        if (i != elements.begin())
            o << ", ";
        o << (*i);
    }
    o <<  " ]";

    o.setf(previousFlags);
    o.precision(previousPrecision);
}


/** Push an int onto the back of the vector after checking */
void VectorRealPos::push_back( RbObject* x ) {
    
    if ( x->isTypeSpec( RealPos::getClassTypeSpec() ) ) {
        elements.push_back(static_cast<RealPos*>( x )->getValue());
    } else if ( x->isConvertibleTo(RealPos::getClassTypeSpec()) ) {
        elements.push_back(static_cast<RealPos*>(x->convertTo(RealPos::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RealPos::getClassName() + "[] with invalid value" );
    }
}


/** Push a double onto the back of the vector after checking */
void VectorRealPos::push_back( double x ) {

    if ( x <= 0.0 )
        throw RbException( "Trying to set " + RealPos::getClassName() + "[] element with nonpositive value" );
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorRealPos::push_front( RbObject* x ) {
    
    if ( x->isTypeSpec( RealPos::getClassTypeSpec() ) ) {
        elements.insert( elements.begin(), static_cast<RealPos*>( x )->getValue());
    } else if ( x->isConvertibleTo(RealPos::getClassTypeSpec()) ) {
        elements.insert( elements.begin(), static_cast<RealPos*>(x->convertTo(RealPos::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RealPos::getClassName() + "[] with invalid value" );
    }
}


/** Push a double onto the front of the vector after checking */
void VectorRealPos::push_front( double x ) {

    if ( x <= 0.0 )
        throw RbException( "Trying to set " + RealPos::getClassName() + "[] element with nonpositive value" );
    
    elements.insert( elements.begin(), x );
}


void VectorRealPos::resize(size_t n) {
    elements.resize(n);
}


void VectorRealPos::setElement(const size_t index, RbLanguageObject* x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( RealPos::getClassTypeSpec() ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<RealPos*>( x )->getValue());
    } else if ( x->isConvertibleTo(RealPos::getClassTypeSpec()) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<RealPos*>(x->convertTo(RealPos::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RealPos::getClassName() + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector of double */
void VectorRealPos::setValue( const std::vector<double>& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
            elements.push_back( x[i] );
        }
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
            elements[i] = x[i];
        }
    }
}


/** Set value of vector using VectorInteger */
void VectorRealPos::setValue( const VectorInteger& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
            elements.push_back( x[i] );
        }
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
            elements[i] = x[i] ;
        }
    }
}


/** Set value of vector using VectorNatural */
void VectorRealPos::setValue( const VectorNatural& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            elements.push_back( x[i] );
        }    }
    else {

        for ( size_t i = 0; i < x.size(); i++ )
            elements[i] = x[i];
    }
}


/** Set value of vector using VectorReal */
void VectorRealPos::setValue( const VectorReal& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
            elements.push_back( x[i] );
        }
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos::getClassName() + "[] with nonpositive value(s)" );
            elements[i] = x[i];
        }
    }
}


/** Set value of vector using VectorRealPos */
void VectorRealPos::setValue( const VectorRealPos& x ) {

    elements = x.elements;
}      


size_t VectorRealPos::size(void) const {
    return elements.size();
}


/* Sort the vector */
void VectorRealPos::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove duplicates and resize the vector */
void VectorRealPos::unique(void) {
    
    sort();
    std::vector<double> uniqueVector;
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

