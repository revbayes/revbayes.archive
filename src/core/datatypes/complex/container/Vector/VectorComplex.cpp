/**
 * @file
 * This file contains the implementation of VectorComplex, a complex type
 * used to hold complex vectors.
 *
 * @brief Implementation of VectorComplex
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

#include "Complex.h"
#include "RbException.h"
#include "RbUtil.h"
#include "VectorInteger.h"
#include "VectorComplex.h"
#include "VectorString.h"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>


// Definition of the static type spec member
const TypeSpec VectorComplex::typeSpec(VectorComplex_name);

/** Default constructor */
VectorComplex::VectorComplex(void) : AbstractVector(Complex_name) {

}


/** Construct vector with one double x */
VectorComplex::VectorComplex(const double x) : AbstractVector(Complex_name) {
    
    elements.push_back( std::complex<double>( x, 0.0 ) );

}


/** Construct vector with n doubles x */
VectorComplex::VectorComplex(const size_t n, const double x) : AbstractVector(Complex_name) {

    for (size_t i = 0; i < n; i++) {
        elements.push_back( std::complex<double>( x, 0.0 ) );
    }
}


/** Constructor from double vector */
VectorComplex::VectorComplex(const std::vector<double>& x) : AbstractVector(Complex_name) {

    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++) {
        elements.push_back( std::complex<double>( *i, 0.0 ) );
    }
    
}


/** Constructor from complex vector */
VectorComplex::VectorComplex(const std::vector<std::complex<double> >& x) : AbstractVector(Complex_name) {

    elements = x;
}


/** Constructor from VectorComplex */
VectorComplex::VectorComplex( const VectorComplex& x ) : AbstractVector(x) {

    elements = x.elements;
}


VectorComplex::VectorComplex(size_t n, std::complex<double> x) : AbstractVector(Complex_name) {

    for (size_t i = 0; i < n; i++) {
        elements.push_back( x );
    }

}

/** Subscript operator */
std::complex<double>& VectorComplex::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
const std::complex<double>& VectorComplex::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorComplex::operator==(const VectorComplex& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorComplex::operator!=(const VectorComplex& x) const {

    return !operator==(x);
}


/** Clone function */
VectorComplex* VectorComplex::clone(void) const {

    return new VectorComplex(*this);
}


void VectorComplex::clear(void) {
    elements.clear();
}


/** Get class vector describing type of object */
const VectorString& VectorComplex::getClass(void) const {

    static VectorString rbClass = VectorString(VectorComplex_name) + AbstractVector::getClass();
    return rbClass;
}


RbPtr<RbObject> VectorComplex::getElement(size_t index) const {
    
    if (index >= elements.size())
        throw RbException("Index out of bounds");
    
    RbPtr<RbObject> c( new Complex(elements[index]) );
    return c;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& VectorComplex::getTypeSpec(void) const {
    return typeSpec;
}


/** Export value as STL vector */
std::vector<std::complex<double> > VectorComplex::getValue(void) const {

    return elements;
}


void VectorComplex::pop_back(void) {
    elements.pop_back();
}


void VectorComplex::pop_front(void) {
    
    // free the memory
    elements.erase(elements.begin());
}





/** Print value for user */
void VectorComplex::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(1);
    for (std::vector<std::complex<double> >::const_iterator i = elements.begin(); i!= elements.end(); i++) 
        {
        if (i != elements.begin())
            o << ", ";
        o << (*i);
        }
    o <<  " ]";

    o.setf(previousFlags);
    o.precision(previousPrecision);
}


/** Push an int onto the back of the vector after checking */
void VectorComplex::push_back( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(Complex_name) ) ) {
        elements.push_back(static_cast<Complex*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Complex_name) ) {
        elements.push_back(static_cast<Complex*>(x->convertTo(Complex_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Complex_name + "[] with invalid value" );
    }
}


/** Append element to end of vector, updating length in process */
void VectorComplex::push_back(std::complex<double> x) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorComplex::push_front( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(Complex_name) ) ) {
        elements.insert( elements.begin(), static_cast<Complex*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Complex_name) ) {
        elements.insert( elements.begin(), static_cast<Complex*>(x->convertTo(Complex_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Complex_name + "[] with invalid value" );
    }
}


/** Add element in front of vector, updating length in process */
void VectorComplex::push_front(std::complex<double> x) {
    
    elements.insert(elements.begin(), x);
}


void VectorComplex::resize(size_t n) {
    elements.resize(n);
}


/** Complete info about object */
std::string VectorComplex::richInfo(void) const {

    std::ostringstream o;
    printValue(o);
    return o.str();
}


void VectorComplex::setElement(const size_t index, RbPtr<RbLanguageObject> x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( TypeSpec(Complex_name) ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<Complex*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Complex_name) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<Complex*>(x->convertTo(Complex_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Complex_name + "[] with invalid value" );
    }
}

/** Set value of vector using STL vector */
void VectorComplex::setValue(const std::vector<std::complex<double> >& x) {

    elements = x;
}   


/** Set value of vector using VectorComplex */
void VectorComplex::setValue(const VectorComplex& x) {

    elements = x.elements;
}   


size_t VectorComplex::size(void) const {
    return elements.size();
}


/** Sort the vector */
void VectorComplex::sort( void ) {
    
//    std::sort(elements.begin(), elements.end());
    return;
    
}

/** Remove consecutive duplicates and resizes the vector */
void VectorComplex::unique(void) {
/*    std::vector<RbLanguageObject*> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (int i = 1 ; i< elements.size() ; i++)
    {
        if (*(static_cast<Complex*>(elements[i])) != *(static_cast<Complex*>(elements[i-1])))
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    length = elements.size();
    return;
    */
}
