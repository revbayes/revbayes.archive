/**
 * @file
 * This file contains the implementation of Move_mscale, which scales a
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Implementation of Move_mscale
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
#include "Move_mscale.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser */
Move_mscale::Move_mscale(void)
    : SimpleMove(getMemberRules()) {
}


/** Constructor for internal use */
Move_mscale::Move_mscale(StochasticNode* node, double lambda, double weight, RandomNumberGenerator* rng)
    : SimpleMove(getMemberRules()) {

    setValue("lambda", new RealPos(lambda));
    setValue("weight", new RealPos(weight));
    setValue("rng",    rng);

    setNodePtr(node);
}


/** Clone object */
RbObject* Move_mscale::clone(void) const {

    return new Move_mscale(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Move_mscale::convertTo(const std::string& type) const {

    return NULL;
}

/** Get class vector describing type of object */
const VectorString& Move_mscale::getClass() const {

    static VectorString rbClass = VectorString(Move_mscale_name) + Move::getClass();
    return rbClass;
}

/** Convert to object of another class. The caller manages the object. */
bool Move_mscale::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Return member rules */
const MemberRules& Move_mscale::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back(new ArgumentRule("lambda", RealPos_name));

        /* Inherit weight and rng from Move, put it at back */
        const MemberRules& inheritedRules = Move::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 

        rulesSet = true;
    }

    return memberRules;
}


/** Perform the move */
double Move_mscale::perform(std::set<StochasticNode*>& affectedNodes) {

    RbDouble*              valPtr = (RbDouble*)(nodePtr->getValuePtr(affectedNodes));
    RandomNumberGenerator* rng    = (RandomNumberGenerator*)(getValue("rng"));
    double                 lambda = ((RbDouble*)(getValue("lambda")))->getValue();

    double curVal = valPtr->getValue();
    double u      = rng->uniform01();
    double newVal = curVal * std::exp(lambda*(u-0.5));

    valPtr->setValue(newVal);
	
    return log(newVal/curVal);
}


