/**
 * @file
 * This file contains the partial implementation of MoveTree, which is
 * the abstract base class for moves updating trees during mcmc inference.
 *
 * @brief Partial implementation of MoveTree
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


#include "ConstantNode.h"
#include "MoveTree.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "TreePlate.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "VectorString.h"

#include <set>


/** Constructor for parser use */
MoveTree::MoveTree( const MemberRules& memberRules) : Move(memberRules) {

}


/** Accept the move: update statistics and call derived method */
void MoveTree::acceptMove(void) {

    // Increase number of accepted
    numAccepted++;

    // Send derived move class an accept message
    accept();
}


/** Get class vector describing type of object */
const VectorString& MoveTree::getClass(void) const { 

    static VectorString rbClass = VectorString( MoveTree_name ) + Move::getClass();
	return rbClass;
}


/** Return member rules */
const MemberRules& MoveTree::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if (!rulesSet) {
        
        memberRules.push_back( new ValueRule ( "tree", TypeSpec(TreePlate_name) ) );

        /* Inherit weight from Move, put it after topology and tree variables */
        const MemberRules& inheritedRules = MoveTree::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        rulesSet = true;
    }

    return memberRules;
}


/**
 * We provide a convenience function in the base class for retrieving the topology.
 */
const Topology& MoveTree::getTopology( void ) const {

    const TreePlate& tree = static_cast<const TreePlate&>( (*members)["tree"].getValue() );
    
    return tree.getTopology();
}


/** Here we call the derived class to perform the move but we also take care of
    several tasks that are common to all tree moves. Specifically, we calculate
    the ln probability ratio, we call the derived class to change the topology and propose the new topology
    value, and we reconnect the tree variables to reflect the new topology. */
double MoveTree::performMove(double& lnProbabilityRatio) {

    
    // Perform move and get info back on moved, affected and desired topology changes
//    double lnHastingsRatio = perform( moved, affected, topChanges );

    // Make the desired topology changes and propose new topology
//    top->changeTopology( topChanges );
//    topNode->setValue( top, affected );

    // Reconnnect the tree variables according to new topology
//    reconnectTreeVariables( treeVars, topChanges );

    // Calculate probability ratio
//    lnProbabilityRatio = topNode->getLnProbabilityRatio();
//    for ( std::set<StochasticNode*>::iterator i=moved.begin(); i!=moved.end(); i++ )
//        lnProbabilityRatio += (*i)->getLnProbabilityRatio();
//    for (std::set<StochasticNode*>::iterator i=affected.begin(); i!=affected.end(); i++)
//        lnProbabilityRatio += (*i)->getLnProbabilityRatio();

    // Increase number of attempts
    numTried++;
    
//    return lnHastingsRatio;
    
    return  0.0;
}


/** Here we reconnect the tree variables according to the old topology, restore the topology
    and all moved tree variables, and finally clear the workspace and send the derived class
    a reject message. */
void MoveTree::rejectMove(void) {

    // Get topology and tree variable info
    RbDagNodePtr topNode( static_cast<StochasticNode*>( (*members)["topology"].getDagNode() ) );


    // Send derived class a reject message
    reject();
}

