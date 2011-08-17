/**
 * @file
 * This file contains the implementation of Character, which is
 * the abstract base class for Character data types in RevBayes.
 *
 * @brief Implementation of Character
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Character.h"
#include "RbNames.h"
#include "VectorString.h"



/** Default constructor */
Character::Character(void) : Categorical() {

}


/** Get class vector describing type of object */
const VectorString& Character::getClass() const {

    static VectorString rbClass = VectorString( Character_name ) + Categorical::getClass();
    return rbClass;
}



