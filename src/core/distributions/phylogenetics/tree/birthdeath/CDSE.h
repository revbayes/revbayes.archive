#ifndef CDSE_H
#define CDSE_H

#include "AbstractBirthDeathProcess.h"
#include "RateMatrix.h"

#include <vector>
#include <boost/array.hpp>

typedef std::vector< double > state_type;

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
     * @since 2016-02-18, version 1.0
     *
     */
    class CDSE {
        
    public:
        
        CDSE( const std::vector<double> &l, const std::vector<double> &m, const RateGenerator* q, double r);
        
        void operator() ( const state_type &x , state_type &dxdt , const double t );
        
    private:
        
        std::vector<double>         lambda;
        std::vector<double>         mu;
        size_t                      numCategories;
        const RateGenerator*        Q;
        double                      rate;
        
        
    };
    
}

#endif
