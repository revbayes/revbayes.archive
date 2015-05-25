#ifndef DirichletSimplexProposal_H
#define DirichletSimplexProposal_H

#include <set>
#include <string>

#include "Proposal.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The Dirichlet-simplex operator.
     *
     * A Dirichlet-simplex proposal randomly changes some values of a simplex, although the other values
     * change too because of the renormalization.
     * First, some random indices are drawn. Then, the proposal draws a new somplex 
     *   u ~ Dirichlet(val[index] * alpha)
     * where alpha is the tuning parameter.The new value is set to u.
     * The simplex is then renormalized.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    class DirichletSimplexProposal : public Proposal {
        
    public:
        DirichletSimplexProposal( StochasticNode<RbVector<double>> *n, double a, size_t nc, double o, double k=0.0);                                                                    //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                                                //!< Clean up proposal
        DirichletSimplexProposal*               clone(void) const;                                                                  //!< Clone object
        double                                  doProposal(void);                                                                   //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                    prepareProposal(void);                                                              //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                    tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
        StochasticNode<RbVector<double> >*      variable;                                                                           //!< The variable the Proposal is working on
        RbVector<double>                        storedValue;                                                                        //!< The stored value of the Proposal used for rejections.
        double                                  alpha;                                                                             //!< The scaling parameter of the Proposal
        size_t                                  nCategories;
        double                                  offset;
        double									kappa;
//        double                                  proposedValue;                                                                      //!< The value we propose.
    };
    
}

#endif

