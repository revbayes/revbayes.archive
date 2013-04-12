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

#include "Cloneable.h"
#include "MatrixReal.h"
#include <complex>
#include <vector>


namespace RevBayesCore {

    class EigenSystem;
    class Simplex;
    class TransitionProbabilityMatrix;


    class RateMatrix : public Cloneable {

    public:
        virtual                            ~RateMatrix(void);                                                                 //!< Destructor

        // overloaded operators
        std::vector<double>&                operator[](size_t i);                                                             //!< Subscript operator
        const std::vector<double>&          operator[](size_t i) const;                                                       //!< Subscript operator (const)
           
        std::vector<std::vector<double> >::const_iterator       begin(void) const;
        std::vector<std::vector<double> >::iterator             begin(void);
        std::vector<std::vector<double> >::const_iterator       end(void) const;
        std::vector<std::vector<double> >::iterator             end(void);

        // RateMatrix functions
        double                              averageRate(void) const;                                                            //!< Calculate the average rate
        size_t                              getNumberOfStates(void) const;                                                      //!< Return the number of states
        const std::vector<double>&          getStationaryFrequencies(void) const;                                                   //!< Return the stationary frequencies
        void                                rescaleToAverageRate(double r);                                               //!< Rescale the rate matrix such that the average rate is "r"
        void                                setStationaryFrequencies(const std::vector<double>& f);                             //!< Directly set the stationary frequencies
        size_t                              size(void) const;                                                                   //!< Get the size of the rate matrix, which is the same as the number of states

        // pure virtual method you have to overwrite
        virtual void                        calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition probabilities for the rate matrix
        virtual RateMatrix*                 clone(void) const = 0;
        virtual void                        update(void) = 0 ;                                                                      //!< Update the transition matrix and all its parameters
        
    protected:
        // prevent instantiation
        RateMatrix(const RateMatrix& m);                                                  //!< Copy constructor
        RateMatrix(size_t n);                                                             //!< Construct rate matrix with n states
        RateMatrix&                         operator=(const RateMatrix& r);

//        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
//        bool                                checkTimeReversibity(double tolerance);                                             //!< Checks if the rate matrix is time reversible
//        void                                tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                //!< Calculate transition probabilities for real case
//        void                                tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;         //!< Calculate transition probabilities for complex case
//
//        bool                                eigensDirty;                                                                        //!< Does the eigensystem need to be recalculated
//        bool                                reversibilityChecked;                                                               //!< Flag indicating if time reversibility has been checked
//        bool                                timeReversible;                                                                     //!< Is the matrix time reversible
        size_t                              numStates;                                                                          //!< The number of character states
        MatrixReal*                         theRateMatrix;                                                                      //!< Holds the rate matrix
        std::vector<double>                 theStationaryFreqs;                                                                 //!< Holds the stationary frequencies
//        EigenSystem*                        theEigenSystem;                                                                     //!< Holds the eigen system
//        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
//        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
    

        // memberfunction return values
        double                              avgRate;
                       
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix& x);                                           //!< Overloaded output operator

}

#endif

