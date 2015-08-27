//
//  AdmixtureEdgeSlide.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/1/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeSlide.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>
#include <list>

using namespace RevBayesCore;

AdmixtureEdgeSlide::AdmixtureEdgeSlide(StochasticNode<AdmixtureTree> *v, std::vector<ContinuousStochasticNode*> br, int ag, bool asa, double l, double w) : MoveOld( v, w), variable( v ), branchRates(br), lambda(l), failed(false), changed(false), allowSisterAdmixture(asa), activeGen(ag) {
    
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
}

/* Clone object */
AdmixtureEdgeSlide* AdmixtureEdgeSlide::clone( void ) const {
    
    return new AdmixtureEdgeSlide( *this );
}

const std::string& AdmixtureEdgeSlide::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeSlide";
    
    return name;
}

void AdmixtureEdgeSlide::findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode *n) {
    
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
double AdmixtureEdgeSlide::performSimpleMove( void ) {
    
    //std::cout << "\nAdmix Edge Beta Slide\n";
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
        // int oldChildBranchIdx = (int)storedAdmixtureChild->getTopologyChild(0).getIndex();
        // int oldParentBranchIdx = (int)storedAdmixtureParent->getTopologyChild(0).getIndex();

        //std::cout << "nd_a\n";
        
        // sample tips for "slide" railings
        AdmixtureNode* nd_a = storedAdmixtureParent;
        while (nd_a->getNumberOfChildren() != 0)
        {
        // MJL 111713: not sure if right
            //if (nd_a->getNumberOfChildren() == 2) fwdProposal *= 0.5;
            nd_a = &nd_a->getChild(rng->uniform01() * nd_a->getNumberOfChildren());

        }
        AdmixtureNode* nodeSrc = nd_a;
        
        //std::cout << "nd_b\n";
        AdmixtureNode* nd_b = storedAdmixtureChild;
        while (nd_b->getNumberOfChildren() != 0)
        {
            // MJL 111713: not sure if right
            //if (nd_b->getNumberOfChildren() == 2) fwdProposal *= 0.5;
            nd_b = &nd_b->getChild(rng->uniform01() * nd_b->getNumberOfChildren());
        }
        AdmixtureNode* nodeDst = nd_b;
        
        //std::cout << "rem edge\n";
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
        storedAge = storedAdmixtureChild->getAge();
        
        // find parent range
        std::list<AdmixtureNode*> path_a;
        //std::cout << "path_a : tip -> root\n";
        while (nd_a != NULL)
        {
          //  std::cout << "\t" << nd_a << "\t" << nd_a->getAge() << "\n";
            path_a.push_back(nd_a);
            nd_a = &nd_a->getParent();
            //if (nd_a != NULL && nd_a->getNumberOfChildren() == 2) bwdProposal *= 0.5;
        }
        nd_a = path_a.back();
        
        // get admixtureChild path
        std::list<AdmixtureNode*> path_b;
        //std::cout << "path_b : tip -> root\n";
        while (nd_b != NULL)
        {
          //  std::cout << "\t" << nd_b << "\t" << nd_b->getAge() << "\n";
            path_b.push_back(nd_b);
            nd_b = &nd_b->getParent();
            //if (nd_b != NULL && nd_b->getNumberOfChildren() == 2) bwdProposal *= 0.5;
        }
        nd_b = path_b.back();
        
        
        // find the node where the paths diverge by traversing both paths from root to tip
        AdmixtureNode* mrca = nd_a;
        //std::cout << "mrca : root -> tip\n";
        // int numSplits = 0;
        while (nd_a == nd_b && !path_a.empty() && !path_b.empty())
        {
            mrca = nd_a;
            //std::cout << "\t" << mrca << "\t" << mrca->getAge() << "\n";
            nd_a = path_a.back();
            nd_b = path_b.back();
            path_a.pop_back();
            path_b.pop_back();
        }
        
        // MJL 111713: not sure if right
        // reverse move, prob of selecting the slide rails
        // bwdProposal *= pow(0.5,path_a.size()+path_b.size());
        
        // sample time from beta, scaled between mrca and min tip time
        //double minAge = storedAdmixtureParent->getAge();
        double minAge = nodeSrc->getAge();
        if (minAge < nodeDst->getAge())
            minAge = nodeDst->getAge();
        double maxAge = mrca->getAge();
        
        //std::cout << "A1\n";
        int mrcaChIdx = 0;

//        if (allowSisterAdmixture == false && mrca->getTopologyChild(0).isTip() == false && mrca->getTopologyChild(1).isTip() == false)
        
        // confused, because there should never exist such an edge..., so why is this a problem? add or replace must allow this state to occur
        if (allowSisterAdmixture == false && mrca->getTopologyChild(0).isTip() == false && mrca->getTopologyChild(1).isTip() == false)
        {
            maxAge = mrca->getTopologyChild(0).getAge();
            if (maxAge < mrca->getTopologyChild(1).getAge())
            {
                maxAge = mrca->getTopologyChild(1).getAge();
                mrcaChIdx = 1;
            }
        }
       // std::cout << mrca->getTopologyChild(0).getAge() << " " << mrca->getTopologyChild(1).getAge() << "\n";
       // std::cout << "A2\n";
        
        //maxAge = mrca->getAge();
        
       // std::cout << minAge << " " << maxAge << "\n";
        
        // get age range between nodes
        double ageRange = maxAge - minAge;
        
        // sample beta rv and compute proposal factors
        double unitAge = (storedAge - minAge) / ageRange;
        double a = lambda * unitAge + 1.0;
        double b = lambda * (1.0 - unitAge) + 1.0;
        
        
        double newUnitAge = RbStatistics::Beta::rv(a, b, *rng);
        
        fwdProposal *= RbStatistics::Beta::pdf(a, b, newUnitAge);
          //      std::cout << "A4\n";
        double newAge = newUnitAge * ageRange + minAge;
        double new_a = lambda * newUnitAge + 1.0;
        double new_b = lambda * (1.0 - newUnitAge) + 1.0;
//        std::cout << unitAge << " " << new_a << " " << new_b << "\n";
        bwdProposal *= RbStatistics::Beta::pdf(new_a, new_b, unitAge);
  //      std::cout << "bwdProposal2 " << bwdProposal << "\n";
        
        //std::cout << "a_path : find admixtureAge\n";
        while (nd_a->getAge() > newAge && !path_a.empty())
        {
            nd_a = path_a.back();
            //std::cout << "\t" << nd_a << "\t" << nd_a->getAge() << "\n";
            path_a.pop_back();
            
            if (nd_a->getParent().getAge() > newAge && nd_a->getAge() < newAge)
                break;
        }
        
        //std::cout << "b_path : find admixtureAge\n";
        while (nd_b->getAge() > newAge && !path_b.empty())
        {
            nd_b = path_b.back();
            //std::cout << "\t" << nd_b << "\t" << nd_b->getAge() << "\n";
            path_b.pop_back();
            
            if (nd_b->getParent().getAge() > newAge && nd_b->getAge() < newAge)
                break;
        }
        
        /*
        std::cout << "sampled nodes for\n";
        std::cout << "t_old   " << storedAge << "\n";
        std::cout << "t_new   " << newAge << "\n";
        std::cout << "nd_a    " << nd_a << "\t" << nd_a->getAge() << "\n";
        std::cout << "nd_a->p " << &nd_a->getParent() << "\t" << nd_a->getParent().getAge() << "\n";
        std::cout << "nd_b    " << nd_b << "\t" << nd_b->getAge() << "\n";
        std::cout << "nd_b->p " << &nd_b->getParent() << "\t" << nd_b->getParent().getAge() << "\n";
        */
         
        AdmixtureNode* root = &tau.getRoot();
        
        // store adjacent nodes to new parent node
        newAdmixtureParentChild = nd_a;
        newAdmixtureParentParent = &nd_a->getParent();
        
        // insert admixtureParent into graph
        storedAdmixtureParent->setAge(newAge);
        storedAdmixtureParent->setParent(root);
        newAdmixtureParentChild->setParent(storedAdmixtureParent);
        newAdmixtureParentParent->addChild(storedAdmixtureParent);
        newAdmixtureParentParent->removeChild(newAdmixtureParentChild);
        storedAdmixtureParent->addChild(newAdmixtureParentChild);
        storedAdmixtureParent->setParent(newAdmixtureParentParent);
        
        // store adjacent nodes to new child node
        newAdmixtureChildChild = nd_b;
        newAdmixtureChildParent = &nd_b->getParent();
        
        // insert admixtureChild into graph
        storedAdmixtureChild->setAge(newAge);
        storedAdmixtureChild->setParent(root);
        newAdmixtureChildChild->setParent(storedAdmixtureChild);
        newAdmixtureChildParent->addChild(storedAdmixtureChild);
        newAdmixtureChildParent->removeChild(newAdmixtureChildChild);
        storedAdmixtureChild->addChild(newAdmixtureChildChild);
        storedAdmixtureChild->setParent(newAdmixtureChildParent);
        
        
        
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
/*
        storedBranchRates.clear();
        double delta = 10.0;
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
        */

        
        // ln hastings ratio
        double lnFwdProposal = log(fwdProposal) + fwdWeightLnProb;
        double lnBwdProposal = log(bwdProposal) + bwdWeightLnProb;
        
       // std::cout << "slide lnPropRat\t" << lnBwdProposal - lnFwdProposal << " = " << lnBwdProposal << " - " << lnFwdProposal << ";\t";
      //  std::cout << storedAge << " -> " << newAge << "\n";
      
        return lnBwdProposal - lnFwdProposal + lnBwdPropRates;
        //return -1000.0;
    }
}


void AdmixtureEdgeSlide::rejectSimpleMove( void ) {
    

    // undo the proposal
    if (!failed)
    {
        
        //std::cout << "reject edge slide\n";
        
        // NOTE: root used to protect from infinite recursions caused by AdmixtureNode::flagNewickComputation() from addChild,removeChild,setParent.
        AdmixtureNode* root = &variable->getValue().getRoot();

        // revert admixture parent
        if (storedAdmixtureParentParent == newAdmixtureParentParent && storedAdmixtureParentChild == newAdmixtureParentChild)
        {
            ; // do nothing
        }
        else
        {
            storedAdmixtureParent->setParent(root);
            
            newAdmixtureParentParent->removeChild(storedAdmixtureParent);
            newAdmixtureParentChild->setParent(newAdmixtureParentParent);
            newAdmixtureParentParent->addChild(newAdmixtureParentChild);
            storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
            storedAdmixtureParentParent->addChild(storedAdmixtureParent);
            storedAdmixtureParent->setParent(storedAdmixtureParentParent);
            
            newAdmixtureParentChild->setParent(newAdmixtureParentParent);
            storedAdmixtureParentChild->setParent(storedAdmixtureParent);
            storedAdmixtureParent->removeChild(newAdmixtureParentChild);
            storedAdmixtureParent->addChild(storedAdmixtureParentChild);
            
            storedAdmixtureParent->setParent(storedAdmixtureParentParent);
        }
        storedAdmixtureParent->setAge(storedAge);

        // revert admixture child
        if (storedAdmixtureChildParent == newAdmixtureChildParent && storedAdmixtureChildChild == newAdmixtureChildChild)
        {
            ; // do nothing
        }
        else
        {
            storedAdmixtureChild->setParent(root);
        
            newAdmixtureChildParent->removeChild(storedAdmixtureChild);
            newAdmixtureChildChild->setParent(newAdmixtureChildParent);
            newAdmixtureChildParent->addChild(newAdmixtureChildChild);
            storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
            storedAdmixtureChildParent->addChild(storedAdmixtureChild);
            storedAdmixtureChild->setParent(storedAdmixtureChildParent);
            
            newAdmixtureChildChild->setParent(newAdmixtureChildParent);
            storedAdmixtureChildChild->setParent(storedAdmixtureChild);
            storedAdmixtureChild->removeChild(newAdmixtureChildChild);
            storedAdmixtureChild->addChild(storedAdmixtureChildChild);
        
            storedAdmixtureChild->setParent(storedAdmixtureChildParent);
            
        }
        
        // restore rates
        for (std::map<int,double>::iterator it = storedBranchRates.begin(); it != storedBranchRates.end(); it++)
        {
            branchRates[it->first]->setValue(new double(it->second));
        }
                
        // revert the age
        storedAdmixtureChild->setAge(storedAge);
        storedAdmixtureChild->setWeight(storedWeight);
        
    }
}

void AdmixtureEdgeSlide::acceptSimpleMove(void) {
    //std::cout << "accept edge slide\n";
}


void AdmixtureEdgeSlide::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
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



void AdmixtureEdgeSlide::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeSlide::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeSlide::isActive(unsigned long g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeSlide::performMove( double &probRatio ) {
    
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
    probRatio = variable->getLnProbabilityRatio();
    
    /*
    for (std::map<int,double>::iterator it = storedBranchRates.begin(); it != storedBranchRates.end(); it++)
    {
        branchRates[it->first]->touch();
        probRatio += branchRates[it->first]->getLnProbabilityRatio();
        //std::cout << branchRates[it->first]->getLnProbabilityRatio() << "\n";
    }
     */

    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << theNode->getName() << "  " << theNode->getLnProbability() << "  " << theNode->getLnProbabilityRatio() << "\n";
        }
    }
    //std::cout << hr << "\t" << probRatio << "\n";
    
    return hr;
}
