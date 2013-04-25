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

#include "MatrixReal.h"

#include <vector>

namespace RevBayesCore {

    class TransitionProbabilityMatrix {

    public:
                                            TransitionProbabilityMatrix(size_t n);                              //!< Constructor

        // overloaded operators
        std::vector<double>&                operator[](size_t i);                                               //!< Subscript operator
        const std::vector<double>&          operator[](size_t i) const;                                         //!< Subscript operator (const)
        
        std::vector<std::vector<double> >::const_iterator       begin(void) const;
        std::vector<std::vector<double> >::iterator             begin(void);
        std::vector<std::vector<double> >::const_iterator       end(void) const;
        std::vector<std::vector<double> >::iterator             end(void);

        size_t                              getNumberOfStates(void) const;
        size_t                              size(void) const;
 
    private:
        size_t                              numStates;                                                          //!< The number of character states
        MatrixReal                          theMatrix;                                                          //!< Holds the transition probability matrix
    
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const TransitionProbabilityMatrix& x);                                           //!< Overloaded output operator

    
}

#endif

