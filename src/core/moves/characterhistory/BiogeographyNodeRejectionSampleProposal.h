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
        double                          doProposal(void);                                                                   //!< Perform proposal
        const std::set<DagNode*>&       getNodes(void) const;                                                               //!< Get the vector of DAG nodes this proposal is working on
        const std::string&              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        double                          sampleNodeCharacters(const std::set<size_t>& indexSet);
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

        
        BiogeographyPathRejectionSampleProposal<charType,treeType>* nodeProposal;
        BiogeographyPathRejectionSampleProposal<charType,treeType>* leftProposal;
        BiogeographyPathRejectionSampleProposal<charType,treeType>* rightProposal;
        
        TransitionProbabilityMatrix nodeTpMatrix;
        TransitionProbabilityMatrix trunkTpMatrix;
        TransitionProbabilityMatrix budTpMatrix;
        
        double                                  lambda;
        
        // flags
        bool                                    fixNodeIndex;
        bool                                    sampleNodeIndex;
        bool                                    sampleSiteIndexSet;
        bool                                    swapBudTrunk;
        bool                                    useAreaAdjacency;
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
sampleSiteIndexSet(true),
useAreaAdjacency(false)

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
    
    nodeProposal  = new BiogeographyPathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    leftProposal  = new BiogeographyPathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    rightProposal = new BiogeographyPathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    
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
sampleSiteIndexSet(true),
useAreaAdjacency(false)

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
    
    nodeProposal  = new BiogeographyPathRejectionSampleProposal<charType,treeType>(*p);
    leftProposal  = new BiogeographyPathRejectionSampleProposal<charType,treeType>(*p);
    rightProposal = new BiogeographyPathRejectionSampleProposal<charType,treeType>(*p);
    
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
    
    if (node->isRoot())
    {
        std::cout << "root accept\n";
    }

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
        const std::vector<CharacterEvent*>& trunkChildState     = histories[ storedTrunkNode->getIndex()   ]->getChildCharacters();
        const std::vector<CharacterEvent*>& trunkParentState    = histories[ storedTrunkNode->getIndex()   ]->getParentCharacters();
        const std::vector<CharacterEvent*>& nodeChildState      = histories[ node->getIndex()              ]->getChildCharacters();
        const std::vector<CharacterEvent*>& nodeParentState     = histories[ node->getIndex()              ]->getParentCharacters();

        
        // sample area to bud
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            
            // account for conditional probability that 3x sampled paths end in sampled node states
            double p = budTpMatrix[ budParentState[*it]->getState() ][ budChildState[*it]->getState() ]
                       * trunkTpMatrix[ trunkParentState[*it]->getState() ][ trunkChildState[*it]->getState() ]
                       * nodeTpMatrix[ nodeParentState[*it]->getState() ][ nodeChildState[*it]->getState() ];
            
//            double p0 = nodeTpMatrix[ nodeParentState[*it]->getState() ][ 0 ]
//                        * trunkTpMatrix[ 0 ][ trunkChildState[*it]->getState() ]
//                        * budTpMatrix[ 0 ][ budChildState[*it]->getState() ];
//            double p1 = nodeTpMatrix[ nodeParentState[*it]->getState() ][ 1 ]
//                        * trunkTpMatrix[ 1 ][ trunkChildState[*it]->getState() ]
//                        * budTpMatrix[ 1 ][ budChildState[*it]->getState() ];
            
            // negative because we divide by p
            lnP += -log(p);
//            lnP += -log(p/(p0+p1));
        }
    }
    return 0.0; // lnP;
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

    // update node state
    //proposedLnProb =
    proposedLnProb += sampleNodeCharacters(siteIndexSet);
    
    p->setCladogenicState(*proposedTrunkNode, proposedCladogenicState);
    p->setCladogenicState(*proposedBudNode, proposedCladogenicState);
    
    // update 3x incident paths
    double nodeLnProb = nodeProposal->doProposal();
    double leftLnProb = leftProposal->doProposal();
    double rightLnProb = rightProposal->doProposal();

    
    if (node->isRoot())
    {
        
//        std::cout << "wha!\n";
        ;
    }
    
// return storedLnProb - proposedLnProb + nodeLnProb + leftLnProb + rightLnProb;
    proposedLnProb += nodeLnProb;
    proposedLnProb += leftLnProb;
    proposedLnProb += rightLnProb;
    
    return proposedLnProb;
}


/**
 *
 */
template<class charType, class treeType>
void RevBayesCore::BiogeographyNodeRejectionSampleProposal<charType, treeType>::prepareProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    const RateMap_Biogeography& rm = static_cast<RateMap_Biogeography&>(this->qmap->getValue());
    
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
    BranchHistory* bh = &(p->getHistory(*(this->node)));
    
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
    
    // resample all characters for now...
    if (sampleSiteIndexSet)
    {
        siteIndexSet.clear();
        siteIndexSet.insert(GLOBAL_RNG->uniform01() * numCharacters); // at least one is inserted
        if (useAreaAdjacency || !true)
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
            for (size_t i = 0; i < numCharacters; i++)
            {
                // just resample all states for now, try something clever later
                //            if (GLOBAL_RNG->uniform01() < lambda)
                {
                    siteIndexSet.insert(i);
                }
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
//    storedLnProb += computeProposal(siteIndexSet);
    storedLnProb = 0.0;

    
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
    
    
    // propose new cladogenic state
    storedCladogenicState = p->getCladogenicState(*storedTrunkNode);
    
    bool nodeHasOneArea = true;
    int numOn = 0;
    for (size_t i = 0; i < nodeState.size(); i++)
    {
        if (nodeState[i]->getState() == 1)
            numOn++;
        if (numOn > 1)
        {
            nodeHasOneArea = false;
            break;
        }
    }
    
//    double u = GLOBAL_RNG->uniform01() * 1.0;
//    if (u < 0.5 || p->useCladogenicEvents() == false || nodeHasOneArea)
//        proposedCladogenicState = 0;
//    else if (u < 1.0)
//        proposedCladogenicState = 1;
//    else if (u < 1.5)
//        proposedCladogenicState = 2;
//    
//    true;

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
//        const std::vector<double>& csf = std::vector<double>(3, 1.0/3);
        const std::vector<double>& csf = p->getCladogenicStateFrequencies();
        
        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& nodeParentState  = histories[ node->getIndex()              ]->getParentCharacters();
        const std::vector<CharacterEvent*>& trunkChildState  = histories[ proposedTrunkNode->getIndex() ]->getChildCharacters();
        const std::vector<CharacterEvent*>& budChildState    = histories[ proposedBudNode->getIndex()   ]->getChildCharacters();
        
        // states to update
        std::vector<CharacterEvent*> nodeChildState    = histories[ node->getIndex()              ]->getChildCharacters();
        std::vector<CharacterEvent*> trunkParentState  = histories[ proposedTrunkNode->getIndex() ]->getParentCharacters();
        std::vector<CharacterEvent*> budParentState    = histories[ proposedBudNode->getIndex()   ]->getParentCharacters();
        
        // these must be computed per site because area rates vary over time (time-hetero. ctmc)
        rm.calculateTransitionProbabilities(*node, nodeTpMatrix);
        rm.calculateTransitionProbabilities(*proposedTrunkNode, trunkTpMatrix);
        rm.calculateTransitionProbabilities(*proposedBudNode, budTpMatrix);

        
        int dOn = 0;
        for (size_t i = 0; i < nodeChildState.size(); i++)
            if (nodeChildState[i]->getState() == 1)
                dOn++;
        
        std::set<size_t> present;
        
        // sample states
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            unsigned int ancS = nodeParentState[*it]->getState();
            unsigned int desS1 = trunkChildState[*it]->getState();
            unsigned int desS2 = budChildState[*it]->getState();
                       
            // Would prefer to sample from f(H_N, H_L, H_R, X_N, X_T, X_B | X_L, X_R, X_A).
            // Unfortunately, this does not seem to scale well with N, since it requires summing
            // over (X_T,X_B) to compute the conditional probability.
            
            // For now, nice n' dumb.
            double u = GLOBAL_RNG->uniform01();
            
            double g0 = nodeTpMatrix[ancS][0] * trunkTpMatrix[0][desS1] * budTpMatrix[0][desS2];
            double g1 = nodeTpMatrix[ancS][1] * trunkTpMatrix[1][desS1] * budTpMatrix[1][desS2];

            // "Overcount", then thin through cladogenesis below
            // Expected freq(X=0|C) after cladogenesis given X=1 prior to speciation
            double c0 = csf[0] * (1.0)+ csf[1] * 0.5 * (1 + 1.0/numCharacters) + csf[2] * 0.5;
            g0 *= c0;
            
            unsigned int s = 0;
//            std::cout << node->getAge() << " " << *it << " " << ( rm.isAreaAvailable(*it, node->getAge()) ? "1" : "0") << "\n";
            if (u < (g1 / (g0 + g1)) && rm.isAreaAvailable(*it, node->getAge()))
            {
                s = 1;
                present.insert(*it);
            }
            
//            s = (GLOBAL_RNG->uniform01() < 0.5 ? 0 : 1);
            
            nodeChildState[*it]->setState(s);
        }
        
        // cladogenesis
        if (present.size() > 1 && p->useCladogenicEvents() == true)
        {
            double u = GLOBAL_RNG->uniform01();
            
            size_t uIdx = 0;
            for (size_t i = 0; i < csf.size(); i++)
            {
                u -= csf[i];
                if (u < 0.0)
                    break;
                uIdx++;
            }
            
            // wide sympatry ABCD->ABCD|ABCD
            if (uIdx == 0)
            {
                proposedCladogenicState = 1;
                for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
                {
                    unsigned int s = nodeChildState[*it]->getState();
                    budParentState[*it]->setState(s);
                    trunkParentState[*it]->setState(s);
                }
            }
            
            // subset sympatry ABCD->A|ABCD
            else if (uIdx == 1)
            {
                proposedCladogenicState = 2;
                std::set<size_t>::iterator it = present.begin();
                for (size_t i = 0; i < GLOBAL_RNG->uniform01() * present.size() + 1; i++)
                    it++;
                size_t budAreaIdx = *it;
                
                for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
                {
                    unsigned int s = nodeChildState[*it]->getState();
                    trunkParentState[*it]->setState(s);
                    
                    unsigned int sb = 0;
                    if (budAreaIdx == *it)
                        sb = 1;
                    budParentState[*it]->setState(sb);
                }
            }
            
            // allopatry ABCD->AB|CD
            else if (uIdx == 2)
            {
                proposedCladogenicState = 3;
                std::vector<size_t> trunkAreas(numCharacters,0);
                std::set<size_t>::iterator it = present.begin();
                for (it = present.begin(); it != present.end(); it++)
                {
                    if (GLOBAL_RNG->uniform01() < 0.5)
                    {
                        trunkAreas[*it] = 1;
                    }
                }

                for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
                {
                    unsigned int s = nodeChildState[*it]->getState();
                    unsigned int st = s * trunkAreas[*it];
                    unsigned int sb = s * (trunkAreas[*it] == 1 ? 0 : 1);
                  
                    trunkParentState[*it]->setState(st);
                    budParentState[*it]->setState(sb);
                }
            }
            
            lnP += log( csf[storedCladogenicState-1] / csf[proposedCladogenicState-1] );
        }
        else
        {
            // narrow sympatry
            proposedCladogenicState = 0;
            for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
            {
                unsigned int s = nodeChildState[*it]->getState();
                budParentState[*it]->setState(s);
                trunkParentState[*it]->setState(s);
            }
        }
        
//        for (size_t i = 0; i < nodeChildState.size(); i++)
//            if (nodeChildState[i]->getState() == 1)
//                dOn--;
//        
//        if (dOn < 0)
//            lnP += 2;

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
    
//    
//    if (node->isRoot())
//    {
//        std::cout << "root reject\n";
//    }
    
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
