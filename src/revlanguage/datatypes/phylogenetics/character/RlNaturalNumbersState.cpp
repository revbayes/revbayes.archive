/**
 * @file
 * This file contains the implementation of RlNaturalNumbersState, which is
 * a RevBayes wrapper around a regular NaturalNumbers character.
 *
 * @brief Implementation of RlNaturalNumbersState
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: RlNaturalNumbersState.cpp $
 */

#include "RlNaturalNumbersState.h"
#include "ConstantNode.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include <sstream>

using namespace RevLanguage;

/** Default constructor */
NaturalNumbersState::NaturalNumbersState(void) : ModelObject<RevBayesCore::NaturalNumbersState>() {
    
}

/** Construct from bool */
NaturalNumbersState::NaturalNumbersState(const RevBayesCore::NaturalNumbersState &d) : ModelObject<RevBayesCore::NaturalNumbersState>( new RevBayesCore::NaturalNumbersState(d) ) {
    
}

NaturalNumbersState::NaturalNumbersState( RevBayesCore::TypedDagNode<RevBayesCore::NaturalNumbersState> *v ) : ModelObject<RevBayesCore::NaturalNumbersState>( v ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
NaturalNumbersState* NaturalNumbersState::clone(void) const {
    
	return new NaturalNumbersState(*this);
}



/** Get Rev type of object */
const std::string& NaturalNumbersState::getClassType(void) {
    
    static std::string revType = "NaturalNumbers";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& NaturalNumbersState::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Get type spec */
const TypeSpec& NaturalNumbersState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}




