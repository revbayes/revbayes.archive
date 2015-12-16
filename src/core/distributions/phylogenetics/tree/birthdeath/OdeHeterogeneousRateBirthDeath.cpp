#include "OdeHeterogeneousRateBirthDeath.h"

using namespace RevBayesCore;


OdeHeterogeneousRateBirthDeath::OdeHeterogeneousRateBirthDeath( const RbVector<double> &l, const RbVector<double> &m, double r ) :
    lambda( l ),
    mu( m ),
    num_categories( l.size() ),
    switch_rate( r ),
    current_rate_category( 0 )
{
    
}


void OdeHeterogeneousRateBirthDeath::operator()(const state_type &x, state_type &dxdt, const double t)
{
    
    for (size_t i=0; i<num_categories; ++i)
    {
        
        /**** Extinction ****/
        
        // extinction event
        dxdt[i] = mu[i];
        
        // no event
        double noEventRate = mu[i] + lambda[i] + switch_rate;
        dxdt[i] -= noEventRate*x[i];
        
        // speciation event
        dxdt[i] += lambda[i]*x[i]*x[i];
        
        // rate-shift event
        for (size_t j=0; j<num_categories; ++j)
        {
            if ( i != j )
            {
                dxdt[i] += switch_rate*x[j] / num_categories;
            }
        }
        
    }
    
    /**** Observation ****/
    
    // no event
    double noEventRate = mu[current_rate_category] + lambda[current_rate_category] + switch_rate;
    dxdt[num_categories] = -noEventRate*x[num_categories];
    
    // speciation event
    dxdt[num_categories] += 2*lambda[current_rate_category]*x[current_rate_category]*x[num_categories];
    
}


void OdeHeterogeneousRateBirthDeath::setCurrentRateCategory(size_t i)
{
    current_rate_category = i;
}

