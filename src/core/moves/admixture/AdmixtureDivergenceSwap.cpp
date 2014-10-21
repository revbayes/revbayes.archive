//
//  AdmixtureDivergenceSwap.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 1/8/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "AdmixtureDivergenceSwap.h"
#include "DistributionBeta.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <list>
#include <cmath>

using namespace RevBayesCore;

AdmixtureDivergenceSwap::AdmixtureDivergenceSwap(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, std::vector< ContinuousStochasticNode*> br, DeterministicNode< RbVector<double> >* res, int ag, bool asa, double w) : MoveOld( v, w), variable( v ), rate(r), residuals(res), branchRates(br), changed(false), failed(false), failedAdd(false), allowSisterAdmixture(asa), activeGen(ag) {
    
    nodes.insert(rate);
    nodes.insert(residuals);
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
    //nodes.insert(dt);
    
}


/* Clone object */
AdmixtureDivergenceSwap* AdmixtureDivergenceSwap::clone( void ) const {
    
    return new AdmixtureDivergenceSwap( *this );
}

const std::string& AdmixtureDivergenceSwap::getMoveName( void ) const {
 
    static std::string name = "AdmixtureDivergenceSwap";
    return name;
}

void AdmixtureDivergenceSwap::findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p)
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
double AdmixtureDivergenceSwap::performSimpleMove( void ) {
    
    //  std::cout << "Admix Node Repl RW\n";
    failed = false;
    failedAdd = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    //AdmixtureNode* root = &tau.getRoot();
    //size_t numTaxa = tau.getNumberOfTips();
    
    std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
    size_t numAdmixtureEdges = admixtureParents.size();
    
    // if no admixtureParent exists, the proposal fails
    if (numAdmixtureEdges == 0)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // otherwise, proceed
    
    // proposal:
    // x  : (A,(B,C)) with (A)->(B)
    // x' : ((A,B),C) with (B)->(C)
    else
    {
        failed = false;

        // proposal densities
        //double fwdProposal = 1.0;
        //double bwdProposal = 1.0;
        
        // sample a random admixture parent node
        size_t index = std::floor(numAdmixtureEdges * rng->uniform01());
        
        // store admixture edge state
        storedAdmixtureParent = admixtureParents[index];
        storedAdmixtureChild = &storedAdmixtureParent->getAdmixtureChild();
        storedAdmixtureAge = storedAdmixtureChild->getAge();
        storedAdmixtureWeight = storedAdmixtureChild->getWeight();
        storedAdmixtureParentChild = &storedAdmixtureParent->getChild(0);
        storedAdmixtureChildChild = &storedAdmixtureChild->getChild(0);

        // find possible attachment points for divergence regraft
        storedRegraftChild = &storedAdmixtureChild->getChild(0);
        storedPruneChild = &storedAdmixtureParent->getChild(0);
        tau.removeAdmixtureEdge(storedAdmixtureParent, false);
        storedRegraftParent = &storedRegraftChild->getParent();
        storedPruneParent = &storedPruneChild->getParent();
        
        while (storedRegraftParent->getNumberOfChildren() != 2)
        {
            storedRegraftChild = storedRegraftParent;
            storedRegraftParent = &storedRegraftParent->getParent();
        }
        
        while (storedPruneParent->getNumberOfChildren() != 2)
        {
            storedPruneChild = storedPruneParent;
            storedPruneParent = &storedPruneParent->getParent();
        }

        // get regraft age bounds
        double maxRegraftAge = storedPruneParent->getAge();
        if (storedRegraftParent->getAge() < maxRegraftAge)
            maxRegraftAge = storedRegraftParent->getAge();
        
        double minRegraftAge = storedPruneChild->getAge();
        if (storedRegraftChild->getAge() < minRegraftAge)
            minRegraftAge = storedRegraftChild->getAge();
        
        // sample new divergence age
        //double newRegraftAge = minRegraftAge + (maxRegraftAge - minRegraftAge) * rng->uniform01();
        
        // regraft
        tau.subtreePruneRegraft(storedPruneParent, storedPruneChild, storedRegraftChild, false);
        
        // sample new admixture edge age
        storedPruneBrother = &storedPruneParent->getChild(0);
        if (storedPruneBrother == storedPruneChild)
            storedPruneBrother = &storedPruneParent->getChild(1);
        
        
        // add admixture edge
        
        /*
        // store admixture edge position
        storedAdmixtureParent = admixtureParents[index];
        storedAdmixtureChild = &storedAdmixtureParent->getAdmixtureChild();
        
        storedAdmixtureChildParent = &storedAdmixtureChild->getParent();
        storedAdmixtureChildChild = &storedAdmixtureChild->getChild(0);
        storedAdmixtureParentParent = &storedAdmixtureParent->getParent();
        storedAdmixtureParentChild = &storedAdmixtureParent->getChild(0);
        
        int oldChildBranchIdx = (int)storedAdmixtureChild->getTopologyChild(0).getIndex();
        int oldParentBranchIdx = (int)storedAdmixtureParent->getTopologyChild(0).getIndex();
        
        // remove admixture edge from graph
        storedAdmixtureChild->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
        
        storedAdmixtureParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
        
        // get age for admixture event
        storedAdmixtureAge = storedAdmixtureChild->getAge();
        storedAdmixtureWeight = storedAdmixtureChild->getWeight();
        
        // initialize NA__
        newAdmixtureChildChild = storedAdmixtureChildChild;
        newAdmixtureChildParent = storedAdmixtureChildParent;
        newAdmixtureParentChild = storedAdmixtureParentChild;
        newAdmixtureParentParent = storedAdmixtureParentParent;
        
        */
        
        tau.getRoot().flagNewickRecomputation();
    }

    return 0.0; // Better return something to keep compilers happy
}


void AdmixtureDivergenceSwap::rejectSimpleMove( void ) {
    
    
    if (failedAdd == true)
    {
        // NOTE: root used to protect from infinite recursions caused by AdmixtureNode::flagNewickComputation() from addChild,removeChild,setParent.
        //AdmixtureNode* root = &variable->getValue().getRoot();
        
        /*
         std::cout << "\n\tbefore reject no residuals, or maxAge<=minAge\n";
         variable->getValue().checkAllEdgesRecursively(root);
         
         std::cout << storedAdmixtureParent << "\n";
         std::cout << storedAdmixtureParentChild << "\t" << storedAdmixtureParentParent << "\n";
         std::cout << newAdmixtureParentChild << "\t" << newAdmixtureParentParent << "\n";
         std::cout << storedAdmixtureChild << "\n";
         std::cout << storedAdmixtureChildChild << "\t" << storedAdmixtureChildParent << "\n";
         std::cout << newAdmixtureChildChild << "\t" << newAdmixtureChildParent << "\n";
         */
        
        // revert admixture parent
        // storedAdmixtureParent->setParent(root);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent);
        storedAdmixtureParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentChild->setParent(storedAdmixtureParent);
        
        storedAdmixtureParent->setAge(storedAdmixtureAge);
        
        // revert admixture child
        //storedAdmixtureChild->setParent(root);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildChild->setParent(storedAdmixtureChild);
        
        storedAdmixtureChild->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setWeight(storedAdmixtureWeight);
        
        //std::cout << "\n\tafter reject\n";
        //variable->getValue().checkAllEdgesRecursively(root);
        
        // revert the age
        
    }
    else if (failed == false)
    {
        // NOTE: root used to protect from infinite recursions caused by AdmixtureNode::flagNewickComputation() from addChild,removeChild,setParent.
        AdmixtureNode* root = &variable->getValue().getRoot();
        
        /*
         std::cout << "\nreject\n";
         std::cout << "stored AP " << storedAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << storedAdmixtureParentChild << "\n";
         std::cout << "new    AP " << newAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << newAdmixtureParentChild << "\n";
         std::cout << "stored AC " << storedAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << storedAdmixtureChildChild << "\n";
         std::cout << "new    AC " << newAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << newAdmixtureChildChild << "\n";
         
         std::cout << "\n\tbefore reject regular\n";
         variable->getValue().checkAllEdgesRecursively(root);
         */
        
        // remove new admixture edge from graph
        storedAdmixtureChild->removeChild(newAdmixtureChildChild);
        newAdmixtureChildChild->setParent(newAdmixtureChildParent);
        newAdmixtureChildParent->removeChild(storedAdmixtureChild);
        newAdmixtureChildParent->addChild(newAdmixtureChildChild);
        
        storedAdmixtureParent->removeChild(newAdmixtureParentChild);
        newAdmixtureParentChild->setParent(newAdmixtureParentParent);
        newAdmixtureParentParent->removeChild(storedAdmixtureParent);
        newAdmixtureParentParent->addChild(newAdmixtureParentChild);
        
        // restore admixture edge values
        storedAdmixtureParent->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setWeight(storedAdmixtureWeight);
        
        // restore stored admixture edge
        storedAdmixtureParent->setParent(root);
        storedAdmixtureParentChild->setParent(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild); //
        storedAdmixtureParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent);
        
        storedAdmixtureChild->setParent(root);
        storedAdmixtureChildChild->setParent(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent);
        
        
        // update with outgroup settings
        storedAdmixtureChild->setOutgroup(storedAdmixtureChildChild->isOutgroup());
        storedAdmixtureParent->setOutgroup(storedAdmixtureParentChild->isOutgroup());
        
        
        // restore rates
        for (std::map<int,double>::iterator it = storedBranchRates.begin(); it != storedBranchRates.end(); it++)
        {
            branchRates[it->first]->setValue(new double(it->second));
        }
        
        //        std::cout << "replace reject\n";
    }
    
    
    
    else
        ;//std::cout << "replace failed\n";
}

void AdmixtureDivergenceSwap::acceptSimpleMove(void)
{
    //std::cout << "add_RW accept\n";
    //  std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
    //  std::cout << "\n";
}


void AdmixtureDivergenceSwap::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "AEA\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
    else if (oldN == rate)
    {
        //std::cout << "AEA\trate\n";
        rate = static_cast<StochasticNode<double>* >(newN);
    }
    else if (oldN == residuals)
    {
        residuals = static_cast<DeterministicNode<RbVector<double> >* >(newN);
    }
    
    for (size_t i = 0; i < branchRates.size(); i++)
    {
        if (oldN == branchRates[i])
        {
            branchRates[i] = static_cast<ContinuousStochasticNode*>(newN);
        }
    }
    
}


void AdmixtureDivergenceSwap::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureDivergenceSwap::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    //std::cout << "replace accept\n";
    
    acceptSimpleMove();
}

bool AdmixtureDivergenceSwap::isActive(unsigned long g) const {
    
    return g > activeGen;
}


double AdmixtureDivergenceSwap::performMove( double &probRatio ) {
    
    //std::cout << "\nAdmix Edge Replace\n";
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf || hr == RbConstants::Double::neginf ) {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch(); // if previously touched, this will overwrite lnProb??
    probRatio = variable->getLnProbabilityRatio();
    //probRatio = 0.0;
    
    for (std::map<int,double>::iterator it = storedBranchRates.begin(); it != storedBranchRates.end(); it++)
    {
        branchRates[it->first]->touch();
        probRatio += branchRates[it->first]->getLnProbabilityRatio();
        //std::cout << branchRates[it->first]->getLnProbabilityRatio() << "\n";
    }
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << probRatio << " " << theNode->getName() << "\t" << theNode->getLnProbability() << " " << theNode->getLnProbabilityRatio() << "\n";
        }
    }
    
    return hr;
}
