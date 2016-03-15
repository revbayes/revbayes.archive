#include "BiSSE.h"

using namespace RevBayesCore;


BiSSE::BiSSE( const std::vector<double> &l, const std::vector<double> &m, const RateGenerator* q, double r ) :
    lambda( l ),
    mu( m ),
    numCategories( l.size() ),
    Q( q ),
    rate( r )
{
    
}


void BiSSE::operator()(const state_type &x, state_type &dxdt, const double t)
{
    double age = 0.0;
    for (size_t i=0; i<numCategories; ++i)
    {
        
        /**** Extinction ****/
        
        // extinction event
        dxdt[i] = mu[i];
        
        // no event
        double noEventRate = mu[i] + lambda[i];
        for (size_t j=0; j<numCategories; ++j)
        {
            if ( i != j )
            {
                noEventRate += Q->getRate(i,j) * rate;
            }
        }
        dxdt[i] -= noEventRate*x[i];
        
        // speciation event
        dxdt[i] += lambda[i]*x[i]*x[i];
        
        // rate-shift event
        for (size_t j=0; j<numCategories; ++j)
        {
            if ( i != j )
            {
                dxdt[i] += Q->getRate(i,j)*rate*x[j];
            }
        }
        
        
        /**** Observation ****/
        
        // no event
        dxdt[i+numCategories] = -noEventRate*x[i+numCategories];
        
        // speciation event
        dxdt[i+numCategories] += 2*lambda[i]*x[i]*x[i+numCategories];
        
        // rate-shift event
        for (size_t j=0; j<numCategories; ++j)
        {
            if ( i != j )
            {
                dxdt[i+numCategories] += Q->getRate(i,j)*rate*x[j+numCategories];
            }
        }
        
    }
    
}

