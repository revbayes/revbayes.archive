/**
 * @file
 * This file contains the declaration of RateMatrix_F81, which is a
 * class that holds a rate matrix for a continuous-time Markov model
 * with parameters for equilibrium frequencies but fixed exchangeabilities.
 *
 * @brief Declaration of RateMatrix_F81
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-12-05 12:20:37 -0800 (Wed, 05 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RateMatrix.h 1901 2012-12-05 20:20:37Z boussau $
 */

#ifndef RateMatrix_F81_H
#define RateMatrix_F81_H

#include "TimeReversibleRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    
    class RateMatrix_F81 : public TimeReversibleRateMatrix {
        
    public:
        RateMatrix_F81(size_t n);                                                                                               //!< Construct rate matrix with n states
        virtual                             ~RateMatrix_F81(void);                                                              //!< Destructor
        
        // RateMatrix functions
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_F81*                     clone(void) const;
        void                                updateMatrix(void);

    private:
        
    };
        
}

#endif

