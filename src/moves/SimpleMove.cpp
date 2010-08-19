/**
 * @file
 * This file contains the partial implementation of SimpleMove, which
 * is the abstract base class for moves updating single stochastic 
 * dag nodes in mcmc inference.
 *
 * @brief Partial implementation of SimpleMove
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

#include "RbNames.h"
#include "SimpleMove.h"
#include "StochasticNode.h"
#include "VectorString.h"

#include <set>


/** Constructor for parser use */
SimpleMove::SimpleMove(const MemberRules& memberRules)
    : Move(memberRules) {

    nodePtr = NULL;
}


/** Accept the move: update statistics and call derived method */
void SimpleMove::acceptMove(void) {

    accept();

    numAccepted++;
    nodePtr->keep();
}

/** Convert to object of another class. The caller manages the object. */
RbObject* SimpleMove::convertTo(const std::string& type) const {

    return NULL;
}

/** Get class vector describing type of object */
const VectorString& SimpleMove::getClass(void) const { 

    static VectorString rbClass = VectorString(SimpleMove_name) + Move::getClass();
	return rbClass;
}

/** Convert to object of another class. The caller manages the object. */
bool SimpleMove::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Perform the move */
void SimpleMove::performMove(double& lnPriorRatio, double& lnLikelihoodRatio, double& lnHastingsRatio) {

    std::set<StochasticNode*> affectedNodes;

    lnHastingsRatio   = perform(affectedNodes);
    lnPriorRatio      = nodePtr->getLnPriorRatio();

    lnLikelihoodRatio = 0.0;
    for (std::set<StochasticNode*>::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); i++)
        lnLikelihoodRatio += (*i)->getLnLikelihoodRatio();

    numTried++;
}


/** Reject the move */
void SimpleMove::rejectMove(void) {

    reject();

    nodePtr->restore();
}


