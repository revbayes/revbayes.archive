#include "EmpiricalTreeDistribution.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "RbException.h"
#include "Tree.h"
#include "TraceTree.h"

using namespace RevBayesCore;



EmpiricalTreeDistribution::EmpiricalTreeDistribution(const TraceTree &t, int b) : TypedDistribution<Tree>( new Tree() ),
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


size_t EmpiricalTreeDistribution::getBurnin( void ) const
{
    
    return burnin;
    
}


size_t EmpiricalTreeDistribution::getCurrentTreeIndex( void ) const
{
    
    return current_tree_index;
    
}


size_t EmpiricalTreeDistribution::getNumberOfTrees( void ) const
{
    
    return trace.size();
    
}


void EmpiricalTreeDistribution::redrawValue( void )
{
    
    // draw a random tree
    RandomNumberGenerator* rng = GLOBAL_RNG;
    size_t total_tree_samples = trace.size();
    double u = rng->uniform01();
    current_tree_index = (size_t)( u * (total_tree_samples - burnin) + burnin );
    
    Tree *psi = new Tree( trace.objectAt(current_tree_index) );
    
    delete this->value;
    this->value = psi;
    
}


void EmpiricalTreeDistribution::setCurrentTree( size_t index )
{
    
    current_tree_index = index;
    
    Tree *psi = new Tree( trace.objectAt(current_tree_index) );
    
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
