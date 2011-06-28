/**
 * @file
 * This file contains the implementation of some common functions
 * in Move, which is the abstract base class for moves in mcmc
 * inference.
 *
 * @brief Partial implementation of Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id:$
 */

#include "DAGNode.h"
#include "Move.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbNames.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <iostream>
#include <string>
#include <vector>


/** Constructor for parser use */
Move::Move(const MemberRules& memberRules)
    : MemberObject(memberRules) {

    numAccepted = 0;
    numTried    = 0;
}


/** Calculate acceptance probability */
double Move::getAcceptanceProbability(void) {

    return numAccepted/(double)numTried;
}


/** Get member rules: weight and random number generator */
const MemberRules& Move::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back(new ValueRule("weight", new RealPos(1.0)));

        rulesSet = true;
    }

    return memberRules;
}


/** Get class vector describing type of object */
const VectorString& Move::getClass(void) const { 

    static VectorString rbClass = VectorString(Move_name) + RbComplex::getClass();
	return rbClass;
}


/** Get methods */
const MethodTable& Move::getMethods(void) const {

    static MethodTable methods = MethodTable( const_cast<MethodTable*>( &(MemberObject::getMethods()) ) );

    return methods;
}


/** Get update weight */
double Move::getUpdateWeight(void) const {

    return static_cast<const Real*>( getValue("weight") )->getValue();
}


/** Reset counters (numTried, numAccepted) */
void Move::resetCounters(void) {

    numAccepted = 0;
    numTried    = 0;
}


/** Set update weight but not negative */
void Move::setUpdateWeight(double weight) {

    if (weight < 0.0)
        throw RbException("Negative update weight not allowed");

    setValue("weight", new Real(weight));
}


/** Only allow constant values for member variables */
void Move::setVariable(const std::string& name, DAGNode* var) {

    if (!var->isDAGType(ConstantNode_name))
        throw RbException("Only constant values allowed for member variables");

    MemberObject::setVariable(name, var);
}


