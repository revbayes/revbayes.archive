#ifndef FixedNodeheightPruneAndRegraftCharacterHistoryProposal_H
#define FixedNodeheightPruneAndRegraftCharacterHistoryProposal_H

#include <string>

#include "Proposal.h"
#include "StochasticNode.h"
#include "Tree.h"

namespace RevBayesCore {
    
    /**
     * The fixed node-height prune-and-regraft operator.
     *
     * A fixed node-height prune-and-regraft proposal is a SPR (subtree prune-and-regraft) proposal on rooted trees without changing the node age.
     * That is, we pick a random node which is not the root.
     * Then, we prune this node and try to attach it anywhere else in the tree at this node age.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-07-12, version 1.0
     *
     */
    class FixedNodeheightPruneAndRegraftCharacterHistoryProposal : public Proposal {
        
    public:
        FixedNodeheightPruneAndRegraftCharacterHistoryProposal( StochasticNode<Tree> *n);                                   //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                        //!< Clean up proposal
        FixedNodeheightPruneAndRegraftCharacterHistoryProposal* clone(void) const;                                          //!< Clone object
        double                                  doProposal(void);                                           //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                    prepareProposal(void);                                      //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                    tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // private helper methods
        void                                    findNewBrothers(std::vector<TopologyNode*> &b, TopologyNode &p, TopologyNode *n);

        // parameters
        StochasticNode<Tree>*                   variable;                                                   //!< The variable the Proposal is working on
        
        // stored objects to undo proposal
        bool                                    failed;
        TopologyNode*                           storedBrother;
        TopologyNode*                           storedNewBrother;
    };
    
}

#endif

