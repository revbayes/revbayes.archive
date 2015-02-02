//
//  AdmixtureEdgeMultiRemove.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 1/13/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeMultiRemove.h"
#include "DistributionBeta.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeMultiRemove::AdmixtureEdgeMultiRemove(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, double d, double p, int ag, double w) : MoveOld( v, w), variable( v ), rate(r), admixtureCount(ac), residuals(res), changed(false), failed(false), activeGen(ag), delta(d), pRemove(p) {
    
    //nodes.insert(rate);
    nodes.insert(residuals);
    nodes.insert(admixtureCount);
}



/* Clone object */
AdmixtureEdgeMultiRemove* AdmixtureEdgeMultiRemove::clone( void ) const {
    
    return new AdmixtureEdgeMultiRemove( *this );
}



const std::string& AdmixtureEdgeMultiRemove::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeMultiRemove";
    
    return name;
}

void AdmixtureEdgeMultiRemove::findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p)
{
    if (p->getNumberOfChildren() == 0)
    {
        s.insert(p);
    }
    else
    {
        for (size_t i = 0; i < p->getNumberOfChildren(); i++)
            findDescendantTips(s, &p->getChild(i));
    }
}

/** Perform the move */
double AdmixtureEdgeMultiRemove::performSimpleMove( void ) {
    
    //std::cout << "\nAdmix Node Rem RW\n";
    storedAdmixtureEdges.clear();
    
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
        std::set<size_t> removeIdx;
        removeIdx.insert(size_t(rng->uniform01()*admixtureParents.size()));
        for (size_t i = 0; i < admixtureParents.size(); i++)
            if (rng->uniform01() < pRemove)
                removeIdx.insert(i);
        
        //std::cout << "save\n";
        for (std::set<size_t>::iterator it = removeIdx.begin(); it != removeIdx.end(); it++)
        {
            AdmixtureNode* p = admixtureParents[*it];
            AdmixtureNode* c = &p->getAdmixtureChild();
            AdmixtureNode* pc = &p->getChild(0);
            AdmixtureNode* cc = &c->getChild(0);
            AdmixtureEdgePosition ap(p, c, pc, cc, c->getAge(), c->getWeight());
//            std::cout << p << "\t" << c << "\t" << pc << "\t" << cc << "\n";
//            std::cout << ap.admixtureParent << "\t" << ap.admixtureChild << "\n";
            
            storedAdmixtureEdges.push_front(ap);
            
            tau.removeAdmixtureEdge(p);
        
            tau.eraseAdmixtureNode(p);
            tau.eraseAdmixtureNode(c);
        }
        
        // update count
        numEvents = tau.getNumberOfAdmixtureChildren();
        admixtureCount->setValue((const int)numEvents);
        
        return 0.0;
    }
}


void AdmixtureEdgeMultiRemove::rejectSimpleMove( void ) {
    
    // we undo the proposal only if it didn't fail
    if ( !failed ) {

//        std::cout << "reject\n";
        // restore nodes to admixtureTree vector
        AdmixtureTree& tau = variable->getValue();
        
        for (std::list<AdmixtureEdgePosition>::iterator it = storedAdmixtureEdges.begin(); it != storedAdmixtureEdges.end(); it++)
        {
            AdmixtureNode* p = (*it).admixtureParent;
            AdmixtureNode* c = (*it).admixtureChild;
            AdmixtureNode* pc = (*it).admixtureParentChild;
            AdmixtureNode* cc = (*it).admixtureChildChild;
            
//            std::cout << p << "\t" << c << "\t" << pc << "\t" << cc << "\n";
            
            tau.addAdmixtureEdge(p,c,pc,cc);
            
            tau.pushAdmixtureNode(p);
            tau.pushAdmixtureNode(c);

        }
        
        numEvents = tau.getNumberOfAdmixtureChildren();
        admixtureCount->setValue((const int)numEvents);
        
        
        //std::cout << "reject\n";
        
    }
}

void AdmixtureEdgeMultiRemove::acceptSimpleMove(void) {
    
    if (!failed)
    {
//        std::cout << "accept\n";
        for (std::list<AdmixtureEdgePosition>::iterator it = storedAdmixtureEdges.begin(); it != storedAdmixtureEdges.end(); it++)
        {
            AdmixtureNode* p = NULL;
            (*it).admixtureChild->setParent(p);
            (*it).admixtureParent->setParent(p);
            
//            std::cout << (*it).admixtureChild << "\t" << (*it).admixtureParent << "\n";
            
            delete (*it).admixtureChild;
            delete (*it).admixtureParent;
        }
    }
}


void AdmixtureEdgeMultiRemove::swapNode(DagNode *oldN, DagNode *newN) {
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


void AdmixtureEdgeMultiRemove::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeMultiRemove::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeMultiRemove::isActive(unsigned long g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeMultiRemove::performMove( double &probRatio ) {
    
    
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
    
    return hr;
}
