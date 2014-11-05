/*
 *  RlNaturalNumbersState.cpp
 *  revbayes-proj
 *
 *  Created by Will Freyman on 9/8/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
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


/** Clone object */
NaturalNumbersState* NaturalNumbersState::clone(void) const {
    
	return new NaturalNumbersState(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* NaturalNumbersState::convertTo(const TypeSpec& type) const {
    
    return RevObject::convertTo(type);
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


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& NaturalNumbersState::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get type spec */
const TypeSpec& NaturalNumbersState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
bool NaturalNumbersState::isConvertibleTo(const TypeSpec& type, bool once) const {
    
    return RevObject::isConvertibleTo(type, once);
}

