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
 * $Id$
 */

#ifndef VectorCharacters_H
#define VectorCharacters_H

#include "Vector.h"
#include "VectorString.h"
#include <ostream>
#include <string>
#include <vector>

class Character;
class CondLike;



class VectorCharacters : public Vector {

    public:
	    virtual Character&                      operator[](size_t i);                              //!< Index op allowing change
	    virtual const Character&                operator[](size_t i) const;                        //!< Const index op

        // Basic utility functions you have to override
        virtual VectorCharacters*               clone(void) const = 0;                             //!< Clone object
        virtual const VectorString&             getClass(void) const;                              //!< Get class

        // VectorCharacterObservations functions
        virtual CondLike*                       getCondLike(void);                                 //!< Initialize and return a conditional likelihood for this state     
        size_t                                  getNumCharacters(void) { return size(); }          //!< How many characters

    protected:
                                                VectorCharacters(const std::string& elemType);     //!< Set type spec of container from type of elements
};

#endif
