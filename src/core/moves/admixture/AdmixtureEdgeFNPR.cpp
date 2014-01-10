//
//  AdmixtureEdgeFNPR.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeFNPR.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>
#include <list>

using namespace RevBayesCore;

AdmixtureEdgeFNPR::AdmixtureEdgeFNPR(StochasticNode<AdmixtureTree> *v, std::vector<ContinuousStochasticNode*> br, int ag, bool asa, double l, double w) : Move( v, w), variable( v ), branchRates(br), lambda(l), changed(false), failed(false), allowSisterAdmixture(asa), activeGen(ag) {
    
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
}

/* Clone object */
AdmixtureEdgeFNPR* AdmixtureEdgeFNPR::clone( void ) const {
    
    return new AdmixtureEdgeFNPR( *this );
}

const std::string& AdmixtureEdgeFNPR::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeFNPR";
    
    return name;
}

void AdmixtureEdgeFNPR::findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode *n) {
    
    if (&p != n) {
        
        size_t numChildren = n->getNumberOfChildren();
        AdmixtureNode* child;
        
        for (size_t i = 0; i < numChildren; i++)
        {
            child = &n->getChild( i );
            if ( child->getAge() < p.getAge() ) {
                b.push_back( child );
            } else {
                findNewBrothers(b, p, child);
            }
        }
    }
}

/** Perform the move */
double AdmixtureEdgeFNPR::performSimpleMove( void ) {
    
    //std::cout << "\nAdmix Edge FNPR\n";
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
    
    // if no admixtureParent exists, the proposal fails
    if (admixtureParents.size() == 0)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // otherwise, proceed
    else
    {
        // proposal densities
        double fwdProposal = 1.0;
        double bwdProposal = 1.0;
        
        failed = false;
        
        // sample a random admixture parent node
        double u = rng->uniform01();
        size_t index = std::floor(admixtureParents.size() * u);
        
        // store admixture edge position
        storedAdmixtureParent = admixtureParents[index];
        storedAdmixtureChild = &storedAdmixtureParent->getAdmixtureChild();
        storedAdmixtureChildParent = &storedAdmixtureChild->getParent();
        storedAdmixtureChildChild = &storedAdmixtureChild->getChild(0);
        storedAdmixtureParentParent = &storedAdmixtureParent->getParent();
        storedAdmixtureParentChild = &storedAdmixtureParent->getChild(0);
        
        // get old branch idx
        int oldChildBranchIdx = (int)storedAdmixtureChild->getTopologyChild(0).getIndex();
        int oldParentBranchIdx = (int)storedAdmixtureParent->getTopologyChild(0).getIndex();
        
        // find new attach point
        std::vector<AdmixtureNode*> brothers;
        AdmixtureNode* nd_a =  &storedAdmixtureParent->getChild(0);
        AdmixtureNode* nd_b = &storedAdmixtureChild->getChild(0);
        //bool swapParent = false;
        // get new parent attach pt
        if (GLOBAL_RNG->uniform01() < 0.5)
        {
            findNewBrothers(brothers, *storedAdmixtureParent, &tau.getRoot());
            nd_a = brothers[int(rng->uniform01() * brothers.size())];
        }
        // get new child attach pt
        else
        {
            findNewBrothers(brothers, *storedAdmixtureChild, &tau.getRoot());
            nd_b = brothers[int(rng->uniform01() * brothers.size())];
        }
        
        // remove admixture edge from graph
        
        tau.removeAdmixtureEdge(storedAdmixtureParent);
        
//        storedAdmixtureChild->removeChild(storedAdmixtureChildChild, false);
//        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent, false);
//        storedAdmixtureChildParent->removeChild(storedAdmixtureChild, false);
//        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild, false);
//        
//        storedAdmixtureParent->removeChild(storedAdmixtureParentChild, false);
//        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent, false);
//        storedAdmixtureParentParent->removeChild(storedAdmixtureParent, false);
//        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild, false);
        
        
        // get age for admixture event
        storedAge = storedAdmixtureChild->getAge();
        
        AdmixtureNode* root = &tau.getRoot();
        
//        // store adjacent nodes to new parent node
//        newAdmixtureParentChild = nd_a;
//        newAdmixtureParentParent = &nd_a->getParent();
//        
//        // insert admixtureParent into graph
//        //storedAdmixtureParent->setAge(newAge);
//        storedAdmixtureParent->setParent(root, false);
//        newAdmixtureParentChild->setParent(storedAdmixtureParent, false);
//        newAdmixtureParentParent->addChild(storedAdmixtureParent, false);
//        newAdmixtureParentParent->removeChild(newAdmixtureParentChild, false);
//        storedAdmixtureParent->addChild(newAdmixtureParentChild, false);
//        storedAdmixtureParent->setParent(newAdmixtureParentParent, false);
//        
//        // store adjacent nodes to new child node
//        newAdmixtureChildChild = nd_b;
//        newAdmixtureChildParent = &nd_b->getParent();
//        
//        // insert admixtureChild into graph
//        //storedAdmixtureChild->setAge(newAge);
//        storedAdmixtureChild->setParent(root, false);
//        newAdmixtureChildChild->setParent(storedAdmixtureChild, false);
//        newAdmixtureChildParent->addChild(storedAdmixtureChild, false);
//        newAdmixtureChildParent->removeChild(newAdmixtureChildChild, false);
//        storedAdmixtureChild->addChild(newAdmixtureChildChild, false);
//        storedAdmixtureChild->setParent(newAdmixtureChildParent, true);
        
        // get weight for admixture event
        storedWeight = storedAdmixtureChild->getWeight();
        
        // forwards proposal
        double admixtureMaxScaler = 0.5;
        double unitWeight = storedWeight / admixtureMaxScaler;
        double a2 = lambda * unitWeight + 1.0;
        double b2 = lambda * (1.0 - unitWeight) + 1.0;
        
        
        double newUnitWeight = RbStatistics::Beta::rv(a2, b2, *rng);
        double fwdWeightLnProb = RbStatistics::Beta::lnPdf(a2, b2, newUnitWeight);
        double newWeight = newUnitWeight * admixtureMaxScaler;
        double new_a2 = lambda * newUnitWeight + 1.0;
        double new_b2 = lambda * (1.0 - newUnitWeight) + 1.0;
        double bwdWeightLnProb = RbStatistics::Beta::lnPdf(new_a2, new_b2, unitWeight);
        //std::cout << bwdWeightLnProb << "\n";
        
        storedAdmixtureChild->setWeight(newWeight);
        
        // update branch rates
        
        double lnBwdPropRates = 0.0;
        
        storedBranchRates.clear();
        double delta = 1.0;
        // ... have old branch idx already
        int newChildBranchIdx = (int)storedAdmixtureChild->getTopologyChild(0).getIndex();
        int newParentBranchIdx = (int)storedAdmixtureParent->getTopologyChild(0).getIndex();
        std::set<int> idxSet;
        idxSet.insert(oldChildBranchIdx);
        idxSet.insert(oldParentBranchIdx);
        idxSet.insert(newChildBranchIdx);
        idxSet.insert(newParentBranchIdx);
        
        for (std::set<int>::iterator it = idxSet.begin(); it != idxSet.end(); it++)
        {
            int idx = *it;
            double v = branchRates[idx]->getValue();
            storedBranchRates[idx] = v;
            double u = exp(delta*(GLOBAL_RNG->uniform01() - 0.5));
            branchRates[idx]->setValue(new double(u * v));
            lnBwdPropRates += log(u);
        }
        
        
        // ln hastings ratio
        double lnFwdProposal = log(fwdProposal) + fwdWeightLnProb;
        double lnBwdProposal = log(bwdProposal) + bwdWeightLnProb;
        
        //std::cout << "fnpr lnPropRat\t" << lnBwdProposal - lnFwdProposal << " = " << lnBwdProposal << " - " << lnFwdProposal << ";\t";
        //std::cout << storedAge << " -> " << newAge << "\n";
        
        return lnBwdProposal - lnFwdProposal + lnBwdPropRates;
        //return -1000.0;
    }
}


void AdmixtureEdgeFNPR::rejectSimpleMove( void ) {
    
    
    // undo the proposal
    if (!failed)
    {
        
        //std::cout << "reject edge slide\n";
        
        // NOTE: root used to protect from infinite recursions caused by AdmixtureNode::flagNewickComputation() from addChild,removeChild,setParent.
        //AdmixtureNode* root = &variable->getValue().getRoot();
        
        
        
        // remove admixture edge from graph
        storedAdmixtureChild->removeChild(newAdmixtureChildChild, false);
        newAdmixtureChildChild->setParent(newAdmixtureChildParent, false);
        newAdmixtureChildParent->removeChild(storedAdmixtureChild, false);
        newAdmixtureChildParent->addChild(newAdmixtureChildChild, false);
        
        storedAdmixtureParent->removeChild(newAdmixtureParentChild, false);
        newAdmixtureParentChild->setParent(newAdmixtureParentParent, false);
        newAdmixtureParentParent->removeChild(storedAdmixtureParent, false);
        newAdmixtureParentParent->addChild(newAdmixtureParentChild, false);
        
        // insert admixtureParent into graph
        storedAdmixtureParentChild->setParent(storedAdmixtureParent, false);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent, false);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild, false);
        storedAdmixtureParent->addChild(storedAdmixtureParentChild, false);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent, false);
        
        // insert admixtureChild into graph
        storedAdmixtureChildChild->setParent(storedAdmixtureChild, false);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild, false);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild, false);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild, false);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent, false);

        
        /*
        // revert admixture parent
        if (storedAdmixtureParentParent == newAdmixtureParentParent && storedAdmixtureParentChild == newAdmixtureParentChild)
        {
            ; // do nothing
        }
        else
        {
            storedAdmixtureParent->setParent(root, false);
            
            newAdmixtureParentParent->removeChild(storedAdmixtureParent, false);
            newAdmixtureParentChild->setParent(newAdmixtureParentParent, false);
            newAdmixtureParentParent->addChild(newAdmixtureParentChild, false);
            storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild, false);
            storedAdmixtureParentParent->addChild(storedAdmixtureParent, false);
            storedAdmixtureParent->setParent(storedAdmixtureParentParent, false);
            
            newAdmixtureParentChild->setParent(newAdmixtureParentParent, false);
            storedAdmixtureParentChild->setParent(storedAdmixtureParent, false);
            storedAdmixtureParent->removeChild(newAdmixtureParentChild, false);
            storedAdmixtureParent->addChild(storedAdmixtureParentChild, false);
            
            storedAdmixtureParent->setParent(storedAdmixtureParentParent, false);
        }
        storedAdmixtureParent->setAge(storedAge);
        
        // revert admixture child
        if (storedAdmixtureChildParent == newAdmixtureChildParent && storedAdmixtureChildChild == newAdmixtureChildChild)
        {
            ; // do nothing
        }
        else
        {
            storedAdmixtureChild->setParent(root, false);
            
            newAdmixtureChildParent->removeChild(storedAdmixtureChild, false);
            newAdmixtureChildChild->setParent(newAdmixtureChildParent, false);
            newAdmixtureChildParent->addChild(newAdmixtureChildChild, false);
            storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild, false);
            storedAdmixtureChildParent->addChild(storedAdmixtureChild, false);
            storedAdmixtureChild->setParent(storedAdmixtureChildParent, false);
            
            newAdmixtureChildChild->setParent(newAdmixtureChildParent, false);
            storedAdmixtureChildChild->setParent(storedAdmixtureChild, false);
            storedAdmixtureChild->removeChild(newAdmixtureChildChild, false);
            storedAdmixtureChild->addChild(storedAdmixtureChildChild, false);
            
            storedAdmixtureChild->setParent(storedAdmixtureChildParent, false);
            
        }
        
        */
        
        // restore rates
        for (std::map<int,double>::iterator it = storedBranchRates.begin(); it != storedBranchRates.end(); it++)
        {
            branchRates[it->first]->setValue(new double(it->second));
        }
        
        // revert the age
        storedAdmixtureChild->setAge(storedAge);
        storedAdmixtureChild->setWeight(storedWeight);
        
    }
    //std::cout << "reject admixture edge FNPR\n";
}

void AdmixtureEdgeFNPR::acceptSimpleMove(void) {
    //std::cout << "accept admixture edge FNPR\n";
}


void AdmixtureEdgeFNPR::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    if (variable == oldN)
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    
    for (size_t i = 0; i < branchRates.size(); i++)
    {
        if (branchRates[i] == oldN)
        {
            branchRates[i] = static_cast<ContinuousStochasticNode*>(newN) ;
        }
    }
    
}



void AdmixtureEdgeFNPR::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeFNPR::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeFNPR::isActive(int g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeFNPR::performMove( double &probRatio ) {
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf || hr == RbConstants::Double::neginf ) {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch();
    probRatio = 0.0;
    
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
            //std::cout << theNode->getName() << "  " << theNode->getLnProbability() << "  " << theNode->getLnProbabilityRatio() << "\n";
        }
    }
    //std::cout << probRatio << "\n";
    
    return hr;
}
