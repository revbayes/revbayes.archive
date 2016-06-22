#ifndef CDCladoSE_H
#define CDCladoSE_H

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
     * This class represents the ordinary differential equations for the character dependent
     * cladogenetic multi-rate birth-death process (ClaSSE: Goldberg and Igic, 2012)
     * Will Freyman 6/22/16
     *
     */
    class CDCladoSE {
        
    public:
        
        CDCladoSE( const std::vector<double> &l, const std::vector<double> &m, const RateGenerator* q, std::map<std::vector<unsigned>, double> c, double r);
        
        void operator() ( const state_type &x , state_type &dxdt , const double t );
        
    private:
        
        std::vector<double>                         lambda;
        std::vector<double>                         mu;
        size_t                                      num_categories;
        const RateGenerator*                        Q;
        std::map<std::vector<unsigned>, double>     cladoProbs; // pair< [ancestor_state, daughter_1_state, daughter_2_state], transition_probability >
        double                                      rate;
        
        
    };
    
}

#endif
