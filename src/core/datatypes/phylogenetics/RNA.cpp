/**
 * @file
 * This file contains the implementation of RNA, which is
 * the data type for RNA characters in RevBayes.
 *
 * @brief Implementation of RNA
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
RNA::RNA( void ) : Nucleotide() {
}


/** Construct from int value. */
RNA::RNA( int x ) : Nucleotide( x ) {

    if ( !isValidState( value ) )
        throw RbException( "Invalid RNA state code '" + RbString( x ) + "'" );
}


/** Construct from char value (= character code: A, C, G, U, -, ?, X) */
RNA::RNA( char c ) : Nucleotide( convertSymbolToState( c ) ) {

    if ( !isValidState( value ) )
        throw RbException( "Invalid RNA state '" + RbString( c ) + "'" );
}


/** Construct from RNA character. The state space is the same so just use Nucleotide copy constructor. */
RNA::RNA( const DNA& x) : Nucleotide( x ) {
}


/** Clone object */
RNA* RNA::clone( void ) const {

	return  new RNA( *this );
}


/** Get string of state labels */
const std::string& RNA::getStateLabels( void ) const {

    static std::string stateLabels = std::string( "ACGU" );
    return stateLabels;
}


/** Convert to type and dim. The caller manages the returned object. */
RbObject* RNA::convertTo( const std::string& type, int dim ) const {

    if ( type == DNA_name && dim == 0 ) 
        return new DNA( *this );

    if ( type == RbString_name && dim == 0) {
        
        std::ostringstream o;
        printValue( o );
        return new RbString( o.str() );
    }

    return Nucleotide::convertTo( type, dim );
}


/** Get class vector describing type of object */
const VectorString& RNA::getClass() const {

    static VectorString rbClass = VectorString( RNA_name ) + RbObject::getClass();
    return rbClass;
}


/** Is convertible to type and dim? */
bool RNA::isConvertibleTo( const std::string& type, int dim, bool once ) const {

    if ( type == DNA_name && dim == 0 ) 
        return true;

    if ( type == RbString_name && dim == 0 )
        return true;

    return Nucleotide::isConvertibleTo( type, dim, once );
}


/** Get complete info about object */
std::string RNA::richInfo( void ) const {

	std::ostringstream o;
    o << "RNA(";
    printValue( o );
	o << ")";

    return o.str();
}

