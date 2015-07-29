#ifndef SpeciesSubtreeScaleProposal_H
#define SpeciesSubtreeScaleProposal_H

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
     * Then, we uniformly pick an age between the parent and the oldest sampled descendant.
     * The picked subtree is then scaled to this new age.
     * All gene-trees that are present in the population will be scaled too!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-06-16, version 1.0
     *
     */
    class SpeciesSubtreeScaleProposal : public Proposal {
        
    public:
        SpeciesSubtreeScaleProposal(StochasticNode<TimeTree> *sp, std::vector< StochasticNode<TimeTree> *> gt);                                               //!<  constructor
        virtual ~SpeciesSubtreeScaleProposal(void);
        
        // Basic utility functions
        void                                            cleanProposal(void);                                        //!< Clean up proposal
        SpeciesSubtreeScaleProposal*                    clone(void) const;                                          //!< Clone object
        double                                          doProposal(void);                                           //!< Perform proposal
        const std::string&                              getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                            prepareProposal(void);                                      //!< Prepare the proposal
        void                                            printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                            tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                            undoProposal(void);                                         //!< Reject the proposal
        
    protected:

        std::vector<TopologyNode*>                      getOldestNodesInPopulation( TimeTree &tau, TopologyNode &n );
        void                                            swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        // parameters
        StochasticNode<TimeTree>*                       speciesTree;                                                   //!< The variable the Proposal is working on
        std::vector< StochasticNode<TimeTree> *>        geneTrees;
       
        // stored objects to undo proposal
        TopologyNode*                                   storedNode;
        double                                          storedAge;
        
    };
    
}

#endif

