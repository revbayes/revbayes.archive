//
//  PathUniformizationSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/7/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__PathUniformizationSampleProposal__
#define __rb_mlandis__PathUniformizationSampleProposal__

#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "HomologousDiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "DistributionPoisson.h"
#include "MatrixReal.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMap.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "TransitionProbabilityMatrix.h"
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
    class PathUniformizationSampleProposal : public Proposal {
        
    public:
        PathUniformizationSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, TypedDagNode<Tree>* t, TypedDagNode<RateMap> *q, double l, TopologyNode* nd=NULL, bool useTail=false);   //!<  constructor
//        PathUniformizationSampleProposal( const PathUniformizationSampleProposal& p );
        
        // Basic utility functions
        void                            assignNode(TopologyNode* nd);
        void                            assignSiteIndexSet(const std::set<size_t>& s);
        virtual double                  computeLnProposal(const TopologyNode& nd, const BranchHistory& bh);
        void                            cleanProposal(void);
        PathUniformizationSampleProposal*    clone(void) const;                                                                  //!< Clone object
        double                          doProposal(void);                                                                   //!< Perform proposal
        const std::string&              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        void                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                            swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
        void                            fillStateCounts(std::vector<CharacterEvent*> s, unsigned* counts);
        
        // parameters
        StochasticNode<AbstractHomologousDiscreteCharacterData>*  ctmc;
        TypedDagNode<Tree>*                     tau;
        TypedDagNode<RateMap>*                  qmap;
                
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
        bool                                    sampleNodeIndex;
        bool                                    sampleSiteIndexSet;
        
        // sampling workspace
        int                                     maxNumJumps;
        bool                                    failed;
        TransitionProbabilityMatrix             tpCtmc;
        std::vector<MatrixReal>                 tpDtmc;
    };
    
}



/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class charType>
RevBayesCore::PathUniformizationSampleProposal<charType>::PathUniformizationSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, TypedDagNode<Tree> *t, TypedDagNode<RateMap>* q, double l, TopologyNode* nd, bool ut) : Proposal(),
ctmc(n),
tau(t),
qmap(q),
node(nd),
lambda(l),
sampleNodeIndex(true),
sampleSiteIndexSet(true),
maxNumJumps(50),
failed(false),
tpCtmc(q->getValue().getNumberOfStates()),
tpDtmc(maxNumJumps, MatrixReal(q->getValue().getNumberOfStates(), q->getValue().getNumberOfStates()))
{

    addNode( ctmc );
    addNode( tau );
    addNode( qmap );
    
    numNodes = t->getValue().getNumberOfNodes();
    numCharacters = n->getValue().getNumberOfCharacters();
    numStates = q->getValue().getNumberOfStates();
    
}

template<class charType>
void RevBayesCore::PathUniformizationSampleProposal<charType>::cleanProposal( void )
{
//    AbstractTreeHistoryCtmc<charType>& p = static_cast< AbstractTreeHistoryCtmc<charType>& >(ctmc->getDistribution());
//    BranchHistory* bh = &p.getHistory(*node);
    
    // delete old events
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = storedHistory.begin(); it_h != storedHistory.end(); it_h++)
    {
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
RevBayesCore::PathUniformizationSampleProposal<charType>* RevBayesCore::PathUniformizationSampleProposal<charType>::clone( void ) const
{
    return new PathUniformizationSampleProposal( *this );
}

template<class charType>
void RevBayesCore::PathUniformizationSampleProposal<charType>::assignNode(TopologyNode* nd)
{
    node = nd;
    sampleNodeIndex = false;
}

template<class charType>
void RevBayesCore::PathUniformizationSampleProposal<charType>::assignSiteIndexSet(const std::set<size_t>& s)
{
    siteIndexSet = s;
    sampleSiteIndexSet = false;
}


template<class charType>
double RevBayesCore::PathUniformizationSampleProposal<charType>::computeLnProposal(const TopologyNode& nd, const BranchHistory& bh)
{
    double lnP = 0.0;
    
    if (nd.isRoot())
        return 0.0;
 
    std::vector<CharacterEvent*> currState = bh.getParentCharacters();
    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh.getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    double branchLength = nd.getBranchLength();
    
     // get sampling RateMatrix
//    const RateMap& rm = qmap->getValue();
    const RateGenerator& rm = *(qmap->getValue().getHomogeneousRateMatrix());
    double clockRate = qmap->getValue().getHomogeneousClockRate();
    
    // stepwise events
    double t = 0.0;
    double dt = 0.0;
    double age = 0.0;
    
    // sum of rates away from parent sequence state
    double sr = 0.0;
    for (size_t i = 0; i < currState.size(); i++)
    {
        unsigned fromState = currState[i]->getState();
        sr += -rm.getRate(fromState,fromState,age,clockRate);
    }
    
    // get transition probs for proposal
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        // next event time
        double idx = (*it_h)->getIndex();                   // 2
        dt = (*it_h)->getTime() - t;                        // t_1 - t_0
        
        // rates for next event
        unsigned fromState = currState[ idx ]->getState();  // k
        unsigned toState = (*it_h)->getState();             // j
        double tr = rm.getRate(fromState,toState,age,clockRate);                 // Q[k][j]
        
        // lnP for stepwise events for p(x->y)
        lnP += log(tr) - sr * dt * branchLength;
        
        // update state
        currState[idx] = *it_h;
        t += dt;
        
        // update sum of rates
        sr += (rm.getRate(fromState,fromState,age,1.0) - rm.getRate(toState,toState,age,1.0)) * clockRate;
    }
    
    // lnL for final non-event
    lnP += -sr * (1.0 - t) * branchLength;
    
    return lnP;
}

template<class charType>
void RevBayesCore::PathUniformizationSampleProposal<charType>::fillStateCounts(std::vector<CharacterEvent*> s, unsigned int *counts)
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
const std::string& RevBayesCore::PathUniformizationSampleProposal<charType>::getProposalName( void ) const
{
    static std::string name = "PathUniformizationSampleProposal";
    
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
double RevBayesCore::PathUniformizationSampleProposal<charType>::doProposal( void )
{
    AbstractTreeHistoryCtmc<charType>& p = static_cast< AbstractTreeHistoryCtmc<charType>& >(ctmc->getDistribution());
    proposedHistory.clear();
    
    if (node->isRoot())
        return 0.0;
    
    // get model parameters
    double branchLength = node->getBranchLength();
    
    
    // get rate matrix info

//    RateMatrix& rm = const_cast<RateMatrix&>( qmap->getValue() );
//    rm.update();
//    rm.calculateTransitionProbabilities(branchLength, tpCtmc);

//    const RateMap& rm = qmap->getValue();
    const RateGenerator& rm = *(qmap->getValue().getHomogeneousRateMatrix());
    rm.calculateTransitionProbabilities(branchLength, tpCtmc);
    
    double age = 0.0;
    double domRate = 0.0;
    for (size_t i = 0; i < rm.size(); i++)
    {
        if (-rm.getRate(i,i,age,1.0) > domRate)
            domRate = -rm.getRate(i,i,age,1.0);
    }
    
    MatrixReal unifQ(numStates, numStates);
    for (size_t i = 0; i < numStates; i++)
    {
        unifQ[i][i] = 1.0;
        for (size_t j = 0; j < numStates; j++)
        {
            unifQ[i][j] += rm.getRate(i,j,age,1.0/domRate);
        }
    }
    
    // initialize first tpDtmc element P to I
    for (size_t i = 0; i < numStates; i++)
    {
        tpDtmc[0][i][i] = 1.0;
        for (size_t j = 0; j < numStates; j++)
        {
            if (i != j)
                tpDtmc[0][i][j] = 0.0;
        }
    }
    
    // clear characters
    BranchHistory* bh = &p.getHistory(*node);
    
    // reject sample path history
    std::vector<CharacterEvent*> parentVector = bh->getParentCharacters();
    std::vector<CharacterEvent*> childVector =  bh->getChildCharacters();
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        std::set<CharacterEvent*,CharacterEventCompare> tmpHistory;
        unsigned int startState = parentVector[*it]->getState();
        unsigned int currState = startState;
        unsigned int endState = childVector[*it]->getState();

//        std::cout << *it << " : " << startState << " " << endState << "\n";
        
        // guarantee we have min number of needed jumps
        // Holboth and Stone (2009) uniformization sampling algorithm
        double u = GLOBAL_RNG->uniform01();
        double cum = 0.0;
        int numJumps = 0;
        
        if (startState == endState)
            cum = RbStatistics::Poisson::pdf(domRate * branchLength, 0) * 1 / tpCtmc[startState][endState];
        
        bool exceed = false;
        if (cum > u)
            exceed = true;

        double probAnotherJump = 0.0;
        while (!exceed)
        {
            numJumps++;
            
            if (numJumps > maxNumJumps)
            {

                failed = true;
                return RbConstants::Double::neginf;
//                std::stringstream ss_err;
//                ss_err << "Exceeded maxNumJumps: " << maxNumJumps;
//                throw RbException( ss_err.str() );
            }
            

            tpDtmc[numJumps] = tpDtmc[numJumps-1] * unifQ;
            
            probAnotherJump = RbStatistics::Poisson::pdf(domRate * branchLength, numJumps); // prob of hitting num jumps
            probAnotherJump *= tpDtmc[numJumps][startState][endState] / tpCtmc[startState][endState];
            
            cum += probAnotherJump;
            if (cum > u)
                exceed = true;
        }

      
        
        // sample jump states
        if (numJumps == 0 || (numJumps == 1 && startState == endState))
        {
            ; // do nothing
        }
        else
        {
            // sample jump times
            for (int i = 0; i < numJumps; i++)
                tmpHistory.insert( new CharacterEvent(*it,0,GLOBAL_RNG->uniform01()) );
            
            
            // only one jump
            if (numJumps == 1 && startState != endState)
            {
                CharacterEvent* evt = *(tmpHistory.begin());
                evt->setState(endState);
            }
            
            // multiple jumps
            else if (numJumps > 1)
            {
                std::set<CharacterEvent*,CharacterEventCompare>::iterator it;
                
                int jumpIdx = 1;
                for (it = tmpHistory.begin(); it != tmpHistory.end(); it++)
                {
                    CharacterEvent* evt = *it;
                    unsigned int nextState = 0;

                    // the last event must transition to endState
                    if (jumpIdx == numJumps)
                    {
                        nextState = endState;
                    }
                    // otherwise, sample next intermediate state
                    else
                    {
                        // construct transition state sample probs
                        const std::vector<double>& rowProb = unifQ[currState];
                        std::vector<double> sampleProbs(numStates, 0.0);
                        double sampleProbSum = 0.0;
                        for (size_t i = 0; i < numStates; i++)
                        {
                            double v = tpDtmc[numJumps-jumpIdx][i][endState] * rowProb[i];
                            sampleProbs[i] = v;
                            sampleProbSum += v;
                        }
                        
                        // sample next state for jumpIdx-th event
                        double uIdx = GLOBAL_RNG->uniform01() * sampleProbSum;
                        for (unsigned int i = 0; i < sampleProbs.size(); i++)
                        {
                            uIdx -= sampleProbs[i];
                            if (uIdx <= 0)
                            {
                                nextState = i;
                                break;
                            }
                        }
                    }
                    
                    // update jump chain state
                    evt->setState(nextState);
                    currState = nextState;
                    jumpIdx++;
                }
            }
        }
        
        
        unsigned prevState = startState;
        for (std::set<CharacterEvent*,CharacterEventCompare>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
        {
            // if non-virtual event, add to proposed history
            if ( (*it)->getState() != prevState )
            {
                proposedHistory.insert(*it);
                prevState = (*it)->getState();
            }
            
            // otherwise, free memory
            else
                delete *it;
            
            
        }
        
//        std::cout << numJumps << " " << proposedHistory.size() << "\n";
    }
    
    // assign values back to model for likelihood
    bh->updateHistory(proposedHistory, siteIndexSet);
//    bh->print();
    // return hastings ratio
    proposedLnProb = computeLnProposal(*node, *bh);
    
    return storedLnProb - proposedLnProb;
}


/**
 *
 */
template<class charType>
void RevBayesCore::PathUniformizationSampleProposal<charType>::prepareProposal( void )
{
    AbstractTreeHistoryCtmc<charType>& p = static_cast< AbstractTreeHistoryCtmc<charType>& >(ctmc->getDistribution());
    
    RateMap& rm = const_cast<RateMap&>( qmap->getValue() );
    rm.updateMap();
    
    storedHistory.clear();
    proposedHistory.clear();
    
    storedLnProb = 0.0;
    proposedLnProb = 0.0;
    failed = false;
    
    if (sampleNodeIndex)
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
//    bh->print();
    
    // store history for events in siteIndexSet
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        if (siteIndexSet.find( (*it_h)->getIndex() ) != siteIndexSet.end())
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
void RevBayesCore::PathUniformizationSampleProposal<charType>::printParameterSummary(std::ostream &o) const
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
void RevBayesCore::PathUniformizationSampleProposal<charType>::undoProposal( void )
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
void RevBayesCore::PathUniformizationSampleProposal<charType>::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    if (oldN == ctmc)
    {
        ctmc = static_cast<StochasticNode<AbstractHomologousDiscreteCharacterData>* >(newN) ;
    }
    else if (oldN == tau)
    {
        tau = static_cast<TypedDagNode<Tree>* >(newN);
    }
    else if (oldN == qmap)
    {
        qmap = static_cast<TypedDagNode<RateMap>* >(newN);
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 */
template<class charType>
void RevBayesCore::PathUniformizationSampleProposal<charType>::tune( double rate )
{
    ; // do nothing
}

#endif /* defined(__rb_mlandis__PathUniformizationSampleProposal__) */
