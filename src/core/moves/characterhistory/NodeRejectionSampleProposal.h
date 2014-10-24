//
//  NodeRejectionSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/7/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__NodeRejectionSampleProposal__
#define __rb_mlandis__NodeRejectionSampleProposal__

#include "AbstractProposal.h"
#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "DistributionPoisson.h"
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
    class NodeRejectionSampleProposal : public AbstractProposal {
        
    public:
        NodeRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, double l, TopologyNode* nd=NULL );                                                                //!<  constructor
        
        // Basic utility functions
        void                            assignNode(TopologyNode* nd);
        void                            assignSiteIndexSet(const std::set<size_t>& s);
        NodeRejectionSampleProposal*    clone(void) const;                                                                  //!< Clone object
        void                            cleanProposal(void);
        double                          doProposal(void);                                                                   //!< Perform proposal
        const std::set<DagNode*>&       getNodes(void) const;                                                               //!< Get the vector of DAG nodes this proposal is working on
        const std::string&              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        void                            sampleNodeCharacters(const std::set<size_t>& indexSet);   //!< Sample the characters at the node
//        void                            sampleNodeCharacters2(const TopologyNode& node, const std::set<size_t>& indexSet);   //!< Sample the characters at the node
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
        std::vector<unsigned>                   storedRootState;
//        std::vector<CharacterEvent*>            storedNodeState2;
//        std::vector<CharacterEvent*>            storedRootState2;
//        
//        std::map<size_t,BranchHistory*>         storedValues;
//        std::map<size_t,BranchHistory*>         proposedValues;
//        BranchHistory*                          storedValue;
        TopologyNode*                           node;
//        int                                     nodeIndex;
        int                                     monitorIndex;
        std::set<size_t>                        siteIndexSet;
        double                                  storedLnProb;
//        BranchHistory*                          proposedValue;
        double                                  proposedLnProb;
        
        PathRejectionSampleProposal<charType,treeType>* nodeProposal;
        PathRejectionSampleProposal<charType,treeType>* leftProposal;
        PathRejectionSampleProposal<charType,treeType>* rightProposal;
        
        TransitionProbabilityMatrix nodeTpMatrix;
        TransitionProbabilityMatrix leftTpMatrix;
        TransitionProbabilityMatrix rightTpMatrix;
        
        double                                  lambda;
        
        // flags
        bool                                    fixNodeIndex;
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
RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::NodeRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, double l, TopologyNode* nd) : AbstractProposal(),
ctmc(n),
tau(t),
qmap(q),
numNodes(t->getValue().getNumberOfNodes()),
numCharacters(n->getValue().getNumberOfCharacters()),
numStates(static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates()),
node(nd),
nodeTpMatrix(2),
leftTpMatrix(2),
rightTpMatrix(2),
lambda(l),
sampleNodeIndex(true),
sampleSiteIndexSet(true)

{
    
//    std::cout << numStates << "\n";
    nodes.insert(ctmc);
    nodes.insert(tau);
    nodes.insert(qmap);

    nodeProposal = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    leftProposal = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    rightProposal = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    
    fixNodeIndex = (node != NULL);
}


template<class charType, class treeType>
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::cleanProposal( void )
{
//    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
//    const std::vector<BranchHistory*>& histories = p->getHistories();
    
    nodeProposal->cleanProposal();
    rightProposal->cleanProposal();
    leftProposal->cleanProposal();
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
template<class charType, class treeType>
RevBayesCore::NodeRejectionSampleProposal<charType, treeType>* RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::clone( void ) const
{
    return new NodeRejectionSampleProposal( *this );
}

template<class charType, class treeType>
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::assignNode(TopologyNode* nd)
{
    node = nd;
    sampleNodeIndex = false;
}

template<class charType, class treeType>
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::assignSiteIndexSet(const std::set<size_t>& s)
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
const std::string& RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::getProposalName( void ) const
{
    static std::string name = "NodeRejectionSampleProposal";
    
    return name;
}


/**
 * Get the vector of nodes on which this proposal is working on.
 *
 * \return  Const reference to a vector of nodes pointer on which the proposal operates.
 */
template<class charType, class treeType>
const std::set<RevBayesCore::DagNode*>& RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::getNodes( void ) const
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
double RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::doProposal( void )
{
    proposedLnProb = 0.0;
    
    double proposedLnProbRatio = 0.0;
    
//    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
//    const std::vector<BranchHistory*>& histories = p->getHistories();
    
    // update node value
    
//    if (node->isRoot())
//        sampleRootCharacters(siteIndexSet);

    sampleNodeCharacters(siteIndexSet);
   
    // update 3x incident paths
    proposedLnProbRatio += nodeProposal->doProposal();
    proposedLnProbRatio += leftProposal->doProposal();
    proposedLnProbRatio += rightProposal->doProposal();

    return proposedLnProbRatio;
}


/**
 *
 */
template<class charType, class treeType>
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::prepareProposal( void )
{
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    
    storedLnProb = 0.0;
    proposedLnProb = 0.0;
    
    const treeType& tree = tau->getValue();
//    size_t numTips = tree.getNumberOfTips();
    if (sampleNodeIndex && !fixNodeIndex)
    {
        node = NULL;
        std::vector<TopologyNode*> nds = tree.getNodes();
        while (node == NULL || node->isTip()) {
            size_t idx = GLOBAL_RNG->uniform01() * nds.size(); //numTips + GLOBAL_RNG->uniform01() * (numNodes-numTips);
            node = nds[idx];
        };
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
   
    // store node state values
    storedNodeState.clear();
    storedRootState.clear();
    
    storedNodeState.resize(numCharacters,0);
    const std::vector<CharacterEvent*>& nodeState = p->getHistory(*node).getChildCharacters();
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        unsigned s = nodeState[*it]->getState();
        storedNodeState[*it] = s;
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
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

template<class charType, class treeType>
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::sampleNodeCharacters(const std::set<size_t>& indexSet)
{

    if (!node->isTip())
    {
        AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
        std::vector<BranchHistory*> histories = p->getHistories();
        
        // get transition probs
        const RateMap& rm = qmap->getValue();
        rm.calculateTransitionProbabilities(*node, nodeTpMatrix);
        rm.calculateTransitionProbabilities(node->getChild(0), leftTpMatrix);
        rm.calculateTransitionProbabilities(node->getChild(1), rightTpMatrix);

        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& nodeParentState = histories[node->getIndex()]->getParentCharacters();
        const std::vector<CharacterEvent*>& leftChildState  = histories[node->getChild(0).getIndex()]->getChildCharacters();
        const std::vector<CharacterEvent*>& rightChildState = histories[node->getChild(1).getIndex()]->getChildCharacters();
        
        // states to update
        std::vector<CharacterEvent*> nodeChildState   = histories[node->getIndex()]->getChildCharacters();
        std::vector<CharacterEvent*> leftParentState  = histories[node->getChild(0).getIndex()]->getParentCharacters();
        std::vector<CharacterEvent*> rightParentState = histories[node->getChild(1).getIndex()]->getParentCharacters();
        
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            unsigned int ancS = nodeParentState[*it]->getState();
            unsigned int desS1 = leftChildState[*it]->getState();
            unsigned int desS2 = rightChildState[*it]->getState();
            
            double u = GLOBAL_RNG->uniform01();
            double g0 = nodeTpMatrix[ancS][0] * leftTpMatrix[0][desS1] * rightTpMatrix[0][desS2];
            double g1 = nodeTpMatrix[ancS][1] * leftTpMatrix[1][desS1] * rightTpMatrix[1][desS2];
            
            unsigned int s = 0;
            if (u < (g1 / (g0 + g1)))
                s = 1;

//            if (u < 0.5)
//            {
//                s = 1;
//            }
//            lnProposal += ???
            
            nodeChildState[*it]->setState(s);
            leftParentState[*it]->setState(s);
            rightParentState[*it]->setState(s);
        }
    }
}

template<class charType, class treeType>
double RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::sampleRootCharacters(const std::set<size_t>& indexSet)
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
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::undoProposal( void )
{
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    const std::vector<BranchHistory*>& histories = p->getHistories();
    
    // restore path state
    nodeProposal->undoProposal();
    rightProposal->undoProposal();
    leftProposal->undoProposal();
    
    // restore node state
    std::vector<CharacterEvent*> nodeChildState = histories[node->getIndex()]->getChildCharacters();
    std::vector<CharacterEvent*> leftParentState = histories[node->getChild(0).getIndex() ]->getParentCharacters();
    std::vector<CharacterEvent*> rightParentState = histories[node->getChild(1).getIndex()]->getParentCharacters();

    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        unsigned s = storedNodeState[*it];
        nodeChildState[*it]->setState(s);
        leftParentState[*it]->setState(s);
        rightParentState[*it]->setState(s);
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
}


/**
 * Swap the current ctmc for a new one.
 *
 * \param[in]     oldN     The old ctmc that needs to be replaced.
 * \param[in]     newN     The new ctmc.
 */
template<class charType, class treeType>
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
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
void RevBayesCore::NodeRejectionSampleProposal<charType, treeType>::tune( double rate )
{
    ; // do nothing
}

#endif /* defined(__rb_mlandis__NodeRejectionSampleProposal__) */
