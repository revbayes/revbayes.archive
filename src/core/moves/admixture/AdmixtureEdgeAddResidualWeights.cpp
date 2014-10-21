//
//  AdmixtureEdgeAddResidualWeights.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeAddResidualWeights.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <list>
#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeAddResidualWeights::AdmixtureEdgeAddResidualWeights(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, double d, int ag, int me, bool asa, double w) : MoveOld( v, w), variable( v ), rate(r), admixtureCount(ac), residuals(res), activeGen(ag), changed(false), failed(false), allowSisterAdmixture(asa), maxEvents(me), delta(d) {
    
    nodes.insert(rate);
    nodes.insert(residuals);
    nodes.insert(admixtureCount);
}

/* Clone object */
AdmixtureEdgeAddResidualWeights* AdmixtureEdgeAddResidualWeights::clone( void ) const {
    
    return new AdmixtureEdgeAddResidualWeights( *this );
}

const std::string& AdmixtureEdgeAddResidualWeights::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeAddResidualWeights";
    
    return name;
}

/** Perform the move */
double AdmixtureEdgeAddResidualWeights::performSimpleMove( void ) {
    
    //std::cout << "\nAdmix Edge Add RW\n";
    
    storedResiduals = residuals->getValue();
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    double fwdProposal = 1.0;
    
    // get tree info
    AdmixtureTree& tau = variable->getValue();
    
    size_t numTaxa = tau.getNumberOfTips();
    AdmixtureNode* nodeSrc = NULL;
    AdmixtureNode* nodeDst = NULL;
    size_t k_a = 0;
    size_t k_b = 0;
    
    if (tau.getNumberOfAdmixtureParents() >= maxEvents)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    
    double maxStoredResidual = 0.0;
    for (size_t i = 0; i < storedResiduals.size(); i++)
        if (storedResiduals[i] > maxStoredResidual)
            maxStoredResidual = storedResiduals[i];
    double lambda = delta / maxStoredResidual;
    
    // get sum of positive residuals for each taxon against all other taxa
    std::vector<double> residualWeights_a(numTaxa,0.0);
    
    double sumResidualWeights_a = 0.0;
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            double r = storedResiduals[i * numTaxa + j];
            r = exp(lambda*r);
            if (r > 0.0 && i != j)
                //if (i != j)
            {
                residualWeights_a[i] += r;
                sumResidualWeights_a += r;
            }

        }
    }
    
    // if there are no positive residuals, abort move
    if (sumResidualWeights_a == 0.0)
    {
        //std::cout << "no pos residuals\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    /*
    std::cout << "rw_a\t";
    for (size_t i = 0; i < numTaxa; i++)
        std::cout << residualWeights_a[i] / sumResidualWeights_a << "\t";
    std::cout << "\n";
     */
    
    // sample taxon A from weights
    double u_a = rng->uniform01() * sumResidualWeights_a;
    double m_a = 0.0;
    //size_t k_a = 0;
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        m_a += residualWeights_a[i];
        if (m_a > u_a)
        {
            k_a = i;
            nodeDst = &tau.getNode(k_a);
            break;
        }
    }
    fwdProposal *= (residualWeights_a[k_a] / sumResidualWeights_a);
    
    // get sum of positive residuals for each taxon wrt taxon A
    std::vector<double> residualWeights_b(numTaxa,0.0);
    double sumResidualWeights_b = 0.0;
    for (size_t i = 0; i < numTaxa; i++)
    {
        double r = storedResiduals[k_a * numTaxa + i];
        r = exp(lambda*r);
        if (r > 0.0 && k_a != i)
            //if (k_a != i)
        {
            residualWeights_b[i] += r;
            sumResidualWeights_b += r;
        }
    }
   
    /*
    std::cout << "rw_b\t";
    for (size_t i = 0; i < numTaxa; i++)
        std::cout << residualWeights_b[i] / sumResidualWeights_b << "\t";
    std::cout << "\n";
    */
    
    // sample taxon B from weights
    double u_b = rng->uniform01() * sumResidualWeights_b;
    double m_b = 0.0;
    //size_t k_b = 0;
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        m_b += residualWeights_b[i];
        if (m_b > u_b)
        {
            k_b = i;
            nodeSrc = &tau.getNode(k_b);
            break;
        }
    }
    fwdProposal *= (residualWeights_b[k_b] / sumResidualWeights_b);
    
    // get path from tip to root for both nodes
    AdmixtureNode* nd_a = nodeSrc;
    std::list<AdmixtureNode*> path_a;
    //std::cout << "path_a : tip -> root\n";
    while (nd_a != NULL)
    {
        //std::cout << "\tnd_a\t" << nd_a->getIndex() << "\t" << nd_a << "\t" << nd_a->getAge() << "\t" << &nd_a->getParent() << "\n";
        path_a.push_back(nd_a);
        nd_a = &nd_a->getParent();
    }
    nd_a = path_a.back();

    AdmixtureNode* nd_b = nodeDst;
    std::list<AdmixtureNode*> path_b;
    //std::cout << "path_b : tip -> root\n";
    while (nd_b != NULL)
    {
        //std::cout << "\tnd_b\t" << nd_b->getIndex() << "\t" << nd_b << "\t" << nd_b->getAge() << "\n";
        path_b.push_back(nd_b);
        nd_b = &nd_b->getParent();
    }
    nd_b = path_b.back();
    
    // find the node where the paths diverge by traversing both paths from root to tip
    AdmixtureNode* mrca = nd_a;
    //std::cout << "mrca : root -> tip\n";
    while (nd_a == nd_b && !path_a.empty() && !path_b.empty())
    {
        mrca = nd_a;
    //    std::cout << "\t" << mrca << "\t" << mrca->getAge() << "\n";
        nd_a = path_a.back();
        nd_b = path_b.back();
        path_a.pop_back();
        path_b.pop_back();
    }
    
    // sample u.a.r. between nd_b and present (we disallow sister branches to admix...)
    double minAge = nodeSrc->getAge();
    if (minAge < nodeDst->getAge())
        minAge = nodeDst->getAge();
    double maxAge = mrca->getAge();
    
    int mrcaChIdx = 0;
    
    //if (allowSisterAdmixture == false && mrca->getTopologyChild(0).isTip() == false && mrca->getTopologyChild(1).isTip() == false)
    if (allowSisterAdmixture == false)// && mrca->getTopologyChild(0).isTip() == false && mrca->getTopologyChild(1).isTip() == false)
    {
        maxAge = mrca->getTopologyChild(0).getAge();
        if (maxAge < mrca->getTopologyChild(1).getAge())
        {
            maxAge = mrca->getTopologyChild(1).getAge();
            mrcaChIdx = 1;
        }
    }
    if (maxAge <= minAge)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    //maxAge = mrca->getAge();
    //double admixtureAge = rng->uniform01() * (maxAge - minAge) + minAge;
    double exp_lambda = 2.0;
    double admixtureAge = RbStatistics::Beta::rv(1.0,exp_lambda, *rng) * (maxAge - minAge) + minAge;

    
    while (nd_a->getAge() > admixtureAge && !path_a.empty())
    {
        nd_a = path_a.back();
        path_a.pop_back();

        if (nd_a->getParent().getAge() > admixtureAge && nd_a->getAge() < admixtureAge)
            break;
    }
    
    while (nd_b->getAge() > admixtureAge && !path_b.empty())
    {
        nd_b = path_b.back();
        path_b.pop_back();
        
        if (nd_b->getParent().getAge() > admixtureAge && nd_b->getAge() < admixtureAge)
            break;
    }
    
    if (GLOBAL_RNG->uniform01() < 0.5)
    {
        AdmixtureNode* tmp = nd_a;
        nd_a = nd_b;
        nd_b = tmp;
    }


  
    // get admixture weight
    double a = 1.0;
    double b = 3.0;
    double admixtureWeight = RbStatistics::Beta::rv(a, b, *rng);
    double lnW = 0.0;
    admixtureWeight /= 2;
    
    // add nodes to tree
    storedAdmixtureParent = new AdmixtureNode((int)tau.getNumberOfNodes());
    tau.pushAdmixtureNode(storedAdmixtureParent);
    storedAdmixtureChild = new AdmixtureNode((int)tau.getNumberOfNodes());
    tau.pushAdmixtureNode(storedAdmixtureChild);
    
    // add edge
    tau.addAdmixtureEdge(storedAdmixtureParent, storedAdmixtureChild, nd_a, nd_b, admixtureAge, admixtureWeight, true);
    
    // update no of adm evts
    numEvents = (int)tau.getNumberOfAdmixtureChildren();
    admixtureCount->setValue((const int)numEvents);
    
    // bombs away
    double lnP = 0.0;
    return 0.0;
    
    return lnP - lnW;// - log(fwdProposal);
    
    
    
    //    // store adjacent nodes to new parent node
    //    storedAdmixtureParentChild = nd_a;
    //    storedAdmixtureParentParent = &nd_a->getParent();
    //
    //    // insert admixtureParent into graph
    //    storedAdmixtureParent = new AdmixtureNode((int)tau.getNumberOfNodes());
    //    storedAdmixtureParent->setAge(admixtureAge);
    //    storedAdmixtureParent->setParent(storedAdmixtureParentParent);
    //    storedAdmixtureParent->addChild(storedAdmixtureParentChild);
    //    storedAdmixtureParent->setOutgroup(storedAdmixtureParentChild->isOutgroup());
    //    storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
    //    storedAdmixtureParentParent->addChild(storedAdmixtureParent);
    //    storedAdmixtureParentChild->setParent(storedAdmixtureParent);
    //    tau.pushAdmixtureNode(storedAdmixtureParent);
    //
    //    // store adjacent nodes to new child node
    //    storedAdmixtureChildChild = nd_b;
    //    storedAdmixtureChildParent = &nd_b->getParent();
    ////    storedAdmixtureChildChild = path_b.back();
    ////    storedAdmixtureChildParent = nd_b;
    //
    //
    //    // insert admixtureChild into graph
    //    storedAdmixtureChild = new AdmixtureNode((int)tau.getNumberOfNodes());
    //    storedAdmixtureChild->setAge(admixtureAge);
    //    storedAdmixtureChild->setParent(storedAdmixtureChildParent);
    //    storedAdmixtureChild->addChild(storedAdmixtureChildChild);
    //    storedAdmixtureChild->setOutgroup(storedAdmixtureChildChild->isOutgroup());
    //    storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
    //    storedAdmixtureChildParent->addChild(storedAdmixtureChild);
    //    storedAdmixtureChildChild->setParent(storedAdmixtureChild);
    //    storedAdmixtureChild->setWeight(admixtureWeight);
    //    tau.pushAdmixtureNode(storedAdmixtureChild);
    //
    //    // create admixture edge
    //    storedAdmixtureChild->setAdmixtureParent(storedAdmixtureParent);
    //    storedAdmixtureParent->setAdmixtureChild(storedAdmixtureChild);
    //    
    //
    //    //std::cout << "\nafter AEA proposal\n";
    //    //tau.checkAllEdgesRecursively(&tau.getRoot());
    
}


void AdmixtureEdgeAddResidualWeights::rejectSimpleMove( void ) {
    
    if (!failed)
    {
    
        // remove nodes from graph structure
        AdmixtureTree& tau = variable->getValue();
        
        tau.removeAdmixtureEdge(storedAdmixtureParent,true);
        
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        
        delete storedAdmixtureParent;
        delete storedAdmixtureChild;
        
        admixtureCount->setValue((const int)(numEvents-1));
    
    }
    
}

void AdmixtureEdgeAddResidualWeights::acceptSimpleMove(void)
{
    //std::cout << "add_RW accept\n";
    //  std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
    //  std::cout << "\n";
    //std::cout << "accept\n";
}


void AdmixtureEdgeAddResidualWeights::swapNode(DagNode *oldN, DagNode *newN) {
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
  //  else if (oldN == delayTimer)
  //  {
  //      delayTimer = static_cast<ConstantNode<int>* >(newN);
  //  }
    else if (oldN == residuals)
    {
        residuals = static_cast<DeterministicNode< RbVector<double> >* >(newN);
    }
    else if (oldN == admixtureCount)
    {
        admixtureCount = static_cast<StochasticNode<int>* >(newN);
    }
}


void AdmixtureEdgeAddResidualWeights::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
    rate->touch();
    admixtureCount->touch();
}


void AdmixtureEdgeAddResidualWeights::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeAddResidualWeights::isActive(unsigned long g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeAddResidualWeights::performMove( double &probRatio ) {
    
    /*
    if (delayTimer->getValue() > 0)
    {
        failed = true;
        //delayTimer->setValue(delayTimer->getValue()-1);
        //        delay--;
        return RbConstants::Double::neginf;
    }
     */
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    //std::cout << "hr  " << hr << "\n";
    if ( hr != hr || hr == RbConstants::Double::inf ) {
        ;
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch();
    //rate->touch();
    admixtureCount->touch();
    probRatio = variable->getLnProbabilityRatio();
    probRatio += admixtureCount->getLnProbabilityRatio();
    //std::cout << "probRatio " << probRatio << "\n";
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            //theNode->touch();
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << theNode->getName() << "\t" << theNode->getLnProbability() << " " << theNode->getLnProbabilityRatio() << "\n";
        }
    }
    
    return hr;
}
