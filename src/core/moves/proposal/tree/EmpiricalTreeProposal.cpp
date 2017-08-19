#include "EmpiricalTreeProposal.h"
#include "EmpiricalTreeDistribution.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "Move.h"
#include "RbConstants.h"

using namespace RevBayesCore;

EmpiricalTreeProposal::EmpiricalTreeProposal( StochasticNode<Tree> *v, bool mh) : Proposal(),
    variable( v ),
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
EmpiricalTreeProposal* EmpiricalTreeProposal::clone( void ) const
{
    
    return new EmpiricalTreeProposal( *this );
}


/** Clean the proposal */
void EmpiricalTreeProposal::cleanProposal( void )
{
    
    // nothing to do
}

const std::string& EmpiricalTreeProposal::getProposalName( void ) const
{
    static std::string name = "EmpiricalTreeMove";
    
    return name;
}



/** Perform the move */
double EmpiricalTreeProposal::doProposal( void )
{
    
    EmpiricalTreeDistribution& dist = static_cast<EmpiricalTreeDistribution &>( variable->getDistribution() );
    
    size_t total_tree_samples = dist.getNumberOfTrees();
    size_t burnin = dist.getBurnin();
    
    // just in case there is only one tree in the empirical tree sample
    if(total_tree_samples - burnin > 1)
    {
        // get the current tree index
        old_tree_index = dist.getCurrentTreeIndex();
        
        // draw a new tree
        // in the old way we simply proposed any tree, even the current tree
        // dist.redrawValue();
        // we try to be more efficient by not proposing the same tree again
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // draw a new tree index until it is different from the old tree index
        double u = rng->uniform01();
        size_t new_tree_index = (size_t)( u * (total_tree_samples - burnin) + burnin );
        
        while ( new_tree_index == old_tree_index )
        {
            u = rng->uniform01();
            new_tree_index = (size_t)( u * (total_tree_samples - burnin) + burnin );
        }
        
        dist.setCurrentTree( new_tree_index );
        
    }
    
    variable->touch( true );
    
    if(metropolisHastings == false)
    {
        return RbConstants::Double::inf;
    }
    
    return 0.0;
}


/** Prepare the proposal */
void EmpiricalTreeProposal::prepareProposal( void )
{
    
    // nothing to do
}



/** Print the parameter summary */
void EmpiricalTreeProposal::printParameterSummary(std::ostream &o) const
{
    
    // nothing to do
}


void EmpiricalTreeProposal::undoProposal( void )
{
    
    // reset to the old tree
    EmpiricalTreeDistribution& dist = static_cast<EmpiricalTreeDistribution &>( variable->getDistribution() );
    dist.setCurrentTree( old_tree_index );
    
}


/** Tune the proposal */
void EmpiricalTreeProposal::tune(double r)
{
    
    // nothing to do
}


void EmpiricalTreeProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<Tree>* >( newN );
    
}

