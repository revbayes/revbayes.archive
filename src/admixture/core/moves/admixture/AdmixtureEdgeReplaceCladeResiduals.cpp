//
//  AdmixtureEdgeReplaceCladeResiduals.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/29/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeReplaceCladeResiduals.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <list>
#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeReplaceCladeResiduals::AdmixtureEdgeReplaceCladeResiduals(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, std::vector< ContinuousStochasticNode*> br, DeterministicNode< RbVector<double> >* res, int ag, bool asa, double w) : MoveOld( v, w), variable( v ), rate(r), branchRates(br), residuals(res), activeGen(ag), changed(false), failed(false), failedAdd(false), allowSisterAdmixture(asa) {

//AdmixtureEdgeReplaceCladeResiduals::AdmixtureEdgeReplaceCladeResiduals(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, int ag, int me, bool asa, double w) : Move( v, w), variable( v ), rate(r), admixtureCount(ac), residuals(res), activeGen(ag), changed(false), failed(false), maxEvents(me), allowSisterAdmixture(asa), numNodes(0) {
    
    nodes.insert(rate);
    nodes.insert(residuals);
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
    //nodes.insert(dt);
    numNodes = (int)variable->getValue().getNumberOfNodes();// - 2 * (int)variable->getValue().getNumberOfAdmixtureParents();
    numTips = (int)variable->getValue().getNumberOfTips();
    
}



/* Clone object */
AdmixtureEdgeReplaceCladeResiduals* AdmixtureEdgeReplaceCladeResiduals::clone( void ) const {
    
    return new AdmixtureEdgeReplaceCladeResiduals( *this );
}



const std::string& AdmixtureEdgeReplaceCladeResiduals::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeReplaceCladeResiduals";
    
    return name;
}

std::pair<AdmixtureNode*,AdmixtureNode*> AdmixtureEdgeReplaceCladeResiduals::sampleAdmixtureEdgeFromResiduals(double& lnP)
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
    //std::cout << "partition sum : ";
    std::map<std::pair<AdmixtureNode*,AdmixtureNode*>,double>::iterator it1, it2;
    for (it1 = cladeResiduals[idx1].begin(); it1 != cladeResiduals[idx1].end(); it1++)
    {
        sum2 += it1->second;
  //      std::cout << it1->second << " ";
    }
//    std::cout << "\n";
    
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

void AdmixtureEdgeReplaceCladeResiduals::findCladeResiduals(void)
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
                // r /= (double)(i+2); // normalizes against # of lineages at this time
                // r /= (clade1.size() * clade2.size()); // normalizes against # mtx elts
                r *= dt;
                r /= 0.5 * (double)((i + 2)*(i + 1)); // normalizes against # of lineages at this time
                std::pair<AdmixtureNode*,AdmixtureNode*> key( *it1, *it2 );
                cladeResidualSums[i] += r;
                cladeResiduals[i][key] = r;
            }
        }
    }
}

void AdmixtureEdgeReplaceCladeResiduals::findCladeOrder(std::set<AdmixtureNode*>& currentNodes)
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

std::set<AdmixtureNode*> AdmixtureEdgeReplaceCladeResiduals::findClades(AdmixtureNode* p)
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
double AdmixtureEdgeReplaceCladeResiduals::performSimpleMove( void ) {
    
  //  std::cout << "\nAdmix Edge Repl clade residuals\n";
  
    clades.clear();
    cladeOrder.clear();
    cladeOrderAges.clear();
    cladeResiduals.clear();
    cladeResidualSums.clear();
    
    storedResiduals = residuals->getValue();
    failed = false;
    failedAdd = false;
    double lnP = 0.0;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // get tree info
    AdmixtureTree& tau = variable->getValue();
    AdmixtureNode* root = &tau.getRoot();
    
    // if no admixtureParent exists, the proposal fails
    if (tau.getNumberOfAdmixtureParents() == 0)
    {
        failed = true;
    //    std::cout << "no admixture evts\n";
        return RbConstants::Double::neginf;
    }
    else
    {
        // remove random edge
        std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
        
        double u = rng->uniform01();
        size_t index = std::floor(admixtureParents.size() * u);
        
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
        storedAdmixtureChild->removeChild(storedAdmixtureChildChild,false);
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent,false);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChild,false);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild,false);
        
        storedAdmixtureParent->removeChild(storedAdmixtureParentChild,false);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent,false);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParent,false);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild,false);
        
        // get age for admixture event
        storedAdmixtureAge = storedAdmixtureChild->getAge();
        storedAdmixtureWeight = storedAdmixtureChild->getWeight();
        
        // update residuals
        variable->touch();
        storedResiduals = residuals->getValue();

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
      //  std::cout << "nds " << nds.first->getIndex() << " " << nds.second->getIndex() << "\n";
        if (GLOBAL_RNG->uniform01() < 0.5)
        {
            newAdmixtureChildChild = nds.first;
            newAdmixtureParentChild = nds.second;
        }
        else
        {
            newAdmixtureChildChild = nds.second;
            newAdmixtureParentChild = nds.first;
        }
        newAdmixtureChildParent = &newAdmixtureChildChild->getParent();
        newAdmixtureParentParent = &newAdmixtureParentChild->getParent();
        
        // sample age
        double maxAge = newAdmixtureChildParent->getAge();
        if (newAdmixtureParentParent->getAge() < maxAge)
            maxAge = newAdmixtureParentParent->getAge();
        double minAge = newAdmixtureChildChild->getAge();
        if (newAdmixtureParentChild->getAge() > minAge)
            minAge = newAdmixtureParentChild->getAge();
        
        double newAge = GLOBAL_RNG->uniform01() * (maxAge-minAge) + minAge;
        
        // sample weight
        double a = 1.0;
        double b = 10.0;
        double newWeight = RbStatistics::Beta::rv(a, b, *rng);
        //double lnW = RbStatistics::Beta::lnPdf(a, b, newWeight);
        newWeight /= 2;
        
        
        // insert admixtureParent into graph
        storedAdmixtureParent->setAge(newAge);
        newAdmixtureParentChild->setParent(storedAdmixtureParent,false);
        newAdmixtureParentParent->addChild(storedAdmixtureParent,false);
        newAdmixtureParentParent->removeChild(newAdmixtureParentChild,false);
        storedAdmixtureParent->addChild(newAdmixtureParentChild,false);
        storedAdmixtureParent->setParent(newAdmixtureParentParent,false);
        
        // insert admixtureChild into graph
        storedAdmixtureChild->setAge(newAge);
        storedAdmixtureChild->setWeight(newWeight);
        newAdmixtureChildChild->setParent(storedAdmixtureChild,false);
        newAdmixtureChildParent->addChild(storedAdmixtureChild,false);
        newAdmixtureChildParent->removeChild(newAdmixtureChildChild,false);
        storedAdmixtureChild->addChild(newAdmixtureChildChild,false);
        storedAdmixtureChild->setParent(newAdmixtureChildParent,false);
        
        // update with outgroup settings
        storedAdmixtureChild->setOutgroup(newAdmixtureChildChild->isOutgroup());
        storedAdmixtureParent->setOutgroup(newAdmixtureParentChild->isOutgroup());
        
        
        tau.getRoot().flagNewickRecomputation();
        
        // update branch rates
        storedBranchRates.clear();
        double lnBwdPropRates = 0.0;
        double delta = 0.5;
        // ... have oldBranchIdx already
        int newChildBranchIdx = (int)storedAdmixtureChild->getTopologyChild(0).getIndex();
        int newParentBranchIdx = (int)storedAdmixtureParent->getTopologyChild(0).getIndex();
        std::set<int> idxSet;
        idxSet.insert(oldChildBranchIdx);
        idxSet.insert(oldParentBranchIdx);
        idxSet.insert(newChildBranchIdx);
        idxSet.insert(newParentBranchIdx);
        
        //for (size_t i = 0; i < idxSet.size(); i++)
        for (std::set<int>::iterator it = idxSet.begin(); it != idxSet.end(); it++)
        {
            int idx = *it;
            double v = branchRates[idx]->getValue();
            storedBranchRates[idx] = v;
            double u = exp(delta*(GLOBAL_RNG->uniform01() - 0.5));
            branchRates[idx]->setValue(new double(u * v));
        //    std::cout << "br " << idx << " " << v << " -> " << u*v << "\n";
            lnBwdPropRates += log(u);
        }
        
        // update newick
        tau.getRoot().flagNewickRecomputation();
        
        // create admixture edge
        storedAdmixtureChild->setAdmixtureParent(storedAdmixtureParent);
        storedAdmixtureParent->setAdmixtureChild(storedAdmixtureChild);
        
        // prior * proposal ratio
        numEvents = (int)tau.getNumberOfAdmixtureChildren();
        //double unitTreeLength = tau.getUnitTreeLength();
        //double lnP = 0.0;//log(unitTreeLength);
        //admixtureCount->setValue(new int(numEvents));
        
        // stats
//        std::cout << "add_CR\t" << lnP << "\t" << lnW << ";\n";
//        std::cout << "a " << storedAdmixtureChild->getAge() << "\n";
//        std::cout << "w " << storedAdmixtureChild->getWeight() << "\n";
//        std::cout << rate->getValue() << "\t" << unitTreeLength << "\t" << numEvents << "\n";
        
        // bombs away
        return lnBwdPropRates;
    }
}


void AdmixtureEdgeReplaceCladeResiduals::rejectSimpleMove( void ) {
    
    
    if (!failed)
    {
        //std::cout << "add_RW reject\n";
        
        // remove new admixture edge from graph
        storedAdmixtureChild->removeChild(newAdmixtureChildChild,false);
        newAdmixtureChildChild->setParent(newAdmixtureChildParent,false);
        newAdmixtureChildParent->removeChild(storedAdmixtureChild,false);
        newAdmixtureChildParent->addChild(newAdmixtureChildChild,false);
        
        storedAdmixtureParent->removeChild(newAdmixtureParentChild,false);
        newAdmixtureParentChild->setParent(newAdmixtureParentParent,false);
        newAdmixtureParentParent->removeChild(storedAdmixtureParent,false);
        newAdmixtureParentParent->addChild(newAdmixtureParentChild,false);
        
        // restore admixture edge values
        storedAdmixtureParent->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setWeight(storedAdmixtureWeight);
        
        // restore stored admixture edge
        storedAdmixtureParentChild->setParent(storedAdmixtureParent,false);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent,false);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild,false); //
        storedAdmixtureParent->addChild(storedAdmixtureParentChild,false);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent,false);
        
        storedAdmixtureChildChild->setParent(storedAdmixtureChild,false);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild,false);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild,false);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild,false);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent,false);
        
        
        // update with outgroup settings
        storedAdmixtureChild->setOutgroup(storedAdmixtureChildChild->isOutgroup());
        storedAdmixtureParent->setOutgroup(storedAdmixtureParentChild->isOutgroup());
        
        // restore rates
        for (std::map<int,double>::iterator it = storedBranchRates.begin(); it != storedBranchRates.end(); it++)
        {
            branchRates[it->first]->setValue(new double(it->second));
        }

        
        // remove nodes from graph structure
        AdmixtureTree& tau = variable->getValue();
        //tau.eraseAdmixtureNode(storedAdmixtureParent);
        //tau.eraseAdmixtureNode(storedAdmixtureChild);
        tau.getRoot().flagNewickRecomputation();
        
        //admixtureCount->setValue(new int(numEvents-1));
//        std::cout << "reject repl clade residuals\n";
    }
    
}

void AdmixtureEdgeReplaceCladeResiduals::acceptSimpleMove(void)
{
    //std::cout << "add_RW accept\n";
    //  std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
    //  std::cout << "\n";
    ;//    std::cout << "accept repl clade residuals\n";
}


void AdmixtureEdgeReplaceCladeResiduals::swapNode(DagNode *oldN, DagNode *newN) {
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
    
    
    for (size_t i = 0; i < branchRates.size(); i++)
    {
        if (oldN == branchRates[i])
        {
            branchRates[i] = static_cast<ContinuousStochasticNode*>(newN);
        }
    }

}

void AdmixtureEdgeReplaceCladeResiduals::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
    //rate->touch();
}


void AdmixtureEdgeReplaceCladeResiduals::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeReplaceCladeResiduals::isActive(unsigned long g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeReplaceCladeResiduals::performMove( double &probRatio ) {
    
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
//    probRatio += admixtureCount->getLnProbabilityRatio();
//    probRatio = 0.0;
    
    for (std::map<int,double>::iterator it = storedBranchRates.begin(); it != storedBranchRates.end(); it++)
    {
        branchRates[it->first]->touch();
        probRatio += branchRates[it->first]->getLnProbabilityRatio();
//        std::cout << branchRates[it->first]->getLnProbabilityRatio() << "\n";
    }
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
//            std::cout << theNode->getName() << "\t" << theNode->getLnProbability() << " " << theNode->getLnProbabilityRatio() << "\n";
        }
    }
    
    return hr;
}
