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

#include "RlCharacterState.h"
#include "RbUtil.h"



/** Default constructor */
RlCharacterState::RlCharacterState(void) : RbLanguageObject() {

}


/** Get class name of object */
const std::string& RlCharacterState::getClassName(void) { 
    
    static std::string rbClassName = "Character State";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlCharacterState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}



