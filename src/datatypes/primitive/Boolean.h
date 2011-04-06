/**
 * @file
 * This file contains the declaration of Boolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Declaration of Boolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef Boolean_H
#define Boolean_H

#include "RbObject.h"

#include <ostream>
#include <string>

class VectorString;


class Boolean : public RbObject {

    public:
        friend class            VectorBoolean;                                                  //!< Give VectorBoolean direct access to value

                                Boolean(void);                                                  //!< Default constructor
                                Boolean(const bool v);                                          //!< Construct from bool

        // Operators
                                operator bool(void) const { return value; }                     //!< Type conversion to bool

        // Basic utility functions
        Boolean*                clone(void) const;                                              //!< Clone object
        RbObject*               convertTo(const std::string& type, int dim) const;              //!< Convert to type and dim
        const VectorString&     getClass(void) const;                                           //!< Get class vector
        bool                    isConvertibleTo(const std::string& type, int dim, bool once) const;     //!< Is convertible to type and dim?
        void                    printValue(std::ostream& o) const;                              //!< Print value (for user)
        std::string             richInfo(void) const;                                           //!< Complete info about object

        // Getters and setters
        void                    setValue(bool x) { value = x; }                                 //!< Set value
        bool                    getValue(void) const { return value; }                          //!< Get value

    private:
        bool&                   getValueRef(void) { return value; }                             //!< Get value reference

        bool                    value;                                                          //!< Value member
};

#endif

