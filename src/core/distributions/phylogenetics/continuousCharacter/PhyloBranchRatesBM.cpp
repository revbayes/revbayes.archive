#include "PhyloBranchRatesBM.h"

#include <stddef.h>
#include <cmath>

#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "Cloner.h"
#include "RbVectorImpl.h"
#include "Tree.h"
#include "TypedDagNode.h"



using namespace RevBayesCore;



// constructor(s)
PhyloBranchRatesBM::PhyloBranchRatesBM(const TypedDagNode< Tree > *t, const TypedDagNode< double >* r, const TypedDagNode< double >* s, const TypedDagNode< double >* d): TypedDistribution< RbVector<double> >(new RbVector<double>()),
    tau( t ),
    root_state( r ),
    sigma( s ),
    drift( d )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( root_state );
    addParameter( sigma );
    addParameter( drift );
    
    simulate();
}



PhyloBranchRatesBM* PhyloBranchRatesBM::clone(void) const
{
    return new PhyloBranchRatesBM( *this );
}


double PhyloBranchRatesBM::computeLnProbability(void)
{
    size_t n_tips = tau->getValue().getNumberOfTips()-1;
    std::vector<double> node_values = std::vector<double>(n_tips-1, 0.0);
    node_values[n_tips-2] = root_state->getValue();
    double ln_prob = recursiveLnProb(tau->getValue().getRoot(), node_values);
    
    return ln_prob;
}


double PhyloBranchRatesBM::recursiveLnProb( const TopologyNode& node, std::vector<double> &parent_values )
{
    
    double ln_prob = 0.0;
    size_t index = node.getIndex();
    size_t n_tips = tau->getValue().getNumberOfTips();
//    double val = (*value)[index];
    
    if ( node.isRoot() == false )
    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        size_t parent_index = node.getParent().getIndex();
        double parent_value = parent_values[parent_index - n_tips];
        double standDev = sigma->getValue() * sqrt(node.getBranchLength());
        double mean = (*value)[parent_index] + drift->getValue() * node.getBranchLength();
        ln_prob += RbStatistics::Normal::lnPdf(parent_value, standDev, mean);
    }
    
    // propagate forward
    size_t num_children = node.getNumberOfChildren();
    
    for (size_t i = 0; i < num_children; ++i)
    {
        ln_prob += recursiveLnProb(node.getChild(i), parent_values);
    }
    
    return ln_prob;
    
}

void PhyloBranchRatesBM::redrawValue(void)
{
    simulate();
}


void PhyloBranchRatesBM::simulate()
{
    
    recursiveSimulate(tau->getValue().getRoot());
}


void PhyloBranchRatesBM::recursiveSimulate(const TopologyNode& from)
{
    
    size_t index = from.getIndex();
    
    if (! from.isRoot())
    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        double standDev = sigma->getValue() * sqrt(from.getBranchLength());
        double mean = (*value)[upindex] + drift->getValue() * from.getBranchLength();
        
        // simulate the new Val
        RandomNumberGenerator* rng = GLOBAL_RNG;
        (*value)[index] = RbStatistics::Normal::rv( mean, standDev, *rng);
        
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i)
    {
        recursiveSimulate(from.getChild(i));
    }
    
}

/** Swap a parameter of the distribution */
void PhyloBranchRatesBM::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == tau )
    {
        tau = static_cast< const TypedDagNode<Tree> * >( newP );
    }
    
    if ( oldP == sigma )
    {
        sigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == drift )
    {
        drift = static_cast< const TypedDagNode< double > * >( newP );
    }
    
}
