#include "Clade.h"
#include "ConstantNode.h"
#include "MultispeciesCoalescentUniformPrior.h"
#include "DistributionExponential.h"
#include "DistributionUniform.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

MultispeciesCoalescentUniformPrior::MultispeciesCoalescentUniformPrior(const TypedDagNode<Tree> *sp, const std::vector<Taxon> &t) : AbstractMultispeciesCoalescent(sp, t)
{
    
}


MultispeciesCoalescentUniformPrior::~MultispeciesCoalescentUniformPrior()
{
    
}





MultispeciesCoalescentUniformPrior* MultispeciesCoalescentUniformPrior::clone( void ) const
{
    
    return new MultispeciesCoalescentUniformPrior( *this );
}


double MultispeciesCoalescentUniformPrior::computeLnCoalescentProbability(size_t k, const std::vector<double> &times, double begin_age, double end_age, size_t index, bool add_final_interval)
{
    
    double theta = 1.0;
    
    double ln_prob_coal = 0;
    double current_time = begin_age;
    for (size_t i=0; i<times.size(); ++i)
    {
        
        // now we do the computation
        //a is the time between the previous and the current coalescences
        double a = times[i] - current_time;

        // get the number j of individuals we had before the current coalescence
        size_t j = k - i;
        
        // compute the number of pairs: pairs = C(j choose 2) = j * (j-1.0) / 2.0
        double n_pairs = j * (j-1.0) / 2.0;
        double lambda = n_pairs * theta;

        // add the density for this coalescent event
        //lnProbCoal += log( lambda ) - lambda * a;
        //Corrected version:
        ln_prob_coal += log( theta ) - lambda * a;
        
    }
    
    // compute the probability of no coalescent event in the final part of the branch
    // only do this if the branch is not the root branch
    if ( add_final_interval == false )
    {
        double final_interval = end_age - current_time;
        size_t j = k - times.size();
        ln_prob_coal -= j*(j-1.0)/2.0 * final_interval * theta;
        
    }
    
    return ln_prob_coal;
}


double MultispeciesCoalescentUniformPrior::drawNe( size_t index )
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    double u = RbStatistics::Uniform::rv( 0, max_theta->getValue(), *rng);
    
    return u;
}



void MultispeciesCoalescentUniformPrior::setMaxTheta(TypedDagNode<double>* m)
{
    
    removeParameter( max_theta );
    
    max_theta = m;
    
    addParameter( max_theta );
}


/** Swap a parameter of the distribution */
void MultispeciesCoalescentUniformPrior::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == max_theta )
    {
        max_theta = static_cast<const TypedDagNode< double >* >( newP );
    }
    
    AbstractMultispeciesCoalescent::swapParameterInternal(oldP, newP);
    
}
