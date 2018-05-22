#include "AVMVNProposal.h"
#include "DirichletDistribution.h"
#include "DistributionMultivariateNormal.h"
#include "MultivariateNormalDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "ScaledDirichletDistribution.h"
#include "TreeUtilities.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
AVMVNProposal::AVMVNProposal( double s, double e, double n0, double c0 ) : Proposal(),
    noTransformScalarVariables(  ),
    logTransformScalarVariables(  ),
//    logitTransformScalarVariables(  ),
    noTransformVectorVariables(  ),
    logConstrainedSumTransformVectorVariables(  ),
    lnHastingsratio( 0.0 ),
    sigma( s ),
    epsilon( e ),
    waitBeforeLearning( n0 ),
    waitBeforeUsing ( c0 ),
    nTried ( 0 ),
    updates (0 ),
    dim ( 0.0 ),
    C_emp(  ),
    x_bar(  ),
    storedValues(  )
{
    if (waitBeforeUsing < waitBeforeLearning)
    {
        throw(RbException("Cannot delay learning empirical covariance matrix longer than using empirical covariance matrix"));
    }
}


/**
 * Add a variable.
 */
void AVMVNProposal::addVariable( StochasticNode<double> *v, std::string& transform)
{
    
    
    // add it to the nodes vector
    addNode( v );
    
    // add it to my internal vector
    // Is this a transformed variable?
    if ( transform == "none" )
    {
        noTransformScalarVariables.push_back( v );
    }
    else if ( transform == "log" )
    {
        logTransformScalarVariables.push_back( v );
    }
    // TODO: figure out how to integrate logit transform.
    //       Must be able to easily access lower and upper bounds on distributions.
    //       Must be valid for variables with bounds that can change
//    else if ( transform == "logit" )
//    {
//    }
    else
    {
        throw(RbException("Unrecognized scalar transform choice for AVMVN. For scalars, choose \"none\"|\"log\""));
    }
    
}


/**
 * Add a variable.
 */
void AVMVNProposal::addVariable( StochasticNode<RbVector<double> > *v, std::string& transform)
{
    
    
    // add it to the nodes vector
    addNode( v );
    
    // add it to my internal vector
    // Is this a transformed variable?
    if ( transform == "none" )
    {
        noTransformVectorVariables.push_back( v );
    }
    else if ( transform == "multivariateLogistic" )
    {
        // Check that this is a simplexed-distribution
        DirichletDistribution* dist = dynamic_cast<DirichletDistribution *>( &v->getDistribution() );
        bool found = dist != NULL ? true : false;
        
        if ( !found )
        {
            ScaledDirichletDistribution* dist = dynamic_cast<ScaledDirichletDistribution *>( &v->getDistribution() );
            found = dist != NULL ? true : false;
        }
        
        if ( !found )
        {
            throw(RbException("Unrecognized vector distribution to AVMVN. Option \"multivariateLogistic\" is for simplexed distributions."));
        }
        
        logConstrainedSumTransformVectorVariables.push_back( v );
    } else
    {
        throw(RbException("Unrecognized vector transform choice for AVMVN. For vectors, choose \"none\"|\"multivariateLogistic\""));
    }
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void AVMVNProposal::cleanProposal( void )
{
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
AVMVNProposal* AVMVNProposal::clone( void ) const
{
    
    return new AVMVNProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& AVMVNProposal::getProposalName( void ) const
{
    static std::string name = "AVMVN";
    
    return name;
}

/**
 * Obtains the values of all variables in the AVMVN proposal.
 */
void AVMVNProposal::getAVMVNMemberVariableValues( std::vector<double> *x )
{
    for (size_t i = 0; i < noTransformScalarVariables.size(); ++i)
    {
        x->push_back( noTransformScalarVariables[i]->getValue() );
    }
    
    for (size_t i = 0; i < logTransformScalarVariables.size(); ++i)
    {
        x->push_back( log(logTransformScalarVariables[i]->getValue()) );
    }
    
//    for (size_t i = 0; i < logitTransformScalarVariables.size(); ++i)
//    {
//        double y = logitTransformScalarVariables[i]->getValue();
//        double lb = logitTransformScalarVariables[i]->getDistribution().getMin();
//        double ub = logitTransformScalarVariables[i]->getDistribution().getMax();
//        double p = (y - lb) / (ub - lb);
//        x->push_back( log(p / (1 - p)) );
//    }
    
    for (size_t i = 0; i < noTransformVectorVariables.size(); ++i)
    {
        std::vector<double> tmp = noTransformVectorVariables[i]->getValue();
        
        for (size_t i=0; i < tmp.size(); ++i)
        {
            x->push_back( tmp[i] );
        }
    }
    
    for (size_t i = 0; i < logConstrainedSumTransformVectorVariables.size(); ++i)
    {
        std::vector<double> tmp = logConstrainedSumTransformVectorVariables[i]->getValue();
        
        for (size_t i=0; i < tmp.size(); ++i)
        {
            x->push_back( log(tmp[i]) );
        }
    }
}

/**
 * Sets the values of all variables in the AVMVN proposal.
 * Calculates hastings ratio at the same time
 */
void AVMVNProposal::setAVMVNMemberVariableValues( std::vector<double> x_prime, std::vector<double> x )
{
    // Calculate hastings ratio and give variables their new values
    size_t index = 0;
    for (size_t i = 0; i < noTransformScalarVariables.size(); ++i)
    {
        noTransformScalarVariables[i]->getValue() = x_prime[index];
        ++index;
    }
    
    for (size_t i = 0; i < logTransformScalarVariables.size(); ++i)
    {
        logTransformScalarVariables[i]->getValue() = exp(x_prime[index]);
        lnHastingsratio += -x[index] + x_prime[index];
        ++index;
    }
    
//    for (std::vector< StochasticNode<double> *>::const_iterator it = logitTransformScalarVariables.begin(); it != logitTransformScalarVariables.end(); it++)
//    for (size_t i = 0; i < logitTransformScalarVariables.size(); ++i)
//    {
//        double p = 1 / (1 + exp(-x_prime[index]));
//        double lb = logitTransformScalarVariables[i]->getDistribution().getMin();
//        double ub = logitTransformScalarVariables[i]->getDistribution().getMax();
//        logitTransformScalarVariables[i]->getValue() = p * (ub - lb) + lb;
//        
//        lnHastingsratio += ( -log(1.0 - x[index]) - log(x[index]) ) - ( -log(1.0 - x_prime[index]) - log(x_prime[index]) );
//        ++index;
//    }
    
    for (size_t i = 0; i < noTransformVectorVariables.size(); ++i)
    {
        std::vector<double> tmp;
        
        for (size_t j=0; j < noTransformVectorVariables[i]->getValue().size(); ++j)
        {
            tmp[j] = x_prime[index];
            ++index;
        }
        noTransformVectorVariables[i]->getValue() = tmp;
    }
    
    for (size_t i = 0; i < logConstrainedSumTransformVectorVariables.size(); ++i)
    {
        std::vector<double> old_sum_constrained = logConstrainedSumTransformVectorVariables[i]->getValue();
        std::vector<double> tmp;
        double sum = 0.0;
        
        for (size_t j=0; i < old_sum_constrained.size(); ++j)
        {
            tmp[j] = x_prime[index];
            sum += x_prime[index];
            ++index;
        }
        
        for (size_t j=0; i < old_sum_constrained.size(); ++j)
        {
            tmp[j] /= sum;
            lnHastingsratio += -log(old_sum_constrained[j]) + log(tmp[j]);
        }
        
        logConstrainedSumTransformVectorVariables[i]->getValue() = tmp;
        
    }

}

/**
 * Perform the proposal.
 *
 * A scaling Proposal draws a random uniform number u ~ unif (-0.5,0.5)
 * and Slides the current vale by a scaling factor
 * sf = exp( sigma * u )
 * where sigma is the tuning parameter of the Proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double AVMVNProposal::doProposal( void )
{
    ++nTried;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    lnHastingsratio = 0.0;
    
    // Get current values in transformed space
    // All variables are accessed in the order they appear in the relevant vector of transformed variables
    // These in turn are accessed in the order: scalar (no transform, log, logit), vector (no transform, sum-constrained-log)
    std::vector<double> x;
    
    getAVMVNMemberVariableValues(&x);
    
    if ( nTried == 1 )
    // First time using move, setting up components
    {
        dim = x.size();
        for ( size_t i=0; i<dim; ++i )
        {
            storedValues.push_back(x[i]);
            x_bar.push_back(0.0);
//            x_bar_minus_1.push_back(0.0);
            
            for (size_t j=0; j < dim; ++j)
            {
                C_emp[i][j] = 0.0;
//                C_emp_minus_1[i][j] = 0.0;
            }
        }
        
    }
    else
    // Adjust empirical covariance matrix and averages
    {
        // Store values
//        storedValues.erase(storedValues.begin(),storedValues.end());
        for ( size_t i=0; i<dim; ++i )
        {
            storedValues[i] = x[i];
        }
        
        if ( nTried > waitBeforeLearning)
        {
            ++updates;
            
            double n = double(updates);
            
//            x_bar_minus_1.erase(x_bar_minus_1.begin(),x_bar_minus_1.end());
            for ( size_t i=0; i<dim; ++i )
            {
                // Update covariances first (to save us tracking current and previous averages)
                for (size_t j=i; j < dim; ++j)
                {
                    C_emp[i][j] = 1/n * ( C_emp[i][j] * (n - 1.0) + ((n - 1.0)/n) * ((x[i] - x_bar[i]) * (x[j] - x_bar[j])) );
                    C_emp[j][i] = C_emp[i][j];
                }
                
                // Update averages
                x_bar[i] = 1/n * x[i] + (n - 1)/n * x_bar[i];
            }

        }
        
    }

    
    // Move
    //    std::vector<double> RbStatistics::MultivariateNormal::rvCovariance(const std::vector<double>& mu, const MatrixReal& sigma, RandomNumberGenerator& rng, double scale)
    MatrixReal vcv( dim );
    
    for (size_t i=0; i<dim; ++i)
    {
        vcv[i][i] = 1.0;
    }
    
    if ( nTried > waitBeforeUsing )
//        vcv = 1/dim * sigma * ((1 - epsilon) * C_emp + epsilon * vcv);
    {
        for (size_t i=0; i<dim; ++i)
        {
            for (size_t j=i; j<dim; ++j)
            {
                vcv[i][j] = 1/dim * sigma * ((1 - epsilon) * C_emp[i][j] + epsilon * vcv[i][j]);
            }
        }
    }
    else
    {
//        vcv = 1/dim * sigma * vcv;
        for (size_t i=0; i<dim; ++i)
        {
            vcv[i][i] = 1/dim * sigma;
        }
  }
    std::vector<double> x_prime = RbStatistics::MultivariateNormal::rvCovariance(x, vcv, *rng, sigma);
    
    setAVMVNMemberVariableValues(x_prime, x);

    
    return lnHastingsratio;

}


/**
 * Prepare the proposal, e.g., pick the UpDown that we want to change.
 * Here we do not need to do any preparation.
 */
void AVMVNProposal::prepareProposal( void )
{
    
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void AVMVNProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "sigma = " << sigma;
    
}


/**
 * Remove a variable.
 */
void AVMVNProposal::removeVariable( StochasticNode<double> *v, std::string& transform)
{
    
    
    // add it to the nodes vector
    removeNode( v );
    
    // add it to my internal vector
    if ( transform == "none" )
    {
        noTransformScalarVariables.erase(std::remove(noTransformScalarVariables.begin(), noTransformScalarVariables.end(), v), noTransformScalarVariables.end());
    }
    else if ( transform == "log" )
    {
        logTransformScalarVariables.erase(std::remove(logTransformScalarVariables.begin(), logTransformScalarVariables.end(), v), logTransformScalarVariables.end());
    }
//    else if ( transform == "logit" )
//    {
//        logitTransformScalarVariables.erase(std::remove(logitTransformScalarVariables.begin(), logitTransformScalarVariables.end(), v), logitTransformScalarVariables.end());
//    }
    else
    {
        throw(RbException("AVMVN attempting to remove a scalar variable it cannot have added."));
    }
}


/**
 * Remove a variable.
 */
void AVMVNProposal::removeVariable( StochasticNode<RbVector<double> > *v, std::string& transform)
{
    
    
    // add it to the nodes vector
    removeNode( v );
    
    // add it to my internal vector
    // Is this a transformed variable?
    if ( transform == "none" )
    {
        noTransformVectorVariables.erase(std::remove(noTransformVectorVariables.begin(), noTransformVectorVariables.end(), v), noTransformVectorVariables.end());
    }
    else if ( transform == "multivariateLogistic")
    {
        logConstrainedSumTransformVectorVariables.erase(std::remove(logConstrainedSumTransformVectorVariables.begin(), logConstrainedSumTransformVectorVariables.end(), v), logConstrainedSumTransformVectorVariables.end());
    }
    else
    {
        throw(RbException("AVMVN attempting to remove a vector variable it cannot have added."));
    }
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void AVMVNProposal::undoProposal( void )
{
    
    setAVMVNMemberVariableValues(storedValues, storedValues);
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void AVMVNProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    for (size_t i = 0; i < noTransformScalarVariables.size(); ++i)
    {
        if ( noTransformScalarVariables[i] == oldN )
        {
            noTransformScalarVariables[i] = static_cast<StochasticNode<double> *>(newN);
        }
    }
    
    for (size_t i = 0; i < logTransformScalarVariables.size(); ++i)
    {
        if ( logTransformScalarVariables[i] == oldN )
        {
            logTransformScalarVariables[i] = static_cast<StochasticNode<double> *>(newN);
        }
    }
    
//    for (size_t i = 0; i < logitTransformScalarVariables.size(); ++i)
//    {
//        if ( logitTransformScalarVariables[i] == oldN )
//        {
//            logitTransformScalarVariables[i] = static_cast<StochasticNode<double> *>(newN);
//        }
//    }
    
    for (size_t i = 0; i < noTransformVectorVariables.size(); ++i)
    {
        if ( noTransformVectorVariables[i] == oldN )
        {
            noTransformVectorVariables[i] = static_cast<StochasticNode<RbVector<double> > *>(newN);
        }
    }

    for (size_t i = 0; i < logConstrainedSumTransformVectorVariables.size(); ++i)
    {
        if ( logConstrainedSumTransformVectorVariables[i] == oldN )
        {
            logConstrainedSumTransformVectorVariables[i] = static_cast<StochasticNode<RbVector<double> > *>(newN);
        }
    }

}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void AVMVNProposal::tune( double rate )
{
    
    if ( rate > 0.44 )
    {
        sigma *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        sigma /= (2.0 - rate/0.44 );
    }
    
    if ( sigma > 1 ) {
        sigma = fmin(10000, sigma);
    } else {
        sigma = fmax(1/10000, sigma);
    }
    
}

