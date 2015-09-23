#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantRateBirthDeathProcess::ConstantRateBirthDeathProcess(const TypedDagNode<double> *org, const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                                                     const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt,
                                                     const std::vector<Taxon> &tn, const std::vector<Clade> &c) : BirthDeathProcess( org, ra, r, ss, cdt, tn, c ),
    speciation( s ),
    extinction( e )
{
    addParameter( speciation );
    addParameter( extinction );

    simulateTree();

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
ConstantRateBirthDeathProcess* ConstantRateBirthDeathProcess::clone( void ) const
{
    
    return new ConstantRateBirthDeathProcess( *this );
}



double ConstantRateBirthDeathProcess::lnSpeciationRate(double t) const
{
    
    return log( speciation->getValue() );
}


double ConstantRateBirthDeathProcess::pSurvival(double start, double end) const 
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



double ConstantRateBirthDeathProcess::rateIntegral(double t_low, double t_high) const {
    
    double rate = (speciation->getValue() - extinction->getValue()) * (t_low - t_high);
        
    return rate;
}



std::vector<double>* ConstantRateBirthDeathProcess::simSpeciations(size_t n, double origin, double r) const
{

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    std::vector<double>* times = new std::vector<double>(n, 0.0);
    
    for (size_t i = 0; i < n; ++i) 
    {
        double u = rng->uniform01();
    
        // get the parameters
        double lambda = speciation->getValue()*r;
        double mu = extinction->getValue() - speciation->getValue()*(1.0-r);
        double div = lambda - mu;
    
        double t = 1.0/div * log((lambda - mu * exp((-div)*origin) - mu * (1.0 - exp((-div) * origin)) * u )/(lambda - mu * exp((-div) * origin) - lambda * (1.0 - exp(( -div ) * origin)) * u ) );  
	
        (*times)[i] = t;
    }
    
    // finally sort the times
    std::sort(times->begin(), times->end());
    
    return times;
}


/** Swap a parameter of the distribution */
void ConstantRateBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
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
