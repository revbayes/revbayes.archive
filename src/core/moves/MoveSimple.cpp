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


/** Constructor */
MoveSimple::MoveSimple(const MemberRules& memberRules) : Move(memberRules) {

//    if ( !members.existsVariable("variable") )
//        throw RbException( "A simple move must have a member called 'variable'" );
}


/** Copy constructor */
MoveSimple::MoveSimple(const MoveSimple &ms) : Move(ms) {
    
}


/** Accept the move: update statistics and call derived method */
void MoveSimple::acceptMove(void) {

//    StochasticNode* nodePtr = static_cast<StochasticNode*>( members["variable"].getVariablePtr() );
    
//    std::cout << "Accept move\n";
    accept();
    size_t tmp_accepted = numAccepted.getValue();
    numAccepted.setValue( tmp_accepted++ );
    for (std::vector<StochasticNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++) {
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

    static MemberRules memberRules = MemberRules();
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

    StochasticNode* nodePtr = nodes[0];
    
//    std::cout << "Perform simple move (" << getClass()[0] << ") on node \"" << nodePtr->getName() << "\"\n";

    double lnHastingsRatio    = perform();
    
    // touch the current node and the affected ones
//    nodePtr->touch();
    
    lnProbabilityRatio = nodePtr->getLnProbabilityRatio();
    
    std::set<StochasticNode* > affectedNodes;
    nodePtr->getAffectedNodes(affectedNodes);
    for (std::set<StochasticNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); i++) {
        StochasticNode* theNode = *i;
        lnProbabilityRatio += theNode->getLnProbabilityRatio();
    }

    size_t tmp_tried = numTried.getValue();
    numTried.setValue( tmp_tried++ );
    
    return lnHastingsRatio;
}


/** Reject the move */
void MoveSimple::rejectMove(void) {

    reject();
    for (size_t i=0; i<nodes.size(); i++) {
        nodes[i]->restore();
    }
}

