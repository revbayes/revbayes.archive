/**
 * @file
 * This file contains the declaration of VectorCharacterObservations, a
 * vector type used to hold the character observations for a taxon.
 *
 * @brief Declaration of VectorCharacterObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef VectorCharacterObservations_H
#define VectorCharacterObservations_H

#include "Vector.h"
#include "VectorString.h"

#include <ostream>
#include <string>
#include <vector>


class VectorCharacterObservations : public Vector {

    public:
                                        VectorCharacterObservations(int numChars);                           //!< Construct an empty cond like vector
                                        VectorCharacterObservations(const VectorCharacterObservations& x);   //!< Copy constructor
                                       ~VectorCharacterObservations(void);                                   //!< Destructor frees memory for cond like array

        // Overloaded assignment operator
        VectorString&                   operator=(const VectorCharacterObservations& x);                     //!< Assignment operator

        // Basic utility functions
        VectorCharacterObservations*    clone(void) const;                                                  //!< Clone object
        const VectorString&             getClass(void) const;                                               //!< Get class
        void                            printValue(std::ostream& o) const;                                  //!< Print value for user
        std::string                     richInfo(void) const;                                               //!< Complete info about object

        // VectorCondLike functions
        size_t                          getNumChars(void) const { return numChars; }                        //!< Get number of characters

    private:
        double*                         condLikeArray;                                                      //!< Array of conditional likelihoods
        size_t                          numChars;                                                           //!< Number of characters
};

#endif
