/**
 * @file
 * This file contains the implementation of Probability, which
 * is the primitive RevBayes type for positive real numbers between 0 and 1.
 *
 *
 * @brief Implementation of Probability
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "Probability.h"
#include "RealPos.h"
#include "RbException.h"
#include "RlString.h"
#include "RbUtil.h"
#include "StringUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Probability::Probability( void ) : RealPos( 1.0 ) {

    setGuiVariableName("Probability");
    setGuiLatexSymbol("P");
}


/** Construct from double */
Probability::Probability( double x ) : RealPos( x ) {
    
    setGuiVariableName("Probability");
    setGuiLatexSymbol("P");
    if ( x < 0.0 || x > 1.0)
        throw RbException( "Creation of " + getClassType() + " with value x=" + x + " outside standard probabilities (0,1)");
}


/** Construct from double */
Probability::Probability( RevBayesCore::TypedDagNode<double> *x ) : RealPos( x ) {
    
    setGuiVariableName("Probability");
    setGuiLatexSymbol("P");
}


/** Clone object */
Probability* Probability::clone( void ) const {
    
	return new Probability( *this );
}


/** Get Rev type of object */
const std::string& Probability::getClassType(void) { 
    
    static std::string revType = "Probability";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Probability::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RealPos::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& Probability::getMethods( void ) const
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
const TypeSpec& Probability::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

