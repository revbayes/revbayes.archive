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
 * $Id$
 */


#include "RbBoolean.h"
#include "Complex.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <iomanip>
#include <sstream>


// Definition of the static type spec member
const TypeSpec Complex::typeSpec(Complex_name);

/** Default constructor */
Complex::Complex(void) : RbLanguageObject() {

    value = std::complex<double>( 0.0, 0.0 );
}


/** Copy constructor */
Complex::Complex(const Complex& c) : RbLanguageObject() {

    value = c.value;
}


/** Construct from double */
Complex::Complex(const double x) : RbLanguageObject() {

    value = std::complex<double>( x, 0.0 );
}


/** Construct from int */
Complex::Complex(const int x) : RbLanguageObject() {

    value = std::complex<double>( (double)x, 0.0 );
}


/** Construct from unsigned int */
Complex::Complex(const unsigned int x) : RbLanguageObject() {

    value = std::complex<double>( (double)x, 0.0 );
}


/** Construct from bool */
Complex::Complex(const bool v) : RbLanguageObject() {

    if (v == true) 
        value = std::complex<double>(1.0, 0.0);
    else 
        value = std::complex<double>(0.0, 0.0);
}


/** Construct from complex */
Complex::Complex(const std::complex<double>& c) : RbLanguageObject() {

    value = c;
}


/** Clone object */
Complex* Complex::clone(void) const {

	return  new Complex(*this);
}


/** Get class vector describing type of object */
const VectorString& Complex::getClass(void) const {

    static VectorString rbClass = VectorString(Complex_name) + RbLanguageObject::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Complex::getTypeSpec(void) const {
    return typeSpec;
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
    o << "Complex(";
    printValue(o);
    o << ")";
    return o.str();
}

////////////////////////////////// Global Complex operators ///////////////////////////////////


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief unary operator+
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator+(const Complex& A) {
    
	Complex B = A;
	return B;
}


/**
 * This function performs unary minus on an integer.
 *
 * @brief operator-
 *
 * @param A The integer operand
 * @return -A
 */
Complex operator-(const Complex& A) {
    
	Complex B( - A.getValue() );
    
    return B;
}


/**
 * This function performs unary not on an integer by
 * comparing the value to 0. It returns true if the
 * value is different from 0.
 *
 * @brief operator!
 *
 * @param A The integer operand
 * @return True if A != 0
 */
RbBoolean operator!(const Complex& A) {
    
    std::complex<double> zero (0, 0);
	RbBoolean B( A.getValue() != zero );
    
    return B;
}


/** Operator == for testing equality */
RbBoolean       operator==(const Complex& A, const Complex& B){
    
    return std::complex<double> ( A ) == std::complex<double> ( B );
}

/** Operator != for testing inequality */
RbBoolean       operator!=(const Complex& A, const Complex& B){
    
    return std::complex<double> ( A ) != std::complex<double> ( B );
}

RbBoolean                     operator< (const Complex& A, const Complex& B){
    
    return std::complex<double> ( A ) < std::complex<double> ( B );

}


