//
//  AdmixtureSubtreePruneRegraft.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 7/7/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureSubtreePruneRegraft.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureSubtreePruneRegraft::AdmixtureSubtreePruneRegraft(StochasticNode<AdmixtureTree> *v, double d, double w) : SimpleMove( v, w), variable( v ), delta(d) {
    
}



/* Clone object */
AdmixtureSubtreePruneRegraft* AdmixtureSubtreePruneRegraft::clone( void ) const {
    
    return new AdmixtureSubtreePruneRegraft( *this );
}



const std::string& AdmixtureSubtreePruneRegraft::getMoveName( void ) const {
    static std::string name = "SubtreePruneRegraft";
    
    return name;
}

/** Perform the move */
double AdmixtureSubtreePruneRegraft::performSimpleMove( void ) {
    
    //std::cout << "\nDiv Node Subtree Prune Regraft\n";
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    
    // pick a random node which is not the root, not a tip, not an admixture node
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        storedNode = &tau.getNode(index);
    } while ( storedNode->isRoot() || storedNode->getNumberOfChildren() != 2);
    storedParent = &storedNode->getParent();
    
    int chIdx1 = (int)(rng->uniform01() * storedNode->getNumberOfChildren());
    int chIdx2 = (chIdx1 == 0 ? 1 : 0);
    storedChild = &storedNode->getChild(chIdx1);
    AdmixtureNode* movingChild = &storedNode->getChild(chIdx2);
    
    // pick a random destination branch
    double chAge = 1.0;
    size_t numTips = tau.getNumberOfTips();
    size_t numBranches = tau.getNumberOfNodes() - numTips;
    do {
        double u = rng->uniform01();
        size_t index = numTips + std::floor(numBranches * u);
        newParent = &tau.getNode(index);
        chAge = 1.0;
        newChild = &newParent->getChild(rng->uniform01() * newParent->getNumberOfChildren());
        chAge = newChild->getAge();
    } while (newParent == storedNode || newChild == storedNode || movingChild->getAge() >= newParent->getAge());
    
    // forbid outgroup regraft
    if (storedNode->isOutgroup() != newChild->isOutgroup())
    {
        //std::cout << "SPR failed, outgroup\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // memory and ages
    /*
    std::cout << "storedNode   " << storedNode << " " << storedNode->getAge() << "\n";
    std::cout << "storedChild1 " << storedChild << " " << storedChild->getAge() << "\n";
    std::cout << "storedChild2 " << movingChild << " " << movingChild->getAge() << "\n";
    std::cout << "storedParent " << storedParent << " " << storedParent->getAge() << "\n";
    std::cout << "newChild     " << newChild << " " << newChild->getAge() <<  "\n";
    std::cout << "newParent    " << newParent << " " << newParent->getAge() << "\n";
    */
    
    // prune storedNode
    storedParent->removeChild(storedNode,false);
    storedNode->removeChild(storedChild,false);
    storedParent->addChild(storedChild,false);
    storedChild->setParent(storedParent,false);

    // regraft storedNode
    newParent->removeChild(newChild,false);
    newParent->addChild(storedNode,false);
    storedNode->addChild(newChild,false);
    storedNode->setParent(newParent,false);
    newChild->setParent(storedNode,false);
    
    // store age
    storedNodeAge = storedNode->getAge();

    // propose age
    double minAge = 0.0;
    for (size_t i = 0; i < storedNode->getNumberOfChildren(); i++)
    {
        double childAge = storedNode->getChild(i).getAge();
        if (childAge > minAge)
            minAge = childAge;
        //std::cout << "chAge   " << childAge << " " << minAge << "\n";
    }
    double maxAge = storedNode->getParent().getAge();
    double ageRange = maxAge - minAge;
    double newAge = minAge + rng->uniform01() * ageRange;
    
    storedNode->setAge(newAge);
    
    // update Newick string for tree
    variable->getValue().getRoot().flagNewickRecomputation();
   
    // proposal done
    double lnProposalRatio = log(ageRange / (storedParent->getAge() - storedChild->getAge()));
    /*
    std::cout << "oldAge " << minAge << " " << storedNodeAge << " " << maxAge << "\n";
    std::cout << "newAge " << minAge << " " << newAge << " " << maxAge << "\n";
    std::cout << "age chk    " << newParent->getAge() << " " << storedNode->getAge() << " " << newChild->getAge() << "\n";
    std::cout << "lnProposal " << lnProposalRatio << "\n";

    std::cout << "...\n";
     */
    
    return lnProposalRatio;
  
}

double AdmixtureSubtreePruneRegraft::proposeAge(AdmixtureNode* p)
{
    //std::cout << "proposeAge()\n";
    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    double minAge = 0.0;
    for (size_t i = 0; i < p->getNumberOfChildren(); i++)
    {
        double childAge = p->getChild(i).getAge();
        if (childAge > minAge)
            minAge = childAge;
        //std::cout << "chAge   " << childAge << " " << minAge << "\n";
    }
    double maxAge = p->getParent().getAge();
    //double age = p->getAge();
    double ageRange = maxAge - minAge;
    double newAge = minAge + (rng->uniform01() * ageRange);
    
    p->setAge(newAge);
    
    return 0.0;
}

void AdmixtureSubtreePruneRegraft::rejectSimpleMove( void ) {
    
    //std::cout << "SPR reject\n";
    
    if (!failed)
    {
      
        // undo regraft storedNode
   //     std::cout << "RR\n";
        newParent->removeChild(storedNode,false);
        storedNode->removeChild(newChild,false);
        newParent->addChild(newChild,false);
        newChild->setParent(newParent,false);
        
        // undo prune storedNode
        storedParent->removeChild(storedChild,false);
        storedNode->addChild(storedChild,false);
        storedChild->setParent(storedNode,false);
        storedParent->addChild(storedNode,false);
        storedNode->setParent(storedParent,false);

        //std::cout << "ages " << storedNode->getAge() << " " << storedNodeAge << "\n";
        storedNode->setAge(storedNodeAge);
        
        // update Newick string for tree
        variable->getValue().getRoot().flagNewickRecomputation();
 
    }
    else
    {
        ;//std::cout << "failed!\n";
    }
 
#ifdef ASSERTIONS_TREE
    if ( fabs(storedChosenAge - storedChosenNode->getAge()) > 1E-8 || fabs(storedUnclesAge - storedUncle->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting SPR proposal: Node ages were not correctly restored!");
    }
#endif
    
}

void AdmixtureSubtreePruneRegraft::acceptSimpleMove( void ) {
    ;//std::cout << "SPR accept\n";
}


void AdmixtureSubtreePruneRegraft::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}
