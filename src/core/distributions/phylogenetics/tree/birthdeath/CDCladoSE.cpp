


#include "CDCladoSE.h"

using namespace RevBayesCore;


CDCladoSE::CDCladoSE( const std::vector<double> &l, const std::vector<double> &m, const RateGenerator* q, std::map<std::vector<unsigned>, double> c, double r ) :
lambda( l ),
mu( m ),
num_categories( l.size() ),
Q( q ),
cladoProbs( c ),
rate( r )
{
    
}


void CDCladoSE::operator()(const state_type &x, state_type &dxdt, const double t)
{
    // ClaSSE equations from Goldberg and Igic, 2012
    
    double age = 0.0;
    for (size_t i=0; i<num_categories; ++i)
    {
        // calculate sum of speciation rates
        // lambda_ijk for all possible values of j and k
        double lambda_sum = 0.0;
        size_t event_index = 0;
        std::map<std::vector<unsigned>, double>::iterator it;
        for (it = cladoProbs.begin(); it != cladoProbs.end(); it++)
        {
            const std::vector<unsigned>& states = it->first;
            if (i == states[0])
            {
                lambda_sum += lambda[event_index];
                ++event_index;
            }
        }
        
        /**** Extinction ****/
        
        // extinction event
        dxdt[i] = mu[i];
        
        // no event
        double no_event_rate = mu[i] + lambda_sum;
        for (size_t j=0; j<num_categories; ++j)
        {
            if ( i != j )
            {
                no_event_rate += Q->getRate(i,j,age,rate);
            }
        }
        dxdt[i] -= no_event_rate*x[i];
        
        // speciation event
        event_index = 0;
        for (it = cladoProbs.begin(); it != cladoProbs.end(); it++)
        {
            const std::vector<unsigned>& states = it->first;
            if (i == states[0])
            {
                dxdt[i] += lambda[event_index]*x[states[1]]*x[states[2]];
                ++event_index;
            }
        }
        
        // rate-shift event
        for (size_t j=0; j<num_categories; ++j)
        {
            if ( i != j )
            {
                dxdt[i] += Q->getRate(i,j,age,rate)*x[j];
            }
        }
        
        
        /**** Observation ****/
        
        // no event
        dxdt[i+num_categories] = -no_event_rate*x[i+num_categories];
        
        // speciation event
        event_index = 0;
        for (it = cladoProbs.begin(); it != cladoProbs.end(); it++)
        {
            const std::vector<unsigned>& states = it->first;
            if (i == states[0])
            {
                dxdt[i+num_categories] += lambda[event_index]*x[states[1]+num_categories]*x[states[2]+num_categories];
                ++event_index;
            }
        }
        
        // rate-shift event
        for (size_t j=0; j<num_categories; ++j)
        {
            if ( i != j )
            {
                dxdt[i+num_categories] += Q->getRate(i,j,age,rate)*x[j+num_categories];
            }
        }
        
    }
    
}