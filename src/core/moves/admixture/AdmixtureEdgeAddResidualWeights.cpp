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

AdmixtureEdgeAddResidualWeights::AdmixtureEdgeAddResidualWeights(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode<std::vector<double> >* res, int ag, int me, bool asa, double w) : Move( v, w), variable( v ), rate(r), admixtureCount(ac), residuals(res), activeGen(ag), changed(false), failed(false), maxEvents(me), allowSisterAdmixture(asa) {
    
    nodes.insert(rate);
    nodes.insert(residuals);
    nodes.insert(admixtureCount);
    //nodes.insert(dt);

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
        //std::cout << "truncating admixture events\n";
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    if (rng->uniform01() < 0.5)
    {
        // get sum of positive residuals for each taxon against all other taxa
        std::vector<double> residualWeights_a(numTaxa,0.0);
        
        double sumResidualWeights_a = 0.0;
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                double r = storedResiduals[i * numTaxa + j];
                if (r > 0.0 && i != j)
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
            if (r > 0.0 && k_a != i)
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
    }
    else
    {
        k_a = rng->uniform01() * numTaxa;
        do
        {
            k_b = rng->uniform01() * numTaxa;
        } while(k_a == k_b);
        nodeDst = &tau.getNode(k_a);
        nodeSrc = &tau.getNode(k_b);
    }
    
    //std::cout << "taxa pair\t" << k_a << "\t" << k_b << "\n";
    //std::cout << "fwdProposal\t" << fwdProposal << "\tlnFwdProposal\t" << log(fwdProposal) << "\n";
    
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

    /*
    // sample u.a.r. between tMRCA and present
    double minAge = nodeSrc->getAge();
    if (nodeDst->getAge() < minAge)
        minAge = nodeDst->getAge();
    double maxAge = mrca->getAge();
    
    double admixtureAge = rng->uniform01() * (maxAge - minAge) + minAge;
*/
    
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
    double admixtureAge = rng->uniform01() * (maxAge - minAge) + minAge;
    
    double a = 1.0;
    double b = 1.5;
    double admixtureWeight = RbStatistics::Beta::rv(a, b, *rng);
    //admixtureWeight = 0.0;

    double lnW = 0.0;
    
    // We define the proposal density to equal the prior density, thus cancelling.
    
    // proposal
   // lnW -= -RbStatistics::Beta::lnPdf(a,b,admixtureWeight); // prob of sampling * prior prob

    // prior
  //  double prior_a = 1.0;
  //  double prior_b = 3.0;
  //  lnW += RbStatistics::Beta::lnPdf(prior_a, prior_b, admixtureWeight);
    
    
    admixtureWeight /= 2;
    
    // attach edge as appropriate from aPath to bPath
    //std::cout << "admixtureAge\t" << admixtureAge << " = u * (" << mrca->getAge() << " - " << minAge << ") + " << minAge << "\n";
    //std::cout << "mrca\t" << mrca << "\n";
    //std::cout << "path_a : mrca -> tip\n";
    
    
    //////////////
    
    // front=0, back=1
    
    //std::cout << "a_path : find admixtureAge\n";
    while (nd_a->getAge() > admixtureAge && !path_a.empty())
    {
        nd_a = path_a.back();
      //  std::cout << "\t" << nd_a << "\t" << nd_a->getAge() << "\n";
        path_a.pop_back();

        if (nd_a->getParent().getAge() > admixtureAge && nd_a->getAge() < admixtureAge)
            break;
    }
    
    
    // ... prob of selecting a certain source branch
    // double t_a = nd_a->getParent().getAge();
    // fwdProposal *= t_a / (t_a - nodeSrc->getAge());
    
    //std::cout << "b_path : find admixtureAge\n";
    while (nd_b->getAge() > admixtureAge && !path_b.empty())
    {
        nd_b = path_b.back();
    //    std::cout << "\t" << nd_b << "\t" << nd_b->getAge() << "\n";
        path_b.pop_back();
        
        if (nd_b->getParent().getAge() > admixtureAge && nd_b->getAge() < admixtureAge)
            break;
    }
    
    
    /*
    std::cout << "sampled nodes for\n";
    std::cout << "t       " << admixtureAge << "\n";
    std::cout << "nd_a    " << nd_a << "\t" << nd_a->getAge() << "\n";
    std::cout << "nd_a->p " << &nd_a->getParent() << "\t" << nd_a->getParent().getAge() << "\n";
    std::cout << "nd_b    " << nd_b << "\t" << nd_b->getAge() << "\n";
    std::cout << "nd_b->p " << &nd_b->getParent() << "\t" << nd_b->getParent().getAge() << "\n";
    */
     
    // store adjacent nodes to new parent node
    storedAdmixtureParentChild = nd_a;
    storedAdmixtureParentParent = &nd_a->getParent();
//    storedAdmixtureParentChild = path_a.back();
//    storedAdmixtureParentParent = nd_a;

    
    // insert admixtureParent into graph
    storedAdmixtureParent = new AdmixtureNode((int)tau.getNumberOfNodes());
    storedAdmixtureParent->setAge(admixtureAge);
    storedAdmixtureParent->setParent(storedAdmixtureParentParent);
    storedAdmixtureParent->addChild(storedAdmixtureParentChild);
    storedAdmixtureParent->setOutgroup(storedAdmixtureParentChild->isOutgroup());
    storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
    storedAdmixtureParentParent->addChild(storedAdmixtureParent);
    storedAdmixtureParentChild->setParent(storedAdmixtureParent);
    tau.pushAdmixtureNode(storedAdmixtureParent);
    
    // store adjacent nodes to new child node
    storedAdmixtureChildChild = nd_b;
    storedAdmixtureChildParent = &nd_b->getParent();
//    storedAdmixtureChildChild = path_b.back();
//    storedAdmixtureChildParent = nd_b;

    
    // insert admixtureChild into graph
    storedAdmixtureChild = new AdmixtureNode((int)tau.getNumberOfNodes());
    storedAdmixtureChild->setAge(admixtureAge);
    storedAdmixtureChild->setParent(storedAdmixtureChildParent);
    storedAdmixtureChild->addChild(storedAdmixtureChildChild);
    storedAdmixtureChild->setOutgroup(storedAdmixtureChildChild->isOutgroup());
    storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
    storedAdmixtureChildParent->addChild(storedAdmixtureChild);
    storedAdmixtureChildChild->setParent(storedAdmixtureChild);
    storedAdmixtureChild->setWeight(admixtureWeight);
    tau.pushAdmixtureNode(storedAdmixtureChild);
    
    // create admixture edge
    storedAdmixtureChild->setAdmixtureParent(storedAdmixtureParent);
    storedAdmixtureParent->setAdmixtureChild(storedAdmixtureChild);
    
    //std::cout << "\nafter AEA proposal\n";
    //tau.checkAllEdgesRecursively(&tau.getRoot());
    
    // prior * proposal ratio
    numEvents = (int)tau.getNumberOfAdmixtureChildren();
    double unitTreeLength = tau.getUnitTreeLength();
    double lnP = log(unitTreeLength);
   // double lnP = log( (rate->getValue() * unitTreeLength) / (numEvents) );
    //  double lnJ = 2 * log(1 - w);
    //    double lnP = log( (rate->getValue() * treeLength) / (numEvents) );

    admixtureCount->setValue(new int(numEvents));
    
    
    //std::cout << "add_RW\t" << lnP << "\t" << lnW << "\t" << log(fwdProposal) << ";\n";
    //std::cout << "a " << storedAdmixtureChild->getAge() << "\n";
    //std::cout << "w " << storedAdmixtureChild->getWeight() << "\n";
    //std::cout << rate->getValue() << "\t" << unitTreeLength << "\t" << numEvents << "\n";
    
    // bombs away
    return lnP - lnW;// - log(fwdProposal);
    
}


void AdmixtureEdgeAddResidualWeights::rejectSimpleMove( void ) {
    
    if (!failed)
    {
        //std::cout << "add_RW reject\n";
        
        // revert edges
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
        
        // remove nodes from graph structure
        AdmixtureTree& tau = variable->getValue();
        
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        
        admixtureCount->setValue(new int(numEvents-1));
        //std::cout << "reject\n";
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
    Move::swapNode(oldN, newN);
    
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
        residuals = static_cast<DeterministicNode<std::vector<double> >* >(newN);
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

bool AdmixtureEdgeAddResidualWeights::isActive(int g) const {
    
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
    //admixtureCount->touch();
    probRatio = admixtureCount->getLnProbabilityRatio();
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
