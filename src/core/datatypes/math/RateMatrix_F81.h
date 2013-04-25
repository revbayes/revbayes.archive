/**
 * @file
 * This file contains the declaration of RateMatrix_F81, which is
 * class that holds a rate matrix for a continuous-time Markov model.
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

#include "MatrixReal.h"
#include "RateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class EigenSystem;
    class Simplex;
    class TransitionProbabilityMatrix;
    
    
    class RateMatrix_F81 : public RateMatrix {
        
    public:
        RateMatrix_F81(const RateMatrix_F81& m);                                                //!< Copy constructor
        RateMatrix_F81(size_t n);                                                               //!< Construct rate matrix with n states
        ~RateMatrix_F81(void);                                                                  //!< Destructor
        
        // overloaded operators
        RateMatrix_F81&                     operator=(const RateMatrix_F81& r);
        
        // RateMatrix functions
        void                                calculateStationaryFrequencies(void);                                               //!< Calculate the stationary frequencies for the rate matrix
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_F81*                     clone(void) const;
        
    private:
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix_F81& x);                                           //!< Overloaded output operator
    
}

#endif

