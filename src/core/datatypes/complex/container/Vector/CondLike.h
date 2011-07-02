/**
 * @file
 * This file contains the declaration of CondLike, a
 * complex type used to hold (vectors of) conditional likelihoods
 * for likelihood calculations.
 *
 * @brief Declaration of CondLike
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CondLike_H
#define CondLike_H

#include "MemberObject.h"
#include "VectorString.h"

#include <ostream>
#include <string>
#include <valarray>

class CondLike : public MemberObject {

    public:
                                         CondLike(size_t nc, size_t ns);                                    //!< Construct an empty cond like vector
                                         CondLike(const CondLike& x);                                       //!< Copy constructor

        // Overloaded operators
        double&                          operator[](size_t i);                                              //!< Index op allowing change
        const double&                    operator[](size_t i) const;                                        //!< Const index op
        CondLike&                        operator=(const CondLike& x);                                      //!< Assignment operator

        // Basic utility functions
        CondLike*                        clone(void) const;                                                 //!< Clone object
        const VectorString&              getClass(void) const;                                              //!< Get class
        void                             printValue(std::ostream& o) const;                                 //!< Print value for user
        std::string                      richInfo(void) const;                                              //!< Complete info about object

        // VectorCondLike functions
        size_t                           getNumChars(void) const { return numChars; }                       //!< Get number of characters
        size_t                           getNumStates(void) const { return numStates; }                     //!< Get number of states
        std::valarray<double>&           getCondLikeCArray(void) { return clArray; }                        //!< Get array of conditional likelihoods

    private:
        std::valarray<double>            clArray;                                                           //!< Array of conditional likelihoods
        size_t                           numChars;                                                          //!< Number of characters
        size_t                           numStates;                                                         //!< Number of states
};
        // overload multiplication operators
        CondLike                         operator*(CondLike& a, CondLike& b);                               //!< operator * (Felsenstein pruning algorithm) 
        CondLike&                        operator*=(CondLike& a, CondLike& b);                              //!< operator *= (Felsenstein pruning algorithm)

#endif
