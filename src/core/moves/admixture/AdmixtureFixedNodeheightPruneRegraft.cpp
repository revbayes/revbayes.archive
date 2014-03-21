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

AdmixtureFixedNodeheightPruneRegraft::AdmixtureFixedNodeheightPruneRegraft(StochasticNode<AdmixtureTree> *v, double w) : SimpleMove( v, w), variable( v ), failed(false) {
    
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
            //std::cout << "\t" << child->getAge() << "\n";
            if ( child->getAge() < p.getAge() ) {
              //  std::cout << "\t\tpush\t" << child << "\n";
                b.push_back( child );
            } else {
              //  std::cout << "\t\trecurse\t" << child << "\n";
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
    
    //std::cout << "\nDiv Node FNPR\n";
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
   
    // pick a random node which is not the root and neithor the direct descendant of the root, and is not an admixture node
    AdmixtureNode* parent;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        parent = &tau.getNode(index);
    } while ( parent->isRoot() || parent->getNumberOfChildren() != 2);
     
    //AdmixtureNode& node          = parent->getChild(0);
    AdmixtureNode& brother       = parent->getChild(int(2*rng->uniform01()));
    AdmixtureNode& grandparent   = parent->getParent();

    // collect the possible reattachement points
    std::vector<AdmixtureNode*> new_brothers;
    findNewBrothers(new_brothers, *parent, &tau.getRoot());
    int index = int(rng->uniform01() * new_brothers.size());
    AdmixtureNode* newBro = new_brothers[index];
   
    if (brother.isOutgroup() != newBro->isOutgroup())
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // now we store all necessary values
    storedBrother       = &brother;
    storedNewBrother    = newBro;
    
    // prune
    grandparent.removeChild( parent );
    parent->removeChild( &brother );
    grandparent.addChild( &brother );
    brother.setParent( &grandparent );
    
    // regraft
    AdmixtureNode* newGrandParent = &newBro->getParent();
    newGrandParent->removeChild( newBro );
    newGrandParent->addChild( parent );
    parent->setParent( newGrandParent );
    parent->addChild( newBro );
    newBro->setParent( parent );

    return 0.0; //RbConstants::Double::neginf;
}


void AdmixtureFixedNodeheightPruneRegraft::rejectSimpleMove( void ) {
    
    if (!failed)
    {
        // undo the proposal
        AdmixtureNode& parent = storedNewBrother->getParent();
        AdmixtureNode& newGrandparent = parent.getParent();
        AdmixtureNode& grandparent = storedBrother->getParent();
        
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
        
  //      std::cout << "after FPNR reject\n";
        //AdmixtureTree& tau = variable->getValue();
        //tau.checkAllEdgesRecursively(&tau.getRoot());
    }
}

void AdmixtureFixedNodeheightPruneRegraft::acceptSimpleMove(void)
{
 //   std::cout << "accept FNPR\n";
    
}


void AdmixtureFixedNodeheightPruneRegraft::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}
