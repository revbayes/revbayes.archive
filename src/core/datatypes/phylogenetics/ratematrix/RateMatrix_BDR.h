/**
 * @file
 * This file contains the declaration of RateMatrix_BDR, which is a
 * class that holds a rate matrix for the chromosome number evolution model. 
 *
 * @brief Declaration of RateMatrix_BDR
 *
 * (c) copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */


#ifndef __RateMatrix_BDR__
#define __RateMatrix_BDR__

#include "AbstractRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateMatrix_BDR : public AbstractRateMatrix {
        
    public:
        RateMatrix_BDR(size_t n);                                                  //!< Construct rate matrix with n states
        virtual                         ~RateMatrix_BDR(void);                     //!< Destructor
        
        // RateMatrix functions
        double                          averageRate(void) const;
        void                            calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition matrix
        RateMatrix_BDR*                 clone(void) const;
        std::vector<double>             getStationaryFrequencies(void) const ;  //!< Return the stationary frequencies, although in this model I don't know them
        void                            update(void);
        void                            setLambda(double l);
        void                            setDelta(double d);
        void                            setRho(double r);
        void                            setMu(double m);
        void                            setLambda_l(double l);
        void                            setDelta_l(double d);
        
        
    private:
        double                          lambda;
        double                          delta;
        double                          rho;
        double                          mu;
        double                          lambda_l;
        double                          delta_l;
        size_t                          matrixSize;                         //!< Number of elements in a row or column of the rate matrix
        std::vector<double>             stationary_freqs;                    //!< Holds the stationary frequencies

        void                            buildRateMatrix(void);
        void                            exponentiateMatrixByScalingAndSquaring(double t,  TransitionProbabilityMatrix& p) const;
        inline void                     multiplyMatrices(TransitionProbabilityMatrix& p,  TransitionProbabilityMatrix& q,  TransitionProbabilityMatrix& r) const;

    };
    
}

#endif /* defined(__RateMatrix_BDR__) */
