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
#include "RateGeneratorSequence.h"
#include "RbException.h"
#include "StochasticNode.h"
//#include "TransitionProbability.h"
#include "TreeChangeEventMessage.h"
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
        NodeRejectionSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, double l=1.0, double r=0.234 );                                  //!<  constructor

        // Basic utility functions
        void                                                        assignNode(TopologyNode* nd);
        void                                                        assignSiteIndexSet(const std::set<size_t>& s);
        NodeRejectionSampleProposal*                                clone(void) const;                                              //!< Clone object
        void                                                        cleanProposal(void);
        virtual double                                              computeLnProposal();
        double                                                      doProposal(void);                                               //!< Perform proposal
        const std::string&                                          getProposalName(void) const;                                    //!< Get the name of the proposal for summary printing
        void                                                        printParameterSummary(std::ostream &o) const;                   //!< Print the parameter summary
        void                                                        prepareProposal(void);                                          //!< Prepare the proposal
        std::set<size_t>                                            sampleCharacters(double p);
        void                                                        setSampledCharacters(const std::set<size_t>& s);
        void                                                        sampleNodeCharacters(void);                                     //!< Sample the characters at the node
        double                                                      sampleRootCharacters(void);                                     //!< Sample the characters at the root
        void                                                        setRateGenerator(const TypedDagNode<RateGenerator> *d);         //!< Set the rate generator.
        void                                                        setRateGenerator(const TypedDagNode<RateGeneratorSequence> *d); //!< Set the rate generator.
        void                                                        tune(double r);                                                 //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                                        undoProposal(void);                                             //!< Reject the proposal

    protected:

        void                                                        swapNodeInternal(DagNode *oldN, DagNode *newN);                 //!< Swap the DAG nodes on which the Proposal is working on

        // parameters
        StochasticNode<AbstractHomologousDiscreteCharacterData>*    ctmc;
        const TypedDagNode<RateGenerator>*                          q_map_site;
        const TypedDagNode<RateGeneratorSequence>*                  q_map_sequence;

        // dimensions
        size_t                                                      numCharacters;
        size_t                                                      numStates;

        // proposal
        std::vector<size_t>                                         storedNodeState;

        TopologyNode*                                               node;
        double                                                      storedLnProb;
        double                                                      proposedLnProb;

        PathRejectionSampleProposal<charType>*                      nodeProposal;
        PathRejectionSampleProposal<charType>*                      leftProposal;
        PathRejectionSampleProposal<charType>*                      rightProposal;

        TransitionProbabilityMatrix                                 nodeTpMatrix;
        TransitionProbabilityMatrix                                 leftTpMatrix;
        TransitionProbabilityMatrix                                 rightTpMatrix;
        
        double                                                      lambda;
        std::set<size_t>                                            sampledCharacters;
        std::set<size_t>                                            allCharacters;

    };

}



/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class charType>
RevBayesCore::NodeRejectionSampleProposal<charType>::NodeRejectionSampleProposal( StochasticNode<AbstractHomologousDiscreteCharacterData> *n, double l, double r ) : Proposal(r),
    ctmc(n),
    q_map_site( NULL ),
    q_map_sequence( NULL ),
    numStates(2),
    numCharacters(n->getValue().getNumberOfCharacters()),
    node( NULL ),
    nodeTpMatrix(2),
    leftTpMatrix(2),
    rightTpMatrix(2),
    lambda(l)
{

    addNode( ctmc );

    nodeProposal = new PathRejectionSampleProposal<charType>(n, l, r);
    leftProposal = new PathRejectionSampleProposal<charType>(n, l, r);
    rightProposal = new PathRejectionSampleProposal<charType>(n, l, r);
    
    for (size_t i = 0; i < numCharacters; i++)
    {
        allCharacters.insert(i);
    }

}


template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::cleanProposal( void )
{
    nodeProposal->cleanProposal();
    rightProposal->cleanProposal();
    leftProposal->cleanProposal();

    storedNodeState.clear();
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
}


template<class charType>
double RevBayesCore::NodeRejectionSampleProposal<charType>::computeLnProposal(void)
{
    double lnP = 0.0;

    if ( node->isTip() == false )
    {
        TreeHistoryCtmc<charType>* c = dynamic_cast< TreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
        if ( ctmc == NULL )
        {
            throw RbException("Failed cast.");
        }

        size_t num_sites = c->getNumberOfSites();

        std::vector<BranchHistory*> histories = c->getHistories();

        TopologyNode &left_child  = node->getChild(0);
        TopologyNode &right_child = node->getChild(1);

        double node_age     = node->getAge();
        double left_age     = left_child.getAge();
        double right_age    = right_child.getAge();

        double node_rate  = c->getBranchRate( node->getIndex() );
        double left_rate  = c->getBranchRate( left_child.getIndex() );
        double right_rate = c->getBranchRate( right_child.getIndex() );

        // get transition probs
//        const RateGenerator& rm = q_map_site->getValue();
        const RateGenerator& rm = ( q_map_sequence != NULL ? q_map_sequence->getValue() : q_map_site->getValue() );
        rm.calculateTransitionProbabilities(node_age, left_age,  left_rate, leftTpMatrix);
        rm.calculateTransitionProbabilities(node_age, right_age, right_rate, rightTpMatrix);

        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& leftChildState  = histories[node->getChild(0).getIndex()]->getChildCharacters();
        const std::vector<CharacterEvent*>& rightChildState = histories[node->getChild(1).getIndex()]->getChildCharacters();

        // states to update
        const std::vector<CharacterEvent*>& nodeChildState  = histories[node->getIndex()]->getChildCharacters();

        if ( node->isRoot() == false )
        {
            double parent_age   = node->getParent().getAge();
            rm.calculateTransitionProbabilities(parent_age, node_age, node_rate, nodeTpMatrix);

            const std::vector<CharacterEvent*>& nodeParentState = histories[node->getIndex()]->getParentCharacters();

            std::set<size_t>::iterator it_s;
            //            for (size_t site_index = 0; site_index < num_sites; ++site_index)
            for (it_s = sampledCharacters.begin(); it_s != sampledCharacters.end(); it_s++)
            {
                size_t site_index = *it_s;
                size_t ancS  = nodeParentState[site_index]->getState();
                size_t desS1 = leftChildState[site_index]->getState();
                size_t desS2 = rightChildState[site_index]->getState();

                std::vector<double> state_probs(numStates,0.0);
                double sum = 0.0;
                for ( size_t i=0; i<numStates; ++i )
                {
                    double p = nodeTpMatrix[ancS][i] * leftTpMatrix[i][desS1] * rightTpMatrix[i][desS2];
                    sum += p;
                    state_probs[i] = p;
                }

                size_t s = nodeChildState[site_index]->getState();
                lnP += log(state_probs[s]/sum);
            }

        }
        else
        {

            std::vector<double> rf = c->getRootFrequencies();

            std::set<size_t>::iterator it_s;
            //            for (size_t site_index = 0; site_index < num_sites; ++site_index)
            for (it_s = sampledCharacters.begin(); it_s != sampledCharacters.end(); it_s++)
            {
                size_t site_index = *it_s;
                size_t desS1 = leftChildState[site_index]->getState();
                size_t desS2 = rightChildState[site_index]->getState();


                std::vector<double> state_probs(numStates,0.0);
                double sum = 0.0;
                for ( size_t i=0; i<numStates; ++i )
                {
                    double p = rf[i] * leftTpMatrix[i][desS1] * rightTpMatrix[i][desS2];
                    sum += p;
                    state_probs[i] = p;
                }

                size_t s = nodeChildState[site_index]->getState();
                lnP += log(state_probs[s]/sum);

            }

        }

    }

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
template<class charType>
double RevBayesCore::NodeRejectionSampleProposal<charType>::doProposal( void )
{
    proposedLnProb = 0.0;

    double proposedLnProbRatio = 0.0;

    // backward proposal probability
//    proposedLnProbRatio += computeLnProposal();

    // update node value
    sampleNodeCharacters();

    // forward proposal probability
//    proposedLnProbRatio -= computeLnProposal();

    // update 3x incident paths
    proposedLnProbRatio += nodeProposal->doProposal();
    proposedLnProbRatio += leftProposal->doProposal();
    proposedLnProbRatio += rightProposal->doProposal();

    return proposedLnProbRatio;
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
 *
 */
template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::prepareProposal( void )
{

    TreeHistoryCtmc<charType>* p = dynamic_cast< TreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
    if ( p == NULL )
    {
        throw RbException("Failed cast.");
    }

    storedLnProb = 0.0;
    proposedLnProb = 0.0;

    const Tree& tree = p->getTree();
    std::vector<TopologyNode*> nds = tree.getNodes();
    node = NULL;

    do
    {
        size_t idx = GLOBAL_RNG->uniform01() * nds.size();
        node = nds[idx];
    } while ( node->isTip() == true );


    // prepare the path proposals
    nodeProposal->assignNode(node);
    nodeProposal->prepareProposal();

    leftProposal->assignNode(&node->getChild(0));
    leftProposal->prepareProposal();

    rightProposal->assignNode(&node->getChild(1));
    rightProposal->prepareProposal();


    // store node state values
    storedNodeState.clear();

    const std::vector<CharacterEvent*>& nodeState = p->getHistory(*node).getChildCharacters();
    size_t num_sites = p->getNumberOfSites();
    storedNodeState.resize(num_sites,0);
    for (size_t site_index = 0; site_index < num_sites; ++site_index)
    {
        size_t s = nodeState[site_index]->getState();
        storedNodeState[site_index] = s;
    }
    
    // sample characters to be updated and pass to proposals
    sampledCharacters = sampleCharacters(lambda);
    nodeProposal->setSampledCharacters(sampledCharacters);
    leftProposal->setSampledCharacters(sampledCharacters);
    rightProposal->setSampledCharacters(sampledCharacters);

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
//    o << "lambda = " << lambda;
}

template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::sampleNodeCharacters( void )
{

    if ( node->isTip() == false )
    {
        TreeHistoryCtmc<charType>* c = dynamic_cast< TreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
        if ( ctmc == NULL )
        {
            throw RbException("Failed cast.");
        }

        size_t num_sites = c->getNumberOfSites();

        const std::vector<BranchHistory*>& histories = c->getHistories();

        TopologyNode &left_child  = node->getChild(0);
        TopologyNode &right_child = node->getChild(1);

        double node_age   = node->getAge();
        double left_age   = left_child.getAge();
        double right_age  = right_child.getAge();

        double node_rate  = c->getBranchRate( node->getIndex() );
        double left_rate  = c->getBranchRate( left_child.getIndex() );
        double right_rate = c->getBranchRate( right_child.getIndex() );

        // get transition probs
        //        const RateGenerator& rm = q_map_site->getValue();
        const RateGenerator& rm = ( q_map_sequence != NULL ? q_map_sequence->getValue() : q_map_site->getValue() );
        rm.calculateTransitionProbabilities(node_age, left_age,  left_rate,  leftTpMatrix);
        rm.calculateTransitionProbabilities(node_age, right_age, right_rate, rightTpMatrix);

        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& leftChildState  = histories[left_child.getIndex()]->getChildCharacters();
        const std::vector<CharacterEvent*>& rightChildState = histories[right_child.getIndex()]->getChildCharacters();

        // states to update
        std::vector<CharacterEvent*>& nodeChildState   = histories[node->getIndex()]->getChildCharacters();
        std::vector<CharacterEvent*>& leftParentState  = histories[left_child.getIndex()]->getParentCharacters();
        std::vector<CharacterEvent*>& rightParentState = histories[right_child.getIndex()]->getParentCharacters();

        if ( node->isRoot() == false )
        {
            double parent_age   = node->getParent().getAge();
            rm.calculateTransitionProbabilities(parent_age, node_age, node_rate, nodeTpMatrix);

            const std::vector<CharacterEvent*>& nodeParentState = histories[node->getIndex()]->getParentCharacters();

            std::set<size_t>::iterator it_s;
//            for (size_t site_index = 0; site_index < num_sites; ++site_index)
            for (it_s = sampledCharacters.begin(); it_s != sampledCharacters.end(); it_s++)
            {
                size_t site_index = *it_s;
                size_t ancS  = nodeParentState[site_index]->getState();
                size_t desS1 = leftChildState[site_index]->getState();
                size_t desS2 = rightChildState[site_index]->getState();

                double u = GLOBAL_RNG->uniform01();

                std::vector<double> state_probs(numStates,0.0);
                double sum = 0.0;
                for ( size_t i=0; i<numStates; ++i )
                {
                    double p = nodeTpMatrix[ancS][i] * leftTpMatrix[i][desS1] * rightTpMatrix[i][desS2];
                    sum += p;
                    state_probs[i] = p;
                }

                unsigned int s = 0;
                for ( size_t i=0; i<numStates; ++i )
                {
                    u -= (state_probs[i]/sum);
                    if ( u <= 0.0 )
                    {
//                        s = i;
                        break;
                    }
                    ++s;
                }

                nodeChildState[site_index]->setState(s);
                leftParentState[site_index]->setState(s);
                rightParentState[site_index]->setState(s);
            }

        }
        else
        {

            std::vector<double> rf = c->getRootFrequencies();

            std::set<size_t>::iterator it_s;
//            for (size_t site_index = 0; site_index < num_sites; ++site_index)
            for (it_s = sampledCharacters.begin(); it_s != sampledCharacters.end(); it_s++)
            {
                size_t site_index = *it_s;
                size_t desS1 = leftChildState[site_index]->getState();
                size_t desS2 = rightChildState[site_index]->getState();

                double u = GLOBAL_RNG->uniform01();

                std::vector<double> state_probs(numStates,0.0);
                double sum = 0.0;
                for ( size_t i=0; i<numStates; ++i )
                {
                    double p = rf[i] * leftTpMatrix[i][desS1] * rightTpMatrix[i][desS2];
                    sum += p;
                    state_probs[i] = p;
                }

                unsigned int s = 0;
                for ( size_t i=0; i<numStates; ++i )
                {
                    u -= (state_probs[i]/sum);
                    if ( u <= 0.0 )
                    {
//                        s = i;
                        break;
                    }
                    ++s;
                }

                nodeChildState[site_index]->setState(s);
                leftParentState[site_index]->setState(s);
                rightParentState[site_index]->setState(s);

            }

        }

    }

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
    TreeHistoryCtmc<charType>* p = dynamic_cast< TreeHistoryCtmc<charType>* >(&ctmc->getDistribution());
    if ( p == NULL )
    {
        throw RbException("Failed cast.");
    }
    size_t num_sites = p->getNumberOfSites();

    const std::vector<BranchHistory*>& histories = p->getHistories();

    // restore node state
    std::vector<CharacterEvent*>& nodeChildState = histories[node->getIndex()]->getChildCharacters();
    std::vector<CharacterEvent*>& leftParentState = histories[node->getChild(0).getIndex() ]->getParentCharacters();
    std::vector<CharacterEvent*>& rightParentState = histories[node->getChild(1).getIndex()]->getParentCharacters();

    for (size_t site_index = 0; site_index < num_sites; ++site_index)
    {
        size_t s = storedNodeState[site_index];
        nodeChildState[site_index]->setState(s);
        leftParentState[site_index]->setState(s);
        rightParentState[site_index]->setState(s);
    }

    // restore path state
    nodeProposal->undoProposal();
    rightProposal->undoProposal();
    leftProposal->undoProposal();
    
    // clear sampled character set
    sampledCharacters.clear();
}

template<class charType>
std::set<size_t> RevBayesCore::NodeRejectionSampleProposal<charType>::sampleCharacters(double p)
{
    
    if (p == 1.0)
    {
        return allCharacters;
    }
    
    std::set<size_t> s;
    s.insert(GLOBAL_RNG->uniform01() * numCharacters);
    for (size_t i = 0; i < numCharacters; i++)
    {
        if (GLOBAL_RNG->uniform01() < p)
        {
            s.insert(i);
        }
    }
    return s;
    
}

template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::setSampledCharacters(const std::set<size_t>& s)
{
    sampledCharacters = s;
}


template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::setRateGenerator(const TypedDagNode<RateGenerator> *d)
{

    q_map_site = d;
    numStates = q_map_site->getValue().getNumberOfStates();

    nodeProposal->setRateGenerator( q_map_site );
    leftProposal->setRateGenerator( q_map_site );
    rightProposal->setRateGenerator( q_map_site );

    nodeTpMatrix = TransitionProbabilityMatrix(numStates);
    leftTpMatrix = TransitionProbabilityMatrix(numStates);
    rightTpMatrix = TransitionProbabilityMatrix(numStates);

}


template<class charType>
void RevBayesCore::NodeRejectionSampleProposal<charType>::setRateGenerator(const TypedDagNode<RateGeneratorSequence> *d)
{

    q_map_sequence = d;
    numStates = q_map_sequence->getValue().getNumberOfStates();

    nodeProposal->setRateGenerator( q_map_sequence );
    leftProposal->setRateGenerator( q_map_sequence );
    rightProposal->setRateGenerator( q_map_sequence );

    nodeTpMatrix = TransitionProbabilityMatrix(numStates);
    leftTpMatrix = TransitionProbabilityMatrix(numStates);
    rightTpMatrix = TransitionProbabilityMatrix(numStates);

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
    else if (oldN == q_map_site)
    {
        q_map_site = static_cast<DeterministicNode<RateGenerator>* >(newN);
    }
    else if (oldN == q_map_sequence)
    {
        q_map_sequence = static_cast<DeterministicNode<RateGeneratorSequence>* >(newN);
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
    double p = this->targetAcceptanceRate;
    if ( rate > p )
    {
        lambda /= (1.0 + ((rate-p)/(1.0 - p)));
    }
    else
    {
        lambda *= (2.0 - rate/p);
        if (lambda > 1.0)
            lambda = 1.0;
    }
}

#endif /* defined(__rb_mlandis__NodeRejectionSampleProposal__) */
