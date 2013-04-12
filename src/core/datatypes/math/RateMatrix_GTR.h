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

#include "MatrixReal.h"
#include "RateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class EigenSystem;
    class Simplex;
    class TransitionProbabilityMatrix;
    
    
    class RateMatrix_GTR : public RateMatrix {
        
    public:
        RateMatrix_GTR(const RateMatrix_GTR& m);                                                //!< Copy constructor
        RateMatrix_GTR(size_t n);                                                               //!< Construct rate matrix with n states
        ~RateMatrix_GTR(void);                                                                  //!< Destructor
        
        // overloaded operators
        RateMatrix_GTR&                         operator=(const RateMatrix_GTR& r);
        
        // RateMatrix functions
        bool                                areEigensDirty(void) const ;                                                        //!< Returns whether the eigensystem is in need of recalculation
        void                                calculateStationaryFrequencies(void);                                               //!< Calculate the stationary frequencies for the rate matrix
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMatrix_GTR*                     clone(void) const;
        bool                                isTimeReversible(void);                                                             //!< Return whether the rate matrix is time reversible
        void                                setDiagonal(void);                                                                  //!< Set the diagonal such that each row sums to zero
        void                                setTimeReversible(bool tf);                                                   //!< Directly set whether the rate matrix is time reversible
        void                                updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors
        void                                update(void);

    private:
        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        bool                                checkTimeReversibity(double tolerance);                                             //!< Checks if the rate matrix is time reversible
        void                                tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                //!< Calculate transition probabilities for real case
        void                                tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;         //!< Calculate transition probabilities for complex case
        
        bool                                eigensDirty;                                                                        //!< Does the eigensystem need to be recalculated
        bool                                reversibilityChecked;                                                               //!< Flag indicating if time reversibility has been checked
        bool                                timeReversible;                                                                     //!< Is the matrix time reversible
        EigenSystem*                        theEigenSystem;                                                                     //!< Holds the eigen system
        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
        
                
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix_GTR& x);                                           //!< Overloaded output operator
    
}

#endif

