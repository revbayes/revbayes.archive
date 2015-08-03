#ifndef EmpiricalTreeProposal_H
#define EmpiricalTreeProposal_H

#include <string>

#include "Proposal.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    /**
     * The narrow-exchange operator.
     *
     * A narrow-exchange proposal is a NNI (nearest neighbour interchange) proposal on rooted trees without changing the node age.
     * That is, we pick a random node which is not the root and neither its parent is the root.
     * Then, we try to exchange the picked node with it's uncle. This move will automatically fail if the uncle is older than the parent.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Will Freyman)
     * @since 2012-07-12, version 1.0
     *
     */
    template <class treeType>
    class EmpiricalTreeProposal : public Proposal {
        
    public:
        EmpiricalTreeProposal( StochasticNode<treeType> *n);                                               //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                        //!< Clean up proposal
        EmpiricalTreeProposal*                  clone(void) const;                                          //!< Clone object
        double                                  doProposal(void);                                           //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                    prepareProposal(void);                                      //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                    tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        StochasticNode<treeType>*               variable;
        size_t                                  oldTreeIndex;
        
    };
    
}

#include "EmpiricalTreeDistribution.h"

template <class treeType>
RevBayesCore::EmpiricalTreeProposal<treeType>::EmpiricalTreeProposal( StochasticNode<treeType> *v) : Proposal(),
    variable( v )
{
    
    addNode( variable );
    
}


/** Clone object */
template <class treeType>
RevBayesCore::EmpiricalTreeProposal<treeType>* RevBayesCore::EmpiricalTreeProposal<treeType>::clone( void ) const
{
    
    return new EmpiricalTreeProposal<treeType>( *this );
}


/** Clean the proposal */
template <class treeType>
void RevBayesCore::EmpiricalTreeProposal<treeType>::cleanProposal( void )
{

    // nothing to do
}

template <class treeType>
const std::string& RevBayesCore::EmpiricalTreeProposal<treeType>::getProposalName( void ) const
{
    static std::string name = "EmpiricalTreeMove";
    
    return name;
}



/** Perform the move */
template <class treeType>
double RevBayesCore::EmpiricalTreeProposal<treeType>::doProposal( void )
{
    
    EmpiricalTreeDistribution<treeType>& dist = static_cast<EmpiricalTreeDistribution<treeType> &>( variable->getDistribution() );
    
    // get the current tree index
    oldTreeIndex = dist.getCurrentTreeIndex();
    
    // draw a new tree
    dist.redrawValue();
    
    variable->touch( true );
    
    return 0.0;
}


/** Prepare the proposal */
template <class treeType>
void RevBayesCore::EmpiricalTreeProposal<treeType>::prepareProposal( void )
{
    
    // nothing to do
}



/** Print the parameter summary */
template <class treeType>
void RevBayesCore::EmpiricalTreeProposal<treeType>::printParameterSummary(std::ostream &o) const
{
    
    // nothing to do
}


template <class treeType>
void RevBayesCore::EmpiricalTreeProposal<treeType>::undoProposal( void )
{
    
    // reset to the old tree
    EmpiricalTreeDistribution<treeType>& dist = static_cast<EmpiricalTreeDistribution<treeType> &>( variable->getDistribution() );
    dist.setCurrentTree( oldTreeIndex );
    
}


/** Tune the proposal */
template <class treeType>
void RevBayesCore::EmpiricalTreeProposal<treeType>::tune(double r)
{
    
    // nothing to do
}


template <class treeType>
void RevBayesCore::EmpiricalTreeProposal<treeType>::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<treeType>* >( newN );
    
}


#endif

