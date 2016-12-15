#include "Clade.h"
#include "ConstantRateBirthDeathProcessTopology.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantRateBirthDeathProcessTopology::ConstantRateBirthDeathProcessTopology(const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
															 const TypedDagNode<double> *r, const std::string& ss, const std::vector<Clade> &ic, const std::string &cdt,
															 const std::vector<Taxon> &tn) : BirthDeathProcess( ra, r, ss, ic, cdt, tn ),
speciation( s ),
extinction( e )
{
	addParameter( speciation );
	addParameter( extinction );
	
	simulateTree();
	log_tree_topology_prob = logProbTreeShape();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
ConstantRateBirthDeathProcessTopology* ConstantRateBirthDeathProcessTopology::clone( void ) const
{
	
	return new ConstantRateBirthDeathProcessTopology( *this );
}



double ConstantRateBirthDeathProcessTopology::lnSpeciationRate(double t) const
{
	
	return log( speciation->getValue() );
}


double ConstantRateBirthDeathProcessTopology::computeProbabilitySurvival(double start, double end) const
{
	
	// compute the rate
	double mu = extinction->getValue();
	double lambda = speciation->getValue();
	double rate = mu - lambda;
	
	// do the integration of int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds )
	// where rate(t,s) = int_{t}^{s} ( mu(x)-lambda(x) dx )
	
	double den = 1.0 + ( exp(-rate*start) * mu / rate ) * ( exp(rate*end) - exp(rate*start) );
	
	return (1.0 / den);
}


ConstantRateBirthDeathProcessTopology::TypeTreeRanking ConstantRateBirthDeathProcessTopology::logNumberRankingsRec(TopologyNode& n /*int n, TypeTree *tree*/) {
	ConstantRateBirthDeathProcessTopology::TypeTreeRanking  rl, rr, res;
	if( n.isTip() ) {
		res.rank = 0.;
		res.leaf = 1;
		return res;
	}
	
	rl = logNumberRankingsRec( n.getChild(0) );
	rr = logNumberRankingsRec( n.getChild(1) );
	res.leaf = rl.leaf+rr.leaf;
	//res.rank = rl.rank+rr.rank+log(2)+logBinomial((unsigned int) (rl.leaf-1), (unsigned int) (res.leaf-2));
	res.rank = rl.rank+rr.rank+log(2) + RbMath::lnChoose((unsigned int) (rl.leaf-1), (unsigned int) (res.leaf-2));

	return res;
}

double ConstantRateBirthDeathProcessTopology::logNumberRankings(Tree *tree) {
	ConstantRateBirthDeathProcessTopology::TypeTreeRanking r = logNumberRankingsRec( tree->getRoot() );
	return r.rank;
}

double ConstantRateBirthDeathProcessTopology::logProbTreeShape() {
//Adding RbMath::lnFactorial(num_taxa-1) to do like they do in the non-topology prior.
	return RbMath::lnFactorial(int(num_taxa)-1) + logProbSubTreeShape( value->getRoot() );
}

double ConstantRateBirthDeathProcessTopology::logProbSubTreeShape(TopologyNode& n) {
	ConstantRateBirthDeathProcessTopology::TypeTreeRanking  r = logNumberRankingsRec(n);
//	return r.rank-2*gsl_sf_lnfact(r.leaf-1)-log(r.leaf);
	return r.rank-2*RbMath::lnFactorial(r.leaf-1)-log(r.leaf);
}



/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double ConstantRateBirthDeathProcessTopology::computeLnProbability( void )
{
	// prepare probability computation
	prepareProbComputation();
	
	// variable declarations and initialization
	double lnProbTimes = 0;
	
	// present time
	double ra = value->getRoot().getAge();
	double presentTime = ra;
		
    if ( ra != root_age->getValue() )
    {
        return RbConstants::Double::neginf;
    }
		
    const std::vector<TopologyNode*> &c = value->getRoot().getChildren();
		
    for (std::vector<TopologyNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
    {
        if ( ra < (*it)->getAge() )
        {
            return RbConstants::Double::neginf;
        }
    }
	
	const std::vector<TopologyNode*>& nodes = value->getNodes();
	for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
	{
		if ( !(*it)->isRoot() )
		{
			if ( (*it)->getAge() >= (*it)->getParent().getAge() )
			{
				return RbConstants::Double::neginf;
			}
		}
	}
	
	
	// what do we condition on?
	// did we condition on survival?
	if ( condition == "survival" )
	{
		lnProbTimes = - log( pSurvival(0,presentTime) );
	}
	
	// we started at the root thus we square the survival prob
	lnProbTimes *= 2.0;
	
	// multiply the probability of a descendant of the initial species
	lnProbTimes += computeLnProbabilityTimes();
	
	// Now we compute the topology part.
	log_tree_topology_prob = logProbTreeShape();
	
	return lnProbTimes + log_tree_topology_prob;
	
}



double ConstantRateBirthDeathProcessTopology::rateIntegral(double t_low, double t_high) const
{
	
	double rate = (speciation->getValue() - extinction->getValue()) * (t_low - t_high);
	
	return rate;
}



double ConstantRateBirthDeathProcessTopology::simulateDivergenceTime(double origin, double present, double r) const
{
	
	// Get the rng
	RandomNumberGenerator* rng = GLOBAL_RNG;
	
	double u = rng->uniform01();
		
    // get the parameters
    double age = present - origin;
    double lambda = speciation->getValue()*r;
    double mu = extinction->getValue() - speciation->getValue()*(1.0-r);
    double div = lambda - mu;
		
    double t = 1.0/div * log((lambda - mu * exp((-div)*age) - mu * (1.0 - exp((-div) * age)) * u )/(lambda - mu * exp((-div) * age) - lambda * (1.0 - exp(( -div ) * age)) * u ) );
		
	
    return present - t;
}


/** Swap a parameter of the distribution */
void ConstantRateBirthDeathProcessTopology::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
	
	if (oldP == speciation)
	{
		speciation = static_cast<const TypedDagNode<double>* >( newP );
	}
	else if (oldP == extinction)
	{
		extinction = static_cast<const TypedDagNode<double>* >( newP );
	}
	else
	{
		// delegate the super-class
		BirthDeathProcess::swapParameterInternal(oldP, newP);
	}
	
}
