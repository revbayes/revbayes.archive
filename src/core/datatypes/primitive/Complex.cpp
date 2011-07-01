/**
 * @file
 * This file contains the implementation of Complex, which
 * is the primitive RevBayes type for Complex numbers.
 *
 * @brief Implementation of Complex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id:$
 */


#include "Boolean.h"
#include "Complex.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <iomanip>
#include <sstream>


/** Default constructor */
Complex::Complex(void) : RbObject() {

    value = std::complex<double>( 0.0, 0.0 );
}


/** Copy constructor */
Complex::Complex(const Complex& c) : RbObject() {

    value = c.value;
}


/** Construct from double */
Complex::Complex(const double x) : RbObject() {

    value = std::complex<double>( x, 0.0 );
}


/** Construct from int */
Complex::Complex(const int x) : RbObject() {

    value = std::complex<double>( (double)x, 0.0 );
}


/** Construct from unsigned int */
Complex::Complex(const unsigned int x) : RbObject() {

    value = std::complex<double>( (double)x, 0.0 );
}


/** Construct from bool */
Complex::Complex(const bool v) : RbObject() {

    if (v == true) 
        value = std::complex<double>(1.0, 0.0);
    else 
        value = std::complex<double>(0.0, 0.0);
}


/** Construct from complex */
Complex::Complex(const std::complex<double>& c) : RbObject() {

    value = c;
}


/** Clone object */
Complex* Complex::clone(void) const {

	return  new Complex(*this);
}


/** Convert to type and dim. The caller manages the returned object. */
RbObject* Complex::convertTo(const std::string& type, size_t dim) const {

    return RbObject::convertTo( type, dim );
}


/** Get class vector describing type of object */
const VectorString& Complex::getClass(void) const {

    static VectorString rbClass = VectorString(Complex_name) + RbObject::getClass();
    return rbClass;
}


/** Is convertible to type and dim? */
bool Complex::isConvertibleTo(const std::string& type, size_t dim, bool once) const {

    return RbObject::isConvertibleTo(type, dim, once);
}


/** Print value for user */
void Complex::printValue(std::ostream &o) const {

    size_t previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    std::fixed(o);
    o.precision( 3 );
    
    o << value;

    o.setf( previousFlags );
    o.precision( previousPrecision );
}


/** Get complete info about object */
std::string Complex::richInfo(void) const {

	std::ostringstream o;
    printValue(o);
    return o.str();
}

