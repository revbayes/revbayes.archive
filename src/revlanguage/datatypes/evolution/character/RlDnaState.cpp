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
#include "RlDnaState.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
DnaState::DnaState(void) : RlModelVariableWrapper<RevBayesCore::DnaState>() {
    
}

/** Construct from bool */
DnaState::DnaState(const RevBayesCore::DnaState &d) : RlModelVariableWrapper<RevBayesCore::DnaState>( new RevBayesCore::DnaState(d) ) {
    
}


/** Clone object */
DnaState* DnaState::clone(void) const {
    
	return new DnaState(*this);
}


/** Convert to type. The caller manages the returned object. */
RbLanguageObject* DnaState::convertTo(const TypeSpec& type) const {
    
    return RbLanguageObject::convertTo(type);
}


/** Get class name of object */
const std::string& DnaState::getClassName(void) { 
    
    static std::string rbClassName = "DNA";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DnaState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DnaState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
bool DnaState::isConvertibleTo(const TypeSpec& type) const {
    
    return RbLanguageObject::isConvertibleTo(type);
}

