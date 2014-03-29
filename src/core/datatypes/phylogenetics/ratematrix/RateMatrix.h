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

        // pure virtual methods you have to overwrite
        virtual double                      getRate(size_t i, size_t j) const = 0;
        virtual RateMatrix*                 clone(void) const = 0;
        virtual void                        updateMatrix(void);
        
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix& x);                                           //!< Overloaded output operator

}

#endif

