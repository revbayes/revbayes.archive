#include "AutocorrelatedLognormalRateDistribution.h"
#include "ConstantNode.h"
#include "AutocorrelatedRateMultiplierJumpProcess.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <iostream>



using namespace RevBayesCore;



/**
 * Default constructor.
 * The default constructor does nothing except allocating the object.
 *
 * \param[in]   d    The distribution from which the new values are drawn if a jump occured.
 * \param[in]   t    The time tree along which the process is applied.
 * \param[in]   d    A scaling parameter for the branch length.
 */
AutocorrelatedRateMultiplierJumpProcess::AutocorrelatedRateMultiplierJumpProcess(TypedDistribution<double> *d, const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* l): TypedDistribution< std::vector< double > >( new std::vector< double >(t->getValue().getNumberOfNodes(), 1.0 ) ), 
    valueDistribution( d ),
    tau( t ), 
    lambda( l ) 
{
    addParameter( tau );
    addParameter( lambda );
    
    const std::set<const DagNode*>& pars = valueDistribution->getParameters();
    // add the parameters of the distribution
    for (std::set<const DagNode*>::iterator it = pars.begin(); it != pars.end(); ++it) 
    {
        addParameter( *it );
    }
    
    redrawValue();
}



/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the process. 
 */
AutocorrelatedRateMultiplierJumpProcess* AutocorrelatedRateMultiplierJumpProcess::clone(void) const 
{
    
    return new AutocorrelatedRateMultiplierJumpProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return   The log-transformed probability density.
 */
double AutocorrelatedRateMultiplierJumpProcess::computeLnProbability(void) {
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    size_t rootIndex= root.getIndex();
    const Tree &tree = tau->getValue();
    double l = lambda->getValue();
    
    
    double lnProb = 0.0;
    
    const std::vector<double> &v = *value;
    for (size_t i = 0; i < v.size(); ++i) 
    {
        // check if the root rate matches 1.0
        // the move could have changed this
        if ( rootIndex != i )
        {
            double bl = tree.getBranchLength( i );
            double p = 1.0 - exp(-l*bl);
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
        }
        else if ( v[rootIndex] != 1.0 )
        {
            lnProb = RbConstants::Double::neginf;
        }
            
    }
    
    return lnProb;
}



/**
 * Redrawing a new value from the process and storing it as a member.
 */
void AutocorrelatedRateMultiplierJumpProcess::redrawValue(void) 
{
    // Get the random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;
        
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    
    // get the index of the root
    size_t rootIndex = root.getIndex();
    
    const Tree &tree = tau->getValue();
    double l = lambda->getValue();
    std::vector<double> &v = *value;
    
    for (size_t i = 0; i < v.size(); ++i) 
    {
        if ( rootIndex != i )
        {
            double bl = tree.getBranchLength( i );
            double p = 1.0 - exp(-l*bl);
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
        else 
        {
            v[rootIndex] = 1.0;
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
void AutocorrelatedRateMultiplierJumpProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) 
    {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == lambda ) 
    {
        lambda = static_cast< const TypedDagNode<double> * >( newP );
    }
    
}




