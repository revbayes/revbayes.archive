/**
 * @file
 * This file contains the implementation of DNA, which is
 * the data type for DNA characters in RevBayes.
 *
 * @brief Implementation of DNA
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "DNA.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "RNA.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor of NA characer */
DNA::DNA( void ) : Nucleotide() {
}


/** Construct from int value. */
DNA::DNA( int x ) : Nucleotide( x ) {

    if ( !isValidState( value ) )
        throw RbException( "Invalid DNA state code '" + RbString( x ) + "'" );
}


/** Construct from char value (= character code: A, C, G, T, -, ?, X) */
DNA::DNA( char c ) : Nucleotide( c ) {

    if ( !isValidState( value ) )
        throw RbException( "Invalid DNA state '" + RbString( c ) + "'" );
}


/** Construct from RNA character. The state space is the same so just use Nucleotide copy constructor. */
DNA::DNA( const RNA& x) : Nucleotide( x ) {
}


/** Clone object */
DNA* DNA::clone( void ) const {

	return  new DNA( *this );
}


/** Get string of state labels */
const std::string& DNA::getStateLabels( void ) const {

    static std::string stateLabels = std::string( "ACGT" );
    return stateLabels;
}


/** Convert to type and dim. The caller manages the returned object. */
RbObject* DNA::convertTo( const std::string& type, int dim ) const {

    if ( type == RNA_name && dim == 0 ) 
        return new RNA( *this );

    if ( type == RbString_name && dim == 0) {
        
        std::ostringstream o;
        printValue( o );
        return new RbString( o.str() );
    }

    return Nucleotide::convertTo( type, dim );
}


/** Get class vector describing type of object */
const VectorString& DNA::getClass() const {

    static VectorString rbClass = VectorString( DNA_name ) + RbObject::getClass();
    return rbClass;
}


/** Is convertible to type and dim? */
bool DNA::isConvertibleTo( const std::string& type, int dim, bool once ) const {

    if ( type == RNA_name && dim == 0 ) 
        return true;

    if ( type == RbString_name && dim == 0 )
        return true;

    return Nucleotide::isConvertibleTo( type, dim, once );
}


/** Get complete info about object */
std::string DNA::richInfo( void ) const {

	std::ostringstream o;
    o << "DNA(";
    printValue( o );
	o << ")";

    return o.str();
}

