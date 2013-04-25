/**
 * @file
 * This file contains the declaration of RateMatrix_HKY, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix_HKY
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-12-05 12:20:37 -0800 (Wed, 05 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RateMatrix.h 1901 2012-12-05 20:20:37Z boussau $
 */

#ifndef RateMatrix_HKY_H
#define RateMatrix_HKY_H

#include "MatrixReal.h"
#include "RateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class EigenSystem;
    class Simplex;
    class TransitionProbabilityMatrix;
    
    
    class RateMatrix_HKY : public RateMatrix {
        
    public:
        RateMatrix_HKY(const RateMatrix_HKY& m);                                                //!< Copy constructor
        RateMatrix_HKY(void);                                                                   //!< Default constructor
        ~RateMatrix_HKY(void);                                                                  //!< Destructor
        
        // overloaded operators
        RateMatrix_HKY&                     operator=(const RateMatrix_HKY& r);
        
        // RateMatrix functions
        void                                calculateStationaryFrequencies(void);                                               //!< Calculate the stationary frequencies for the rate matrix
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_HKY*                     clone(void) const;
        void                                setKappa(double k);
        
    private:
        double                              kappa;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix_HKY& x);                                           //!< Overloaded output operator
    
}

#endif

