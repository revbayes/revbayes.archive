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

#include "RbObject.h"
#include "RbPrimitive.h"

class RbDumpState;

class RbInt : public RbPrimitive {

    public:

            RbInt(const int v);                         //!< Constructor from int
            RbInt(const RbInt& d);
            ~RbInt(void);

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        RbObject*           clone() const;                              //!< Clone object
        bool                equals(const RbInt* obj) const;          //!< Equals comparison
        bool                equals(const RbObject* obj) const;          //!< Equals comparison
        const StringVector& getClass() const { return rbClass; }        //!< Get class
        void                print(std::ostream& o) const;               //!< Print complete object info
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< General info on object

        // Type conversion
        bool                isConvertibleTo(const std::string& type) const;
        RbObject*           convertTo(const std::string& type) const;       //!< Convert to type
                            operator int() const;                          //!< Type conversion to int

        // Pointer-based comparison -- throw not supported error by default
        bool                lessThan(const RbObject* o) const;                        //!< Less than

        // Pointer-based arithmetic -- throw not supported error by default
        RbObject*           add(const RbObject* o) const;                             //!< Addition
        RbObject*           subtract(const RbObject* o) const;                        //!< Subtraction
        RbObject*           multiply(const RbObject* o) const;                        //!< Multiplication
        RbObject*           divide(const RbObject* o) const;                          //!< Division
        RbObject*           raiseTo(const RbObject* o) const;
        int                 getValue(void) const { return value; }
        void                setValue(int x) { value = x; }

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     void                dump(std::ostream& o);              //!< Dump to ostream c
     void                resurrect(const RbDumpState& x);    //!< Resurrect from dumped state

    private:
	    int     value;              //!< Value member
};

#endif

