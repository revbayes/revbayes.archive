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
#include <vector>


namespace RevBayesCore {

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

        // public methods
        size_t                              getNumberOfStates(void) const;                                                          //!< Return the number of states
        void                                rescaleToAverageRate(double r);                                                         //!< Rescale the rate matrix such that the average rate is "r"
        void                                setDiagonal(void);                                                                      //!< Set the diagonal such that each row sums to zero
        size_t                              size(void) const;                                                                       //!< Get the size of the rate matrix, which is the same as the number of states

        // pure virtual methods you have to overwrite
        virtual double                      averageRate(void) const = 0;                                                            //!< Calculate the average rate
        virtual void                        calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition probabilities for the rate matrix
        virtual RateMatrix*                 clone(void) const = 0;
        virtual const std::vector<double>&  getStationaryFrequencies(void) const = 0;                                               //!< Return the stationary frequencies
        virtual void                        updateMatrix(void) = 0;                                                                 //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
    protected:
        // prevent instantiation
        RateMatrix(const RateMatrix& m);                                                  //!< Copy constructor
        RateMatrix(size_t n);                                                             //!< Construct rate matrix with n states
        RateMatrix&                         operator=(const RateMatrix& r);

        // protected methods available for derived classes
        bool                                checkTimeReversibity(double tolerance);
        
        // protected members available for derived classes
        size_t                              numStates;                                                                          //!< The number of character states
        MatrixReal*                         theRateMatrix;                                                                      //!< Holds the rate matrix
        bool                                needsUpdate;
              
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix& x);                                           //!< Overloaded output operator

}

#endif

