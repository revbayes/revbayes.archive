/**
 * @file
 * This file contains the declaration of RateMatrix_JC, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix_JC
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-12-05 12:20:37 -0800 (Wed, 05 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RateMatrix.h 1901 2012-12-05 20:20:37Z boussau $
 */

#ifndef RateMatrix_JC_H
#define RateMatrix_JC_H

#include "MatrixReal.h"
#include "RateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class EigenSystem;
    class Simplex;
    class TransitionProbabilityMatrix;
    
    
    class RateMatrix_JC : public RateMatrix {
        
    public:
        RateMatrix_JC(const RateMatrix_JC& m);                                                //!< Copy constructor
        RateMatrix_JC(size_t n);                                                               //!< Construct rate matrix with n states
        RateMatrix_JC(void);                                                                   //!< Default constructor
        ~RateMatrix_JC(void);                                                                  //!< Destructor
        
        // overloaded operators
        RateMatrix_JC&                         operator=(const RateMatrix_JC& r);
        
        // RateMatrix functions
        void                                calculateStationaryFrequencies(void);                                               //!< Calculate the stationary frequencies for the rate matrix
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_JC*                      clone(void) const;
        bool                                isTimeReversible(void);                                                             //!< Return whether the rate matrix is time reversible
        void                                setTimeReversible(bool tf);                                                   //!< Directly set whether the rate matrix is time reversible
//        void                                setStationaryFrequencies(const std::vector<double>& f);                             //!< Directly set the stationary frequencies
        void                                update(void);
        
    private:
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix_JC& x);                                           //!< Overloaded output operator
    
}

#endif

