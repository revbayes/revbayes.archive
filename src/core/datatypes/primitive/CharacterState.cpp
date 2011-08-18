/**
 * @file
 * This file contains the implementation of CharacterState, which is
 * the abstract base class for character state data types in RevBayes.
 *
 * @brief Implementation of CharacterState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "CharacterState.h"
#include "RbException.h"
#include "RbUtil.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor */
CharacterState::CharacterState( void ) : Categorical() {
}


/** Constructor from int value: call base class constructor */
CharacterState::CharacterState( int x ) : Categorical( x ) {
}


/** Construct from char value, taken to be a valid state symbol or NA symbol */
CharacterState::CharacterState( char c ) : Categorical( c ) {
}


/** Get class vector describing type of object */
const VectorString& CharacterState::getClass() const {

    static VectorString rbClass = VectorString( CharacterState_name ) + Categorical::getClass();
    return rbClass;
}

