/**
 * @file
 * This file contains the implementation of Move_mslide,
 * which moves a real value uniformly within a sliding window.
 *
 * @brief Implementation of Move_mslide
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
#include "Distribution.h"
#include "Move_mslide.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser */
Move_mslide::Move_mslide(void)
    : SimpleMove(getMemberRules()) {
}


/** Constructor for internal use */
Move_mslide::Move_mslide(StochasticNode* node, double delta, double weight, RandomNumberGenerator* rng)
    : SimpleMove(getMemberRules()) {

    setValue("delta",  new RealPos(delta));
    setValue("weight", new RealPos(weight));
    setValue("rng",    rng);

    setNodePtr(node);
}


/** Clone object */
Move_mslide* Move_mslide::clone(void) const {

    return new Move_mslide(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Move_mslide::convertTo(const std::string& type) const {

    return NULL;
}

/** Get class vector describing type of object */
const VectorString& Move_mslide::getClass() const {

    static VectorString rbClass = VectorString(Move_mslide_name) + Move::getClass();
    return rbClass;
}


/** Return member rules */
const MemberRules& Move_mslide::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back(new ArgumentRule("delta", RealPos_name));

        /* Inherit weight and rng from Move, put it at back */
        const MemberRules& inheritedRules = Move::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 

        rulesSet = true;
    }

    return memberRules;
}

/** Convert to object of another class. The caller manages the object. */
bool Move_mslide::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Perform the move */
double Move_mslide::perform(std::set<StochasticNode*>& affectedNodes) {

    Real*              valPtr = (Real*)(nodePtr->getValuePtr(affectedNodes));
    RandomNumberGenerator* rng    = (RandomNumberGenerator*)(getValue("rng"));
    double                 delta  = ((Real*)(getValue("delta")))->getValue();

    double minVal = ((Real*)(nodePtr->getDistribution()->getMin()))->getValue();
    double maxVal = ((Real*)(nodePtr->getDistribution()->getMax()))->getValue();

    double curVal = valPtr->getValue();
    double u      = rng->uniform01();
    double newVal = curVal + (delta*(u-0.5));

    /* reflect the new value */
    do {
        if (newVal < minVal)
            newVal = 2.0 * minVal - newVal;
        else if (newVal > maxVal)
            newVal = 2.0 * maxVal - newVal;
    } while (newVal < minVal || newVal > maxVal);

    valPtr->setValue(newVal);
	
    return 0.0;
}


