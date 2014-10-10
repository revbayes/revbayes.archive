/**
 * @file
 * This file contains the implementation of RlBoolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of RlBoolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-09-04 20:14:58 +0200 (Tue, 04 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: RlBoolean.cpp 1793 2012-09-04 18:14:58Z hoehna $
 */


#include "ConstantNode.h"
#include "RlAminoAcidState.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
AminoAcidState::AminoAcidState(void) : ModelObject<RevBayesCore::AminoAcidState>() {
    
}

/** Construct from bool */
AminoAcidState::AminoAcidState(const RevBayesCore::AminoAcidState &d) : ModelObject<RevBayesCore::AminoAcidState>( new RevBayesCore::AminoAcidState(d) ) {
    
}


/** Clone object */
AminoAcidState* AminoAcidState::clone(void) const {
    
	return new AminoAcidState(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* AminoAcidState::convertTo(const TypeSpec& type) const {
    
    return RevObject::convertTo(type);
}


/** Get Rev type of object */
const std::string& AminoAcidState::getClassType(void) { 
    
    static std::string revType = "AminoAcid";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& AminoAcidState::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& AminoAcidState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
bool AminoAcidState::isConvertibleTo(const TypeSpec& type, bool once) const {
    
    return RevObject::isConvertibleTo(type, once);
}

