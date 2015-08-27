#include "BranchRateJumpProcess.h"
#include "ConstantNode.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <iostream>
#include <cmath>



using namespace RevBayesCore;



/**
 * Default constructor.
 * The default constructor does nothing except allocating the object.
 *
 * \param[in]   d    The distribution from which the new values are drawn if a jump occured.
 * \param[in]   t    The time tree along which the process is applied.
 * \param[in]   l    A scaling parameter for the branch length.
 * \param[in]   r    The instantaneous jump probability.
 */
BranchRateJumpProcess::BranchRateJumpProcess(TypedDistribution<double> *d, const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* l, const TypedDagNode< double >* r): TypedDistribution< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes()-1, 1.0 ) ),
    valueDistribution( d ),
    tau( t ), 
    lambda( l ),
    instantaneousJumpProbability( r )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( lambda );
    addParameter( instantaneousJumpProbability );
    
    // add the parameters of the distribution
    const std::set<const DagNode*>& pars = valueDistribution->getParameters();
    for (std::set<const DagNode*>::iterator it = pars.begin(); it != pars.end(); ++it)
    {
        addParameter( *it );
    }
    
    redrawValue();
}



/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process. 
 */
BranchRateJumpProcess* BranchRateJumpProcess::clone(void) const 
{
    
    return new BranchRateJumpProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return   The log-transformed probability density.
 */
double BranchRateJumpProcess::computeLnProbability(void) 
{
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    size_t rootIndex= root.getIndex();
    const Tree &tree = tau->getValue();
    double l = lambda->getValue();
    double rho = instantaneousJumpProbability->getValue();
    
    
    double lnProb = 0.0;
    
    const std::vector<double> &v = *value;
    for (size_t i = 0; i < v.size(); ++i) 
    {
        double bl = tree.getBranchLength( i );
        double p = rho + ( 1.0 - exp(-l*bl) ) * (1.0 - rho);
        if ( v[i] == 1.0 ) 
        {
            lnProb += log( 1.0 - p );
        }
        else
        {
            lnProb += log( p );
            valueDistribution->getValue() = v[i];
            lnProb += valueDistribution->computeLnProbability();
        }
        
        // check if the root rate matches 1.0
        // the move could have changed this
        if ( rootIndex == i )
        {
            throw RbException("We should not have a rate multiplier for the root branch!!!");
        }
            
    }
    
    return lnProb;
}


/**
 * Get the distribution of the values if a jump occurred.
 *
 * \return    The pointer to the value distribution.
 */
TypedDistribution<double>* BranchRateJumpProcess::getValueDistribution( void ) const
{
    
    return valueDistribution;
}



/**
 * Redrawing a new value from the process and storing it as a member.
 */
void BranchRateJumpProcess::redrawValue(void) 
{
    // Get the random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    const Tree &tree        = tau->getValue();
    double l                = lambda->getValue();
    double rho              = instantaneousJumpProbability->getValue();
    std::vector<double> &v  = *value;
    
    for (size_t i = 0; i < v.size(); ++i) 
    {
        double bl = tree.getBranchLength( i );
        double p = rho + ( 1.0 - exp(-l*bl) ) * (1.0 - rho);
        double u = rng->uniform01();
        if ( u > p ) // we did not get a change event
        {
            v[i] = 1.0;
        }
        else // we got a change event
        {
            valueDistribution->redrawValue();
            v[i] = valueDistribution->getValue();
        }
        
    } 

}


/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void BranchRateJumpProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == tau ) 
    {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    else if ( oldP == lambda )
    {
        lambda = static_cast< const TypedDagNode<double> * >( newP );
    }
    else if ( oldP == instantaneousJumpProbability )
    {
        instantaneousJumpProbability = static_cast< const TypedDagNode<double> * >( newP );
    }
    else
    {
        valueDistribution->swapParameter(oldP,newP);
    }
    
}


