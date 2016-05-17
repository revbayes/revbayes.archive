#ifndef NodeRejectionSampleProposal_H
#define NodeRejectionSampleProposal_H

#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "HomologousDiscreteCharacterData.h"
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
     * @author The RevBayes Development Core Team (Michael Landis)
     * @since 2009-09-08, version 1.0
     *
     */
    
    template<class charType>
    class NodeRejectionSampleProposal : public Proposal {
        
    public:
        NodeRejectionSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, StochasticNode<Tree>* t, DeterministicNode<RateMap> *q, double l, TopologyNode* nd=NULL );                                                                //!<  constructor
        
        // Basic utility functions
        void                            assignNode(TopologyNode* nd);
        void                            assignSiteIndexSet(const std::set<size_t>& s);
        NodeRejectionSampleProposal*    clone(void) const;                                                                  //!< Clone object
        void                            cleanProposal(void);
        double                          doProposal(void);                                                                   //!< Perform proposal
        const std::string&              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        void                            sampleNodeCharacters(const std::set<size_t>& indexSet);   //!< Sample the characters at the node
//        void                            sampleNodeCharacters2(const TopologyNode& node, const std::set<size_t>& indexSet);   //!< Sample the characters at the node
        double                          sampleRootCharacters(const std::set<size_t>& indexSet);
        void                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                            swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
        // parameters
        StochasticNode<AbstractHomologousDiscreteCharacterData>*  ctmc;
        StochasticNode<Tree>*                   tau;
        DeterministicNode<RateMap>*             qmap;

        // dimensions
        size_t                                  num_nodes;
        size_t                                  numCharacters;
        size_t                                  num_states;
        
        // proposal
        std::vector<size_t>                     storedNodeState;
        std::vector<size_t>                     storedRootState;
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
        
        PathRejectionSampleProposal<charType>* nodeProposal;
        PathRejectionSampleProposal<charType>* leftProposal;
        PathRejectionSampleProposal<charType>* rightProposal;
        
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
template<class charType>
RevBayesCore::NodeRejectionSampleProposal<charType>::NodeRejectionSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, StochasticNode<Tree> *t, DeterministicNode<RateMap>* q, double l, TopologyNode* nd) : Proposal(),
    ctmc(n),
    tau(t),
    qmap(q),
    num_nodes(t->getValue().getNumberOfNodes()),
    numCharacters(n->getValue().getNumberOfCharacters()),
    num_states(q->getValue().getNumberOfStates()),
    node(nd),
    nodeTpMatrix(2),
    leftTpMatrix(2),
    rightTpMatrix(2),
    lambda(l),
    sampleNodeIndex(true),
    sampleSiteIndexSet(true)
{
    
//    std::cout << num_states << "\n";
    addNode( ctmc );
    addNode( tau );
    addNode( qmap );

    nodeProposal = new PathRejectionSampleProposal<charType>(n,t,q,l,nd);
    leftProposal = new PathRejectionSampleProposal<charType>(n,t,q,l,nd);
    rightProposal = new PathRejectionSampleProposal<charType>(n,t,q,l,nd);
    
    fixNodeIndex = (node != NULL);
}


template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::cleanProposal( void )
{
//    AbstractTreeHistoryCtmc<charType>* p = static_cast< AbstractTreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
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
template<class charType>
RevBayesCore::NodeRejectionSampleProposal<charType>* RevBayesCore::NodeRejectionSampleProposal<charType>::clone( void ) const
{
    return new NodeRejectionSampleProposal( *this );
}

template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::assignNode(TopologyNode* nd)
{
    node = nd;
    sampleNodeIndex = false;
}

template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::assignSiteIndexSet(const std::set<size_t>& s)
{
    siteIndexSet = s;
    sampleSiteIndexSet = false;
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
template<class charType>
const std::string& RevBayesCore::NodeRejectionSampleProposal<charType>::getProposalName( void ) const
{
    static std::string name = "NodeRejectionSampleProposal";
    
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
double RevBayesCore::NodeRejectionSampleProposal<charType>::doProposal( void )
{
    proposedLnProb = 0.0;
    
    double proposedLnProbRatio = 0.0;
    
//    AbstractTreeHistoryCtmc<charType>* p = static_cast< AbstractTreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
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
template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::prepareProposal( void )
{
    AbstractTreeHistoryCtmc<charType>* p = static_cast< AbstractTreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
    
    storedLnProb = 0.0;
    proposedLnProb = 0.0;
    
    const Tree& tree = tau->getValue();
//    size_t numTips = tree.getNumberOfTips();
    if (sampleNodeIndex && !fixNodeIndex)
    {
        node = NULL;
        std::vector<TopologyNode*> nds = tree.getNodes();
        while (node == NULL || node->isTip()) {
            size_t idx = GLOBAL_RNG->uniform01() * nds.size(); //numTips + GLOBAL_RNG->uniform01() * (num_nodes-numTips);
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
        size_t s = nodeState[*it]->getState();
        storedNodeState[*it] = s;
    }
    if (node->isRoot())
    {
        storedRootState.resize(numCharacters,0);
        const std::vector<CharacterEvent*>& rootState = p->getHistory(*node).getParentCharacters();
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            size_t s = rootState[*it]->getState();
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
template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::sampleNodeCharacters(const std::set<size_t>& indexSet)
{

    if (!node->isTip())
    {
        AbstractTreeHistoryCtmc<charType>* p = static_cast< AbstractTreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
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
            size_t ancS = nodeParentState[*it]->getState();
            size_t desS1 = leftChildState[*it]->getState();
            size_t desS2 = rightChildState[*it]->getState();
            
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

template<class charType>
double RevBayesCore::NodeRejectionSampleProposal<charType>::sampleRootCharacters(const std::set<size_t>& indexSet)
{
    double lnP = 0.0;
    if (!node->isRoot())
        return 0.0;

    AbstractTreeHistoryCtmc<charType>* p = static_cast< AbstractTreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
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
template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::undoProposal( void )
{
    AbstractTreeHistoryCtmc<charType>* p = static_cast< AbstractTreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
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
        size_t s = storedNodeState[*it];
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
            size_t s = storedRootState[*it];
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
template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::swapNodeInternal(DagNode *oldN, DagNode *newN)
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
    
    nodeProposal->swapNode(oldN, newN);
    leftProposal->swapNode(oldN, newN);
    rightProposal->swapNode(oldN, newN);
}


/**
 * Tune the Proposal to accept the desired acceptance ratio. 
 */
template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::tune( double rate )
{
    ; // do nothing
}

#endif /* defined(__rb_mlandis__NodeRejectionSampleProposal__) */
