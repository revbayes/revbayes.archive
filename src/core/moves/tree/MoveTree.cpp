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


#include "DeterministicNode.h"
#include "MoveTree.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "VectorString.h"

#include <set>


/** Constructor for parser use */
MoveTree::MoveTree(const MemberRules& memberRules)
    : Move(memberRules) {
}


/** Accept the move: update statistics and call derived method */
void MoveTree::acceptMove(void) {

    // Tell all moved nodes that we want to keep the current state
    for ( std::set<StochasticNode*>::iterator i=moved.begin(); i!=moved.end(); i++ ) {
        (*i)->keep();
    }

    // Increase number of accepted
    numAccepted++;

    // Clear workspace
    moved.clear();
    topChanges.clear();

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

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {
        
        memberRules.push_back( new ValueRule ( "topology",      TypeSpec(Topology_name) ) );
        memberRules.push_back( new ValueRule ( "treeVariables", TypeSpec(List_name)     ) );

        /* Inherit weight from Move, put it after topology and tree variables */
        const MemberRules& inheritedRules = MoveTree::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        rulesSet = true;
    }

    return memberRules;
}


/**
 * We provide a convenience function in the base class for retrieving a named
 * tree variable from the list of tree variables in the form of an STL vector
 * of stochastic nodes, where indices correspond to topology node indices in
 * the topology ordering the tree plate.
 */
std::vector<StochasticNode*> MoveTree::getTreeVariable( const std::string& name ) const {

//    DeterministicNode*   treeVarsNode = static_cast<DeterministicNode*>( members["treeVars"].getReference() );
//    List*         treeVars     = static_cast<List*>      ( treeVarsNode->getMemberObject() );
//
//    const ArgumentFrame& elems = treeVars->getElements();
//    std::vector<StochasticNode*> temp;
//    for ( size_t i=0; i<elems.size(); i++ ) {
//    
//        temp.push_back( static_cast<StochasticNode*>( elems[i].getReference() ) );
//    }
//
//    return temp;
    
    throw RbException("Not implemented method MoveTree::getTreeVariable");
}


/**
 * We provide a convenience function in the base class for retrieving the topology.
 */
const Topology* MoveTree::getTopology( void ) const {

//    return static_cast<const Topology*>( members["topology"].getValue() );
}


/** Here we call the derived class to perform the move but we also take care of
    several tasks that are common to all tree moves. Specifically, we calculate
    the ln probability ratio, we change the topology and propose the new topology
    value, and we reconnect the tree variables to reflect the new topology. */
double MoveTree::performMove(double& lnProbabilityRatio) {

    // Capture multiple try (a few minor changes needed to support this case but not added yet)
    if ( topChanges.size() != 0 )
        throw RbException( "Multiple try not supported yet" );

    // Get topology and tree variable info
//    StochasticNode* topNode         = static_cast<StochasticNode*>   ( members["topology"].getVariablePtr() );
    StochasticNode          *topNode = NULL;
//    DeterministicNode* treeVarsNode = static_cast<DeterministicNode*>( members["treeVars"].getReference() );
//    List*           treeVars        = static_cast<List*>             ( treeVarsNode->getMemberObject() );
    Topology*       top             = static_cast<Topology*>         ( topNode->getValue()->clone() );

    // Declare local variable needed to retrieve set of affected nodes
    std::set<StochasticNode*> affected;

    // Perform move and get info back on moved, affected and desired topology changes
    double lnHastingsRatio = perform( moved, affected, topChanges );

    // Make the desired topology changes and propose new topology
    top->changeTopology( topChanges );
    topNode->setValue( top, affected );

    // Reconnnect the tree variables according to new topology
//    reconnectTreeVariables( treeVars, topChanges );

    // Calculate probability ratio
    lnProbabilityRatio = topNode->getLnProbabilityRatio();
    for ( std::set<StochasticNode*>::iterator i=moved.begin(); i!=moved.end(); i++ )
        lnProbabilityRatio += (*i)->getLnProbabilityRatio();
    for (std::set<StochasticNode*>::iterator i=affected.begin(); i!=affected.end(); i++)
        lnProbabilityRatio += (*i)->getLnProbabilityRatio();

    // Increase number of attempts
    numTried++;
    
    return lnHastingsRatio;
}


/**
 * This function reconnects the tree variables according to the specified topology changes. 
 * It reuses the DAG functionality used by the parser to reconnect DAGs in the workspace when
 * the user replaces one variable with another.
 * 
 * @todo Make sure that swapParentNode works properly also for this case
 *
 * @note The child and parent need not be directly connected in the graph for the tree variable.
 *       Therefore, we find the immediate child of the parent to switch, which leads to the
 *       target child node. This is done using the isParentInDAG function, to test whether the
 *       immediate child is a parent of the target child
 */
void MoveTree::reconnectTreeVariables( List* treeVars, std::vector<TopologyChange>& topChanges ) {

//    for ( size_t i=0; i<treeVars->getElementsSize(); i++ ) {
//    
//        DeterministicNode* treeVarNode = static_cast<DeterministicNode*>( treeVars->getElement( i ) );
//        List*                  treeVar = static_cast<List*>      ( treeVarNode->getMemberObject() );
//
//        for ( std::vector<TopologyChange>::iterator j=topChanges.begin(); j!=topChanges.end(); j++ ) {
//        
//            StochasticNode* childNode = static_cast<StochasticNode*>( treeVar->getElement( (*j).node          ) );
//            StochasticNode* oldParent = static_cast<StochasticNode*>( treeVar->getElement( (*j).oldParentNode ) );
//            StochasticNode* newParent = static_cast<StochasticNode*>( treeVar->getElement( (*j).newParentNode ) );
//            
//            std::set<VariableNode*> immediateChildren = oldParent->getChildren();
//            for ( std::set<VariableNode*>::iterator k=immediateChildren.begin(); k!=immediateChildren.end(); k++ ) {            
//
//                std::list<DAGNode*> done;
//                done.clear();
//                if ( (*k) == childNode || childNode->isParentInDAG( (*k), done ) )
//                    (*k)->swapParentNode( oldParent, newParent );
//            }
//        }
//    }
}


/** Here we reconnect the tree variables according to the old topology, restore the topology
    and all moved tree variables, and finally clear the workspace and send the derived class
    a reject message. */
void MoveTree::rejectMove(void) {

//    // Get topology and tree variable info
//    StochasticNode* topNode      = static_cast<StochasticNode*>( members["topology"].getReference() );
//    MemberNode*     treeVarsNode = static_cast<MemberNode*>    ( members["treeVars"].getReference() );
//    List*           treeVars     = static_cast<List*>          ( treeVarsNode->getMemberObject() );
//
//    // Reconnnect the tree variables according to old topology
//    for ( std::vector<TopologyChange>::iterator i=topChanges.begin(); i!=topChanges.end(); i++ ) {
//        int temp = (*i).newParentNode;
//        (*i).newParentNode = (*i).oldParentNode;
//        (*i).oldParentNode = temp;
//    }
//    reconnectTreeVariables( treeVars, topChanges );
//
//    // Restore topology and all other moved nodes
//    topNode->restore();
//    for ( std::set<StochasticNode*>::iterator i=moved.begin(); i!=moved.end(); i++ )
//        (*i)->restore();
//
//    // Clear workspace
//    moved.clear();
//    topChanges.clear();
//
//    // Send derived class a reject message
//    reject();
}

