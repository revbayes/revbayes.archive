
//  AdmixtureEdgeAddCladeResiduals.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/28/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeAddCladeResiduals.h"

#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <list>
#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeAddCladeResiduals::AdmixtureEdgeAddCladeResiduals(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode<RbVector<double> >* res, int ag, int me, bool asa, double w) : MoveOld( v, w), variable( v ), rate(r), admixtureCount(ac), residuals(res), activeGen(ag), changed(false), failed(false), allowSisterAdmixture(asa), maxEvents(me),  numNodes(0) {
    
    nodes.insert(rate);
    nodes.insert(residuals);
    nodes.insert(admixtureCount);
    numNodes = (int)variable->getValue().getNumberOfNodes();
    numTips = (int)variable->getValue().getNumberOfTips();
    
}



/* Clone object */
AdmixtureEdgeAddCladeResiduals* AdmixtureEdgeAddCladeResiduals::clone( void ) const {
    
    return new AdmixtureEdgeAddCladeResiduals( *this );
}



const std::string& AdmixtureEdgeAddCladeResiduals::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeAddCladeResiduals";
    
    return name;
}

std::pair<AdmixtureNode*,AdmixtureNode*> AdmixtureEdgeAddCladeResiduals::sampleAdmixtureEdgeFromResiduals(double& lnP)
{

    // sample a partition based on sum of residuals
    double sum1 = 0.0;
    //std::cout << "cladeResidualSums : ";
    for (size_t i = 0; i < cladeResidualSums.size(); i++)
    {
        sum1 += cladeResidualSums[i];
      //  std::cout << cladeResidualSums[i] << " ";
    }
//    std::cout << "\n";
    
    double u1 = GLOBAL_RNG->uniform01() * sum1;
    size_t idx1 = 0;
    for (size_t i = 0; i < cladeResidualSums.size(); i++)
    {
        u1 -= cladeResidualSums[i];
        if (u1 < 0.0)
        {
            idx1 = i;
            lnP += log(cladeResidualSums[i] / sum1);
            break;
        }
    }
  //  std::cout << "picked partition " << idx1 << " (" << cladeResidualSums[idx1]/sum1 << ")\n";
    
    // sample a pair of edges from partition
    double sum2 = 0.0;
//    std::cout << "partition sum : ";
    std::map<std::pair<AdmixtureNode*,AdmixtureNode*>,double>::iterator it1, it2;
    for (it1 = cladeResiduals[idx1].begin(); it1 != cladeResiduals[idx1].end(); it1++)
    {
        sum2 += it1->second;
  //      std::cout << it1->second << " ";
    }
  //  std::cout << "\n";
    
    double u2 = GLOBAL_RNG->uniform01() * sum2;
    int idx2 = 0;
    for (it1 = cladeResiduals[idx1].begin(); it1 != cladeResiduals[idx1].end(); it1++)
    {
        u2 -= it1->second;
        if (u2 < 0.0)
        {
            lnP += log(it1->second / sum2);
    //        std::cout << "picked pair " << idx2 << " (" << it1->second/sum2 << ") : " << it1->first.first->getIndex() << "," << it1->first.second->getIndex() << "\n";
            return it1->first;
        }
        idx2++;
    }
    
    // should never reach here
//    std::cout << "error!?\n";
    return (cladeResiduals[idx1].begin())->first;
}

void AdmixtureEdgeAddCladeResiduals::findCladeResiduals(void)
{
    cladeResiduals.resize(cladeOrder.size());
    cladeResidualSums.resize(cladeOrder.size(), 0.0);
    double t = 1.0;
    
    // for each partition in jump chain
    for (size_t i = 0; i < cladeOrder.size(); i++)
    {
        double dt = t - cladeOrderAges[i];
        t -= dt;
        //std::cout << "dt " << i << " : " << dt << "\n";


        // for all pairs of partitions
        for (std::set<AdmixtureNode*>::iterator it1 = cladeOrder[i].begin(); it1 != cladeOrder[i].end(); it1++)
        {
            const std::set<AdmixtureNode*>& clade1 = clades[*it1];
            for (std::set<AdmixtureNode*>::iterator it2 = it1; ++it2 != cladeOrder[i].end(); )
            {
                const std::set<AdmixtureNode*>& clade2 = clades[*it2];
                
                // get residuals for (clade1,clade2) tips
                double r = 0.0;
                for (std::set<AdmixtureNode*>::const_iterator jt1 = clade1.begin(); jt1 != clade1.end(); jt1++)
                {
                    for (std::set<AdmixtureNode*>::const_iterator jt2 = clade2.begin(); jt2 != clade2.end(); jt2++)
                    {
                        size_t idx = (*jt1)->getIndex() * numTips + (*jt2)->getIndex();
                        double v = storedResiduals[idx];
                        if (v > 0)
                            r += v;
                    }
                }
                r *= dt;
                r /= 0.5 * (double)((i + 2)*(i + 1)); // normalizes against # of lineages at this time
                // r /= (double)(i+2); // normalizes against # of lineages at this time
                //r /= (clade1.size() * clade2.size()); // normalizes against # mtx elts
                std::pair<AdmixtureNode*,AdmixtureNode*> key( *it1, *it2 );
                cladeResidualSums[i] += r;
                cladeResiduals[i][key] = r;
            }
        }
    }
}

void AdmixtureEdgeAddCladeResiduals::findCladeOrder(std::set<AdmixtureNode*>& currentNodes)
{
    // find the oldest node
    AdmixtureNode* p = *currentNodes.begin();
    double age = p->getAge();

    if (currentNodes.size() == numTips)
    {
        cladeOrderAges.push_back(0.0);
        return;
    }
    
    else if (currentNodes.size() > 1)
    {
        for (std::set<AdmixtureNode*>::iterator it = currentNodes.begin(); it != currentNodes.end(); it++)
        {
            //std::cout << (*it)->getIndex() << "," << (*it)->getAge() << " ";
            if ((*it)->getAge() > age)
            {
                p = *it;
                age = p->getAge();
            }
        }
        //std::cout << "\n";
    }
    
    // get time of partition change
    if (currentNodes.size() > 1)
        cladeOrderAges.push_back(age);

    // update currentNodes
    currentNodes.erase(p);
    for (size_t i = 0; i < p->getNumberOfChildren(); i++)
        currentNodes.insert(&p->getChild(i));
    
    cladeOrder.push_back(currentNodes);
    
    if (currentNodes.size() > 0)
        findCladeOrder(currentNodes);
    
}

std::set<AdmixtureNode*> AdmixtureEdgeAddCladeResiduals::findClades(AdmixtureNode* p)
{
    std::set<AdmixtureNode*> tips;
    
    size_t nch = p->getNumberOfChildren();
    for (size_t i = 0; i < nch; i++)
    {
        std::set<AdmixtureNode*> childTips = findClades(&p->getChild(i));
        for (std::set<AdmixtureNode*>::iterator it = childTips.begin(); it != childTips.end(); it++)
        {
            tips.insert(*it);
        }
    }
    
    if (p->isTip())
        tips.insert(p);

    clades[p] = tips;
    
    return tips;
}

/** Perform the move */
double AdmixtureEdgeAddCladeResiduals::performSimpleMove( void ) {
    
//    std::cout << "\nAdmix Edge Add clade residuals\n";
    
    clades.clear();
    cladeOrder.clear();
    cladeResiduals.clear();
    cladeResidualSums.clear();
    cladeOrderAges.clear();

    storedResiduals = residuals->getValue();
    failed = false;
    double lnP = 0.0;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    // get tree info
    AdmixtureTree& tau = variable->getValue();
    AdmixtureNode* root = &tau.getRoot();
    
    // pass if maxEvents reached
    if (tau.getNumberOfAdmixtureParents() >= maxEvents)
    {
  //      std::cout << "truncating admixture events\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    else
    {
        
        // get clades
        findClades(root);
        
        // get all sister lineages
        std::set<AdmixtureNode*> currentNodes;
        currentNodes.insert(root);
        findCladeOrder(currentNodes);

        // get clade residuals
        findCladeResiduals();
        
        // sample admixture edge from residuals
        std::pair<AdmixtureNode*,AdmixtureNode*> nds = sampleAdmixtureEdgeFromResiduals(lnP);
  //      std::cout << "nds " << nds.first->getIndex() << " " << nds.second->getIndex() << "\n";
        if (GLOBAL_RNG->uniform01() < 0.5)
        {
            storedAdmixtureChildChild = nds.first;
            storedAdmixtureParentChild = nds.second;
        }
        else
        {
            storedAdmixtureChildChild = nds.second;
            storedAdmixtureParentChild = nds.first;
        }
        storedAdmixtureChildParent = &storedAdmixtureChildChild->getParent();
        storedAdmixtureParentParent = &storedAdmixtureParentChild->getParent();
        
        // sample age
        double maxAge = storedAdmixtureChildParent->getAge();
        if (storedAdmixtureParentParent->getAge() < maxAge)
            maxAge = storedAdmixtureParentParent->getAge();
        double minAge = storedAdmixtureChildChild->getAge();
        if (storedAdmixtureParentChild->getAge() > minAge)
            minAge = storedAdmixtureParentChild->getAge();
        
        double admixtureAge = GLOBAL_RNG->uniform01() * (maxAge-minAge) + minAge;
    //    std::cout << minAge << " < " << admixtureAge << " < " << maxAge << "\n";
        
        // sample weight
        double a = 1.0;
        double b = 1.5;
        double admixtureWeight = RbStatistics::Beta::rv(a, b, *rng);
        // double lnW = RbStatistics::Beta::lnPdf(a, b, admixtureWeight);
        admixtureWeight /= 2;
        
        /*
        // insert admixtureParent into graph
        storedAdmixtureParent = new AdmixtureNode((int)tau.getNumberOfNodes());
        storedAdmixtureParent->setAge(admixtureAge);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent,false);
        storedAdmixtureParent->addChild(storedAdmixtureParentChild,false);
        storedAdmixtureParent->setOutgroup(storedAdmixtureParentChild->isOutgroup());
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild,false);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent,false);
        storedAdmixtureParentChild->setParent(storedAdmixtureParent,false);
        tau.pushAdmixtureNode(storedAdmixtureParent);
        
        // insert admixtureChild into graph
        storedAdmixtureChild = new AdmixtureNode((int)tau.getNumberOfNodes());
        storedAdmixtureChild->setAge(admixtureAge);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent,false);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild,false);
        storedAdmixtureChild->setOutgroup(storedAdmixtureChildChild->isOutgroup());
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild,false);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild,false);
        storedAdmixtureChildChild->setParent(storedAdmixtureChild,false);
        storedAdmixtureChild->setWeight(admixtureWeight);
        tau.pushAdmixtureNode(storedAdmixtureChild);
        
        // update branch rates
        // ... ??
        
        // create admixture edge
        storedAdmixtureChild->setAdmixtureParent(storedAdmixtureParent);
        storedAdmixtureParent->setAdmixtureChild(storedAdmixtureChild);
        */
        
      
        storedAdmixtureParent = new AdmixtureNode((int)tau.getNumberOfNodes());
        tau.pushAdmixtureNode(storedAdmixtureParent);
        storedAdmixtureChild = new AdmixtureNode((int)tau.getNumberOfNodes());
        tau.pushAdmixtureNode(storedAdmixtureChild);
        
        storedAdmixtureChild->setOutgroup(storedAdmixtureChildChild->isOutgroup());
        storedAdmixtureParent->setOutgroup(storedAdmixtureParentChild->isOutgroup());
        
        tau.addAdmixtureEdge(storedAdmixtureParent, storedAdmixtureChild, storedAdmixtureParentChild, storedAdmixtureChildChild, admixtureAge, admixtureWeight, false);

        // update newick
        tau.getRoot().flagNewickRecomputation();
        
        // prior * proposal ratio
        numEvents = (int)tau.getNumberOfAdmixtureChildren();
        //double unitTreeLength = tau.getUnitTreeLength();
        //double lnP = 0.0;//log(unitTreeLength);
        admixtureCount->setValue((const int)numEvents);
        
        // stats
        // std::cout << "add_CR\t" << lnP << "\t" << lnW << ";\n";
        // std::cout << "a " << storedAdmixtureChild->getAge() << "\n";
        // std::cout << "w " << storedAdmixtureChild->getWeight() << "\n";
        // std::cout << rate->getValue() << "\t" << unitTreeLength << "\t" << numEvents << "\n";
        
        // bombs away
        // return -(lnP + lnW);
        return 0.0;
    }
}


void AdmixtureEdgeAddCladeResiduals::rejectSimpleMove( void ) {
    
    
    if (!failed)
    {
        //std::cout << "add_RW reject\n";
        
        // remove nodes from graph structure
        AdmixtureTree& tau = variable->getValue();
        
        // revert edges
        /*
         storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
         storedAdmixtureChildParent->removeChild(storedAdmixtureChild);
         storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
         storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
         storedAdmixtureParentParent->removeChild(storedAdmixtureParent);
         storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
         */
        
        tau.removeAdmixtureEdge(storedAdmixtureParent,false);
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        tau.getRoot().flagNewickRecomputation();
        
        admixtureCount->setValue((const int)(numEvents-1));
        // std::cout << "reject add clade residuals\n";
    }
    // true;
}

void AdmixtureEdgeAddCladeResiduals::acceptSimpleMove(void)
{
    //std::cout << "add_RW accept\n";
    //  std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
    //  std::cout << "\n";
//    std::cout << "accept add clade residuals\n";
  //  true;
    ;
}


void AdmixtureEdgeAddCladeResiduals::swapNode(DagNode *oldN, DagNode *newN) {
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
        residuals = static_cast<DeterministicNode< RbVector<double> >* >(newN);
    }
    else if (oldN == admixtureCount)
    {
        admixtureCount = static_cast<StochasticNode<int>* >(newN);
    }
}


void AdmixtureEdgeAddCladeResiduals::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
    rate->touch();
    admixtureCount->touch();
}


void AdmixtureEdgeAddCladeResiduals::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeAddCladeResiduals::isActive(unsigned long g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeAddCladeResiduals::performMove( double &probRatio ) {
    
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
    
//    std::cout << "hr  " << hr << "\n";
    if ( hr != hr || hr == RbConstants::Double::inf ) {
        ;
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch();
    //rate->touch();
    //admixtureCount->touch();
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
    //        std::cout << theNode->getName() << "\t" << theNode->getLnProbability() << " " << theNode->getLnProbabilityRatio() << "\n";
        }
    }
    
    return hr;
}
