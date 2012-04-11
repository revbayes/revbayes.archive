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

#include <set>


/** Constructor */
MoveSimple::MoveSimple(const MemberRules& memberRules) : Move(memberRules) {

}


/**
 * Accept the move.
 * We need to tell that the node!
 */
void MoveSimple::acceptMove(void) {
//    std::cerr << "Accept move on:\t" << node->getName() << std::endl;
    
    // delegate to derived classes
    accept();
    
    // increment the acceptance counter
    size_t tmp_accepted = numAccepted.getValue();
    numAccepted.setValue( tmp_accepted++ );
    
    // tell my node to keep itself
    // this will automatically call keep for the affected nodes
    node->keep();
}


/* Add a DAG node */
void MoveSimple::addDagNode(StochasticNode *d) {
    
    // test if we already have a node
    if ( nodes.size() > 0 ) {
        if ( nodes[0] != d ) {
            throw RbException("A simple move received a second node although simple moves only work on a single node!");
        }
    }
    else {
        nodes.push_back( d );
    
        // set the short cut access
        node = d;
    }
}



/** Get class name of object */
const std::string& MoveSimple::getClassName(void) { 
    
    static std::string rbClassName = "Simple move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& MoveSimple::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}


/**
 * Get the DAG nodes.
 */
std::vector<StochasticNode*>& MoveSimple::getDagNodes(void) {
    
    return nodes;
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


/** 
 *  Perform the move. 
 *  Currently we just delegate to the derived class, but in the future we might want to do some general stuff here.
 *  We increment our counter for the tries.
 */
double MoveSimple::performMove( double& lnProbabilityRatio) {
    
//    std::cerr << "performing move on node:\t" << node->getName() << std::endl;
    // call the derived class' perform operation
    double lnHastingsRatio  = perform();
    
    lnProbabilityRatio = node->getLnProbabilityRatio();
    
    std::set<StochasticNode* > affectedNodes;
    node->getAffectedNodes(affectedNodes);
//    std::cerr << "Number of affected nodes:\t\t" << affectedNodes.size() << std::endl;
    for (std::set<StochasticNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
        StochasticNode* theNode = *i;
//        std::cerr << "computing probability of affected node:\t" << theNode->getName() << std::endl;
        lnProbabilityRatio += theNode->getLnProbabilityRatio();
    }
    
    size_t tmp_tried = numTried.getValue();
    numTried.setValue( tmp_tried++ );
    
    // Return acceptance ratio
    return lnHastingsRatio;
}


/**
 * Reject the move.
 * We need to tell that the node!
 */
void MoveSimple::rejectMove(void) {
//    std::cerr << "Reject move on:\t" << node->getName() << std::endl;
    
    // delegate to derived classes
    reject();
    
    // tell my node to keep itself
    // this will automatically call keep for the affected nodes
    node->restore();
}


void MoveSimple::replaceDagNodes(std::vector<StochasticNode*> &n) {

    // release all nodes
    nodes.clear();
	    
    // add all nodes
    if ( n.size() != 1) {
        throw RbException("A simple move expects exactly one node but got a different number.");
    }
        
    StochasticNode* theNode = n[0];
    if (theNode != NULL) {
        nodes.push_back(theNode);
//        node->setDagNode( theNode );
        node = theNode;
    }
	    
}


/** We catch here the setting of the member variables to store our parameters. */
void MoveSimple::setMemberVariable(std::string const &name, const Variable* var) {
    
    Move::setMemberVariable(name, var);

}

