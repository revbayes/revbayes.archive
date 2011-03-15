/**
 * @file
 * This file contains the declaration of Boolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Declaration of Boolean
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

#ifndef Boolean_H
#define Boolean_H

#include "RbObject.h"

#include <ostream>
#include <string>

class VectorString;


class Boolean : public RbObject {

    public:
                                    Boolean(void);                                   //!< Default constructor
                                    Boolean(const bool v);                           //!< Construct from bool

        // Basic utility functions
        RbObject*                   clone(void) const;                               //!< Clone object
        bool                        equals(const RbObject* obj) const;               //!< Equals comparison
        const VectorString&         getClass(void) const;                            //!< Get class vector
        void                        printValue(std::ostream& o) const;               //!< Print value (for user)
        std::string                 toString(void) const;                            //!< Complete info about object

        // Type conversion
        RbObject*                   convertTo(const std::string& type, int dim=0) const;                        //!< Convert to type and dim
        bool                        isConvertibleTo(const std::string& type, int dim=0, bool once=false) const; //!< Is convertible to type and dim?
                                    operator bool(void) const { return value; }      //!< Type conversion to bool

        // Getters and setters
        void                        setValue(bool x) { value = x; }                  //!< Set value
        bool                        getValue(void) const { return value; }           //!< Get value

	protected:

    private:
        bool                        value;                                           //!< Value member
};

#endif

