//
//  AdmixtureEdgeDivergenceMerge.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/9/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeDivergenceMerge.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <list>
#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeDivergenceMerge::AdmixtureEdgeDivergenceMerge(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, DeterministicNode<std::vector<double> >* res, ConstantNode<int>* dt, bool asa, double w) : Move( v, w), variable( v ), admixtureRate(r), residuals(res), delayTimer(dt), changed(false), failed(false), allowSisterAdmixture(asa) {
    
    nodes.insert(admixtureRate);
    nodes.insert(residuals);
    nodes.insert(delayTimer);
    
}



/* Clone object */
AdmixtureEdgeDivergenceMerge* AdmixtureEdgeDivergenceMerge::clone( void ) const {
    
    return new AdmixtureEdgeDivergenceMerge( *this );
}



const std::string& AdmixtureEdgeDivergenceMerge::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeDivergenceMerge";
    
    return name;
}

void AdmixtureEdgeDivergenceMerge::findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p)
{
    if (p->getNumberOfChildren() == 0)
    {
        //std::cout << p << "\t" << p->getIndex() << "\n";
        s.insert(p);
    }
    else
    {
        for (size_t i = 0; i < p->getNumberOfChildren(); i++)
            findDescendantTips(s, &p->getChild(i));
    }
}


/** Perform the move */
double AdmixtureEdgeDivergenceMerge::performSimpleMove( void ) {
    
    std::cout << "\nAdmix Edge Div Merge\n";
    failed = false;
    storedResiduals = residuals->getValue();
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    AdmixtureNode* root = &tau.getRoot();
    
    std::vector<AdmixtureNode*> firstAdmixtureParentPerLineage = tau.getFirstAdmixtureParentPerLineage();
    
    // if no children exist, the proposal fails
    if (firstAdmixtureParentPerLineage.size() == 0)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // otherwise, proceed
    else
    {
        failed = false;
        
        // sample a random admixture parent node
        double u = rng->uniform01();
        size_t index = std::floor(firstAdmixtureParentPerLineage.size() * u);
        
        
        // 1. preserve state
        
        // store admixture edge position
        storedAdmixtureParent = firstAdmixtureParentPerLineage[index];
        storedAdmixtureChild = &storedAdmixtureParent->getAdmixtureChild();
        storedAdmixtureChildParent = &storedAdmixtureChild->getParent();
        storedAdmixtureChildChild = &storedAdmixtureChild->getChild(0);
        storedAdmixtureParentParent = &storedAdmixtureParent->getParent();
        storedAdmixtureParentChild = &storedAdmixtureParent->getChild(0);
        
        
        // abort if near root
        if (storedAdmixtureChildParent->isRoot() == true)
        {
            failed = true;
            return RbConstants::Double::neginf;
        }
        
        // abort for admixture events on sister lineages, since the move is effectively an NNI, but complicates the pointer updates
        if (storedAdmixtureChildParent == storedAdmixtureParentParent)
        {
            failed = true;
            return RbConstants::Double::neginf;
        }
        
        // disallow merge between outgroups
        if (storedAdmixtureParent->isOutgroup() != storedAdmixtureChild->isOutgroup())
        {
            failed = true;
            return RbConstants::Double::neginf;
        }
        
        // get admixture child divergence node position
        storedAdmixtureChildGrandparent = &storedAdmixtureChildParent->getParent();
        storedAdmixtureChildBrother = &storedAdmixtureChildParent->getChild(0);
        if (storedAdmixtureChildBrother == storedAdmixtureChild)
            storedAdmixtureChildBrother = &storedAdmixtureChildParent->getChild(1);
    
        // get age,weight for admixture event
        storedAdmixtureAge = storedAdmixtureChild->getAge();
        storedDivergenceAge = storedAdmixtureChildParent->getAge();
        storedAdmixtureWeight = storedAdmixtureChild->getWeight();
        
        //std::cout << "before\n";
        //tau.checkAllEdgesRecursively(&tau.getRoot());
        
        //std::cout << "\n";
        //std::cout << storedAdmixtureParent << "  " << storedAdmixtureParentParent << "  " << storedAdmixtureParentChild << "\n";
        //std::cout << storedAdmixtureChild << "  " << storedAdmixtureChildParent << "  " << storedAdmixtureChildChild << "\n";
        //std::cout << storedAdmixtureChildGrandparent << "  " << storedAdmixtureChildBrother << "\n\n";
        
        // 2. update state
        
        // dummy NULL ptr to block the flagNewickRecomputation recursion...
        AdmixtureNode* p = NULL;
        
        // remove admixture edge from graph
        storedAdmixtureChild->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
        
        storedAdmixtureParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);

       // std::cout << "admixture removed\n";
       // tau.checkAllEdgesRecursively(&tau.getRoot());
        
        // sever child clade
        storedAdmixtureChildParent->setParent(p);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildBrother);
        storedAdmixtureChildGrandparent->removeChild(storedAdmixtureChildParent);
        storedAdmixtureChildGrandparent->addChild(storedAdmixtureChildBrother);
        storedAdmixtureChildBrother->setParent(storedAdmixtureChildGrandparent);

        // transplant child clade to parent position
        storedAdmixtureChildParent->setAge(storedAdmixtureAge);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentParent->addChild(storedAdmixtureChildParent);
        storedAdmixtureChildParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->setParent(storedAdmixtureParentParent);
        
       // std::cout << "after\n";
       // tau.checkAllEdgesRecursively(&tau.getRoot());
        
        // remove admixture nodes
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        
        // MH ratio
        size_t numEvents = tau.getNumberOfAdmixtureChildren();
        double unitTreeLength = tau.getUnitTreeLength();
        double lnP = log( (numEvents+1) / (admixtureRate->getValue() * unitTreeLength));
        
        return lnP;
    }
}


void AdmixtureEdgeDivergenceMerge::rejectSimpleMove( void ) {
    
    
    if (!failed)
    {
     
        std::cout << "reject\n";
        
        // dummy NULL ptr
        AdmixtureNode* p = NULL;
        
        // remove child clade from parent position
        storedAdmixtureChildParent->setParent(p);
        storedAdmixtureParentParent->removeChild(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        
        // restore child clade to child position
        storedAdmixtureChildParent->setAge(storedDivergenceAge);
        storedAdmixtureChildGrandparent->removeChild(storedAdmixtureChildBrother);
        storedAdmixtureChildGrandparent->addChild(storedAdmixtureChildParent);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildBrother);
        storedAdmixtureChildBrother->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->setParent(storedAdmixtureChildGrandparent);
        
        // restore admixture edge to graph
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild);
        storedAdmixtureChildChild->setParent(storedAdmixtureChild);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent);
        
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent);
        storedAdmixtureParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParent);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent);
        
        
        AdmixtureTree& tau = variable->getValue();
    //    tau.checkAllEdgesRecursively(&tau.getRoot());

        tau.pushAdmixtureNode(storedAdmixtureParent);
        tau.pushAdmixtureNode(storedAdmixtureChild);
    }
    
    else
        std::cout << "failed\n";
}

void AdmixtureEdgeDivergenceMerge::acceptSimpleMove(void)
{
    //std::cout << "add_RW accept\n";
    if (!failed)
    {
        AdmixtureNode* p = NULL;
        storedAdmixtureParent->setParent(p);
        storedAdmixtureChild->setParent(p);
        delete storedAdmixtureParent;
        delete storedAdmixtureChild;
    }
    //  std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
    //  std::cout << "\n";
}


void AdmixtureEdgeDivergenceMerge::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "AEA\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
    else if (oldN == delayTimer)
    {
        delayTimer = static_cast<ConstantNode<int>* >(newN);
    }
    else if (oldN == residuals)
    {
        residuals = static_cast<DeterministicNode<std::vector<double> >* >(newN);
    }
    else if (oldN == admixtureRate)
    {
        admixtureRate = static_cast<StochasticNode<double>* >(newN);
    }
}


void AdmixtureEdgeDivergenceMerge::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    std::cout << "merge_RW reject\n";
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeDivergenceMerge::acceptMove( void ) {
    // nothing to do
    changed = false;

    std::cout << "merge_RW accept\n";
    
    acceptSimpleMove();
}

double AdmixtureEdgeDivergenceMerge::performMove( double &probRatio ) {
    
    
    if (delayTimer->getValue() > 0)
    {
        failed = true;
        //delayTimer->setValue(delayTimer->getValue()-1);
        //        delay--;
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
            std::cout << theNode->getName() << "\t" << probRatio << "\n";
        }
    }
    
    return hr;
}