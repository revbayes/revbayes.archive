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


/**
 * Accept the move.
 * We need to tell that the node!
 */
void MoveSimple::acceptMove(void) {
    
    // delegate to derived classes
    accept();
    
    // increment the acceptance counter
    size_t tmp_accepted = numAccepted.getValue();
    numAccepted.setValue( tmp_accepted++ );
    
    // tell my node to keep itself
    // this will automatically call keep for the affected nodes
    node->getDagNode()->keep();
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
 * We reset here the vector of nodes because the DAG node variable might have changed its pointer
 * and now points to another DAG node.
 */
std::vector<StochasticNode*>& MoveSimple::getDagNodes(void) {
    // we empty the vector first
    nodes.clear();
    
    // and then insert the only node
    nodes.push_back( static_cast<StochasticNode*>(node->getDagNode() ) );
    
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
    
    // call the derived class' perform operation
    double lnHastingsRatio  = perform();
    
    static_cast<StochasticNode*>( node->getDagNode() )->getLnProbabilityRatio();
    
    std::set<StochasticNode* > affectedNodes;
    node->getDagNode()->getAffectedNodes(affectedNodes);
    for (std::set<StochasticNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); i++) {
        StochasticNode* theNode = *i;
        lnProbabilityRatio += theNode->getLnProbabilityRatio();
    }
    
    size_t tmp_tried = numTried.getValue();
    numTried.setValue( tmp_tried++ );
    
    // Return acceptance ratio
    return lnProbabilityRatio + lnHastingsRatio;
}


/**
 * Reject the move.
 * We need to tell that the node!
 */
void MoveSimple::rejectMove(void) {
    
    // delegate to derived classes
    reject();
    
    // tell my node to keep itself
    // this will automatically call keep for the affected nodes
    node->getDagNode()->restore();
}


void MoveSimple::replaceDagNodes(std::vector<StochasticNode*> &n) {

    // release all nodes
    nodes.clear();
	    
    // add all nodes
    if ( n.size() == 1) {
        throw RbException("A simple move expects exactly one node but got a different number.");
    }
        
    StochasticNode* theNode = n[0];
    if (theNode != NULL) {
        nodes.push_back(theNode);
        node->setDagNode( theNode );
    }
	    
}


/** We catch here the setting of the member variables to store our parameters. */
void MoveSimple::setMemberVariable(std::string const &name, Variable* var) {
    
    // test whether we want to set the variable 
    if ( name == "variable" ) {
        node = var;
    }
    else {
        Move::setMemberVariable(name, var);
    }
}

