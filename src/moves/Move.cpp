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
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Move.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "Workspace.h"
#include "WrapperRule.h"

#include <iostream>
#include <string>
#include <vector>


/** Constructor for parser use */
Move::Move(const MemberRules& memberRules)
    : MemberObject(memberRules, MethodTable()) {

    numAccepted = 0;
    numTried    = 0;
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Move::convertTo(const std::string& type) const {

    return NULL;
}

/** Execute member method. We throw an error because there are no visible member methods */
const RbObject* Move::executeOperation(const std::string& name, std::vector<DAGNode*>& args) {

    throw RbException ("Object does not have methods");
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

        memberRules.push_back(new ArgumentRule("weight", new RealPos(1.0)));
        memberRules.push_back(new WrapperRule( "rng",    Workspace::globalWorkspace().getVariable("_rng")));

        rulesSet = true;
    }

    return memberRules;
}


/** Get class vector describing type of object */
const VectorString& Move::getClass(void) const { 

    static VectorString rbClass = VectorString(Move_name) + RbComplex::getClass();
	return rbClass;
}


/** Get method inits */
const MethodTable& Move::getMethodInits(void) const {

    static MethodTable methodInits = MethodTable();

    return methodInits;
}


/** Get update weight */
double Move::getUpdateWeight(void) const {

    return ((Real*)(getValue("weight")))->getValue();
}

/** Convert to object of another class. The caller manages the object. */
bool Move::isConvertibleTo(const std::string& type) const {

    return false;
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

    if (!var->isType(ConstantNode_name))
        throw RbException("Only constant values allowed for member variables");

    MemberObject::setVariable(name, var);
}


