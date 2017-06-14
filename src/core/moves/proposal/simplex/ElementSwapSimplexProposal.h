#ifndef ElementSwapSimplexProposal_H
#define ElementSwapSimplexProposal_H

#include <set>
#include <string>

#include "SimpleProposal.h"
<<<<<<< HEAD
=======
#include "Simplex.h"
>>>>>>> development
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The element swap simplex operator.
     *
     * An element swap simplex proposal simply selects two random elements of a simplex
     * and swaps them.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
<<<<<<< HEAD
    class ElementSwapSimplexProposal : public SimpleProposal<RbVector<double> > {
        
    public:
        ElementSwapSimplexProposal( StochasticNode<RbVector<double> > *n );                                                //!<  constructor
=======
    class ElementSwapSimplexProposal : public SimpleProposal<Simplex> {
        
    public:
        ElementSwapSimplexProposal( StochasticNode<Simplex> *n );                                                //!<  constructor
>>>>>>> development
        
        // Basic utility functions
        void                                    cleanProposal(void);                                                                //!< Clean up proposal
        ElementSwapSimplexProposal*             clone(void) const;                                                                  //!< Clone object
<<<<<<< HEAD
        double                                  propose(RbVector<double> &v);                                                       //!< Perform proposal
=======
        double                                  propose(Simplex &v);                                                       //!< Perform proposal
>>>>>>> development
        const std::string&                      getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                    prepareProposal(void);                                                              //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                    tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
<<<<<<< HEAD
        RbVector<double>                        storedValue;                                                                        //!< The stored value of the Proposal used for rejections.
=======
        Simplex                                 storedValue;                                                                        //!< The stored value of the Proposal used for rejections.
>>>>>>> development
        //        double                              proposedValue;                                                                      //!< The value we propose.
    };
    
}

<<<<<<< HEAD
#endif
=======
#endif
>>>>>>> development
