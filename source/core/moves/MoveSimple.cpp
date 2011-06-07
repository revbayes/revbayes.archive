/**
 * @file
 * This file contains the partial implementation of MoveSimple, which
 * is the abstract base class for moves updating single stochastic 
 * dag nodes in mcmc inference.
 *
 * @brief Partial implementation of MoveSimple
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "RbNames.h"
#include "MoveSimple.h"
#include "StochasticNode.h"
#include "VectorString.h"

#include <set>


/** Constructor for parser use */
MoveSimple::MoveSimple(const MemberRules& memberRules)
    : Move(memberRules) {

    nodePtr = NULL;
}


/** Accept the move: update statistics and call derived method */
void MoveSimple::acceptMove(void) {

    accept();

    numAccepted++;
    nodePtr->keep();
}


/** Get class vector describing type of object */
const VectorString& MoveSimple::getClass(void) const { 

    static VectorString rbClass = VectorString(MoveSimple_name) + Move::getClass();
	return rbClass;
}


/** Perform the move */
void MoveSimple::performMove(double& lnProbabilityRatio, double& lnHastingsRatio) {

    std::set<StochasticNode*> affectedNodes;

    lnHastingsRatio   = perform(affectedNodes);

    lnProbabilityRatio = nodePtr->getLnProbabilityRatio();

    for (std::set<StochasticNode*>::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); i++)
        lnProbabilityRatio += (*i)->getLnProbabilityRatio();

    numTried++;
}


/** Reject the move */
void MoveSimple::rejectMove(void) {

    reject();

    nodePtr->restore();
}


