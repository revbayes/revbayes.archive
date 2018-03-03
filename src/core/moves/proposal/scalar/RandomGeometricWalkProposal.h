#ifndef RandomGeometricWalkProposal_H
#define RandomGeometricWalkProposal_H

#include "Proposal.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The random-interger-walk operator.
     *
     * This is a very simple move on integer numbers that proposes with probability p = 0.5
     * to increase the current value and with probability p = 0.5 to decrease the
     * current value. The number of steps taken by this move (value by which the current value is increased or decrease)
     * is geometrically distributed.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    class RandomGeometricWalkProposal : public Proposal {
        
    public:
        RandomGeometricWalkProposal( StochasticNode<long> *n, double a);                                                                    //!<  constructor
        
        // Basic utility functions
        void                                cleanProposal(void);                                                                //!< Clean up proposal
        RandomGeometricWalkProposal*        clone(void) const;                                                                  //!< Clone object
        double                              doProposal(void);                                                                   //!< Perform proposal
        const std::string&                  getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                prepareProposal(void);                                                              //!< Prepare the proposal
        void                                printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
        StochasticNode<long>*               variable;                                                                           //!< The variable the Proposal is working on
        long                                stored_value;                                                                        //!< The stored value of the Proposal used for rejections.
        double                              alpha;
        
    };
    
}

#endif

