/**
 * @file
 * This file contains the implementation of Real, which
 * is the primitive RevBayes type for real numbers.
 *
 * @brief Implementation of Real
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
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <iomanip>
#include <sstream>


// Definition of the static type spec member
const TypeSpec Real::typeSpec(Real_name);

/** Default constructor */
Real::Real(void) : RbLanguageObject(), value(0.0) {
}


/** Construct from double */
Real::Real(const double v) : RbLanguageObject(), value(v) {
}


/** Construct from int */
Real::Real(const int v) : RbLanguageObject(), value(v) {
}


/** Construct from unsigned int */
Real::Real(const unsigned int v) : RbLanguageObject(), value(v) {
}


/** Construct from bool */
Real::Real(const bool v) : RbLanguageObject() {

    if (v) value = 1.0;
    else value = 0.0;
}

/** Get brief info about object */
std::string Real::briefInfo(void) const {
    
	std::ostringstream o;
    printValue(o);
    return o.str();
}

/** Clone object */
Real* Real::clone(void) const {

	return  new Real(*this);
}


/** Convert to type. The caller manages the returned object. */
RbObject* Real::convertTo( const TypeSpec& type ) const {

    if ( type == RbBoolean_name )
        return new RbBoolean(value == 0.0);
    if ( type == RealPos_name && value > 0.0)
        return new RealPos(value);
    if ( type == Probability_name && value >= 0.0 && value <= 1.0)
        return new Probability(value);

    return RbLanguageObject::convertTo( type );
}


/** Get class vector describing type of object */
const VectorString& Real::getClass(void) const {

    static VectorString rbClass = VectorString(Real_name) + RbLanguageObject::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Real::getTypeSpec(void) const {
    return typeSpec;
}


/** Is convertible to type? */
bool Real::isConvertibleTo(const TypeSpec& type) const {

    if (type == RbBoolean_name)
        return true;
//    if (type == RealPos_name && value > 0.0)
//        return true;
//    if (type == Probability_name && value >= 0.0 && value <= 1.0)
//        return true;

    return RbLanguageObject::isConvertibleTo(type);
}


/** Print value for user */
void Real::printValue(std::ostream &o) const {

    size_t previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    std::fixed( o );
    o.precision( 3 );
    o << value;

    o.setf( previousFlags );
    o.precision( previousPrecision );
}


/** Get complete info about object */
std::string Real::richInfo(void) const {

	std::ostringstream o;
    o << "Real(";
    printValue(o);
	o << ")";
    return o.str();
}


////////////////////////////////// Global Real operators ///////////////////////////////////


/**
 * This function performs unary plus on a real,
 * which simply returns a copy of the real.
 *
 * @brief unary operator+
 *
 * @param  A The real operand
 * @return A copy of the operand
 */
Real operator+(const Real& A) {

	Real B = A;
	return B;
}


/**
 * This function performs unary minus on a real.
 *
 * @brief operator-
 *
 * @param A The real operand
 * @return -A
 */
Real operator-(const Real& A) {

	Real B( - A.getValue() );

    return B;
}


/**
 * This function performs unary not on a real by
 * comparing the value to 0.0. It returns true if the
 * value is different from 0.0.
 *
 * @brief operator!
 *
 * @param A The real operand
 * @return True if A != 0.0
 */
RbBoolean operator!(const Real& A) {

	RbBoolean B( A.getValue() != 0.0 );

    return B;
}

