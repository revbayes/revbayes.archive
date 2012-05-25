/**
 * @file
 * This file contains the implementation of RlDiscreteCharacterState, which is
 * the abstract base class for discrete character data types in RevBayes.
 *
 * @brief Implementation of RlDiscreteCharacterState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RlDiscreteCharacterState.h"
#include "ConstArgumentRule.h"
#include "MemberObject.h"
#include "Natural.h"
#include "RbException.h"
#include "RbString.h"
#include "RbUtil.h"
#include "RlVector.h"



/** Constructor taking the number of states */
RlDiscreteCharacterState::RlDiscreteCharacterState() : RlCharacterState() {

}


/** Get class name of object */
const std::string& RlDiscreteCharacterState::getClassName(void) { 
    
    static std::string rbClassName = "Discrete character";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlDiscreteCharacterState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlCharacterState::getClassTypeSpec() ) );
    
	return rbClass; 
}


const MemberRules& RlDiscreteCharacterState::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ConstArgumentRule( "state"  , RbString::getClassTypeSpec() ) );
       
        rulesSet = true;
    }
    
    return memberRules;
}


/** Initialize the discrete character state variable. We just set the value. */
void RlDiscreteCharacterState::initialize(const std::vector<RbObject*> &attributes) {

    throw RbException("Missing implementation in RlDiscreteCharacterState::initialize()");
    // set the state
//    setState(static_cast<RbString*>( attributes[0] )->getValue()[0]);
//    numStates = static_cast<Natural*>( (RbLanguageObject*)(*attr)[0] )->getValue();
}

