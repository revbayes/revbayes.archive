/**
 * @file
 * This file contains the declaration of RbInt, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Declaration of RbInt
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef RbInt_H
#define RbInt_H

#include <iostream>

#include "RbPrimitive.h"

class RbInt : public RbPrimitive {

    public:

            RbInt(const int v);                         //!< Constructor from int

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        RbObject*           clone() const;                              //!< Clone object
        bool                equals(const RbObject* obj) const;          //!< Equals comparison
        const StringVector& getClass() const { return rbClass; }        //!< Get class
        void                print(std::ostream& o) const;               //!< Print complete object info
        void                printValue(std::ostream& o) const;          //!< Print value (for user)

        // Type conversion
        RbObject*           convertTo(const std::string& type) const;       //!< Convert to type
                                    operator int() const { return value; }               //!< Type conversion to int

        // Pointer-based comparison -- throw not supported error by default
        bool                lessThan(const RbObject* o) const;                        //!< Less than

        // Pointer-based arithmetic -- throw not supported error by default
        RbObject*           add(const RbObject* o) const;                             //!< Addition
        RbObject*           subtract(const RbObject* o) const;                        //!< Subtraction
        RbObject*           multiply(const RbObject* o) const;                        //!< Multiplication
        RbObject*           divide(const RbObject* o) const;                          //!< Division
        RbObject*           raiseTo(const RbObject* o) const;

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     // virtual void                dump(std::ostream& o) = 0;              //!< Dump to ostream c
     // virtual void                resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state

    private:
	    int     value;              //!< Value member
};

#endif

