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

AdmixtureEdgeRemoveResidualWeights::AdmixtureEdgeRemoveResidualWeights(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, double d, int ag, double w) : MoveOld( v, w), variable( v ), rate(r), admixtureCount(ac), residuals(res), changed(false), failed(false), activeGen(ag), delta(d) {
    
    //nodes.insert(rate);
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
    
    //std::cout << "\nAdmix Node Rem RW\n";
    
    // Get random number generator
    failed = false;
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
        
        
        /*
         
        // get set of tips descendant from AC and AP
        //std::cout << "tips_ap\n";
        std::set<AdmixtureNode*> tips_ap;
        findDescendantTips(tips_ap, storedAdmixtureParent);
        //std::cout << "tips_ac\n";
        std::set<AdmixtureNode*> tips_ac;
        findDescendantTips(tips_ac, storedAdmixtureChild);
*/
        
        
        // remove nodes from admixtureTree vector
        tau.eraseAdmixtureNode(storedAdmixtureParent);
        tau.eraseAdmixtureNode(storedAdmixtureChild);
        
        // get sum of positive residuals for each taxon against all other taxa
        storedResiduals = residuals->getValue();
        size_t numTaxa = tau.getNumberOfTips();
        double bwdProposal = 0.0;
        
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
        
        size_t k_a = admixtureParent->getIndex();
        bwdProposal *= (residualWeights_a[k_a] / sumResidualWeights_a);
        
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
        
        size_t k_b = admixtureChild->getIndex();
        bwdProposal *= (residualWeights_b[k_b] / sumResidualWeights_b);
        
        // prior & propsal admixture weights cancel...
        //double lnW = 0.0;
        
        // prior * proposal ratio
        numEvents = (int)tau.getNumberOfAdmixtureChildren();
        admixtureCount->setValue(new int(numEvents));

        return 0.0;
        //double lnFwdProposal = -log(numEvents+1);
        //return lnW + log(bwdProposal) - lnFwdProposal;
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
        
        //std::cout << "reject\n";
        
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
    }
}


void AdmixtureEdgeRemoveResidualWeights::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "AER\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
    /*
    else if (oldN == rate)
    {
        //std::cout << "AER\trate\n";
        rate = static_cast<StochasticNode<double>* >(newN);
    }
    */
    else if (oldN == residuals)
    {
        residuals = static_cast<DeterministicNode< RbVector<double> >* >(newN);
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

bool AdmixtureEdgeRemoveResidualWeights::isActive(unsigned long g) const {
    
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
    
    probRatio = variable->getLnProbabilityRatio();
    probRatio += admixtureCount->getLnProbabilityRatio();
    //std::cout << probRatio << "\n";
    
    ;
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << theNode->getName() << "\t" << probRatio << "\n";
        }
    }
    
    //std::cout << hr << "\n";
    
    return hr;
}
