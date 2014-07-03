//
//  EpochPathRejectionSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 6/12/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__EpochPathRejectionSampleProposal__
#define __rb_mlandis__EpochPathRejectionSampleProposal__

#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "PathRejectionSampleProposal.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMap.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "TopologyNode.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>
#include <set>
#include <string>

namespace RevBayesCore {
    
    /**
     * The scaling operator.
     *
     * A scaling proposal draws a random uniform number u ~ unif(-0.5,0.5)
     * and scales the current vale by a scaling factor
     * sf = exp( lambda * u )
     * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    
    template<class charType, class treeType>
    class EpochPathRejectionSampleProposal : public PathRejectionSampleProposal<charType,treeType> {
        
    public:
        EpochPathRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, double l, TopologyNode* nd=NULL);   //!<  constructor
        EpochPathRejectionSampleProposal( const EpochPathRejectionSampleProposal& p );
        
        virtual void                                prepareProposal(void);
        virtual double                              computeLnProposal(const TopologyNode& nd, const BranchHistory& bh);
//        virtual double                              computeLnProposal_test(const TopologyNode& nd, const BranchHistory& bh);
        virtual double                              doProposal(void);                                                                   //!< Perform proposal
//        virtual double                              doProposal_test(void);
//        virtual double                              undoProposal(void);
        
    protected:
        
        unsigned                                    getEpochIndex(double age) const;
        std::vector<double>                         epochs;
        
    };
}



template<class charType, class treeType>
RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::EpochPathRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, double l, TopologyNode* nd) : PathRejectionSampleProposal<charType,treeType> (n, t, q, l, nd)
{

    const RateMap_Biogeography& rm = static_cast<RateMap_Biogeography&>(this->qmap->getValue());
    epochs = rm.getEpochs();
}

template<class charType, class treeType>
RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::EpochPathRejectionSampleProposal(const EpochPathRejectionSampleProposal& m) : PathRejectionSampleProposal<charType,treeType> (m.n, m.t, m.q, m.l, m.nd)
{
    if (this != &m)
    {
        epochs = m.epochs;
    }
}



template<class charType, class treeType>
double RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::computeLnProposal(const TopologyNode& nd, const BranchHistory& bh)
{
    
    double lnP = 0.0;

    std::vector<CharacterEvent*> currState = bh.getParentCharacters();
    unsigned counts[this->numStates];
    for (size_t i = 0; i < this->numStates; i++)
        counts[i] = 0;
    this->fillStateCounts(currState, counts);
    
    if (nd.isRoot())
    {
        return 0.0;
    }
  
    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh.getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
        
    const treeType& tree = this->tau->getValue();
    double branchLength = nd.getBranchLength();
    double currAge = (nd.isRoot() ? 1e10 : nd.getParent().getAge());
    double startAge = currAge;
    double endAge = nd.getAge();
    const RateMap_Biogeography& rm = static_cast<const RateMap_Biogeography&>(this->qmap->getValue());
    
    // handle stratified/epoch models
    const std::vector<double>& epochs = rm.getEpochs();
    size_t epochIdx = getEpochIndex(currAge);
    double epochEndAge = epochs[epochIdx];
    
    // stepwise events
    double t = 0.0;
    double dt = 0.0;
    double da = 0.0;
    
    bool debug = !true;

    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        // next event time
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        da = dt * branchLength;
        
        // reject extinction
        unsigned s = (*it_h)->getState();
        
        // if epoch crossed, compute prob no events until boundary then advance epochIdx
        while (currAge - da < epochEndAge)
        {
            // waiting factor
            double sr = rm.getSumOfRates(nd, currState, counts, currAge);
            lnP += -sr * (currAge - epochEndAge);
            
            if (debug) std::cout << "epoch " << lnP << " " << currAge  << " "  << epochIdx << " da=" << currAge-epochEndAge << " ...  ... " << sr << "\n";
            
            // if before branch end, advance epoch
            if (endAge < epochEndAge)
            {
                epochIdx++;
                da -= (currAge - epochEndAge);
                currAge = epochEndAge;
                epochEndAge = epochs[epochIdx];
            }
            // otherwise, exit loop
            else
                break;
        }
        
        // lnL for stepwise events for p(x->y)
        double tr = rm.getSiteRate(nd, currState[ (*it_h)->getIndex() ], *it_h, currAge);
        double sr = rm.getSumOfRates(nd, currState, counts, currAge);
        lnP += -(sr * da) + log(tr);
        if (debug) std::cout << "event " << lnP << " " << currAge  << " "  << epochIdx << " da=" << da << " " << s << " " << tr << " " << sr << "\n";
        
        // update counts
        counts[currState[idx]->getState()] -= 1;
        counts[s] += 1;
        
        // update time and state
        currState[idx] = *it_h;
        t += dt;
        currAge -= da;
        
    }
    
    // lnL for final non-event
    while (epochEndAge > endAge)
    {
        // waiting factor
        double sr = rm.getSumOfRates(nd, currState, counts, currAge);
        lnP += -sr * (currAge - epochEndAge);
        if (debug) std::cout << "wait1 " << lnP << " " << currAge << " " << epochIdx << " da=" << currAge-epochEndAge << " ...  ... " << sr << "\n";
        
        
        // advance epoch
//        if (epochEndAge > endAge)
        {
            epochIdx++;
            currAge = epochEndAge;
            epochEndAge = epochs[epochIdx];
        }
    }
    double sr = rm.getSumOfRates(nd, currState, counts, currAge);
    lnP += -sr * (currAge - endAge);
    if (debug) std::cout << currAge << " " << endAge << "\n";
    if (debug) std::cout << "wait2 " << lnP << " " << currAge  << " " << epochIdx << " da=" << currAge-endAge << " ...  ... " << sr << "\n";

    return lnP;
}


/**
 * Perform the Proposal.
 *
 * A scaling Proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and scales the current vale by a scaling factor
 * sf = exp( lambda * u )
 * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
template<class charType, class treeType>
double RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::doProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType,treeType>& p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>& >(this->ctmc->getDistribution());
    this->proposedHistory.clear();
    
    // get model parameters
    const RateMap& rm = this->qmap->getValue();
    double branchLength = this->node->getBranchLength();
    if (this->node->isRoot())
    {
        return 0.0;
        branchLength = 1e10;//2*tree.getTreeLength();
    }
    
    // get epoch variables
    double startAge = this->node->getParent().getAge();
    double endAge = this->node->getAge();
    
    // clear characters
    BranchHistory* bh = &p.getHistory(*(this->node));
    
    // reject sample path history
    std::vector<CharacterEvent*> parentVector = bh->getParentCharacters();
    std::vector<CharacterEvent*> childVector =  bh->getChildCharacters();
    for (std::set<size_t>::iterator it = this->siteIndexSet.begin(); it != this->siteIndexSet.end(); it++)
    {
        std::set<CharacterEvent*> tmpHistory;
        unsigned int currState = parentVector[*it]->getState();
        unsigned int endState = childVector[*it]->getState();
        do
        {
            // delete previously rejected events
            tmpHistory.clear();
         
            // proceed with rejection sampling
            currState = parentVector[*it]->getState();
            
            double t = 0.0;

            double currAge = startAge;
            int epochIdx = getEpochIndex(startAge);
            double epochAge = epochs[epochIdx];
            
            // repeated rejection sampling
            do
            {
                unsigned int nextState = (currState == 1 ? 0 : 1);
                double r = rm.getSiteRate( *(this->node), currState, nextState, *it, currAge);
     
                
                // MJL: figure this out later...
                
//                // force valid time if event needed
//                if (t == 0.0 && currState != endState)
//                {
//                    double u = GLOBAL_RNG->uniform01();
//                    double truncate = 1.0;
//                    t += -log(1 - u * (1 - exp(-truncate * r * branchLength))) / (r * branchLength);
//                }
//                // standard sampling otherwise
//                else
                
                
                double dt = 0.0;
                if (r > 0.0)
                    dt = RbStatistics::Exponential::rv(r * branchLength, *GLOBAL_RNG);
                double da = dt * branchLength;
                
                // sample time from next interval (by memorylessness)
                if (currAge - da < epochAge || r == 0.0)
                {
                    t = (startAge - epochAge) / branchLength;
                    currAge = epochAge;
                    epochIdx++;
                    epochAge = epochs[epochIdx];
                    
                }
                else
                {
                    t += dt;
                    currAge -= da;
                    
                    if (t < 1.0)
                    {
                        currState = nextState;
                        CharacterEvent* evt = new CharacterEvent(*it, nextState, t);
                        tmpHistory.insert(evt);
                    }
                }
                
                if (t >= 1.0 && currState != endState)
                {
                    for (std::set<CharacterEvent*>::iterator it_h = tmpHistory.begin(); it_h != tmpHistory.end(); it_h++)
                        delete *it_h;
                }

            }
            while(t < 1.0);
        }
        while (currState != endState);

        
        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
        {
            this->proposedHistory.insert(*it);
        }
    }
    
    // assign values back to model for likelihood
    bh->updateHistory(this->proposedHistory, this->siteIndexSet);
    
//    bh->print();
    
    // return hastings ratio
    this->proposedLnProb = computeLnProposal( *(this->node), *bh);
    
//    std::cout << this->node->getAge() + this->node->getBranchLength() << " " << this->node->getAge() << "\n";
//    std::cout << " " << this->storedLnProb << " " << this->proposedLnProb << "\n";
    
    
    return this->storedLnProb - this->proposedLnProb;
}







template<class charType, class treeType>
unsigned RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::getEpochIndex(double age) const
{
    // epochs are ordered from oldest to youngest, typically over (-neginf, 0.0)
    unsigned index = 0;
    while (age <= epochs[index] && index < epochs.size() - 1)
    {
        index++;
    };
    return index;
}

template<class charType, class treeType>
void RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::prepareProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType,treeType>& p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>& >(this->ctmc->getDistribution());
    
    this->storedHistory.clear();
    this->proposedHistory.clear();
    
    this->storedLnProb = 0.0;
    this->proposedLnProb = 0.0;
    
    if (this->sampleNodeIndex && !this->fixNodeIndex)
    {
        size_t nodeIndex = GLOBAL_RNG->uniform01() * this->numNodes;
        this->node = &this->tau->getValue().getNode(nodeIndex);
    }
    this->sampleNodeIndex = true;
    
    if (this->sampleSiteIndexSet)
    {
        this->siteIndexSet.clear();
        this->siteIndexSet.insert(GLOBAL_RNG->uniform01() * this->numCharacters); // at least one is inserted
        for (size_t i = 0; i < this->numCharacters; i++)
        {
            if (GLOBAL_RNG->uniform01() < this->lambda)
            {
                this->siteIndexSet.insert(i);
            }
        }
    }
    this->sampleSiteIndexSet = true;
    
    //    std::cout << "Sites: ";
    //    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    //        std::cout << *it << " ";
    //    std::cout << "\n";
    
    if (this->node->isRoot())
    {
        return;
    }
    
    BranchHistory* bh = &p.getHistory(*(this->node));
    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh->getHistory();
    
    // store history for events in siteIndexSet
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        if (this->siteIndexSet.find( (*it_h)->getIndex() ) != this->siteIndexSet.end())
        {
            this->storedHistory.insert(*it_h);
        }
    }
    
    // flag node as dirty
    p.fireTreeChangeEvent(*(this->node));
    
    double x = computeLnProposal( *(this->node), *bh);
    
    this->storedLnProb = x;
    
    
}

//template<class charType, class treeType>
//double RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::computeLnProposal_test(const TopologyNode& nd, const BranchHistory& bh)
//{
//    double lnP = 0.0;
//    
//    std::vector<CharacterEvent*> currState = bh.getParentCharacters();
//    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh.getHistory();
//    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
//    
//    unsigned counts[this->numStates];
//    for (size_t i = 0; i < this->numStates; i++)
//        counts[i] = 0;
//    fillStateCounts(currState, counts);
//    
//    const treeType& tree = this->tau->getValue();
//    double branchLength = nd.getBranchLength();
//    if (nd.isRoot())
//    {
//        return 0.0;
//        branchLength = 1000.0;
//    }
//    double currAge = (!nd.isRoot() ? nd.getParent().getAge() : 10e200);
//    
//    // get sampling ratemap
//    const RateMap& rm = this->qmap->getValue();
//    
//    // stepwise events
//    double t = 0.0;
//    double dt = 0.0;
//    for (it_h = history.begin(); it_h != history.end(); it_h++)
//    {
//        // next event time
//        double idx = (*it_h)->getIndex();
//        dt = (*it_h)->getTime() - t;
//        
//        //        double tr = rm.getRate(nd, currState, *it_h, counts, currAge);
//        double tr = rm.getSiteRate(nd, currState[ (*it_h)->getIndex() ], *it_h, currAge);
//        double sr = rm.getSumOfRates(nd, currState, counts, currAge);
//        
//        // lnP for stepwise events for p(x->y)
//        lnP += log(tr) - sr * dt * branchLength;
//        
//        // update counts
//        counts[ currState[idx]->getState() ] -= 1;
//        counts[ (*it_h)->getState() ] += 1;
//        
//        // update state
//        currState[idx] = *it_h;
//        t += dt;
//        currAge -= dt * branchLength;
//    }
//    // lnL for final non-event
//    double sr = rm.getSumOfRates(nd, currState, counts, currAge);
//    lnP += -sr * (1.0 - t) * branchLength;
//    
//    if (nd.isRoot())
//        return 0.0;
//    
//    return lnP;
//}
//



///**
// * Perform the Proposal.
// *
// * A scaling Proposal draws a random uniform number u ~ unif(-0.5,0.5)
// * and scales the current vale by a scaling factor
// * sf = exp( lambda * u )
// * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
// *
// * \return The hastings ratio.
// */
//template<class charType, class treeType>
//double RevBayesCore::EpochPathRejectionSampleProposal<charType, treeType>::doProposal_test( void )
//{
//    AbstractTreeHistoryCtmc<charType,treeType>& p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>& >(this->ctmc->getDistribution());
//    this->proposedHistory.clear();
//    
//    // get model parameters
//    const treeType& tree = this->tau->getValue();
//    double branchLength = this->node->getBranchLength();
//    if (this->node->isRoot())
//    {
//        return 0.0;
//        branchLength = 1000.0;//2*tree.getTreeLength();
//    }
//    
//    const RateMap& rm = this->qmap->getValue();
//    
//    // clear characters
//    BranchHistory* bh = &p.getHistory( *(this->node) );
//    
//    // reject sample path history
//    std::vector<CharacterEvent*> parentVector = bh->getParentCharacters();
//    std::vector<CharacterEvent*> childVector =  bh->getChildCharacters();
//    for (std::set<size_t>::iterator it = this->siteIndexSet.begin(); it != this->siteIndexSet.end(); it++)
//    {
//        std::set<CharacterEvent*> tmpHistory;
//        unsigned int currState = parentVector[*it]->getState();
//        unsigned int endState = childVector[*it]->getState();
//        do
//        {
//            // delete previously rejected events
//            tmpHistory.clear();
//            
//            // proceed with rejection sampling
//            currState = parentVector[*it]->getState();
//            double t = 0.0;
//            
//            // repeated rejection sampling
//            do
//            {
//                unsigned int nextState = (currState == 1 ? 0 : 1);
//                double r = rm.getSiteRate(*(this->node), currState, nextState);
//                
//                // force valid time if event needed
//                //                if (t == 0.0 && currState != endState)
//                //                {
//                //                    double u = GLOBAL_RNG->uniform01();
//                //                    double truncate = 1.0;
//                //                    t += -log(1 - u * (1 - exp(-truncate * r * branchLength))) / (r * branchLength);
//                //                }
//                //                // standard sampling otherwise
//                //                else
//                t += RbStatistics::Exponential::rv(r * branchLength, *GLOBAL_RNG);
//                
//                if (t < 1.0)
//                {
//                    currState = nextState;
//                    CharacterEvent* evt = new CharacterEvent(*it, nextState, t);
//                    tmpHistory.insert(evt);
//                }
//                else if (currState != endState)
//                {
//                    for (std::set<CharacterEvent*>::iterator it_h = tmpHistory.begin(); it_h != tmpHistory.end(); it_h++)
//                        delete *it_h;
//                }
//            }
//            while(t < 1.0);
//        }
//        while (currState != endState);
//        
//        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
//        {
//            this->proposedHistory.insert(*it);
//        }
//    }
//    
//    // assign values back to model for likelihood
//    bh->updateHistory(this->proposedHistory, this->siteIndexSet);
//    
//    // return hastings ratio
//    this->proposedLnProb = computeLnProposal(*(this->node), *bh);
//    
//    return this->storedLnProb - this->proposedLnProb;
//}


#endif /* defined(__rb_mlandis__EpochPathRejectionSampleProposal__) */
