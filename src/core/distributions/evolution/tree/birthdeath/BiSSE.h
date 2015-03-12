#ifndef BiSSE_H
#define BiSSE_H

#include "AbstractBirthDeathProcess.h"
#include "RateMatrix.h"

#include <vector>
#include <boost/array.hpp>

typedef boost::array< double , 4 > state_type;

namespace RevBayesCore {
    
    /**
     * @brief Multi-rate birth-death ODE.
     *
     *
     * This class represents the ordinary differential equations for the 
     * multi-rate birth-death process.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-03-11, version 1.0
     *
     */
    class BiSSE {
        
    public:
        
        BiSSE( const std::vector<double> &l, const std::vector<double> &m, const RateMatrix* q);
        
        void operator() ( const state_type &x , state_type &dxdt , const double t );
        
    private:
        
        std::vector<double>         lambda;
        std::vector<double>         mu;
        size_t                      numCategories;
        const RateMatrix*           Q;

        
    };
    
}

#endif
