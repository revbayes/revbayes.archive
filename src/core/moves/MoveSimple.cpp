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

#include "RbUtil.h"
#include "MoveSimple.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "VectorString.h"

#include <set>


/** Constructor for parser use */
MoveSimple::MoveSimple(const MemberRules& memberRules)
    : Move(memberRules) {

//    if ( !members.existsVariable("variable") )
//        throw RbException( "A simple move must have a member called 'variable'" );
}


/** Copy constructor */
MoveSimple::MoveSimple(const MoveSimple &ms) : Move(ms) {
    
}


/** Accept the move: update statistics and call derived method */
void MoveSimple::acceptMove(void) {

//    StochasticNode* nodePtr = static_cast<StochasticNode*>( members["variable"].getVariablePtr() );

    accept();
    numAccepted++;
    for (std::vector<VariableNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++) {
        (*it)->keep();
    }
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
double MoveSimple::performMove(double& lnProbabilityRatio) {

    StochasticNode* nodePtr = (StochasticNode*) nodes[0];
    std::set<StochasticNode*> affectedNodes;

    double lnHastingsRatio    = perform(affectedNodes);
    lnProbabilityRatio = nodePtr->getLnProbabilityRatio();

    for (std::set<StochasticNode*>::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); i++)
        lnProbabilityRatio += (*i)->getLnProbabilityRatio();

    numTried++;
    
    return lnHastingsRatio;
}


/** Reject the move */
void MoveSimple::rejectMove(void) {

    reject();
    for (size_t i=0; i<nodes.size(); i++) {
        ((StochasticNode*)nodes[i])->restore();
    }
}

