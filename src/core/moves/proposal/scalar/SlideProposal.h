#ifndef SlideProposal_H
#define SlideProposal_H

#include <set>
#include <string>

#include "Proposal.h"
#include "ContinuousStochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The sliding operator.
     *
     * A sliding proposal draws a random uniform number u ~ unif(-0.5,0.5)
     * and Slides the current vale by a scaling factor
     * sf = exp( lambda * u )
     * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    class SlideProposal : public Proposal {
        
    public:
        SlideProposal( ContinuousStochasticNode *n, double l);                                                                      //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                                                //!< Clean up proposal
        SlideProposal*                          clone(void) const;                                                                  //!< Clone object
        double                                  doProposal(void);                                                                   //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                    printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                    prepareProposal(void);                                                              //!< Prepare the proposal
        void                                    tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
    private:
        // parameters
        
        ContinuousStochasticNode*               variable;                                                                           //!< The variable the Proposal is working on
        double                                  storedValue;                                                                        //!< The stored value of the Proposal used for rejections.
        double                                  lambda;                                                                             //!< The value we propose.
    };
    
}

#endif

