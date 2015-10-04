#include "EmpiricalTreeProposal.h"
#include "EmpiricalTreeDistribution.h"

using namespace RevBayesCore;

EmpiricalTreeProposal::EmpiricalTreeProposal( StochasticNode<Tree> *v) : Proposal(),
    variable( v )
{
    
    addNode( variable );
    
}


/** Clone object */
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
    
    // get the current tree index
    oldTreeIndex = dist.getCurrentTreeIndex();
    
    // draw a new tree
    dist.redrawValue();
    
    variable->touch( true );
    
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
    dist.setCurrentTree( oldTreeIndex );
    
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

