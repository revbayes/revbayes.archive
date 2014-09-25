//
//  AdmixtureEdgeRemove.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeRemove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeRemove::AdmixtureEdgeRemove(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, ConstantNode<int>* dt, double w) : MoveOld( v, w), variable( v ), rate(r), delayTimer(dt), changed(false), failed(false) {
    
    nodes.insert(rate);
    nodes.insert(dt);
}



/* Clone object */
AdmixtureEdgeRemove* AdmixtureEdgeRemove::clone( void ) const {
    
    return new AdmixtureEdgeRemove( *this );
}



const std::string& AdmixtureEdgeRemove::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeRemove";
    
    return name;
}


/** Perform the move */
double AdmixtureEdgeRemove::performSimpleMove( void ) {
    
    //std::cout << "rem\n";
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
    
    //std::cout << "\nAER\n";
    //std::cout << "before AER proposal\n";
    //tau.checkAllEdgesRecursively(&tau.getRoot());
    
    // if no admixtureParent exists, the proposal fails
    if (admixtureParents.size() == 0)
    {
        //std::cout << "AER nothing to remove\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // otherwise, proceed
    else
    {
        failed = false;
        
        // sample a random admixture parent node
        double u = rng->uniform01();
        size_t index = std::floor(admixtureParents.size() * u);
        AdmixtureNode* admixtureParent = admixtureParents[index];
        AdmixtureNode* admixtureChild = &admixtureParent->getAdmixtureChild();
        //double w = admixtureChild->getWeight();

        // get all nodes adjacent to proposal
        storedAdmixtureChild = admixtureChild;
        storedAdmixtureChildChild = &admixtureChild->getChild(0);
        storedAdmixtureChildParent = &admixtureChild->getParent();
        
        storedAdmixtureParent = admixtureParent;
        storedAdmixtureParentChild = &admixtureParent->getChild(0);
        storedAdmixtureParentParent = &admixtureParent->getParent();

        // update edges to remove admixtureChild in graph
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(admixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
    
        // update edges to remove admixtureParent in graph
        storedAdmixtureParentParent->removeChild(admixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        
        // remove nodes from admixtureTree vector
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        
        //std::cout << "after AER proposal\n";
        //tau.checkAllEdgesRecursively(&tau.getRoot());
        
        // prior * proposal ratio
        size_t numEvents = tau.getNumberOfAdmixtureChildren();
        //if (numEvents == 0)
        //    numEvents = 1;
        double unitTreeLength = tau.getUnitTreeLength();
        double lnP = log( (numEvents+1) / (rate->getValue() * unitTreeLength));
        //double lnJ = -2 * log(1 - w); // inverse of admixture edge add jacobian
        
        // quick fix -- should inherit from Move instead of SimpleMove
       // lnP -= variable->getLnProbabilityRatio();
        
        //std::cout << "remove\t" << lnP << "\t" << rate->getValue() << "\t" << unitTreeLength << "\t" << numEvents << "\n";
        return lnP;// + lnJ;
    }
}


void AdmixtureEdgeRemove::rejectSimpleMove( void ) {
    
    // we undo the proposal only if it didn't fail
    if ( !failed ) {
        
        
        // update edges to restore admixtureChild in graph
        storedAdmixtureChildChild->setParent(storedAdmixtureChild);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild);
        
        // update edges to restore admixtureParent in graph
        storedAdmixtureParentChild->setParent(storedAdmixtureParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent);
        
        // restore nodes to admixtureTree vector
        AdmixtureTree& tau = variable->getValue();
        
        
        tau.pushAdmixtureNode(storedAdmixtureParent);
        tau.pushAdmixtureNode(storedAdmixtureChild);
        
       // std::cout << "remove fail";
       // std::cout << tau.getNumberOfAdmixtureChildren() << "\t";
       // std::cout << "\n";
        
        //std::cout << "after AER reject\n";
        //tau.checkAllEdgesRecursively(&tau.getRoot());

 
    }
}

void AdmixtureEdgeRemove::acceptSimpleMove(void) {
    
    
    if (!failed)
    {
       // std::cout << "remove success";
       // std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
       // std::cout << "\n";
        ; // do nothing
    }
}


void AdmixtureEdgeRemove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "AER\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
    else if (oldN == rate)
    {
        //std::cout << "AER\trate\n";
        rate = static_cast<StochasticNode<double>* >(newN);
    }
    else if (oldN == delayTimer)
    {
        delayTimer = static_cast<ConstantNode<int>* >(newN);
    }
}


void AdmixtureEdgeRemove::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeRemove::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

double AdmixtureEdgeRemove::performMove( double &probRatio ) {
    
    if (delayTimer->getValue() > 0)
    {
        failed = true;
        //delayTimer->setValue(delayTimer->getValue()-1);
        return RbConstants::Double::neginf;
    }
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf ) {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch();
    
    probRatio = 0.0;
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << theNode->getName() << "\t" << probRatio << "\n";
        }
    }
    
    return hr;
}
