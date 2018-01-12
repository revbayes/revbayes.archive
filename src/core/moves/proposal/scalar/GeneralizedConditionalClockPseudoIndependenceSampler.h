#ifndef GeneralizedConditionalClockISProposal_H
#define GeneralizedConditionalClockISProposal_H

#include "ContinuousStochasticNode.h"
#include "RbVector.h"
#include "Proposal.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief A rough approximation to an independence sampler on the clock rate for a strict-clock tree.
     *
     * @author The RevBayes Development Core Team
     * @copyright GPL version 3
     * @since 2015-05-21, version 1.0
     *
     */
    class GeneralizedConditionalClockISProposal : public Proposal {
        
    public:
        GeneralizedConditionalClockISProposal(StochasticNode<double>* n, TypedDagNode<double> *stl, double s, int x, int N, bool l);                             //!< Constructor
        
        void                                        cleanProposal(void);                                                                //!< Clean up proposal
        GeneralizedConditionalClockISProposal*                       clone(void) const;                                                                  //!< Clone object
        double                                      doProposal(void);                                                                   //!< Perform proposal
        const std::string&                          getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                        printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                        prepareProposal(void);                                                              //!< Prepare the proposal
        void                                        tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                        undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                        swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
        StochasticNode<double>                     *variable;                                                                             //!< The clock rate parameter on which the Proposal acts.
        TypedDagNode<double>                       *substitutionTreeLength;                                                                             //!< The tree.
        double                                      sigma;                                                                             //!< The variance of the proposal
        double                                      nInv;                                                                             //!< Number of invariant sites in alignment
        double                                      nSites;                                                                             //!< Number of sites in alignment
        bool                                        isLogScale;                                                                             //!< Is the rate variable really log(rate)?
        double                                      storedValue;                                                                        //!< The stored value of the last modified element.
        
        
    };
    
}

#endif

