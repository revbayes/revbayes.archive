/**
 * @file
 * This file contains the declaration of PosReal, which is
 * used to hold strictly positive scalar values.
 *
 * @brief Declaration of PosReal
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef PosReal_H
#define PosReal_H

#include "RbDouble.h"

#include <ostream>
#include <string>

class StringVector;

class PosReal : public RbDouble {

    public:
                            PosReal(const double v);                   //!< Construct from double
                            PosReal(const int v);                      //!< Construct from int 
                            PosReal(const bool v);                     //!< Construct from bool

        // Basic utility functions
        RbObject*           clone(void) const;                          //!< Clone object
        const StringVector& getClass(void) const;                       //!< Get class vector
        std::string         toString(void) const;                       //!< Complete info about object

        // Type conversion
        RbObject*           convertTo(const std::string& type) const;       //!< Convert to type

        // Setter disallowing nonpositive values
        void                setValue(double x);                         //!< Set value
};

#endif

