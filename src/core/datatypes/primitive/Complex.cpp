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

#include <iomanip>
#include <sstream>


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


/** Overloaded equals operator */
bool Complex::operator==(const Complex &x) const {
    
    return value == x.value;
}

/** Overloaded operator +=. */
Complex& Complex::operator+=(const Complex &A) {
    value += A.getValue();
    
    return *this;
}

/** Overloaded operator +=. */
Complex& Complex::operator+=(const double A) {
    value += A;
    
    return *this;
}

/** Overloaded operator -=. */
Complex& Complex::operator-=(const Complex &A) {
    value -= A.getValue();
    
    return *this;
}

/** Overloaded operator -=. */
Complex& Complex::operator-=(const double A) {
    value -= A;
    
    return *this;
}

/** Overloaded operator *=. */
Complex& Complex::operator*=(const Complex &A) {
    value *= A.getValue();
    
    return *this;
}

/** Overloaded operator *=. */
Complex& Complex::operator*=(const double A) {
    value *= A;
    
    return *this;
}

/** Overloaded operator /=. */
Complex& Complex::operator/=(const Complex &A) {
    value /= A.getValue();
    
    return *this;
}

/** Overloaded operator /=. */
Complex& Complex::operator/=(const double A) {
    value /= A;
    
    return *this;
}


/** Clone object */
Complex* Complex::clone(void) const {

	return  new Complex(*this);
}


/** Get class name of object */
const std::string& Complex::getClassName(void) { 
    
    static std::string rbClassName = "Complex";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Complex::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Complex::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Get a pointer to the elementary value. */
void* Complex::getLeanValue(std::vector<size_t> &length) const {
    return static_cast<void*>( &const_cast<std::complex<double>& >( value ) );
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


void Complex::setLeanValue(const RbValue<void *> &val) {
    value = *static_cast<std::complex<double> *>( val.value );
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
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator+
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator+(const Complex& A, const Complex& B) {
    
	Complex C = A.getValue() + B.getValue();
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator+
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator+(const Complex& A, const double B) {
    
	Complex C = A.getValue() + B;
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator+
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator+(const double A, const Complex& B) {
    
	Complex C = A + B.getValue();
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator-
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator-(const Complex& A, const Complex& B) {
    
	Complex C = A.getValue() - B.getValue();
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator-
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator-(const Complex& A, const double B) {
    
	Complex C = A.getValue() - B;
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator-
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator-(const double A, const Complex& B) {
    
	Complex C = A - B.getValue();
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator*
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator*(const Complex& A, const Complex& B) {
    
	Complex C = A.getValue() * B.getValue();
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator*
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator*(const Complex& A, const double B) {
    
	Complex C = A.getValue() * B;
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator*
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator*(const double A, const Complex& B) {
    
	Complex C = A * B.getValue();
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator/
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator/(const Complex& A, const Complex& B) {
    
	Complex C = A.getValue() / B.getValue();
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator/
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator/(const Complex& A, const double B) {
    
	Complex C = A.getValue() / B;
	return C;
}


/**
 * This function performs unary plus on a complex,
 * which simply returns a copy of the complex.
 *
 * @brief binary operator/
 *
 * @param  A The complex operand
 * @return A copy of the operand
 */
Complex operator/(const double A, const Complex& B) {
    
	Complex C = A / B.getValue();
	return C;
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
RbBoolean operator!=(const Complex& A, const Complex& B){
    
    return std::complex<double> ( A ) != std::complex<double> ( B );
}

RbBoolean operator< (const Complex& A, const Complex& B){
    
    return RbBoolean( std::complex<double> ( A ) < std::complex<double> ( B ) );

}


