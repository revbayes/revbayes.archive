#include "CDSE.h"

using namespace RevBayesCore;


CDSE::CDSE( const std::vector<double> &l, const std::vector<double> &m, const RateGenerator* q, double r ) :
    lambda( l ),
    mu( m ),
    num_categories( l.size() ),
    Q( q ),
    rate( r )
{
    
}


void CDSE::operator()(const state_type &x, state_type &dxdt, const double t)
{

    for (size_t i=0; i<num_categories; ++i)
    {
        
        /**** Extinction ****/
        
        // extinction event
        dxdt[i] = mu[i];
        
        // no event
        double no_event_rate = mu[i] + lambda[i];
        for (size_t j=0; j<num_categories; ++j)
        {
            if ( i != j )
            {
                no_event_rate += Q->getRate(i,j)*rate;
            }
        }
        dxdt[i] -= no_event_rate*x[i];
        
        // speciation event
        dxdt[i] += lambda[i]*x[i]*x[i];
        
        // rate-shift event
        for (size_t j=0; j<num_categories; ++j)
        {
            if ( i != j )
            {
                dxdt[i] += Q->getRate(i,j)*rate*x[j];
            }
        }
        
        
        /**** Observation ****/
        
        // no event
        dxdt[i+num_categories] = -no_event_rate*x[i+num_categories];
        
        // speciation event
        dxdt[i+num_categories] += 2*lambda[i]*x[i]*x[i+num_categories];
        
        // rate-shift event
        for (size_t j=0; j<num_categories; ++j)
        {
            if ( i != j )
            {
                dxdt[i+num_categories] += Q->getRate(i,j)*rate*x[j+num_categories];
            }
        }
        
    }
    
}

