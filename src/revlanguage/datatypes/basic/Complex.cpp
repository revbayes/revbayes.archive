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

    setGuiVariableName("Complex");
    setGuiLatexSymbol("C");
}


/** Copy constructor */
Complex::Complex(const Complex& c) : ModelObject<std::complex<double> >( c ) {

    setGuiVariableName("Complex");
    setGuiLatexSymbol("C");
}


/** Construct from double */
Complex::Complex(double x) : ModelObject<std::complex<double> >( new std::complex<double>( x, 0.0 ) ) {

    setGuiVariableName("Complex");
    setGuiLatexSymbol("C");
}


/** Construct from int */
Complex::Complex(int x) : ModelObject<std::complex<double> >( new std::complex<double>( double(x), 0.0 ) ) {

    setGuiVariableName("Complex");
    setGuiLatexSymbol("C");
}


/** Construct from complex */
Complex::Complex(const std::complex<double>& c) : ModelObject<std::complex<double> >( new std::complex<double>(c) ) {

    setGuiVariableName("Complex");
    setGuiLatexSymbol("C");
}


/** Clone object */
Complex* Complex::clone(void) const {

	return new Complex(*this);
}


/** Get Rev type of object */
const std::string& Complex::getClassType(void) { 
    
    static std::string revType = "Complex";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Complex::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& Complex::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
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
    
    dagNode->printValue( o, "" );

    o.setf( previousFlags );
    o.precision( previousPrecision );
}


