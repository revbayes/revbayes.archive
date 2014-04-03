/**
 * @file
 * This file contains the declaration of TimeReversibleRateMatrix, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of TimeReversibleRateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-12-05 12:20:37 -0800 (Wed, 05 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RateMatrix.h 1901 2012-12-05 20:20:37Z boussau $
 */

#ifndef TimeReversibleRateMatrix_H
#define TimeReversibleRateMatrix_H

#include "MatrixReal.h"
#include "RateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TimeReversibleRateMatrix : public RateMatrix {
        
    public:
        ~TimeReversibleRateMatrix(void);                                                                            //!< Destructor
        
        
        // pure virtual method
        virtual void                        calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition probabilities for the rate matrix
        virtual TimeReversibleRateMatrix*   clone(void) const = 0;
        virtual void                        updateMatrix(void) = 0;                                                                 //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)

        // public methods
        double                              averageRate(void) const;                                                            //!< Calculate the average rate
        void                                computeOffDiagonal(void);
        const std::vector<double>&          getExchangeabilityRates(void) const;
        const std::vector<double>&          getStationaryFrequencies(void) const;                                                   //!< Return the stationary frequencies
        bool                                isTimeReversible(void);                                                             //!< Return whether the rate matrix is time reversible
        void                                setExchangeabilityRates(const std::vector<double> &er);
        void                                setStationaryFrequencies(const std::vector<double>& f);                             //!< Directly set the stationary frequencies

        
    protected:        
        TimeReversibleRateMatrix(size_t n);                                                                         //!< Construct rate matrix with n states
        TimeReversibleRateMatrix(const TimeReversibleRateMatrix& m);                                                //!< Copy constructor

        // overloaded operators
        TimeReversibleRateMatrix&           operator=(const TimeReversibleRateMatrix& r);
    
        std::vector<double>                 exchangeabilityRates;
        std::vector<double>                 stationaryFreqs;                                                                 //!< Holds the stationary frequencies
        
    };
        
}

#endif

