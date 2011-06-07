/**
 * @file
 * This file contains the implementation of Nucleotide, which is
 * the abstract base class for nucleotide data types in RevBayes.
 *
 * @brief Implementation of Nucleotide
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "Nucleotide.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor */
Nucleotide::Nucleotide( void ) : CharacterState() {
}


/** Constructor from int value: call base class constructor */
Nucleotide::Nucleotide( int x ) : CharacterState( x ) {
}


/** Construct from char value, taken to be a valid state symbol or NA symbol */
Nucleotide::Nucleotide( char c ) : CharacterState( c ) {
}


/** Get class vector describing type of object */
const VectorString& Nucleotide::getClass() const {

    static VectorString rbClass = VectorString( Nucleotide_name ) + CharacterState::getClass();
    return rbClass;
}


/** Get string of NA labels */
const std::string& Nucleotide::getNALabels() const {

    static std::string NALabels = std::string( "-?X" );
    return NALabels;
}

