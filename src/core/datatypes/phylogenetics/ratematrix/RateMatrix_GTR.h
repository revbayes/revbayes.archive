/**
 * @file
 * This file contains the declaration of RateMatrix_GTR, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix_GTR
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-12-05 12:20:37 -0800 (Wed, 05 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RateMatrix.h 1901 2012-12-05 20:20:37Z boussau $
 */

#ifndef RateMatrix_GTR_H
#define RateMatrix_GTR_H

#include "TimeReversibleRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class EigenSystem;
    class TransitionProbabilityMatrix;
    
    
    class RateMatrix_GTR : public TimeReversibleRateMatrix {
        
    public:
        RateMatrix_GTR(size_t n);                                                                                               //!< Construct rate matrix with n states
        RateMatrix_GTR(const RateMatrix_GTR& m);                                                                                //!< Copy constructor
        virtual                             ~RateMatrix_GTR(void);                                                              //!< Destructor
        
        // overloaded operators
        RateMatrix_GTR&                     operator=(const RateMatrix_GTR& r);
        
        // RateMatrix functions
        RateMatrix_GTR&                     assign(const Assignable &m);
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_GTR*                     clone(void) const;
        void                                updateMatrix(void);
        
    private:
        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        void                                tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                      //!< Calculate transition probabilities for real case
        void                                tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;               //!< Calculate transition probabilities for complex case
        void                                updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors
        
        EigenSystem*                        theEigenSystem;                                                                     //!< Holds the eigen system
        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
        
                
    };
        
}

#endif

