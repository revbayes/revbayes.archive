/**
 * @file
 * This file contains the declaration of TransitionProbabilityMatrix, which is
 * class that holds a matrix of transition.
 *
 * @brief Declaration of TransitionProbabilityMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef TransitionProbabilityMatrix_H
#define TransitionProbabilityMatrix_H

#include "Matrix.h"

#include <vector>


class TransitionProbabilityMatrix {

    public:
                                            TransitionProbabilityMatrix(void);                                 //!< Default constructor (never call this except from the workspace once)
                                            TransitionProbabilityMatrix(const TransitionProbabilityMatrix& m); //!< Copy constructor
                                            TransitionProbabilityMatrix(size_t n);                             //!< Construct rate matrix with n states
                                           ~TransitionProbabilityMatrix(void);                                 //!< Destructor

        // overloaded operators
        TransitionProbabilityMatrix&        operator=(const TransitionProbabilityMatrix& m);
        std::vector<double>&                operator[](size_t i);                                              //!< Subscript operator
        const std::vector<double>&          operator[](size_t i) const;                                        //!< Subscript operator (const)
    
        // Basic utility functions
        TransitionProbabilityMatrix*        clone(void) const;                                                 //!< Clone object
 
    private:
        size_t                              numStates;                                                         //!< The number of character states
        Matrix<double>                      theMatrix;                                                         //!< Holds the transition probability matrix
    
};

#endif

