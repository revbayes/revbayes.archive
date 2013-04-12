//
//  AdmixtureFixedNodeheightPruneRegraft.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureFixedNodeheightPruneRegraft.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureFixedNodeheightPruneRegraft::AdmixtureFixedNodeheightPruneRegraft(StochasticNode<AdmixtureTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
AdmixtureFixedNodeheightPruneRegraft* AdmixtureFixedNodeheightPruneRegraft::clone( void ) const {
    
    return new AdmixtureFixedNodeheightPruneRegraft( *this );
}


void AdmixtureFixedNodeheightPruneRegraft::findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode *n) {
  
    // MJL shouldn't tips be allowed ... don't see why not, only need to check for tip when calling findNewBrothers again...
    
    if (&p != n) {
        
        // tips won't be called...
        size_t numChildren = n->getNumberOfChildren();
        AdmixtureNode* child;
        
        for (size_t i = 0; i < numChildren; i++)
        {
            child = &n->getChild( i );
            std::cout << "\t" << child->getAge() << "\n";
            if ( child->getAge() < p.getAge() ) {
                std::cout << "\t\tpush\t" << child << "\n";
                b.push_back( child );
            } else {
                std::cout << "\t\trecurse\t" << child << "\n";
                findNewBrothers(b, p, child);
            }
        }
    }
}


const std::string& AdmixtureFixedNodeheightPruneRegraft::getMoveName( void ) const {
    static std::string name = "FNPR";
    
    return name;
}


/** Perform the move */
double AdmixtureFixedNodeheightPruneRegraft::performSimpleMove( void ) {
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    
    std::cout << "\nFNPR\n";
    std::cout << "before FPNR proposal\n";
    tau.checkAllEdgesRecursively(&tau.getRoot());
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    AdmixtureNode* node;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getParent().isRoot() );
    std::cout << "picked\t" << node << "\n";
    
    AdmixtureNode* parent        = &node->getParent();
    AdmixtureNode& grandparent   = parent->getParent();
    AdmixtureNode& brother       = parent->getChild( 0 );
    
    if (!parent->checkParent())
        std::cout << "p->chP = F\n";
    if (!grandparent.checkChildren())
        std::cout << "gp->chC = F\n";
    
    // check if we got the correct child, only possible for divergence nodes
    if ( parent->getNumberOfChildren() > 1)
        if (&brother == node )
            brother = parent->getChild( 1 );
    

    // collect the possible reattachement points
    std::vector<AdmixtureNode*> new_brothers;
    findNewBrothers(new_brothers, *parent, &tau.getRoot());
    int index = int(rng->uniform01() * new_brothers.size());
    AdmixtureNode* newBro = new_brothers[index];
    std::cout << "get brothers\n";
    std::cout << parent->getAge() << "\t" << new_brothers.size() << "\n";
    for (size_t i = 0; i < new_brothers.size(); i++)
        std::cout << "\t" << new_brothers[i];
    std::cout << "\n";
    
    
    // now we store all necessary values
    storedBrother       = &brother;
    storedNewBrother    = newBro;
    
    std::cout << "prune\n";
    
    // prune
    std::cout << "grandparent.removeChild(parent)\n";
    grandparent.removeChild( parent )
    ;
    std::cout << "parent.removeChild(brother)\n";
    parent->removeChild( &brother );
    grandparent.addChild( &brother );
    brother.setParent( &grandparent );
    
    std::cout << "regraft\n";
    // regraft
    AdmixtureNode* newGrandParent = &newBro->getParent();
    std::cout << "nGP->ch\t";
    size_t nCh = newGrandParent->getNumberOfChildren();
    for (size_t i = 0; i < nCh; i++)
        std::cout << &newGrandParent->getChild(i) << "\t";
    std::cout << "\n";
    std::cout << (newGrandParent->isRoot() ? "root" : "notroot") << "\n";
    std::cout << parent << "\t" << &grandparent << "\t" << &brother << "\n";
    std::cout << newBro << "\n";
    std::cout << newGrandParent << "\t" << &newGrandParent->getParent() << "\t" << newGrandParent->getNumberOfChildren() << "\n";
    
    std::cout << "newGrandParent->removeChild(newBro)\n";
    newGrandParent->removeChild( newBro );
    
    std::cout << "nGP->rm(nB)\n";
    newGrandParent->addChild( parent );
    parent->setParent( newGrandParent );
    parent->addChild( newBro );
    newBro->setParent( parent );
    
    
    std::cout << "after FPNR proposal\n";
    tau.checkAllEdgesRecursively(&tau.getRoot());
    

    return 0.0; //RbConstants::Double::neginf;
}


void AdmixtureFixedNodeheightPruneRegraft::rejectSimpleMove( void ) {
    
    // undo the proposal
    TopologyNode& parent = storedNewBrother->getParent();
    TopologyNode& newGrandparent = parent.getParent();
    TopologyNode& grandparent = storedBrother->getParent();
    
    // prune
    newGrandparent.removeChild( &parent );
    parent.removeChild( storedNewBrother );
    newGrandparent.addChild( storedNewBrother );
    storedNewBrother->setParent( &newGrandparent );
    
    // regraft
    grandparent.removeChild( storedBrother );
    parent.addChild( storedBrother );
    storedBrother->setParent( &parent );
    grandparent.addChild( &parent );
    parent.setParent( &grandparent );
    
    std::cout << "after FPNR reject\n";
    AdmixtureTree& tau = variable->getValue();
    tau.checkAllEdgesRecursively(&tau.getRoot());
    
}


void AdmixtureFixedNodeheightPruneRegraft::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}

/*
 
 double AdmixtureFixedNodeheightPruneRegraft::performSimpleMove( void ) {
 
 // Get random number generator
 RandomNumberGenerator* rng     = GLOBAL_RNG;
 
 AdmixtureTree& tau = variable->getValue();
 
 // pick a random node which is not the root and neithor the direct descendant of the root
 AdmixtureNode* node;
 do {
 double u = rng->uniform01();
 size_t index = std::floor(tau.getNumberOfNodes() * u);
 node = &tau.getNode(index);
 } while ( node->isRoot() || node->getParent().isRoot() );
 
 AdmixtureNode* parent        = &node->getParent();
 AdmixtureNode& grandparent   = parent->getParent();
 AdmixtureNode& brother       = parent->getChild( 0 );
 
 // check if we got the correct child
 if ( parent->getNumberOfChildren() > 1 && &brother == node ) {
 brother = parent->getChild( 1 );
 }
 std::cout << "get brothers\n";
 std::cout << parent->getAge() << "\n";
 // collect the possible reattachement points
 std::vector<AdmixtureNode*> new_brothers;
 findNewBrothers(new_brothers, *parent, &tau.getRoot());
 int index = int(rng->uniform01() * new_brothers.size());
 AdmixtureNode* newBro = new_brothers[index];
 
 
 // now we store all necessary values
 storedBrother       = &brother;
 storedNewBrother    = newBro;
 
 std::cout << "prune\n";
 
 // prune
 std::cout << "grandparent.removeChild(parent)\n";
 grandparent.removeChild( parent );
 std::cout << "parent.removeChild(brother)\n";
 parent->removeChild( &brother );
 grandparent.addChild( &brother );
 brother.setParent( &grandparent );
 
 std::cout << "regraft\n";
 // regraft
 AdmixtureNode* newGrandParent = &newBro->getParent();
 std::cout << "newGrandParent->removeChild(newBro)\n";
 newGrandParent->removeChild( newBro );
 newGrandParent->addChild( parent );
 parent->setParent( newGrandParent );
 parent->addChild( newBro );
 newBro->setParent( parent );
 
 return 0.0;
 }
 
 
 void AdmixtureFixedNodeheightPruneRegraft::rejectSimpleMove( void ) {
 
 // undo the proposal
 TopologyNode& parent = storedNewBrother->getParent();
 TopologyNode& newGrandparent = parent.getParent();
 TopologyNode& grandparent = storedBrother->getParent();
 
 // prune
 newGrandparent.removeChild( &parent );
 parent.removeChild( storedNewBrother );
 newGrandparent.addChild( storedNewBrother );
 storedNewBrother->setParent( &newGrandparent );
 
 // regraft
 grandparent.removeChild( storedBrother );
 parent.addChild( storedBrother );
 storedBrother->setParent( &parent );
 grandparent.addChild( &parent );
 parent.setParent( &grandparent );
 
 
 }
 
 
 void AdmixtureFixedNodeheightPruneRegraft::swapNode(DagNode *oldN, DagNode *newN) {
 // call the parent method
 SimpleMove::swapNode(oldN, newN);
 
 variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
 }
*/
