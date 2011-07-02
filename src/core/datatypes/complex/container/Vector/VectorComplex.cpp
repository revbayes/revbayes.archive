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
 * @extends RbComplex
 *
 * $Id$
 */

#include "Complex.h"
#include "ContainerIterator.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorInteger.h"
#include "VectorComplex.h"
#include "VectorString.h"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>



/** Default constructor */
VectorComplex::VectorComplex(void) : Vector(Complex_name) {

}


/** Construct vector with one double x */
VectorComplex::VectorComplex(const double x) : Vector(Complex_name) {

    elements.push_back(new Complex(x));
    length[0] = 1;
}


/** Construct vector with n doubles x */
VectorComplex::VectorComplex(const size_t n, const double x) : Vector(Complex_name) {

    for (size_t i = 0; i < n; i++)
        elements.push_back(new Complex(x));
    length[0] = elements.size();
}


/** Constructor from double vector */
VectorComplex::VectorComplex(const std::vector<double>& x) : Vector(Complex_name) {

    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++)
        elements.push_back(new Complex(*i));
    length[0] = elements.size();
}


/** Constructor from complex vector */
VectorComplex::VectorComplex(const std::vector<std::complex<double> >& x) : Vector(Complex_name) {

    for (std::vector<std::complex<double> >::const_iterator i=x.begin(); i!=x.end(); i++)
        elements.push_back(new Complex(*i));
    length[0] = elements.size();
}


/** Constructor from VectorComplex */
VectorComplex::VectorComplex( const VectorComplex& x ) : Vector(Complex_name) {

    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( new Complex( x[i] ) );
    length[0] = elements.size();
}


VectorComplex::VectorComplex(size_t n, std::complex<double> x) : Vector(Complex_name) {

    for (size_t i = 0; i < n; i++)
        elements.push_back(new Complex(x));
    length[0] = elements.size();
}

/** Subscript operator */
std::complex<double>& VectorComplex::operator[](size_t i) {

    if (i >= int(elements.size()))
        throw RbException("Index out of bounds");

    return static_cast<Complex*>(elements[i])->getValueRef();
}


/** Subscript const operator */
const std::complex<double>& VectorComplex::operator[](size_t i) const {

    if (i >= int(elements.size()))
        throw RbException("Index out of bounds");
    return static_cast<Complex*>(elements[i])->getValueRef();
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


/** Get class vector describing type of object */
const VectorString& VectorComplex::getClass(void) const {

    static VectorString rbClass = VectorString(VectorComplex_name) + Vector::getClass();
    return rbClass;
}


/** Export value as STL vector */
std::vector<std::complex<double> > VectorComplex::getValue(void) const {

    std::vector<std::complex<double> > temp;
    for (size_t i=0; i<size(); i++)
        temp.push_back(operator[](i));
    return temp;
}


/** Print value for user */
void VectorComplex::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(1);
    for (std::vector<RbObject*>::const_iterator i = elements.begin(); i!= elements.end(); i++) 
        {
        if (i != elements.begin())
            o << ", ";
        o << *(*i);
        }
    o <<  " ]";

    o.setf(previousFlags);
    o.precision(previousPrecision);
}


/** Append element to end of vector, updating length in process */
void VectorComplex::push_back(std::complex<double> x) {

    elements.push_back(new Complex(x));
    length[0]++;
}


/** Add element in front of vector, updating length in process */
void VectorComplex::push_front(std::complex<double> x) {

    elements.insert(elements.begin(), new Complex(x));
    length[0]++;
}


/** Complete info about object */
std::string VectorComplex::richInfo(void) const {

    std::ostringstream o;
    printValue(o);
    return o.str();
}


/** Set value of vector using STL vector */
void VectorComplex::setValue(const std::vector<std::complex<double> >& x) {

    clear();
    for (std::vector<std::complex<double> >::const_iterator i=x.begin(); i!=x.end(); i++) {   
        elements.push_back(new Complex(*i));
        length[0]++;
    }
}   


/** Set value of vector using VectorComplex */
void VectorComplex::setValue(const VectorComplex& x) {

    clear();
    for (size_t i=0; i<x.size(); i++) 
        {   
        elements.push_back(new Complex(x[i]));
        length[0]++;
        }
}   

