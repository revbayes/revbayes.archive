#ifndef SingleElementScaleMove_H
#define SingleElementScaleMove_H

#include "RbVector.h"
#include "Proposal.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a single element randomly picked from a vector.
     *
     *
     * This move randomly picks an element of a vector of positive real numbers,
     * proposes a scaling factor and then scales the value.
     * The actual scaling factor is computed by sf = exp( lambda * ( u - 0.5 ) )
     * where u ~ Uniform(0,1).
     * It generally makes more sense to apply the scaling move on a vector of positive
     * real numbers but technically it works on negative numbers too. However,
     * the move will never change the sign of the value and thus is incomplete if applied
     * to variable defined on the whole real line.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2015-05-21, version 1.0
     *
     */
    class SingleElementScaleProposal : public Proposal {
        
    public:
        SingleElementScaleProposal(std::vector<StochasticNode<double> *> n, double l);                                 //!< Constructor
        
        void                                        cleanProposal(void);                                                                //!< Clean up proposal
        SingleElementScaleProposal*                 clone(void) const;                                                                  //!< Clone object
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
        
        std::vector<StochasticNode<double> *>       variables;
        
        double                                      lambda;                                                                             //!< The scale parameter of the move (larger lambda -> larger proposals).
        size_t                                      index;                                                                              //!< The index of the last modified element.
        double                                      storedValue;                                                                        //!< The stored value of the last modified element.
        
        
        
    };
    
}

#endif

