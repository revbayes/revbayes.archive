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


/** Clone object */
PomoState* PomoState::clone(void) const {
    
	return new PomoState(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* PomoState::convertTo(const TypeSpec& type) const {
    
    return RevObject::convertTo(type);
}


/** Get Rev type of object */
const std::string& PomoState::getClassType(void) {
    
    static std::string revType = "Pomo";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& PomoState::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get type spec */
const TypeSpec& PomoState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
bool PomoState::isConvertibleTo(const TypeSpec& type, bool once) const {
    
    return RevObject::isConvertibleTo(type, once);
}

