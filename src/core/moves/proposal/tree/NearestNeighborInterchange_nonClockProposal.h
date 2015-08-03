#ifndef NearestNeighborInterchange_nonClockProposal_H
#define NearestNeighborInterchange_nonClockProposal_H

#include <string>

#include "Proposal.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    /**
     * The subtree-prune-and-regraft operator.
     *
     * A subtree-prune-and-regraft proposal is a SPR proposal on unrooted trees without changing the branch lengths.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-07-12, version 1.0
     *
     */
    class NearestNeighborInterchange_nonClockProposal : public Proposal {
        
    public:
        NearestNeighborInterchange_nonClockProposal( StochasticNode<Topology> *n );                                               //!<  constructor
        
        // Basic utility functions
        void                                                cleanProposal(void);                                        //!< Clean up proposal
        NearestNeighborInterchange_nonClockProposal*        clone(void) const;                                          //!< Clone object
        double                                              doProposal(void);                                           //!< Perform proposal
        const std::string&                                  getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                                prepareProposal(void);                                      //!< Prepare the proposal
        void                                                printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                                tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                                undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        void                                                swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        // member variables
        StochasticNode<Topology>*                           tree;
        
        // stored objects to undo proposal
        TopologyNode*                                       storedChoosenNode;
        TopologyNode*                                       storedUncle;
        
    };
    
}

#endif

