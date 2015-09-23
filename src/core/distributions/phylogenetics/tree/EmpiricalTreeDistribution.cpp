#include "EmpiricalTreeDistribution.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "RbException.h"
#include "Tree.h"
#include "TreeTrace.h"

using namespace RevBayesCore;



EmpiricalTreeDistribution::EmpiricalTreeDistribution(const TreeTrace &t, int b) : TypedDistribution<Tree>( new Tree() ),
    probability( 0.0 ),
    burnin( b ),
    trace( t )
{
    // make sure burnin is proper
    if (trace.size() <= burnin)
    {
        throw RbException("Burnin size is too large for the trace.");
    }
    
    if (burnin == -1)
    {
        burnin = trace.size() / 4;
    }
    else
    {
        burnin = (size_t)(b);
    }
    
    // all the trees have the same probability
    probability = 1 / (double)(trace.size() - burnin);
    
    // draw the first value
    redrawValue();
}


EmpiricalTreeDistribution::~EmpiricalTreeDistribution()
{
    
}


EmpiricalTreeDistribution* EmpiricalTreeDistribution::clone( void ) const
{
    
    return new EmpiricalTreeDistribution( *this );
}


void EmpiricalTreeDistribution::redrawValue( void )
{
    
    // draw a random tree
    RandomNumberGenerator* rng = GLOBAL_RNG;
    size_t total_tree_samples = trace.size();
    current_tree_index = (size_t)( rng->uniform01() * (total_tree_samples - burnin) + burnin - 1 );
    
    // reset the listeners
    const std::set<TreeChangeEventListener*> l = this->value->getTreeChangeEventHandler().getListeners();
    
    
    Tree *psi = new Tree( trace.objectAt(current_tree_index) );
    for (std::set<TreeChangeEventListener*>::const_iterator it = l.begin(); it != l.end(); ++it)
    {
        this->value->getTreeChangeEventHandler().removeListener( *it );
        psi->getTreeChangeEventHandler().addListener( *it );
    }
    
    delete this->value;
    this->value = psi;
    
}


size_t EmpiricalTreeDistribution::getCurrentTreeIndex( void )
{
    
    return current_tree_index;
    
}


void EmpiricalTreeDistribution::setCurrentTree( size_t index )
{
    
    current_tree_index = index;
    
    // reset the listeners
    const std::set<TreeChangeEventListener*> l = this->value->getTreeChangeEventHandler().getListeners();
    
    Tree *psi = new Tree( trace.objectAt(current_tree_index) );
    for (std::set<TreeChangeEventListener*>::const_iterator it = l.begin(); it != l.end(); ++it)
    {
        this->value->getTreeChangeEventHandler().removeListener( *it );
        psi->getTreeChangeEventHandler().addListener( *it );
    }
    
    delete this->value;
    this->value = psi;
    
}


double EmpiricalTreeDistribution::computeLnProbability( void )
{
    
    return log( probability );
    
}


void EmpiricalTreeDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
}
