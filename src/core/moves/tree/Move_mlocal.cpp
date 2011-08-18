/**
 * @file
 * This file contains the implementation of Move_mlocal, which changes
 * a topology and associated branch lengths according to the LOCAL
 * mechanism.
 *
 * @reference Larget B, Simon DL. 1999. Markov chain Monte Carlo algorithms
 *            for the Bayesian analysis of phylogenetic trees. Molecular Biology
 *            and Evolution 16:750-759.
 *
 *
 * @brief Implementation of Move_mlocal
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

#include "Distribution.h"
#include "DistributionDirichlet.h"
#include "Move_mlocal.h"
#include "Natural.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbStatisticsHelper.h"
#include "RealPos.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser */
Move_mlocal::Move_mlocal( void ) : MoveTree( getMemberRules() ) {
}


/** Clone object */
Move_mlocal* Move_mlocal::clone( void ) const {

    return new Move_mlocal( *this );
}


/** Get class vector describing type of object */
const VectorString& Move_mlocal::getClass() const {

    static VectorString rbClass = VectorString( Move_mlocal_name ) + MoveTree::getClass();
    return rbClass;
}


/** Return member rules */
const MemberRules& Move_mlocal::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
		{
        /* Inherit topology, treeVariables and weight from MoveTree, put them at front */
        const MemberRules& inheritedRules = MoveTree::getMemberRules();
        memberRules.insert( memberRules.begin(), inheritedRules.begin(), inheritedRules.end() ); 

        memberRules.push_back( new ValueRule ( "branchlengths", RbString_name ) );   // Identifier of branch length tree variable
        memberRules.push_back( new ValueRule ( "lambda",        RealPos_name  ) );

        rulesSet = true;
		}

    return memberRules;
}


/**
 * The move operates on a subtree consisting of three descendant nodes, which are each
 * others closest relatives. They are labeled a, b and c respectively, with c being the
 * sister of a and b. The ancestor of a and b is u, and the ancestor of all three nodes
 * is v. The ancestor of v is vAnc.
 *
 *  The mechanism itself is rather complex. See the reference for details.
 *
 * @reference Larget B, Simon DL. 1999. Markov chain Monte Carlo algorithms
 *            for the Bayesian analysis of phylogenetic trees. Molecular Biology
 *            and Evolution 16:750-759.
 */
double Move_mlocal::perform( std::set<StochasticNode*>& movedNodes, std::set<StochasticNode*>& affectedNodes,
                            std::vector<TopologyChange>& topologyChanges) {

//    // Get random number generator    
//    RandomNumberGenerator*       rng      = GLOBAL_RNG;
//
//    // Get topology
//    const Topology*              topology = getTopology();
//
//    // Get branch length parameter
//    std::string                  brlenId  = static_cast<const RbString*>( getValue("branchlengths") )->getValue();
//    std::vector<StochasticNode*> brlens   = getTreeVariable( brlenId );
//
//    // Get tuning parameter
//    double                       lambda   = static_cast<const RealPos*>( getValue("lambda") )->getValue();
//
//    // Declare node index variables
//    int a, b, c, u, v;
//
//    // Find these nodes in the current topology
//    TopologyNode* theNode = topology->getInteriorNode( int( rng->uniform01() * topology->getNumberOfInteriorNodes() ) );
//    u = theNode->getIndex();
//    v = theNode->getParentIndex();
//    std::vector<int> children = theNode->getChildrenIndices();
//    if ( rng->uniform01() < 0.5 ) {
//        a = children[0];
//        b = children[1];
//    }
//    else {
//        a = children[1];
//        b = children[0];
//    }
//    TopologyNode* theParentNode = theNode->getParent();
//    if ( theParentNode->getChild( 0 ) == theNode )
//        c = theParentNode->getChild( 1 )->getIndex();
//    else
//        c = theParentNode->getChild( 0 )->getIndex();
//
//    // Find the corresponding branch lengths
//    double aLength = static_cast<const RealPos*>( brlens[a]->getValue() )->getValue();
//    double cLength = static_cast<const RealPos*>( brlens[c]->getValue() )->getValue();
//    double uLength = static_cast<const RealPos*>( brlens[u]->getValue() )->getValue();
//    double vLength = static_cast<const RealPos*>( brlens[v]->getValue() )->getValue();
//
//    // Randomly pick a factor to be used in modifying the branch lengths
//    double factor = exp( lambda * ( rng->uniform01() - 0.5 ) );
//
//    // We either change the lengths of branches on the path between a and c,
//    // or the path between a and vAnc
//    // Since we randomly oriented the tree with respect to a and b above,
//    // this is equivalent to the original formulation of the move
//    if ( rng->uniform01() < 0.5 ) {
//        
//        // Change path between a and c
//        double sumLength   = ( aLength + uLength + cLength ) * factor;
//
//        // Declare new branch lengths
//        double aLengthPrim, uLengthPrim, cLengthPrim;
//
//        // Select new positions for u and v, measured from a, on new total length
//        double uAttachPrim = rng->uniform01() * sumLength;
//        double vAttachPrim = rng->uniform01() * sumLength;
//        
//        if ( uAttachPrim > vAttachPrim ) {
//
//            // Change topology
//            TopologyChange topChange1, topChange2;
//            
//            // Swap a and c (step 1)
//            topChange1.node          = a;
//            topChange1.oldParentNode = u;
//            topChange1.newParentNode = v;
//            
//            // Swap a and c (step 2)
//            topChange2.node          = c;
//            topChange2.oldParentNode = v;
//            topChange2.newParentNode = u;
//
//            // Tell caller about topology modification
//            topologyChanges.push_back( topChange1 );
//            topologyChanges.push_back( topChange2 );
//
//            // Find new branch lengths
//            aLengthPrim = vAttachPrim;
//            uLengthPrim = uAttachPrim - vAttachPrim;
//            cLengthPrim = sumLength - uAttachPrim;
//        }
//        else {
//
//            // Find new branch lengths
//            aLengthPrim = uAttachPrim;
//            uLengthPrim = vAttachPrim - uAttachPrim;
//            cLengthPrim = sumLength - vAttachPrim;
//
//        }
//
//        // Propose new branch lengths
//        brlens[a]->setValue( new RealPos( aLengthPrim ), affectedNodes );
//        brlens[u]->setValue( new RealPos( uLengthPrim ), affectedNodes );
//        brlens[c]->setValue( new RealPos( cLengthPrim ), affectedNodes );
//
//        // Tell caller about the modified branch lengths
//        movedNodes.insert( brlens[a] );
//        movedNodes.insert( brlens[u] );
//        movedNodes.insert( brlens[c] );
//    }
//    else {
//
//        // Change path between a and vAnc
//        double sumLength   = ( aLength + uLength + vLength ) * factor;
//
//        // Declare new branch lengths
//        double aLengthPrim, uLengthPrim, vLengthPrim;
//
//        // Select new positions for u and v, measured from a, on new total length
//        double uAttachPrim = rng->uniform01() * sumLength;
//        double vAttachPrim = rng->uniform01() * sumLength;
//        
//        if ( uAttachPrim > vAttachPrim ) {
//
//            // Change topology
//            TopologyChange topChange1, topChange2;
//            
//            // Swap b and c (step 1)
//            topChange1.node          = b;
//            topChange1.oldParentNode = u;
//            topChange1.newParentNode = v;
//            
//            // Swap b and c (step 2)
//            topChange2.node          = c;
//            topChange2.oldParentNode = v;
//            topChange2.newParentNode = u;
//
//            // Tell caller about topology modification
//            topologyChanges.push_back( topChange1 );
//            topologyChanges.push_back( topChange2 );
//
//            // Find new branch lengths
//            aLengthPrim = vAttachPrim;
//            uLengthPrim = uAttachPrim - vAttachPrim;
//            vLengthPrim = sumLength - uAttachPrim;
//        }
//        else {
//
//            // Find new branch lengths
//            aLengthPrim = uAttachPrim;
//            uLengthPrim = vAttachPrim - uAttachPrim;
//            vLengthPrim = sumLength - vAttachPrim;
//        }
//
//        // Propose new branch lengths
//        brlens[a]->setValue( new RealPos( aLengthPrim ), affectedNodes );
//        brlens[u]->setValue( new RealPos( uLengthPrim ), affectedNodes );
//        brlens[v]->setValue( new RealPos( vLengthPrim ), affectedNodes );
//
//        // Tell caller about the modified branch lengths
//        movedNodes.insert( brlens[a] );
//        movedNodes.insert( brlens[u] );
//        movedNodes.insert( brlens[v] );
//    }
//
//    // Calculate the proposal ratio
//    double lnProposalRatio = 3.0 * log ( factor );
//    
//    return lnProposalRatio;
    
    return 0.0;
}

