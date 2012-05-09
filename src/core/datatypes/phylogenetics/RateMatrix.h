/**
 * @file
 * This file contains the declaration of RateMatrix, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RateMatrix_H
#define RateMatrix_H

#include "Matrix.h"
#include <complex>
#include <vector>

class EigenSystem;
class Simplex;
class TransitionProbabilityMatrix;


class RateMatrix {

    public:
                                            RateMatrix(void);                                                                 //!< Default constructor (never call this except from the workspace once)
                                            RateMatrix(const RateMatrix& m);                                                  //!< Copy constructor
                                            RateMatrix(size_t n);                                                             //!< Construct rate matrix with n states
                                           ~RateMatrix(void);                                                                 //!< Destructor

        // overloaded operators
        RateMatrix&                         operator=(const RateMatrix& r);
        std::vector<double>&                operator[](size_t i);                                                             //!< Subscript operator
        const std::vector<double>&          operator[](size_t i) const;                                                       //!< Subscript operator (const)
            
        // RateMatrix functions
        double                              averageRate(void) const;                                                            //!< Calculate the average rate
        void                                calculateStationaryFrequencies(void);                                               //!< Calculate the stationary frequencies for the rate matrix
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        bool                                getAreEigensDirty(void) { return areEigensDirty; }                                  //!< Returns whether the eigensystem is in need of recalculation
        bool                                getIsTimeReversible(void);                                                          //!< Return whether the rate matrix is time reversible
        size_t                              getNumberOfStates(void) const { return numStates; }                                 //!< Return the number of states
        void                                rescaleToAverageRate(const double r);                                               //!< Rescale the rate matrix such that the average rate is "r"
        void                                setDiagonal(void);                                                                  //!< Set the diagonal such that each row sums to zero
        void                                setIsTimeReversible(const bool tf);                                                 //!< Directly set whether the rate matrix is time reversible
        void                                setStationaryFrequencies(const std::vector<double>& f);                             //!< Directly set the stationary frequencies
        void                                updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors

    private:
        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        bool                                checkTimeReversibity(double tolerance);                                             //!< Checks if the rate matrix is time reversible
        void                                tiProbsEigens(const double t, TransitionProbabilityMatrix& P) const;                //!< Calculate transition probabilities for real case
        void                                tiProbsComplexEigens(const double t, TransitionProbabilityMatrix& P) const;         //!< Calculate transition probabilities for complex case

        bool                                areEigensDirty;                                                                     //!< Does the eigensystem need to be recalculated
        bool                                reversibilityChecked;                                                               //!< Flag indicating if time reversibility has been checked
        bool                                isReversible;                                                                       //!< Is the matrix time reversible
        size_t                              numStates;                                                                          //!< The number of character states
        Matrix<double>*                     theRateMatrix;                                                                      //!< Holds the rate matrix
        Simplex*                            theStationaryFreqs;                                                                 //!< Holds the stationary frequencies
        EigenSystem*                        theEigenSystem;                                                                     //!< Holds the eigen system
        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
    

        // memberfunction return values
        double                              avgRate;
                       
};

#endif

