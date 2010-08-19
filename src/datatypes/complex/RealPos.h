/**
 * @file
 * This file contains the declaration of RealPos, which is
 * used to hold strictly positive scalar values.
 *
 * @brief Declaration of RealPos
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

#ifndef RealPos_H
#define RealPos_H

#include "Real.h"

#include <ostream>
#include <string>

class VectorString;

class RealPos : public Real {

    public:
                                    RealPos(const double v);                        //!< Construct from double
                                    RealPos(const int v);                           //!< Construct from int 
                                    RealPos(const bool v);                          //!< Construct from bool

        // Basic utility functions
        RbObject*                   clone(void) const;                               //!< Clone object
        const VectorString&         getClass(void) const;                            //!< Get class vector
        std::string                 toString(void) const;                            //!< Complete info about object

        // Setter disallowing nonpositive values
        void                        setValue(double x);                              //!< Set value

	protected:
        RbObject*                   convertTo(const std::string& type) const;        //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;  //!< Is convertible to type and dim?
};

#endif

