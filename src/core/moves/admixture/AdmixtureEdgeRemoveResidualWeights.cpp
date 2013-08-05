//
//  AdmixtureEdgeRemoveResidualWeights.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeRemoveResidualWeights.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeRemoveResidualWeights::AdmixtureEdgeRemoveResidualWeights(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode<std::vector<double> >* res, int ag, double w) : Move( v, w), variable( v ), rate(r), admixtureCount(ac), residuals(res), changed(false), failed(false), activeGen(ag) {
    
    nodes.insert(rate);
    nodes.insert(residuals);
    nodes.insert(admixtureCount);
    //nodes.insert(dt);
}



/* Clone object */
AdmixtureEdgeRemoveResidualWeights* AdmixtureEdgeRemoveResidualWeights::clone( void ) const {
    
    return new AdmixtureEdgeRemoveResidualWeights( *this );
}



const std::string& AdmixtureEdgeRemoveResidualWeights::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeRemoveResidualWeights";
    
    return name;
}

void AdmixtureEdgeRemoveResidualWeights::findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p)
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
double AdmixtureEdgeRemoveResidualWeights::performSimpleMove( void ) {
    
    std::cout << "\nAdmix Node Rem RW\n";
    
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
        failed = false;
        
        // sample a random admixture parent node
        double u = rng->uniform01();
        size_t index = std::floor(admixtureParents.size() * u);
        AdmixtureNode* admixtureParent = admixtureParents[index];
        AdmixtureNode* admixtureChild = &admixtureParent->getAdmixtureChild();
        double w = admixtureChild->getWeight();
        
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
        
        /*
        std::cout << storedAdmixtureChild << " " << &storedAdmixtureChild->getParent() << " " << &storedAdmixtureChild->getChild(0) << "\n";
        std::cout << storedAdmixtureChildParent << " " << &storedAdmixtureChildParent->getChild(0) << "\n";
        std::cout << storedAdmixtureChildChild << " " << &storedAdmixtureChildChild->getParent() << "\n";
*/
        
        // get set of tips descendant from AC and AP
        //std::cout << "tips_ap\n";
        std::set<AdmixtureNode*> tips_ap;
        findDescendantTips(tips_ap, storedAdmixtureParent);
        //std::cout << "tips_ac\n";
        std::set<AdmixtureNode*> tips_ac;
        findDescendantTips(tips_ac, storedAdmixtureChild);

        
        
        // remove nodes from admixtureTree vector
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        
        // get sum of positive residuals for each taxon against all other taxa
        storedResiduals = residuals->getValue();
        size_t numTaxa = tau.getNumberOfTips();
        double bwdProposal = 0.0;
        std::vector<double> residualWeights_a(numTaxa,0.0);
        double sumResidualWeights_a = 0.0;
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                double r = storedResiduals[i * numTaxa + j];
                if (r > 0.0 && i != j)
                {
                    residualWeights_a[i] = r;
                    sumResidualWeights_a += r;
                }
            }
        }
//        bwdProposal *= (residualWeights_a[k_a] / sumResidualWeights_a);
        
        //std::cout << tips_ap.size() << "\n";
        
        
        // get bwd proposal probs given residuals probs
        double t = storedAdmixtureParentParent->getAge() - storedAdmixtureParentChild->getAge();
        for (std::set<AdmixtureNode*>::iterator it1 = tips_ap.begin(); it1 != tips_ap.end(); it1++)
        {
            size_t k_a = (*it1)->getIndex();
            double p_a = residualWeights_a[k_a] / sumResidualWeights_a;
            double t_a = storedAdmixtureParentParent->getAge() - (*it1)->getAge();
            
            // get sum of positive residuals for each taxon wrt taxon A
            std::vector<double> residualWeights_b(numTaxa,0.0);
            double sumResidualWeights_b = 0.0;
            for (size_t i = 0; i < numTaxa; i++)
            {
                double r = storedResiduals[k_a * numTaxa + i];
                if (r > 0.0 && k_a != i)
                {
                    residualWeights_b[i] = r;
                    sumResidualWeights_b += r;
                }
            }
        
            double p_b = 0.0;
            for (std::set<AdmixtureNode*>::iterator it2 = tips_ac.begin(); it2 != tips_ac.end(); it2++)
            {
                p_b += residualWeights_b[(*it2)->getIndex()] / sumResidualWeights_b;
//                bwdProposal += p_a * (residualWeights_b[(*it2)->getIndex()] / sumResidualWeights_b) * (t / t_a);
            }
            
            bwdProposal += p_a * (t / t_a) * p_b;
        }
     
        
        
        // The prior and proposal densities are equal for the edge weight, thus cancelling.
        
        double lnW = 0.0;
        
        // bwd proposal
        // double a = 1.0;
        // double b = 3.0;
        // lnW = RbStatistics::Beta::lnPdf(a,b,storedAdmixtureChild->getWeight());

        // prior
        // double prior_a = 1.0;
        // double prior_b = 3.0;
        // lnW += RbStatistics::Beta::lnPdf(prior_a, prior_b, storedAdmixtureChild->getWeight());
        
        
        // prior * proposal ratio
        numEvents = (int)tau.getNumberOfAdmixtureChildren();
        double unitTreeLength = tau.getUnitTreeLength();
//        double lnP = log( (numEvents+1) / (rate->getValue() * unitTreeLength));
        double lnP = -log(unitTreeLength);
        //double lnJ = -2 * log(1 - w); // inverse of admixture edge add jacobian
        
        // quick fix -- should inherit from Move instead of SimpleMove
        // lnP -= variable->getLnProbabilityRatio();
        
        std::cout << "remove_RW\t" << lnP << "\t" << rate->getValue() << "\t" << unitTreeLength << "\t" << numEvents << "\n";
        //std::cout << bwdProposal << "\n";

         admixtureCount->setValue(new int(numEvents));
        
        return lnP + lnW;// + log(bwdProposal); // + lnJ;
        //return lnP;
    }
}


void AdmixtureEdgeRemoveResidualWeights::rejectSimpleMove( void ) {
    
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
        
        admixtureCount->setValue(new int(numEvents+1));
        
        
        tau.pushAdmixtureNode(storedAdmixtureParent);
        tau.pushAdmixtureNode(storedAdmixtureChild);
        
        std::cout << "reject\n";
        
    }
}

void AdmixtureEdgeRemoveResidualWeights::acceptSimpleMove(void) {
    
    if (!failed)
    {
        AdmixtureNode* p = NULL;
        storedAdmixtureParent->setParent(p);
        storedAdmixtureChild->setParent(p);
        delete storedAdmixtureChild;
        delete storedAdmixtureParent;
        
        std::cout << "accept\n";
    }
}


void AdmixtureEdgeRemoveResidualWeights::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
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
//    else if (oldN == delayTimer)
 //   {
 //       delayTimer = static_cast<ConstantNode<int>* >(newN);
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


void AdmixtureEdgeRemoveResidualWeights::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeRemoveResidualWeights::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeRemoveResidualWeights::isActive(int g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeRemoveResidualWeights::performMove( double &probRatio ) {
    
    
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
    //rate->touch();
    admixtureCount->touch();
    
    probRatio = admixtureCount->getLnProbabilityRatio();
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
