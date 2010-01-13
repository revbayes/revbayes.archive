/**
 * @file
 * This file contains the declaration of RbBool, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Declaration of RbBool
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

#ifndef RbBool_H
#define RbBool_H

#include "RbObject.h"

#include <ostream>
#include <string>

class StringVector;


class RbBool : public RbObject {

    public:
	                        RbBool(const bool v);                       //!< Construct from bool

        // Basic utility functions
        RbObject*           clone(void) const;                          //!< Clone object
        bool                equals(const RbObject* obj) const;          //!< Equals comparison
        const StringVector& getClass(void) const;                       //!< Get class vector
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< Complete info about object

        // Type conversion
        RbObject*           convertTo(const std::string& type) const;   //!< Convert to type
                            operator bool(void) const { return value; } //!< Type conversion to bool

        // Getters and setters
        void                setValue(bool x) { value = x; }             //!< Set value
	    bool                getValue(void) const { return value; }      //!< Get value

    private:
	    bool                value;                                      //!< Value member
};

#endif

