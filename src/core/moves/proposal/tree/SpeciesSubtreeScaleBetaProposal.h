#ifndef SpeciesSubtreeScaleBetaProposal_H
#define SpeciesSubtreeScaleBetaProposal_H

#include <string>

#include "Proposal.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    /**
     * The species-subtree-scale operator.
     *
     * A subtree-scale proposal is a scaling proposal on rooted subtrees without changing the topology.
     * That is, we pick a random node which is not the root.
     * Then, we randomly pick an age between the parent and the oldest sampled descendant drawn from a beta distribution.
     * The picked subtree is then scaled to this new age.
     * All gene-trees that are present in the population will be scaled too!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-06-24, version 1.0
     *
     */
    class SpeciesSubtreeScaleBetaProposal : public Proposal {
        
    public:
        SpeciesSubtreeScaleBetaProposal(StochasticNode<TimeTree> *sp, std::vector< StochasticNode<TimeTree> *> gt, double a);                                               //!<  constructor
        
        // Basic utility functions
        void                                            addGeneTree(StochasticNode<TimeTree> *gt);                                      //!< Add a DAG Node holding a gene tree on which this move should operate on
        void                                            cleanProposal(void);                                        //!< Clean up proposal
        SpeciesSubtreeScaleBetaProposal*                clone(void) const;                                          //!< Clone object
        double                                          doProposal(void);                                           //!< Perform proposal
        const std::string&                              getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                            prepareProposal(void);                                      //!< Prepare the proposal
        void                                            printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                            removeGeneTree(StochasticNode<TimeTree> *gt);                                   //!< Remove a DAG Node holding a gene tree on which this move should operate on
        void                                            tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                            undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        std::vector<TopologyNode*>                      getOldestNodesInPopulation( TimeTree &tau, TopologyNode &n );
        void                                            swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        // parameters
        StochasticNode<TimeTree>*                       speciesTree;                                                   //!< The variable the Proposal is working on
        std::vector< StochasticNode<TimeTree> *>        geneTrees;
        
        // tuning parameter
        double                                          alpha;
        
        // stored objects to undo proposal
        TopologyNode*                                   storedNode;
        double                                          storedAge;
        
    };
    
}

#endif

