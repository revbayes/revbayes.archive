

#include <cmath>
#include "Morphospeciation_ODE.h"

using namespace RevBayesCore;


Morphospeciation_ODE::Morphospeciation_ODE( size_t n, size_t k, double lambda, double lambda_a, double mu, double p, double r, double rr ) :
    lambda_asymmetric( lambda ),
    lambda_anagenetic( lambda_a ),
    mu( mu ),
    psi( p ),
    rho( r ),
    num_states( k ),
    num_sites( n ),
    rate( rr )
{

}


void Morphospeciation_ODE::operator()(const state_type &x, state_type &dxdt, const double t)
{
    // catch negative extinction probabilities that can result from
    // rounding errors in the ODE stepper
    state_type safe_x = x;
    for (size_t i = 0; i < num_states; ++i)
    {
        safe_x[i] = ( x[i] < 0.0 ? 0.0 : x[i] );
    }
    
    double age = 0.0;
    for (size_t i = 0; i < num_states; ++i)
    {
        
        // no event
        double no_event_rate = lambda_anagenetic + lambda_asymmetric + mu + psi;

        dxdt[i] = - no_event_rate * safe_x[i];
        
        dxdt[i] += lambda_asymmetric * p(t) * safe_x[i];
        
        // anagenetic state change
        for (size_t j = 0; j < num_states; ++j)
        {
            double total = 0.0;

            if ( i != j )
            {
                total += safe_x[j];
            }

            dxdt[i] += total * (lambda_asymmetric * p(t) + lambda_anagenetic) / double(num_states - 1);
        }
        
        dxdt[i] /= double(num_sites);
        
    } // end for num_states
    
}

/**
 * p(t)
 */
double Morphospeciation_ODE::p( double dt ) const
{
    if ( dt == 0) return 1.0;

    // get the parameters
    double b = lambda_asymmetric;
    double d = mu;
    double f = psi;
    double r = rho;

    double diff = b - d - f;

    double A = sqrt( diff*diff + 4.0*b*f);
    double B = - (diff - 2 * b * r) / A;

    double ln_e = -A*dt;

    double tmp = (1.0 + B) + exp(ln_e)*(1.0 - B);

    return (b + d + f - A * ((1.0+B)-exp(ln_e)*(1.0-B))/tmp)/(2.0*b);
}
