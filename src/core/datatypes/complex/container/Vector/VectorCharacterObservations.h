/**
 * @file
 * This file contains the declaration of VectorCharacterObservations, an
 * abstract base class for different types of character observation vectors.
 *
 * @brief Declaration of VectorCharacterObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
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
    
        // Basic utility functions you have to override
        virtual VectorCharacterObservations*    clone(void) const = 0;                                    //!< Clone object
        virtual const VectorString&             getClass(void) const;                                     //!< Get class

        // VectorCharacterObservations functions
        size_t                                  getNumCharacters(void) { return size(); }

    protected:
                                                VectorCharacterObservations(const std::string& elemType); //!< Set type spec of container from type of elements
};

#endif
