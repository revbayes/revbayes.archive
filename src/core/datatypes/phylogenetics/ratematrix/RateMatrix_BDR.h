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
        
        void                            setAlpha(double a);
        void                            setBeta(double b);
        void                            setLambdaA(double l_a);
        void                            setMuA(double m_a);
        void                            setLambdaI(double l_i);
        void                            setMuI(double m_i);
        void                            setLambdaAI(double l_ai);
        void                            setLambdaIA(double l_ia);
        void                            setDenovoI(double d_i);
        void                            setDenovoA(double d_a);
        
    private:
        
        double                          alpha;
        double                          beta;
        double                          lambda_a;
        double                          mu_a;
        double                          lambda_i;
        double                          mu_i;
        double                          lambda_ai;
        double                          lambda_ia;
        double                          denovo_i;
        double                          denovo_a;
        size_t                          matrixSize;                         //!< Number of elements in a row or column of the rate matrix
        size_t                          maxGenes;

        void                            buildRateMatrix(void);
        void                            exponentiateMatrixByScalingAndSquaring(double t,  TransitionProbabilityMatrix& p) const;
        inline void                     multiplyMatrices(TransitionProbabilityMatrix& p,  TransitionProbabilityMatrix& q,  TransitionProbabilityMatrix& r) const;

    };
    
}

#endif /* defined(__RateMatrix_BDR__) */
