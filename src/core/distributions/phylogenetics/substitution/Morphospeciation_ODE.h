#ifndef Morphospeciation_ODE_H
#define Morphospeciation_ODE_H

#include "AbstractBirthDeathProcess.h"
#include "RateMatrix.h"

#include <vector>
#include <boost/array.hpp>

typedef std::vector< double > state_type;

namespace RevBayesCore {
    
    /**
     * @brief Multi-rate cladogenetic birth-death ODE.
     *
     */
    class Morphospeciation_ODE {
        
    public:
        
        Morphospeciation_ODE( size_t n, size_t k, double lambda_asymmetric, double lambda_anagenetic, double mu, double psi, double rho, double rate );
        
        void operator() ( const state_type &x , state_type &dxdt , const double t );
        
    protected:

        double                                      p( double t ) const;                //!< p(t)

        
        double                                      mu;                                 //!< extinction rate
        double                                      lambda_asymmetric;                  //!< asymmetric (budding) speciation rate
        double                                      lambda_anagenetic;                  //!< anagenetic speciation rate
        double                                      psi;                                //!< fossilization rate
        double                                      rho;                                //!< extant sampling fraction
        size_t                                      num_states;                         //!< the number of character states
        size_t                                      num_sites;                          //!< the number of character states
        double                                      rate;                               //!< clock rate for anagenetic change
    };
    
}

#endif
