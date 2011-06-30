/**
 * @file
 * This file contains the declaration of CharacterObservation, which is
 * the abstract base class for character observations, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of CharacterObservation
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#ifndef CharacterObservation_H
#define CharacterObservation_H

#include "RbObject.h"
#include <ostream>



class CharacterObservation : public RbObject {

    public:
        // Basic utility functions you should not have to override
        virtual void                    printValue(std::ostream& o) const = 0;                //!< Print value (for user)

        // Basic utility functions you have to override
        virtual CharacterObservation*   clone(void) const = 0;                                //!< Clone object
        virtual const VectorString&     getClass(void) const;                                 //!< Get class vector
        virtual std::string             richInfo(void) const = 0;                             //!< Complete info about object

    protected:
                                        CharacterObservation(void);                           //!< Default constructor
};

#endif

