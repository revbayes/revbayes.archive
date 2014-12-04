/**
 * @file
 * This file contains the declaration of RateMatrix_Mk1, which is a
 * class that holds a rate matrix for the Markov k-state 1 parameter model. 
 * Parameters:
 * k states
 * lambda: rate
 *
 * @brief Declaration of RateMatrix_Mk1
 *
 * (c) copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */


#ifndef __RateMatrix_Mk1__
#define __RateMatrix_Mk1__

#include "AbstractRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateMatrix_Mk1 : public AbstractRateMatrix {
        
    public:
        RateMatrix_Mk1(size_t n);                                                  //!< Construct rate matrix with n states
        RateMatrix_Mk1(const RateMatrix_Mk1& m);                                  //!< Copy constructor
        virtual                         ~RateMatrix_Mk1(void);                     //!< Destructor
        
        // overloaded operators
        RateMatrix_Mk1&                operator=(const RateMatrix_Mk1& r);
        
        // RateMatrix functions
        double                          averageRate(void) const;
        void                            calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_Mk1*					clone(void) const;
        const std::vector<double>&      getStationaryFrequencies(void) const ;  //!< Return the stationary frequencies, although in this model I don't know them
        void                            updateMatrix(void);
        void                            setLambda(double l);
        
        
    private:
        double                          lambda;
        size_t matrixSize;                  //!< Number of elements in a row or column of the rate matrix 
        double precision;                  //!< Precision for exponentiation through repeated squaring
        std::vector<double>                 stationaryFreqs;        //!< Holds the stationary frequencies
		
        void buildRateMatrix(void);
    };
    
}

#endif /* defined(__RateMatrix_Mk1__) */
