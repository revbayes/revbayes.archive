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

void EmpiricalTreeDistribution::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, long &rv) const
{
    
    if ( n == "getTreeIndex" )
    {
        rv = long(current_tree_index) + 1;
    }
    else
    {
        throw RbException("A empirical-tree distribution does not have a member method called '" + n + "'.");
    }
    
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


void EmpiricalTreeDistribution::setValue(Tree *v, bool f)
{
    
    bool found = false;
    for(size_t i = burnin; i < trace.size(); ++i)
    {
        if(trace.objectAt(i) == *v)
        {
            found = true;
            current_tree_index = i;
            break;
        }
    }
    
    if(found == false)
    {
        RbException("The starting tree is not in the empirical tree sample.");
    }
    
    TypedDistribution<Tree>::setValue(v, f);
    
}


double EmpiricalTreeDistribution::computeLnProbability( void )
{
    
    return log( probability );
    
}


void EmpiricalTreeDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
}
