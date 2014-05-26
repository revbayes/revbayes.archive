//
//  TipRejectionSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/7/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TipRejectionSampleProposal__
#define __rb_mlandis__TipRejectionSampleProposal__

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
//#include "TransitionProbability.h"
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
    class TipRejectionSampleProposal : public Proposal {
        
    public:
        TipRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, double l );                                                                //!<  constructor
        
        // Basic utility functions
        void                            assignNodeIndex(size_t idx);
        void                            assignSiteIndexSet(const std::set<size_t>& s);
        TipRejectionSampleProposal*     clone(void) const;                                                                  //!< Clone object
        void                            cleanProposal(void);
        double                          doProposal(void);                                                                   //!< Perform proposal
        const std::vector<DagNode*>&    getNodes(void) const;                                                               //!< Get the vector of DAG nodes this proposal is working on
        const std::string&              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        void                            sampleTipCharacters(const TopologyNode& node, const std::set<size_t>& indexSet);    //!< Sample the characters at the node
        void                            swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the DAG nodes on which the Proposal is working on
        void                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        // parameters
        StochasticNode<AbstractCharacterData>*  ctmc;
        StochasticNode<treeType>*               tau;
        DeterministicNode<RateMap>*             qmap;
        std::vector<DagNode*>                   nodes;
        
        // dimensions
        size_t                                  numNodes;
        size_t                                  numCharacters;
        size_t                                  numStates;
                
        // proposal
        std::map<size_t,BranchHistory*>         storedValues;
        std::map<size_t,BranchHistory*>         proposedValues;
        BranchHistory*                          storedValue;
        size_t                                  nodeIndex;
        std::set<size_t>                        siteIndexSet;
        double                                  storedLnProb;
        BranchHistory*                          proposedValue;
        double                                  proposedLnProb;
        
        PathRejectionSampleProposal<charType,treeType>* nodeProposal;
        TransitionProbabilityMatrix nodeTpMatrix;
        
        double                                  lambda;
        
        // flags
        bool                                    sampleNodeIndex;
        bool                                    sampleSiteIndexSet;
        
    };
    
}



/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class charType, class treeType>
RevBayesCore::TipRejectionSampleProposal<charType, treeType>::TipRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, double l) : Proposal(),
ctmc(n),
tau(t),
qmap(q),
numNodes(t->getValue().getNumberOfNodes()),
numCharacters(n->getValue().getNumberOfCharacters()),
numStates(static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates()),
storedValue(NULL),
proposedValue(NULL),
nodeTpMatrix(numStates),
lambda(l),
sampleNodeIndex(true),
sampleSiteIndexSet(true)

{
    nodes.push_back(ctmc);
    nodes.push_back(tau);
    nodes.push_back(qmap);
    
    nodeProposal = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l);
}


template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::cleanProposal( void )
{
    const TopologyNode& node = tau->getValue().getNode(nodeIndex);
    nodeProposal->cleanProposal();
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the proposal.
 */
template<class charType, class treeType>
RevBayesCore::TipRejectionSampleProposal<charType, treeType>* RevBayesCore::TipRejectionSampleProposal<charType, treeType>::clone( void ) const
{
    return new TipRejectionSampleProposal( *this );
}

template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::assignNodeIndex(size_t idx)
{
    nodeIndex = idx;
    sampleNodeIndex = false;
}

template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::assignSiteIndexSet(const std::set<size_t>& s)
{
    siteIndexSet = s;
    sampleSiteIndexSet = false;
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
template<class charType, class treeType>
const std::string& RevBayesCore::TipRejectionSampleProposal<charType, treeType>::getProposalName( void ) const
{
    static std::string name = "TipRejectionSampleProposal";
    
    return name;
}


/**
 * Get the vector of nodes on which this proposal is working on.
 *
 * \return  Const reference to a vector of nodes pointer on which the proposal operates.
 */
template<class charType, class treeType>
const std::vector<RevBayesCore::DagNode*>& RevBayesCore::TipRejectionSampleProposal<charType, treeType>::getNodes( void ) const
{
    
    return nodes;
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
double RevBayesCore::TipRejectionSampleProposal<charType, treeType>::doProposal( void )
{
    proposedLnProb = 0.0;
    proposedValues.clear();
    
    double proposedLnProbRatio = 0.0;
    
    const TopologyNode& node = tau->getValue().getNode(nodeIndex);

    // update 1x pathEnd and 1x pathHistory values
    
    sampleTipCharacters(node,siteIndexSet);
    proposedLnProbRatio += nodeProposal->doProposal();

    
    return proposedLnProbRatio;
}


/**
 *
 */
template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::prepareProposal( void )
{
    storedLnProb = 0.0;
    storedValues.clear();
    
    size_t numTips = tau->getValue().getNumberOfTips();
    if (sampleNodeIndex)
    {
        nodeIndex = GLOBAL_RNG->uniform01() * numTips;
    }
    
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
    
    TopologyNode& node = tau->getValue().getNode(nodeIndex);
    
    nodeProposal->assignNode(&node);
    nodeProposal->assignSiteIndexSet(siteIndexSet);
    nodeProposal->prepareProposal();
        
    sampleNodeIndex = true;
    sampleSiteIndexSet = true;
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}


template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::sampleTipCharacters(const TopologyNode& node, const std::set<size_t>& indexSet)
{
    

    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    
    qmap->getValue().calculateTransitionProbabilities(node, nodeTpMatrix);
    
    std::vector<BranchHistory*> histories = p->getHistories();

    
    // for sampling probs
    const std::vector<CharacterEvent*>& nodeParentState = histories[node.getIndex()]->getParentCharacters();
    const std::vector<double>& tipProbs = p->getTipProbs(node.getIndex());
    
    // to update
    std::vector<CharacterEvent*> nodeChildState = histories[node.getIndex()]->getChildCharacters();
        
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        unsigned int ancS = nodeParentState[*it]->getState();
        
        double u = GLOBAL_RNG->uniform01();
        double g0 = nodeTpMatrix[ancS][0] * (1.0 - tipProbs[*it]);
        double g1 = nodeTpMatrix[ancS][1] * tipProbs[*it];
        
        unsigned int s = 0;
        if (u < g1 / (g0 + g1))
            s = 1;
        
        //nodeChildState[*it] = new CharacterEvent(*it, s, 1.0);
        nodeChildState[*it]->setState(s);
    }
    
    histories[node.getIndex()]->setChildCharacters(nodeChildState);
}

/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the ctmc/DAG-node to its original value.
 */
template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::undoProposal( void )
{
    // swap current value and stored value
    //const TopologyNode& node = tau->getValue().getNode(nodeIndex);
    nodeProposal->undoProposal();
}


/**
 * Swap the current ctmc for a new one.
 *
 * \param[in]     oldN     The old ctmc that needs to be replaced.
 * \param[in]     newN     The new ctmc.
 */
template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
{
    
    if (oldN == ctmc)
    {
        ctmc = static_cast<StochasticNode<AbstractCharacterData>* >(newN) ;
    }
    else if (oldN == tau)
    {
        tau = static_cast<StochasticNode<treeType>* >(newN);
    }
    else if (oldN == qmap)
    {
        qmap = static_cast<DeterministicNode<RateMap>* >(newN);
    }
    
    nodeProposal->swapNode(oldN, newN);

}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 */
template<class charType, class treeType>
void RevBayesCore::TipRejectionSampleProposal<charType, treeType>::tune( double rate )
{
    ; // do nothing
}

#endif /* defined(__rb_mlandis__TipRejectionSampleProposal__) */
