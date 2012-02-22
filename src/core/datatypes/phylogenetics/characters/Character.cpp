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
#include "RbUtil.h"
#include "VectorString.h"



/** Default constructor */
Character::Character(void) : Categorical() {

}


/** Get class name of object */
const std::string& Character::getClassName(void) { 
    
    static std::string rbClassName = "Character";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Character::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Categorical::getClassTypeSpec() ) );
    
	return rbClass; 
}



