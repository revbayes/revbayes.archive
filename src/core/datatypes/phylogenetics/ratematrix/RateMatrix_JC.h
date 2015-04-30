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

#include "TimeReversibleRateMatrix.h"


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    
    class RateMatrix_JC : public TimeReversibleRateMatrix {
        
    public:
        RateMatrix_JC(size_t n);                                                                                                                                    //!< Construct rate matrix with n states
        virtual                             ~RateMatrix_JC(void);                                                                                                   //!< Destructor
        
        // RateMatrix functions
        virtual RateMatrix_JC&              assign(const Assignable &m);                                                                                            //!< Assign operation that can be called on a base class instance.
        void                                calculateStationaryFrequencies(void);                                                                                   //!< Calculate the stationary frequencies for the rate matrix
        void                                calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;    //!< Calculate the transition matrix
        RateMatrix_JC*                      clone(void) const;
        void                                update(void);
        
    private:
        
        
    };
        
}

#endif

