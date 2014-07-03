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


#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Complex.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <iomanip>
#include <sstream>

using namespace RevLanguage;


/** Default constructor */
Complex::Complex(void) : ModelObject<std::complex<double> >() {

}


/** Copy constructor */
Complex::Complex(const Complex& c) : ModelObject<std::complex<double> >( c ) {

}


/** Construct from double */
Complex::Complex(double x) : ModelObject<std::complex<double> >( new std::complex<double>( x, 0.0 ) ) {

}


/** Construct from int */
Complex::Complex(int x) : ModelObject<std::complex<double> >( new std::complex<double>( double(x), 0.0 ) ) {

}


/** Construct from complex */
Complex::Complex(const std::complex<double>& c) : ModelObject<std::complex<double> >( new std::complex<double>(c) ) {

}


/** Clone object */
Complex* Complex::clone(void) const {

	return new Complex(*this);
}


/** Get class name of object */
const std::string& Complex::getClassName(void) { 
    
    static std::string rbClassName = "Complex";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Complex::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Complex::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void Complex::printValue(std::ostream &o) const {

    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    std::fixed(o);
    o.precision( 3 );
    
    o << value->getValue();

    o.setf( previousFlags );
    o.precision( previousPrecision );
}


