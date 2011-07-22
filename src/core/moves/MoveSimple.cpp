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
#include "RbException.h"
#include "StochasticNode.h"
#include "VectorString.h"

#include <set>


/** Constructor for parser use */
MoveSimple::MoveSimple(const MemberRules& memberRules)
    : Move(memberRules) {

    if ( !members.existsVariable("variable") )
        throw RbException( "A simple move must have a member called 'variable'" );
}


/** Accept the move: update statistics and call derived method */
void MoveSimple::acceptMove(void) {

    StochasticNode* nodePtr = static_cast<StochasticNode*>( members["variable"].getReference() );

    accept();
    numAccepted++;
    nodePtr->keep();
}


/** Get class vector describing type of object */
const VectorString& MoveSimple::getClass(void) const { 

    static VectorString rbClass = VectorString(MoveSimple_name) + Move::getClass();
	return rbClass;
}


/** Return member rules */
const MemberRules& MoveSimple::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
		{
        /* Inherit weight from Move */
        const MemberRules& inheritedRules = Move::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        rulesSet = true;
		}

    return memberRules;
}


/** Perform the move */
void MoveSimple::performMove(double& lnProbabilityRatio, double& lnHastingsRatio) {

    StochasticNode* nodePtr = static_cast<StochasticNode*>( members["variable"].getReference() );
//    DAGNode *var = members["variable"].getVariable(); 
//    StochasticNode* nodePtr = static_cast<StochasticNode*>( var );
    std::set<StochasticNode*> affectedNodes;

    lnHastingsRatio    = perform(affectedNodes);
    lnProbabilityRatio = nodePtr->getLnProbabilityRatio();

    for (std::set<StochasticNode*>::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); i++)
        lnProbabilityRatio += (*i)->getLnProbabilityRatio();

    numTried++;
}


/** Reject the move */
void MoveSimple::rejectMove(void) {

    StochasticNode* nodePtr = static_cast<StochasticNode*>( members["variable"].getReference() );

    reject();
    nodePtr->restore();
}

