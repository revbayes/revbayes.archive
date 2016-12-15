/**
 * @file
 * This file contains the implementation of RlPomoState, which is
 * a RevBayes wrapper around a regular PomoState.
 *
 * @brief Implementation of RlPomoState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-09-04 20:14:58 +0200 (Tue, 04 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: RlPomoState.cpp 1793 2012-09-04 18:14:58Z Boussau $
 */


#include "ConstantNode.h"
#include "RlPomoState.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
PomoState::PomoState(void) : ModelObject<RevBayesCore::PomoState>() {
    
}

/** Construct from bool */
PomoState::PomoState(const RevBayesCore::PomoState &d) : ModelObject<RevBayesCore::PomoState>( new RevBayesCore::PomoState(d) ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
PomoState* PomoState::clone(void) const {
    
	return new PomoState(*this);
}

/** Get Rev type of object */
const std::string& PomoState::getClassType(void) {
    
    static std::string rev_type = "Pomo";
    
	return rev_type;
}

/** Get class type spec describing type of object */
const TypeSpec& PomoState::getClassTypeSpec(void) {
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/** Get type spec */
const TypeSpec& PomoState::getTypeSpec( void ) const {
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}

