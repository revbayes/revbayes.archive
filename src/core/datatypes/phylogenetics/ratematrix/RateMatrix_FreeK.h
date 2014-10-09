//
//  RateMatrix_FreeBinary.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateMatrix_FreeBinary__
#define __rb_mlandis__RateMatrix_FreeBinary__

#include "GeneralRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateMatrix_FreeBinary : public GeneralRateMatrix {
        
    public:
        RateMatrix_FreeBinary(size_t n);                                                                                               //!< Construct rate matrix with n states
        RateMatrix_FreeBinary(const RateMatrix_FreeBinary& m);                                                                                //!< Copy constructor
        virtual                         ~RateMatrix_FreeBinary(void);                                                              //!< Destructor
        
        // overloaded operators
        RateMatrix_FreeBinary&          operator=(const RateMatrix_FreeBinary& r);
        
        // RateMatrix functions
        double                          averageRate(void) const;
        void                            calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_FreeBinary*          clone(void) const;
        void                            fillRateMatrix(void);
        void                            updateMatrix(void);
        
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

#endif /* defined(__rb_mlandis__RateMatrix_FreeBinary__) */
