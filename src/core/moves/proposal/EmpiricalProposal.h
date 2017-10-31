#ifndef EmpiricalProposal_H
#define EmpiricalProposal_H

#include <string>

#include "EmpiricalDistribution.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * An empirical distribution proposal.
     *
     * Propose a new value from the empirical distribution.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Will Pett)
     * @since 2012-07-12, version 1.0
     *
     */
    template <class valueType>
    class EmpiricalProposal : public Proposal {
        
    public:
        EmpiricalProposal( StochasticNode<valueType> *n, bool mh);                                               //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                        //!< Clean up proposal
        EmpiricalProposal*                      clone(void) const;                                          //!< Clone object
        double                                  doProposal(void);                                           //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                    prepareProposal(void);                                      //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                    tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        StochasticNode<valueType>*              variable;
        size_t                                  old_index;
        bool                                    metropolisHastings;

    };
    
}


template <class valueType>
RevBayesCore::EmpiricalProposal<valueType>::EmpiricalProposal( RevBayesCore::StochasticNode<valueType> *v, bool mh) : RevBayesCore::Proposal(),
    variable( v ),
    old_index(),
    metropolisHastings( mh )
{

    addNode( variable );

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
template <class valueType>
RevBayesCore::EmpiricalProposal<valueType>* RevBayesCore::EmpiricalProposal<valueType>::clone( void ) const
{

    return new EmpiricalProposal( *this );
}


/** Clean the proposal */
template <class valueType>
void RevBayesCore::EmpiricalProposal<valueType>::cleanProposal( void )
{

    // nothing to do
}


template <class valueType>
const std::string& RevBayesCore::EmpiricalProposal<valueType>::getProposalName( void ) const
{
    static std::string name = "EmpiricalMove";

    return name;
}



/** Perform the move */
template <class valueType>
double RevBayesCore::EmpiricalProposal<valueType>::doProposal( void )
{

    EmpiricalDistribution<valueType>& dist = static_cast<EmpiricalDistribution<valueType> &>( variable->getDistribution() );

    size_t total_tree_samples = dist.getNumberOfSamples();
    size_t burnin = dist.getBurnin();

    // just in case there is only one tree in the empirical tree sample
    if (total_tree_samples - burnin > 1)
    {
        // get the current tree index
        old_index = dist.getCurrentIndex();

        // draw a new tree
        // in the old way we simply proposed any tree, even the current tree
        // dist.redrawValue();
        // we try to be more efficient by not proposing the same tree again
        RandomNumberGenerator* rng = GLOBAL_RNG;

        // draw a new tree index until it is different from the old tree index
        double u = rng->uniform01();
        size_t new_tree_index = (size_t)( u * (total_tree_samples - burnin) + burnin );

        while ( new_tree_index == old_index )
        {
            u = rng->uniform01();
            new_tree_index = (size_t)( u * (total_tree_samples - burnin) + burnin );
        }

        dist.setCurrentIndex( new_tree_index );

    }

    variable->touch( true );

    if (metropolisHastings == false)
    {
        return RbConstants::Double::inf;
    }

    return 0.0;
}


/** Prepare the proposal */
template <class valueType>
void RevBayesCore::EmpiricalProposal<valueType>::prepareProposal( void )
{

    // nothing to do
}



/** Print the parameter summary */
template <class valueType>
void RevBayesCore::EmpiricalProposal<valueType>::printParameterSummary(std::ostream &o) const
{

    // nothing to do
}


template <class valueType>
void RevBayesCore::EmpiricalProposal<valueType>::undoProposal( void )
{

    // reset to the old tree
    EmpiricalDistribution<valueType>& dist = static_cast<EmpiricalDistribution<valueType> &>( variable->getDistribution() );
    dist.setCurrentIndex( old_index );

}


/** Tune the proposal */
template <class valueType>
void RevBayesCore::EmpiricalProposal<valueType>::tune(double r)
{

    // nothing to do
}


template <class valueType>
void RevBayesCore::EmpiricalProposal<valueType>::swapNodeInternal(RevBayesCore::DagNode *oldN, RevBayesCore::DagNode *newN)
{

    variable = static_cast<StochasticNode<valueType>* >( newN );

}


#endif

