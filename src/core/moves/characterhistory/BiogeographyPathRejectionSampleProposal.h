//
//  BiogeographyPathRejectionSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 6/12/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographyPathRejectionSampleProposal__
#define __rb_mlandis__BiogeographyPathRejectionSampleProposal__

#include "BiogeographicTreeHistoryCtmc.h"
#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "PathRejectionSampleProposal.h"
#include "Proposal.h"
#include "RateMap_Biogeography.h"
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
    class BiogeographyPathRejectionSampleProposal : public PathRejectionSampleProposal<charType,treeType> {
        
    public:
        BiogeographyPathRejectionSampleProposal( StochasticNode<AbstractDiscreteCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, double l, TopologyNode* nd=NULL);   //!<  constructor
//        BiogeographyPathRejectionSampleProposal( const BiogeographyPathRejectionSampleProposal& p );
        
        virtual void                                prepareProposal(void);
        virtual double                              computeLnProposal(const TopologyNode& nd, const BranchHistory& bh);
//        virtual double                              computeLnProposal_test(const TopologyNode& nd, const BranchHistory& bh);
        virtual double                              doProposal(void);                                                                   //!< Perform proposal
        virtual const std::string&                  getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        
    protected:
        
        unsigned                                    getEpochIndex(double age) const;
        std::vector<double>                         epochs;
        bool                                        useAreaAdjacency;
        bool                                        useTail;
        
    };
}



template<class charType, class treeType>
RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::BiogeographyPathRejectionSampleProposal( StochasticNode<AbstractDiscreteCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, double l, TopologyNode* nd) : PathRejectionSampleProposal<charType,treeType> (n, t, q, l, nd)
{

    const RateMap_Biogeography& rm = static_cast<RateMap_Biogeography&>(this->qmap->getValue());
    epochs = rm.getEpochs();
    useAreaAdjacency = false;
    
    const BiogeographicTreeHistoryCtmc<charType,treeType>& p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>& >(this->ctmc->getDistribution());
    useTail = p.getUseTail();
}

//template<class charType, class treeType>
//RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::BiogeographyPathRejectionSampleProposal(const BiogeographyPathRejectionSampleProposal& m) : PathRejectionSampleProposal<charType,treeType> (m.ctmc, m.tau, m.qmap, m.lambda, m.node)
//{
//    if (this != &m)
//    {
//        epochs = m.epochs;
//        useAreaAdjacency = m.useAreaAdjacency;
//    }
//}



template<class charType, class treeType>
double RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::computeLnProposal(const TopologyNode& nd, const BranchHistory& bh)
{
    
    double lnP = 0.0;

    std::vector<CharacterEvent*> currState = bh.getParentCharacters();
    unsigned counts[this->numStates];
    for (size_t i = 0; i < this->numStates; i++)
        counts[i] = 0;
    this->fillStateCounts(currState, counts);
    
    if (nd.isRoot() && !useTail)
        return 0.0;
  
    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh.getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    
    double branchLength = nd.getBranchLength();
    double currAge = (nd.isRoot() ? nd.getAge()*5 : nd.getParent().getAge());
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
        CharacterEvent* evt = *it_h;
        double tr = rm.getSiteRate(nd, currState[ evt->getIndex() ], evt, currAge);
        double sr = rm.getSumOfRates(nd, currState, counts, currAge);
        lnP += -(sr * da) + log(tr);
        
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
double RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::doProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType,treeType>& p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>& >(this->ctmc->getDistribution());
    this->proposedHistory.clear();
    
    // get model parameters
    const RateMap& rm = this->qmap->getValue();
    double branchLength = this->node->getBranchLength();
    if (this->node->isRoot())
    {
        if (!useTail)
            return 0.0;
        else
            branchLength = this->node->getAge() * 5; //1e10;//2*tree.getTreeLength();
    }
    
    // get epoch variables
//    double startAge = this->node->getParent().getAge();
    double startAge = (this->node->isRoot() ? this->node->getAge() * 5 : this->node->getParent().getAge());
//    double endAge = this->node->getAge();
    
    // clear characters
    BranchHistory* bh = &p.getHistory(*(this->node));
    
    // reject sample path history
    bool failed = false;
    std::vector<CharacterEvent*> parentVector = bh->getParentCharacters();
    std::vector<CharacterEvent*> childVector =  bh->getChildCharacters();
    for (std::set<size_t>::iterator it = this->siteIndexSet.begin(); it != this->siteIndexSet.end(); it++)
    {
        int attempts = 0;
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
                double r = rm.getSiteRate( *(this->node), currState, nextState, (int)(*it), currAge);
     
                
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
                
                if (attempts > 100)
                    failed = true;
                
                if (t >= 1.0 && currState != endState && !failed)
                {
                    for (std::set<CharacterEvent*>::iterator it_h = tmpHistory.begin(); it_h != tmpHistory.end(); it_h++)
                        delete *it_h;
                    attempts++;
                }

            }
            while(t < 1.0);
            
        }
        while (currState != endState && failed == false);

        
        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
        {
            this->proposedHistory.insert(*it);
        }
    }
    
    // assign values back to model for likelihood
    bh->updateHistory(this->proposedHistory, this->siteIndexSet);
    
    // auto-reject if too many failed rejection samplings
    if (failed)
        this->proposedLnProb = RbConstants::Double::neginf;
    
    // otherwise, proceed with MH
    else
        this->proposedLnProb = RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::computeLnProposal( *(this->node), *bh);
    
    return this->storedLnProb - this->proposedLnProb;
}







template<class charType, class treeType>
unsigned RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::getEpochIndex(double age) const
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
void RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::prepareProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType,treeType>& p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>& >(this->ctmc->getDistribution());
    const RateMap_Biogeography& rm = static_cast<RateMap_Biogeography&>(this->qmap->getValue());

    
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
    BranchHistory* bh = &p.getHistory(*(this->node));
    
    
    
    if (this->sampleSiteIndexSet)
    {
        this->siteIndexSet.clear();
        this->siteIndexSet.insert(GLOBAL_RNG->uniform01() * this->numCharacters); // at least one is inserted
        if (useAreaAdjacency)
        {
            const std::set<size_t>& s = rm.getRangeAndFrontierSet(*(this->node), bh, this->node->getAge() );
            for (std::set<size_t>::const_iterator s_it = s.begin(); s_it != s.end(); s_it++)
            {
                if (GLOBAL_RNG->uniform01() < this->lambda)
                {
                    this->siteIndexSet.insert(*s_it);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < this->numCharacters; i++)
            {
                if (GLOBAL_RNG->uniform01() < this->lambda)
                {
                    this->siteIndexSet.insert(i);
                }
            }
        }
    }
    this->sampleSiteIndexSet = true;
    
    
    
    // store history for events in siteIndexSet
    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh->getHistory();
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
    
    double x = RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::computeLnProposal( *(this->node), *bh);
    
    this->storedLnProb = x;
    
    
}

template<class charType, class treeType>
const std::string& RevBayesCore::BiogeographyPathRejectionSampleProposal<charType, treeType>::getProposalName( void ) const
{
    static std::string name = "BiogeographyPathRejectionSampleProposal";
    
    return name;
}

#endif /* defined(__rb_mlandis__BiogeographyPathRejectionSampleProposal__) */
