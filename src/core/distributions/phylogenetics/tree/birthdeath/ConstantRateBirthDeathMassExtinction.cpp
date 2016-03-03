#include "Clade.h"
#include "ConstantRateBirthDeathMassExtinction.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantRateBirthDeathMassExtinction::ConstantRateBirthDeathMassExtinction(const TypedDagNode<double> *o, const TypedDagNode<double> *ro, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                                                     const TypedDagNode< RbVector<double> >* met, const TypedDagNode< RbVector<double> >* mep, 
                                                     const TypedDagNode<double> *r, const std::string& ss, const std::vector<Clade> &ic, const std::string &cdt,
                                                     const std::vector<Taxon> &tn) : BirthDeathProcess( o, ro, r, ss, ic, cdt, tn),
    speciation( s ),
    extinction( e ),
    massExtinctionTimes( met ),
    massExtinctionSurvivalProbabilities( mep )
{
    addParameter( speciation );
    addParameter( extinction );
    addParameter( massExtinctionTimes );
    addParameter( massExtinctionSurvivalProbabilities );

    simulateTree();
    
}



ConstantRateBirthDeathMassExtinction* ConstantRateBirthDeathMassExtinction::clone( void ) const
{
    
    return new ConstantRateBirthDeathMassExtinction( *this );
}



double ConstantRateBirthDeathMassExtinction::lnSpeciationRate(double t) const
{

    return speciation->getValue();
}


double ConstantRateBirthDeathMassExtinction::pSurvival(double start, double end) const
{
    
    // compute the rate
    double mu = extinction->getValue();
    double lambda = speciation->getValue();
    double rate = mu - lambda;
    
    // do the integration of int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds )
    // where rate(t,s) = int_{t}^{s} ( mu(x)-lambda(x) dx ) - sum_{for all t < m_i < s in massExtinctionTimes }( log(massExtinctionSurvivalProbability[i]) )
    
    // we compute the integral stepwise for each epoch between mass-extinction events
    // add mass-extinction
    double accumulatedMassExtinction = 1.0;
    double prev_time = start;
    double den = 1.0;
    const std::vector<double> &met = massExtinctionTimes->getValue();
    const std::vector<double> &mep = massExtinctionSurvivalProbabilities->getValue();
    if ( met.size() > 0 ) 
    {
        for (size_t j=0; j<met.size(); ++j ) 
        {
            if ( start < met[j] && end > met[j] ) 
            {
                // compute the integral for this time episode until the mass-extinction event
                den += exp(-rate*start) * mu / (rate * accumulatedMassExtinction ) * ( exp(rate* met[j]) - exp(rate*prev_time));
                // store the current time so that we remember from which episode we need to integrate next
                prev_time = met[j];
                accumulatedMassExtinction *= mep[j];
                // integrate over the tiny time interval of the mass-extinction event itself and add it to the integral
                den -= (mep[j]-1) / accumulatedMassExtinction * exp( rate*(met[j] - start) );
            }
        }
    }
    
    // add the integral of the final epoch until the present time
    den += exp(-rate*start) * mu / (rate * accumulatedMassExtinction ) * ( exp(rate*end) - exp(rate*prev_time));
    
    return (1.0 / den);
    
}


double ConstantRateBirthDeathMassExtinction::rateIntegral(double t_low, double t_high) const {
    
    double b = (speciation->getValue() - extinction->getValue()) * (t_low - t_high);
    
    // add mass-extinction
    const std::vector<double> &met = massExtinctionTimes->getValue();
    const std::vector<double> &mep = massExtinctionSurvivalProbabilities->getValue();
    for (size_t j=0; j<met.size(); ++j ) 
    {
        if ( t_low < met[j] && t_high > met[j] ) 
        {
            b -= log(mep[j]);
        }
    }
    
    return b;
    
}




double ConstantRateBirthDeathMassExtinction::simulateDivergenceTime(double origin, double present, double rho) const
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    
    double u = rng->uniform01();
    
    // get the parameters
    double age = present - origin;
    double b = speciation->getValue();
    double d = extinction->getValue();
    
    // compute the time for this draw
    double t = 0.0;
    if ( b > d )
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(rho*b+(b*(1-rho)-d)*exp((d-b)*age) ) ) ) - (b*(1-rho)-d) ) / (rho * b) ) + (d-b)*age )  /  (d-b);
    }
    else
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-(b-d)/(rho*b*exp((b-d)*age)+(b*(1-rho)-d) ) ) ) - (b*(1-rho)-d) ) / (rho * b) ) + (d-b)*age )  /  (d-b);
    }
    
    return present - t;
}



/** Swap a parameter of the distribution */
void ConstantRateBirthDeathMassExtinction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == speciation) 
    {
        speciation = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == extinction)
    {
        extinction = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == massExtinctionTimes) 
    {
        massExtinctionTimes = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == massExtinctionSurvivalProbabilities)
    {
        massExtinctionSurvivalProbabilities = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else
    {
        BirthDeathProcess::swapParameterInternal(oldP, newP);
    }
    
}
