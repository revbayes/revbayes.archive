/**
 * @file
 * This file contains the declaration of VectorCondLike, a
 * complex type used to hold vectors of conditional likelihoods
 * for likelihood calculations.
 *
 * @brief Declaration of VectorCondLike
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef VectorCondLike_H
#define VectorCondLike_H

#include "MemberObject.h"
#include "VectorString.h"

#include <ostream>
#include <string>
#include <vector>


class VectorCondLike : public MemberObject {

    public:
                                VectorCondLike(int numChars, int numStates);                //!< Construct an empty cond like vector
                                VectorCondLike(const VectorCondLike& x);                    //!< Copy constructor
                               ~VectorCondLike(void);                                       //!< Destructor frees memory for cond like array

        // Overloaded assignment operator
        VectorCondLike&         operator=(const VectorCondLike& x);                         //!< Assignment operator

        // Basic utility functions
        VectorCondLike*         clone(void) const;                                          //!< Clone object
        const VectorString&     getClass(void) const;                                       //!< Get class
        void                    printValue(std::ostream& o) const;                          //!< Print value for user
        std::string             richInfo(void) const;                                       //!< Complete info about object

        // VectorCondLike functions
        size_t                  getNumChars(void) const { return numChars; }                //!< Get number of characters
        size_t                  getNumStates(void) const { return numStates; }              //!< Get number of states
        const double*           getCondLikeCArray(void) const { return condLikeArray; }     //!< Get array of conditional likelihoods

    private:
        double*                 condLikeArray;                                              //!< Array of conditional likelihoods
        size_t                  numChars;                                                   //!< Number of characters
        size_t                  numStates;                                                  //!< Number of states
};

#endif
