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
#include "RbUtil.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor */
Probability::Probability( void ) : RealPos( 1.0 ) {
}


/** Construct from double */
Probability::Probability( double x ) : RealPos( x ) {
    
    if ( x < 0.0 || x > 1.0)
        throw RbException( "Creation of " + Probability_name + " with value x=" + x + "outside standard probabilities (0,1)");
}


/** Construct from int */
Probability::Probability( int x ) : RealPos( x ) {
    
    if ( x < 0 || x > 1)
        throw RbException( "Creation of " + Probability_name + " with value x=" + x + "outside standard probabilities (0,1)");
}


/** Construct from unsigned int */
Probability::Probability( unsigned int x ) : RealPos( x ) {
    
    if ( x > 1)
        throw RbException( "Creation of " + Probability_name + " with value x=" + int(x) + "outside standard probabilities (0,1)");
}


/** Get brief info about object */
std::string Probability::briefInfo( void ) const {
    
	std::ostringstream o;
    o << "Prob(";
    printValue( o );
	o << ")";
    
    return o.str();
}


/** Clone object */
Probability* Probability::clone( void ) const {
    
	return  new Probability( *this );
}


/** Get class vector describing type of object */
const VectorString& Probability::getClass() const {
    
    static VectorString rbClass = VectorString( Probability_name ) + RealPos::getClass();
    return rbClass;
}


/** Get complete info about object */
std::string Probability::richInfo( void ) const {
    
	std::ostringstream o;
    o << "Prob(";
    printValue( o );
	o << ")";
    
    return o.str();
}


/** Set value */
void Probability::setValue( double x ) {
    
    if ( x < 0.0 || x > 1.0)
        throw RbException( "Creation of " + Probability_name + " with value x=" + x + "outside standard probabilities (0,1)");
    
    RealPos::setValue( x );
}

