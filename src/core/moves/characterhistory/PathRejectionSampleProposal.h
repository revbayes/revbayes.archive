//
//  PathRejectionSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/7/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__PathRejectionSampleProposal__
#define __rb_mlandis__PathRejectionSampleProposal__

#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "HomologousDiscreteCharacterData.h"
#include "DistributionBinomial.h"
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
     * @author The RevBayes Development Core Team (Michael Landis)
     * @since 2009-09-08, version 1.0
     *
     */
    
    template<class charType>
    class PathRejectionSampleProposal : public Proposal {
        
    public:
        PathRejectionSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, StochasticNode<Tree>* t, DeterministicNode<RateMap> *q, double l, TopologyNode* nd=NULL, bool useTail=false);   //!<  constructor
        
        // Basic utility functions
        void                            assignNode(TopologyNode* nd);
        void                            assignSiteIndexSet(const std::set<size_t>& s);
        virtual double                  computeLnProposal(const TopologyNode& nd, const BranchHistory& bh);
        void                            cleanProposal(void);
        PathRejectionSampleProposal*    clone(void) const;                                                                  //!< Clone object
        double                          doProposal(void);                                                                   //!< Perform proposal
        virtual const std::string&      getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        void                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                            swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
        void                            fillStateCounts(std::vector<CharacterEvent*> s, unsigned* counts);
        double                          getBranchRate(size_t index) const;
        
        // parameters
        StochasticNode<AbstractHomologousDiscreteCharacterData>*  ctmc;
        StochasticNode<Tree>*                   tau;
        DeterministicNode<RateMap>*             qmap;
        
        //BranchHistory*                          storedValue;
        std::multiset<CharacterEvent*,CharacterEventCompare> storedHistory;
        std::multiset<CharacterEvent*,CharacterEventCompare> proposedHistory;
        
        TopologyNode*                           node;
        std::set<size_t>                        siteIndexSet;
        
        double                                  storedLnProb;
        double                                  proposedLnProb;
        
        size_t                                  numNodes;
        size_t                                  numCharacters;
        size_t                                  numStates;
        
        double                                  lambda;
        bool                                    fixNodeIndex;
        bool                                    sampleNodeIndex;
        bool                                    sampleSiteIndexSet;
        bool                                    useTail;
        
        bool                                    printDebug;
        
    };
    
}



/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class charType>
RevBayesCore::PathRejectionSampleProposal<charType>::PathRejectionSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, StochasticNode<Tree> *t, DeterministicNode<RateMap>* q, double l, TopologyNode* nd, bool ut) : Proposal(),
    ctmc(n),
    tau(t),
    qmap(q),
    node(nd),
    lambda(l),
    sampleNodeIndex(true),
    sampleSiteIndexSet(true),
    useTail(ut)
{
    addNode(ctmc);
    addNode(tau);
    addNode(qmap);
    
    numNodes = t->getValue().getNumberOfNodes();
    numCharacters = n->getValue().getNumberOfCharacters();
    numStates = q->getValue().getNumberOfStates();
 
    printDebug = false;
    
    fixNodeIndex = (node != NULL);
}

template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::cleanProposal( void )
{
    
    if (node->isRoot() && !useTail)
        return;
    
    AbstractTreeHistoryCtmc<charType>& p = static_cast< AbstractTreeHistoryCtmc<charType>& >(ctmc->getDistribution());
    BranchHistory* bh = &p.getHistory(*node);

    if (printDebug) { std::cout << node->getIndex() << " ACCEPT\n"; bh->print(); std::cout << "------\n"; }
    
    // delete old events
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = storedHistory.begin(); it_h != storedHistory.end(); it_h++)
    {
//         std::cout << "clean deleting" <<  *it_h << "\n";
        delete *it_h;
    }
    storedHistory.clear();
    proposedHistory.clear();
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
template<class charType>
RevBayesCore::PathRejectionSampleProposal<charType>* RevBayesCore::PathRejectionSampleProposal<charType>::clone( void ) const
{
    return new PathRejectionSampleProposal( *this );
}

template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::assignNode(TopologyNode* nd)
{
    node = nd;
    sampleNodeIndex = false;
}

template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::assignSiteIndexSet(const std::set<size_t>& s)
{
    siteIndexSet = s;
    sampleSiteIndexSet = false;
}


template<class charType>
double RevBayesCore::PathRejectionSampleProposal<charType>::computeLnProposal(const TopologyNode& nd, const BranchHistory& bh)
{
    double lnP = 0.0;

    std::vector<CharacterEvent*> currState = bh.getParentCharacters();
    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh.getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;

    unsigned counts[numStates];
    for (size_t i = 0; i < numStates; i++)
        counts[i] = 0;
    fillStateCounts(currState, counts);

    double branchLength = nd.getBranchLength();
    if (nd.isRoot() && useTail)
    {
        branchLength = nd.getAge() * 5;
    }
    else
        return 0.0;

    double currAge = 0.0;
    if (nd.isRoot())
        currAge = nd.getAge() + branchLength;
    else
        currAge = nd.getParent().getAge();
    
    // get sampling ratemap
    const RateMap& rm_tmp = qmap->getValue();
    const RateMapUsingMatrix& rm = static_cast<const RateMapUsingMatrix &>( rm_tmp );
    
    // stepwise events
    double t = 0.0;
    double dt = 0.0;
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        // next event time
        double idx = (*it_h)->getCharacterIndex();
        dt = (*it_h)->getTime() - t;
    
//        double tr = rm.getRate(nd, currState, *it_h, counts, currAge);
        double tr = rm.getSiteRate( currState[ (*it_h)->getCharacterIndex() ], *it_h, getBranchRate(nd.getIndex()), currAge);
        double sr = rm.getSumOfRates( currState, counts, getBranchRate(nd.getIndex()), currAge);
        
        // lnP for stepwise events for p(x->y)
        lnP += log(tr) - sr * dt * branchLength;
        
        // update counts
        counts[ currState[idx]->getState() ] -= 1;
        counts[ (*it_h)->getState() ] += 1;
        
        // update state
        currState[idx] = *it_h;
        t += dt;
        currAge -= dt * branchLength;
    }

    // lnL for final non-event
    double sr = rm.getSumOfRates(nd, currState, counts, currAge);
    lnP += -sr * (1.0 - t) * branchLength;
    
    return lnP;
}

template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::fillStateCounts(std::vector<CharacterEvent*> s, unsigned int *counts)
{
    for (size_t i = 0; i < s.size(); i++)
        counts[ s[i]->getState() ] += 1;
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
template<class charType>
const std::string& RevBayesCore::PathRejectionSampleProposal<charType>::getProposalName( void ) const
{
    static std::string name = "PathRejectionSampleProposal";
    
    return name;
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
template<class charType>
double RevBayesCore::PathRejectionSampleProposal<charType>::doProposal( void )
{
    AbstractTreeHistoryCtmc<charType>& p = static_cast< AbstractTreeHistoryCtmc<charType>& >(ctmc->getDistribution());
    proposedHistory.clear();
    
    // get model parameters
//    const treeType& tree = this->tau->getValue();
    double branchLength = node->getBranchLength();
    if (node->isRoot() && useTail)
    {
        branchLength = node->getAge() * 5;
    }
    else if (node->isRoot())
        return 0.0;
    
    const RateMap& rm = qmap->getValue();

    // clear characters
    BranchHistory* bh = &p.getHistory(*node);
    if (printDebug) { std::cout << "BEFORE PATH PROPOSAL\n"; bh->print(); }

    // reject sample path history
    std::vector<CharacterEvent*> parentVector = bh->getParentCharacters();
    std::vector<CharacterEvent*> childVector =  bh->getChildCharacters();
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        std::set<CharacterEvent*> tmpHistory;
        size_t currState = parentVector[*it]->getState();
        size_t endState = childVector[*it]->getState();
        do
        {
            // delete previously rejected events
            tmpHistory.clear();
                      
            // proceed with rejection sampling
            currState = parentVector[*it]->getState();
            double t = 0.0;
//            double sr = 0.0;
            
            // repeated rejection sampling
            do
            {
                double r = 0.0;
                size_t nextState = 0;
                if (numStates == 2)
                {
                    nextState = (currState == 1 ? 0 : 1);
                    r = rm.getSiteRate(currState, nextState, getBranchRate(node->getIndex()));
                }
                
                else
                {
                    std::vector<double> rates(numStates,0.0);
                    for (unsigned i = 0; i < numStates; i++)
                    {
                        if (i == currState)
                            continue;
                        double v = rm.getSiteRate(currState, i, getBranchRate(node->getIndex()));
                        rates[i] = v;
                        r += v;
                    }
                    double u = GLOBAL_RNG->uniform01() * r;
                    for (unsigned i = 0; i < numStates; i++)
                    {
                        u -= rates[i];
                        if (u <= 0.0)
                        {
                            nextState = i;
                            break;
                        }
                    }
                }
                
                // force valid time if event needed
                if (t == 0.0 && currState != endState)
                {
                    double u = GLOBAL_RNG->uniform01();
                    double truncate = 1.0;
                    t += -log(1 - u * (1 - exp(-truncate * r * branchLength))) / (r * branchLength);
                }
                // standard sampling otherwise
                else
                    t += RbStatistics::Exponential::rv(r * branchLength, *GLOBAL_RNG);
                
                if (t < 1.0)
                {
                    currState = nextState;
                    CharacterEvent* evt = new CharacterEvent(*it, nextState, t);
                    tmpHistory.insert(evt);
                }
                else if (currState != endState)
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
            proposedHistory.insert(*it);
        }
    }
    
//    std::cout << proposedHistory.size() << "\n";
    // assign values back to model for likelihood
    bh->updateHistory(proposedHistory, siteIndexSet);
    
    // return hastings ratio
    proposedLnProb = computeLnProposal(*node, *bh);
    
    return storedLnProb - proposedLnProb;
}


/**
 *
 */
template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::prepareProposal( void )
{
    AbstractTreeHistoryCtmc<charType>& p = static_cast< AbstractTreeHistoryCtmc<charType>& >(ctmc->getDistribution());
    
    storedHistory.clear();
    proposedHistory.clear();
    
    storedLnProb = 0.0;
    proposedLnProb = 0.0;
    
    if (sampleNodeIndex && !fixNodeIndex)
    {
        size_t nodeIndex = GLOBAL_RNG->uniform01() * numNodes;
        node = &tau->getValue().getNode(nodeIndex);
    }
    sampleNodeIndex = true;
  
    if (sampleSiteIndexSet)
    {
        siteIndexSet.clear();
        siteIndexSet.insert(GLOBAL_RNG->uniform01() * numCharacters); // at least one is inserted
        for (size_t i = 0; i < numCharacters; i++)
        {
            if (GLOBAL_RNG->uniform01() < lambda)
            {
                siteIndexSet.insert(i);
            }
        }
    }
    sampleSiteIndexSet = true;
    
//    std::cout << "Sites: ";
//    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
//        std::cout << *it << " ";
//    std::cout << "\n";
    
//    if (node->isRoot())
//    {
//        return;
//    }
    
    BranchHistory* bh = &p.getHistory(*node);
    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh->getHistory();

    // store history for events in siteIndexSet
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        if (siteIndexSet.find( (*it_h)->getCharacterIndex() ) != siteIndexSet.end())
        {
            storedHistory.insert(*it_h);
        }
    }
  
    // flag node as dirty
    p.fireTreeChangeEvent(*node);
    
    double x = computeLnProposal(*node, *bh);
    
    storedLnProb = x;
    

}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the ctmc/DAG-node to its original value.
 */
template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::undoProposal( void )
{
    AbstractTreeHistoryCtmc<charType>* p = static_cast< AbstractTreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
    
    // swap current value and stored value
    BranchHistory* bh = &p->getHistory(*node);
    bh->updateHistory(storedHistory, siteIndexSet);
    
    // delete new events
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = proposedHistory.begin(); it_h != proposedHistory.end(); it_h++)
    {
        delete *it_h;
    }
    storedHistory.clear();
    proposedHistory.clear();
}


/**
 * Swap the current ctmc for a new one.
 *
 * \param[in]     oldN     The old ctmc that needs to be replaced.
 * \param[in]     newN     The new ctmc.
 */
template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    if (oldN == ctmc)
    {
        ctmc = static_cast<StochasticNode<AbstractHomologousDiscreteCharacterData>* >(newN) ;
    }
    else if (oldN == tau)
    {
        tau = static_cast<StochasticNode<Tree>* >(newN);
    }
    else if (oldN == qmap)
    {
        qmap = static_cast<DeterministicNode<RateMap>* >(newN);
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 */
template<class charType>
void RevBayesCore::PathRejectionSampleProposal<charType>::tune( double rate )
{
    ; // do nothing
}

#endif /* defined(__rb_mlandis__PathRejectionSampleProposal__) */
