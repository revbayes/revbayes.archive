//
//  BiogeographyNodeRejectionSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/7/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographyNodeRejectionSampleProposal__
#define __rb_mlandis__BiogeographyNodeRejectionSampleProposal__

#include "BiogeographicTreeHistoryCtmc.h"
#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "DistributionPoisson.h"
#include "BiogeographyPathRejectionSampleProposal.h"
#include "PathRejectionSampleProposal.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMap.h"
#include "RbException.h"
#include "StochasticNode.h"
//#include "TransitionProbability.h"
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
    class BiogeographyNodeRejectionSampleProposal : public Proposal {
        
    public:
        BiogeographyNodeRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, double l, TopologyNode* nd=NULL );                                                                //!<  constructor
        BiogeographyNodeRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, BiogeographyPathRejectionSampleProposal<charType,treeType>* p, double l, TopologyNode* nd=NULL );                                                                //!<  constructor
        
        
        // Basic utility functions
        void                            assignNode(TopologyNode* nd);
        void                            assignSiteIndexSet(const std::set<size_t>& s);
        BiogeographyNodeRejectionSampleProposal*    clone(void) const;                                                                  //!< Clone object
        void                            cleanProposal(void);
        double                          computeProposal(const std::set<size_t>& indexSet);
        double                          computeEpochProposal(const std::set<size_t>& indexSet);
        double                          doProposal(void);                                                                   //!< Perform proposal
        const std::set<DagNode*>&       getNodes(void) const;                                                               //!< Get the vector of DAG nodes this proposal is working on
        const std::string&              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        double                          sampleNodeCharacters(const std::set<size_t>& indexSet);
        double                          sampleEpochNodeCharacters(const std::set<size_t>& indexSet);
        double                          sampleRootCharacters(const std::set<size_t>& indexSet);
        void                            swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the DAG nodes on which the Proposal is working on
        void                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        // parameters
        StochasticNode<AbstractCharacterData>*  ctmc;
        StochasticNode<treeType>*               tau;
        DeterministicNode<RateMap>*             qmap;
        std::set<DagNode*>                      nodes;
        
        // dimensions
        size_t                                  numNodes;
        size_t                                  numCharacters;
        size_t                                  numStates;
        
        // proposal
        std::vector<unsigned>                   storedNodeState;
        std::vector<unsigned>                   storedBudState;
        std::vector<unsigned>                   storedTrunkState;
        std::vector<unsigned>                   storedRootState;
        int                                     monitorIndex;
        std::set<size_t>                        siteIndexSet;
        double                                  storedLnProb;
        double                                  proposedLnProb;
        
        int                                     storedCladogenicState;
        int                                     proposedCladogenicState;
        TopologyNode*                           node;
        TopologyNode*                           storedBudNode;
        TopologyNode*                           storedTrunkNode;
        TopologyNode*                           proposedBudNode;
        TopologyNode*                           proposedTrunkNode;

        
        PathRejectionSampleProposal<charType,treeType>* nodeProposal;
        PathRejectionSampleProposal<charType,treeType>* leftProposal;
        PathRejectionSampleProposal<charType,treeType>* rightProposal;
        
        TransitionProbabilityMatrix nodeTpMatrix;
        TransitionProbabilityMatrix trunkTpMatrix;
        TransitionProbabilityMatrix budTpMatrix;
        
        double                                  lambda;
        
        // flags
        bool                                    fixNodeIndex;
        bool                                    sampleNodeIndex;
        bool                                    sampleSiteIndexSet;
        bool                                    swapBudTrunk;
    };
}



/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class charType, class treeType>
RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::BiogeographyNodeRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, double l, TopologyNode* nd) : Proposal(),
ctmc(n),
tau(t),
qmap(q),
numNodes(t->getValue().getNumberOfNodes()),
numCharacters(n->getValue().getNumberOfCharacters()),
numStates(static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates()),
node(nd),
nodeTpMatrix(2),
trunkTpMatrix(2),
budTpMatrix(2),
lambda(l),
sampleNodeIndex(true),
sampleSiteIndexSet(true)

{
    
    storedCladogenicState = 0;
    proposedCladogenicState = 0;
    storedBudNode = NULL;
    storedTrunkNode = NULL;
    proposedTrunkNode = NULL;
    proposedBudNode = NULL;
    swapBudTrunk = false;
    
    nodes.insert(ctmc);
    nodes.insert(tau);
    nodes.insert(qmap);
    
    nodeProposal  = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    leftProposal  = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    rightProposal = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    
    storedNodeState.resize(numCharacters,0);
    storedBudState.resize(numCharacters,0);
    storedTrunkState.resize(numCharacters,0);
    storedRootState.resize(numCharacters,0);
    
    fixNodeIndex = (node != NULL);
}


/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class charType, class treeType>
RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::BiogeographyNodeRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, BiogeographyPathRejectionSampleProposal<charType,treeType>* p, double l, TopologyNode* nd) : Proposal(),
ctmc(n),
tau(t),
qmap(q),
numNodes(t->getValue().getNumberOfNodes()),
numCharacters(n->getValue().getNumberOfCharacters()),
numStates(static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates()),
node(nd),
nodeTpMatrix(2),
trunkTpMatrix(2),
budTpMatrix(2),
lambda(l),
sampleNodeIndex(true),
sampleSiteIndexSet(true)

{
    
    storedCladogenicState = 0;
    proposedCladogenicState = 0;
    storedBudNode = NULL;
    storedTrunkNode = NULL;
    proposedTrunkNode = NULL;
    proposedBudNode = NULL;
    swapBudTrunk = false;
    
    nodes.insert(ctmc);
    nodes.insert(tau);
    nodes.insert(qmap);
    
    nodeProposal  = new PathRejectionSampleProposal<charType,treeType>(*p);
    leftProposal  = new PathRejectionSampleProposal<charType,treeType>(*p);
    rightProposal = new PathRejectionSampleProposal<charType,treeType>(*p);
    
    storedNodeState.resize(numCharacters,0);
    storedBudState.resize(numCharacters,0);
    storedTrunkState.resize(numCharacters,0);
    storedRootState.resize(numCharacters,0);
    
    fixNodeIndex = (node != NULL);
}


template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::cleanProposal( void )
{
    nodeProposal->cleanProposal();
    rightProposal->cleanProposal();
    leftProposal->cleanProposal();
    
//    std::cout << "ACCEPT\n";
//    
//    std::cout << "\n";
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
template<class charType, class treeType>
RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>* RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::clone( void ) const
{
    return new BiogeographyNodeRejectionSampleProposal( *this );
}


template<class charType, class treeType>
double RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::computeProposal(const std::set<size_t>& indexSet)
{
    double lnP = 0.0;
    if (!node->isTip())
    {
        
        BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
        const std::vector<BranchHistory*>& histories = p->getHistories();
        
        // get transition probs
        const RateMap& rm = qmap->getValue();
        rm.calculateTransitionProbabilities(*node, nodeTpMatrix);
        rm.calculateTransitionProbabilities(*storedTrunkNode, trunkTpMatrix);
        rm.calculateTransitionProbabilities(*storedBudNode, budTpMatrix);
        
        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& budChildState       = histories[ storedBudNode->getIndex()     ]->getChildCharacters();
        const std::vector<CharacterEvent*>& budParentState      = histories[ storedBudNode->getIndex()     ]->getParentCharacters();
        const std::vector<CharacterEvent*>& nodeChildState      = histories[ node->getIndex()              ]->getChildCharacters();
        const std::vector<CharacterEvent*>& nodeParentState     = histories[ node->getIndex()              ]->getParentCharacters();
        const std::vector<CharacterEvent*>& trunkChildState     = histories[ storedTrunkNode->getIndex()   ]->getChildCharacters();
        const std::vector<CharacterEvent*>& trunkParentState    = histories[ storedTrunkNode->getIndex()   ]->getParentCharacters();
        
        // sample area to bud
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {

            double p =  budTpMatrix[ budParentState[*it]->getState() ][ budChildState[*it]->getState() ]
                        * trunkTpMatrix[ trunkParentState[*it]->getState() ][ trunkChildState[*it]->getState() ]
                        * nodeTpMatrix[ nodeParentState[*it]->getState() ][ nodeChildState[*it]->getState() ];
            
            lnP += -log(p);
        }
    }
    return lnP;
}

template<class charType, class treeType>
double RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::computeEpochProposal(const std::set<size_t>& indexSet)
{
    double lnP = 0.0;
    if (!node->isTip())
    {
        
        BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
        const std::vector<BranchHistory*>& histories = p->getHistories();
        
        // get transition probs
        const RateMap& rm = qmap->getValue();
        
        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& budChildState       = histories[ storedBudNode->getIndex()     ]->getChildCharacters();
        const std::vector<CharacterEvent*>& budParentState      = histories[ storedBudNode->getIndex()     ]->getParentCharacters();
        const std::vector<CharacterEvent*>& nodeChildState      = histories[ node->getIndex()              ]->getChildCharacters();
        const std::vector<CharacterEvent*>& nodeParentState     = histories[ node->getIndex()              ]->getParentCharacters();
        const std::vector<CharacterEvent*>& trunkChildState     = histories[ storedTrunkNode->getIndex()   ]->getChildCharacters();
        const std::vector<CharacterEvent*>& trunkParentState    = histories[ storedTrunkNode->getIndex()   ]->getParentCharacters();
        
        // sample area to bud
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            rm.calculateTransitionProbabilities(*node, nodeTpMatrix, *it);
            rm.calculateTransitionProbabilities(*storedTrunkNode, trunkTpMatrix, *it);
            rm.calculateTransitionProbabilities(*storedBudNode, budTpMatrix, *it);
            
            double p =  budTpMatrix[ budParentState[*it]->getState() ][ budChildState[*it]->getState() ]
                        * trunkTpMatrix[ trunkParentState[*it]->getState() ][ trunkChildState[*it]->getState() ]
                        * nodeTpMatrix[ nodeParentState[*it]->getState() ][ nodeChildState[*it]->getState() ];
            
            lnP += -log(p);
        }
    }
    return lnP;
}

template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::assignNode(TopologyNode* nd)
{
    node = nd;
    sampleNodeIndex = false;
}

template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::assignSiteIndexSet(const std::set<size_t>& s)
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
const std::string& RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::getProposalName( void ) const
{
    static std::string name = "BiogeographyNodeRejectionSampleProposal";
    
    return name;
}


/**
 * Get the vector of nodes on which this proposal is working on.
 *
 * \return  Const reference to a vector of nodes pointer on which the proposal operates.
 */
template<class charType, class treeType>
const std::set<RevBayesCore::DagNode*>& RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::getNodes( void ) const
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
double RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::doProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    proposedLnProb = 0.0;

    // assign prepared bud/clado states
    p->setBuddingState(*proposedTrunkNode, 0);
    p->setBuddingState(*proposedBudNode, 1);
    p->setCladogenicState(*proposedTrunkNode, proposedCladogenicState);
    p->setCladogenicState(*proposedBudNode, proposedCladogenicState);

    // update node state
    proposedLnProb += sampleNodeCharacters(siteIndexSet);
    
    // update 3x incident paths
    double nodeLnProb = nodeProposal->doProposal();
    double leftLnProb = leftProposal->doProposal();
    double rightLnProb = rightProposal->doProposal();
//    std::cout << "nodeLnProb " << nodeLnProb << "\n";
//    std::cout << "leftLnProb " << leftLnProb << "\n";
//    std::cout << "rightLnProb " << rightLnProb << "\n";
    
    proposedLnProb += nodeLnProb + leftLnProb + rightLnProb;
    
//    std::cout << storedLnProb << " " << proposedLnProb << "\n";
    return storedLnProb - proposedLnProb;
}


/**
 *
 */
template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::prepareProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    
    storedLnProb = 0.0;
    proposedLnProb = 0.0;
    
    const treeType& tree = tau->getValue();
    if (sampleNodeIndex && !fixNodeIndex)
    {
        node = NULL;
        std::vector<TopologyNode*> nds = tree.getNodes();
        while (node == NULL || node->isTip())
        {
            size_t idx = GLOBAL_RNG->uniform01() * nds.size(); //numTips + GLOBAL_RNG->uniform01() * (numNodes-numTips);
            node = nds[idx];
        };
    }
    sampleNodeIndex = true;
    
    // propose new bud
    const std::vector<int>& buddingState = p->getBuddingStates();
    if ( buddingState[ node->getChild(0).getIndex() ] == 1 )
    {
        storedBudNode = &node->getChild(0);
        storedTrunkNode = &node->getChild(1);
    }
    else
    {
        storedBudNode = &node->getChild(1);
        storedTrunkNode = &node->getChild(0);
    }
    swapBudTrunk = GLOBAL_RNG->uniform01() < 0.5;
//    swapBudTrunk = false; // debug variable
    if (swapBudTrunk)
    {
        proposedBudNode = storedTrunkNode;
        proposedTrunkNode = storedBudNode;
    }
    else
    {
        proposedBudNode = storedBudNode;
        proposedTrunkNode = storedTrunkNode;
    }
    
    // propose new cladogenic state
    storedCladogenicState = p->getCladogenicState(*proposedTrunkNode);
    double u = GLOBAL_RNG->uniform01();
    if (u < 0.5 || !p->useCladogenicEvents())
        proposedCladogenicState = 0;
    else if (u < 1.0)
        proposedCladogenicState = 1;
    else if (u < 1.0)
        proposedCladogenicState = 2;
    
    
    if (sampleSiteIndexSet)
    {
        siteIndexSet.clear();
        siteIndexSet.insert(GLOBAL_RNG->uniform01() * numCharacters); // at least one is inserted
        for (size_t i = 0; i < numCharacters; i++)
        {
            // just resample all states for now, try something clever later
            //if (GLOBAL_RNG->uniform01() < lambda || budTrunkMove)
            {
                siteIndexSet.insert(i);
            }
        }
    }
    sampleSiteIndexSet = true;
    
    // prepare the path proposals
    nodeProposal->assignNode(node);
    nodeProposal->assignSiteIndexSet(siteIndexSet);
    nodeProposal->prepareProposal();
    
    leftProposal->assignNode(&node->getChild(0));
    leftProposal->assignSiteIndexSet(siteIndexSet);
    leftProposal->prepareProposal();
    
    rightProposal->assignNode(&node->getChild(1));
    rightProposal->assignSiteIndexSet(siteIndexSet);
    rightProposal->prepareProposal();
    
    // update proposal prob
    storedLnProb += computeProposal(siteIndexSet);

    
    // store node state values
    storedNodeState.clear();
    storedRootState.clear();
    storedBudState.clear();
    storedTrunkState.clear();
    
    storedNodeState.resize(numCharacters,0);
    storedBudState.resize(numCharacters,0);
    storedTrunkState.resize(numCharacters,0);
    const std::vector<CharacterEvent*>& nodeState  = p->getHistory(*node).getChildCharacters();
    const std::vector<CharacterEvent*>& budState   = p->getHistory(*storedBudNode).getParentCharacters();
    const std::vector<CharacterEvent*>& trunkState = p->getHistory(*storedTrunkNode).getParentCharacters();
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        storedNodeState[*it]    = nodeState[*it]->getState();
        storedBudState[*it]     = budState[*it]->getState();
        storedTrunkState[*it]   = trunkState[*it]->getState();
    }
    
    
    if (node->isRoot())
    {
        storedRootState.resize(numCharacters,0);
        const std::vector<CharacterEvent*>& rootState = p->getHistory(*node).getParentCharacters();
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            unsigned s = rootState[*it]->getState();
            storedRootState[*it] = s;
        }
    }
    
//    std::cout << "before\n";
//    p->getHistory(*node).print();
//    p->getHistory(*storedTrunkNode).print();
//    p->getHistory(*storedBudNode).print();
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
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

template<class charType, class treeType>
double RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::sampleNodeCharacters(const std::set<size_t>& indexSet)
{
    double lnP = 0.0;
    if (!node->isTip())
    {
        
        BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
        const std::vector<BranchHistory*>& histories = p->getHistories();
        
        // get transition probs
        const RateMap_Biogeography& rm = static_cast<const RateMap_Biogeography&>(qmap->getValue());
                
        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& nodeParentState  = histories[ node->getIndex()              ]->getParentCharacters();
        const std::vector<CharacterEvent*>& trunkChildState  = histories[ proposedTrunkNode->getIndex() ]->getChildCharacters();
        const std::vector<CharacterEvent*>& budChildState    = histories[ proposedBudNode->getIndex()   ]->getChildCharacters();
        
        // states to update
        std::vector<CharacterEvent*> nodeChildState    = histories[ node->getIndex()              ]->getChildCharacters();
        std::vector<CharacterEvent*> trunkParentState  = histories[ proposedTrunkNode->getIndex() ]->getParentCharacters();
        std::vector<CharacterEvent*> budParentState    = histories[ proposedBudNode->getIndex()   ]->getParentCharacters();
        
//        std::cout << "node_ch  ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << nodeChildState[i]->getState();
//        std::cout << "\n";
//
//        std::cout << "trunk_pa ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << trunkParentState[i]->getState();
//        std::cout << "\n";
//
//        std::cout << "bud_pa   ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << budParentState[i]->getState();
//        std::cout << "\n";
//        std::cout << "....\n";
        
        // sample area to bud
        
        // 0: sympatry, 1: peripatry, 2: allopatry
        int budIdx = -1;
        if (proposedCladogenicState != 0)
        {
            while (budIdx == -1 || !rm.isAreaAvailable(budIdx, node->getAge()))
            {
                budIdx = (int)(nodeChildState.size() * GLOBAL_RNG->uniform01());
            }
            budParentState[budIdx]->setState(1);
            if (proposedCladogenicState == 1)
                trunkParentState[budIdx]->setState(1);
            else if (proposedCladogenicState == 2)
                trunkParentState[budIdx]->setState(0);
            nodeChildState[budIdx]->setState(1);
            siteIndexSet.insert(budIdx);
        }
        
        // sample states
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
//            unsigned int ancS = nodeParentState[*it]->getState();
//            unsigned int desS1 = trunkChildState[*it]->getState();
//            unsigned int desS2 = budChildState[*it]->getState();

            rm.calculateTransitionProbabilities(*node, nodeTpMatrix);
            rm.calculateTransitionProbabilities(*proposedTrunkNode, trunkTpMatrix);
            rm.calculateTransitionProbabilities(*proposedBudNode, budTpMatrix);

            // Would prefer to sample from f(H_N, H_L, H_R, X_N, X_T, X_B | X_L, X_R, X_A).
            // Unfortunately, this does not seem to scale well with N, since it requires summing
            // over (X_T,X_B) to compute the conditional probability.
            
            // For now, nice n' dumb.
            double u = GLOBAL_RNG->uniform01();
            unsigned int s = 0;
            if (u < 0.5 && rm.isAreaAvailable(*it, node->getAge()))
                s = 1;
            
            if (*it != (size_t)budIdx)
            {
                nodeChildState[*it]->setState(s);
                trunkParentState[*it]->setState(s);
                if (proposedCladogenicState == 0)
                    budParentState[*it]->setState(s);
                else if (proposedCladogenicState == 1 || proposedCladogenicState == 2)
                    budParentState[*it]->setState(0);
            }
            
            double p =  budTpMatrix[ budParentState[*it]->getState() ][ budChildState[*it]->getState() ]
                        * trunkTpMatrix[ trunkParentState[*it]->getState() ][ trunkChildState[*it]->getState() ]
                        * nodeTpMatrix[ nodeParentState[*it]->getState() ][ nodeChildState[*it]->getState() ];
            
            lnP += -log(p);

        }
        
//        std::cout << "node_ch  ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << nodeChildState[i]->getState();
//        std::cout << "\n";
//        
//        std::cout << "trunk_pa ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << trunkParentState[i]->getState();
//        std::cout << "\n";
//        
//        std::cout << "bud_pa   ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << budParentState[i]->getState();
//        std::cout << "\n";
//        std::cout << "----\n";

    }
    return lnP;
}


template<class charType, class treeType>
double RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::sampleEpochNodeCharacters(const std::set<size_t>& indexSet)
{
    double lnP = 0.0;
    if (!node->isTip())
    {
        
        BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
        const std::vector<BranchHistory*>& histories = p->getHistories();
        
        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& nodeParentState  = histories[ node->getIndex()              ]->getParentCharacters();
        const std::vector<CharacterEvent*>& trunkChildState  = histories[ proposedTrunkNode->getIndex() ]->getChildCharacters();
        const std::vector<CharacterEvent*>& budChildState    = histories[ proposedBudNode->getIndex()   ]->getChildCharacters();
        
        // states to update
        std::vector<CharacterEvent*> nodeChildState    = histories[ node->getIndex()              ]->getChildCharacters();
        std::vector<CharacterEvent*> trunkParentState  = histories[ proposedTrunkNode->getIndex() ]->getParentCharacters();
        std::vector<CharacterEvent*> budParentState    = histories[ proposedBudNode->getIndex()   ]->getParentCharacters();
        
        //        std::cout << "node_ch  ";
        //        for (size_t i = 0; i < numCharacters; i++)
        //            std::cout << nodeChildState[i]->getState();
        //        std::cout << "\n";
        //
        //        std::cout << "trunk_pa ";
        //        for (size_t i = 0; i < numCharacters; i++)
        //            std::cout << trunkParentState[i]->getState();
        //        std::cout << "\n";
        //
        //        std::cout << "bud_pa   ";
        //        for (size_t i = 0; i < numCharacters; i++)
        //            std::cout << budParentState[i]->getState();
        //        std::cout << "\n";
        //        std::cout << "....\n";
        
        // sample area to bud
        
        int budIdx = -1;
        if (proposedCladogenicState != 0)
        {
            budIdx = nodeChildState.size() * GLOBAL_RNG->uniform01();
            budParentState[budIdx]->setState(1);
            if (proposedCladogenicState == 1)
                trunkParentState[budIdx]->setState(1);
            else if (proposedCladogenicState == 2)
                trunkParentState[budIdx]->setState(0);
            nodeChildState[budIdx]->setState(1);
            siteIndexSet.insert(budIdx);
        }
        
        // sample states
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            // get transition probs
            const RateMap& rm = qmap->getValue();
            rm.calculateTransitionProbabilities(*node, nodeTpMatrix, *it);
            rm.calculateTransitionProbabilities(*proposedTrunkNode, trunkTpMatrix, *it);
            rm.calculateTransitionProbabilities(*proposedBudNode, budTpMatrix, *it);
            
//            unsigned int ancS = nodeParentState[*it]->getState();
//            unsigned int desS1 = trunkChildState[*it]->getState();
//            unsigned int desS2 = budChildState[*it]->getState();
            
            // Would prefer to sample from f(H_N, H_L, H_R, X_N, X_T, X_B | X_L, X_R, X_A).
            // Unfortunately, this does not seem to scale well with N, since it requires summing
            // over (X_T,X_B) to compute the conditional probability.
            
            // For now, nice n' dumb.
            double u = GLOBAL_RNG->uniform01();
            unsigned int s = 0;
            if (u < 0.5)
                s = 1;
            
            if (*it != (size_t)budIdx)
            {
                nodeChildState[*it]->setState(s);
                trunkParentState[*it]->setState(s);
                if (proposedCladogenicState == 0)
                    budParentState[*it]->setState(s);
                else if (proposedCladogenicState == 1 || proposedCladogenicState == 2)
                    budParentState[*it]->setState(0);
            }
            
            double p =  budTpMatrix[ budParentState[*it]->getState() ][ budChildState[*it]->getState() ]
                        * trunkTpMatrix[ trunkParentState[*it]->getState() ][ trunkChildState[*it]->getState() ]
                        * nodeTpMatrix[ nodeParentState[*it]->getState() ][ nodeChildState[*it]->getState() ];
            
            lnP += -log(p);
            
        }
        
        //        std::cout << "node_ch  ";
        //        for (size_t i = 0; i < numCharacters; i++)
        //            std::cout << nodeChildState[i]->getState();
        //        std::cout << "\n";
        //
        //        std::cout << "trunk_pa ";
        //        for (size_t i = 0; i < numCharacters; i++)
        //            std::cout << trunkParentState[i]->getState();
        //        std::cout << "\n";
        //
        //        std::cout << "bud_pa   ";
        //        for (size_t i = 0; i < numCharacters; i++)
        //            std::cout << budParentState[i]->getState();
        //        std::cout << "\n";
        //        std::cout << "----\n";
        
    }
    return lnP;
}


template<class charType, class treeType>
double RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::sampleRootCharacters(const std::set<size_t>& indexSet)
{
    double lnP = 0.0;
    if (!node->isRoot())
        return 0.0;
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    BranchHistory* bh = &p->getHistory(*node);
    std::vector<CharacterEvent*> parentState = bh->getParentCharacters();
    
    double r0 = qmap->getValue().getSiteRate(*node,1,0);
    double r1 = qmap->getValue().getSiteRate(*node,0,1);
    unsigned n1_old = 0;
    unsigned n1_new = 0;
    
    double p1 = r1 / (r0 + r1);
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        unsigned s = 0;
        double u = GLOBAL_RNG->uniform01();
        if (u < p1)
        {
            s = 1;
            n1_new++;
        }
        if (parentState[*it]->getState() == 1)
        {
            n1_old++;
        }
        
        parentState[*it]->setState(s); //new CharacterEvent(*it,s,0.0);
    }
    //bh->setParentCharacters(parentState);
    
    size_t n = siteIndexSet.size();
    size_t n0_old = n - n1_old;
    size_t n0_new = n - n1_new;
    double p0 = 1.0 - p1;
    
    lnP = n1_old * log(p1) + n0_old * log(p0) - n1_new * log(p1) - n0_new * log(p0);
    return 0.0;
    return lnP;
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the ctmc/DAG-node to its original value.
 */
template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::undoProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    const std::vector<BranchHistory*>& histories = p->getHistories();
    
    // restore path state
    nodeProposal->undoProposal();
    rightProposal->undoProposal();
    leftProposal->undoProposal();
    
    // restore node state
    std::vector<CharacterEvent*> nodeChildState   = histories[node->getIndex()]->getChildCharacters();
    std::vector<CharacterEvent*> budParentState   = histories[storedBudNode->getIndex()]->getParentCharacters();
    std::vector<CharacterEvent*> trunkParentState = histories[storedTrunkNode->getIndex()]->getParentCharacters();
    
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        nodeChildState[*it]->setState(storedNodeState[*it]);
        budParentState[*it]->setState(storedBudState[*it]);
        trunkParentState[*it]->setState(storedTrunkState[*it]);
    }
    
    // restore root state
    if (node->isRoot())
    {
        std::vector<CharacterEvent*> rootState = histories[node->getIndex()]->getParentCharacters();
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            unsigned s = storedRootState[*it];
            rootState[*it]->setState(s);
        }
    }
    
    p->setBuddingState(*storedTrunkNode, 0);
    p->setBuddingState(*storedBudNode, 1);
    
    p->setCladogenicState(*proposedTrunkNode, storedCladogenicState);
    p->setCladogenicState(*proposedBudNode, storedCladogenicState);


//    std::cout << "REJECT\n";
//    
//    std::cout << "\n";
}


/**
 * Swap the current ctmc for a new one.
 *
 * \param[in]     oldN     The old ctmc that needs to be replaced.
 * \param[in]     newN     The new ctmc.
 */
template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
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
    leftProposal->swapNode(oldN, newN);
    rightProposal->swapNode(oldN, newN);
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 */
template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::tune( double rate )
{
    ; // do nothing
}

#endif /* defined(__rb_mlandis__BiogeographyNodeRejectionSampleProposal__) */
