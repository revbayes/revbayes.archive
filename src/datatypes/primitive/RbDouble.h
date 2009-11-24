/**
 * @file
 * This file contains the declaration of RbDouble, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Declaration of RbDouble
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

#ifndef RbDouble_H
#define RbDouble_H

#include <iostream>

#include "RbPrimitive.h"

class RbDouble : public RbPrimitive {

    public:

            RbDouble(const double v);                         //!< Constructor from double
            RbDouble(const RbDouble& v);

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        RbObject*                  clone() const { return new RbDouble(*this); }   //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object

        // Type conversion
        RbObject*                  convertTo(const std::string& type) const;       //!< Convert to type
        bool                       isConvertibleTo(const std::string& type) const; //! Is convertible to type?
                                   operator double() { return value; }             //!< Type conversion to duble
        double                     getValue() { return value; }
        void                       setValue(double x) { value = x; }

        bool                       lessThan(const RbObject* o) const;  //!< Less than

        // Pointer-based arithmetic -- throw not supported error by default
        RbObject*                  add(const RbObject* o) const;       //!< Addition
        RbObject*                  subtract(const RbObject* o) const;  //!< Subtraction
        RbObject*                  multiply(const RbObject* o) const;  //!< Multiplication
        RbObject*                  divide(const RbObject* o) const;    //!< Division
        RbObject*                  raiseTo(const RbObject* o) const;   //!< Power

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     // virtual void                dump(std::ostream& o) = 0;              //!< Dump to ostream c
     // virtual void                resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state

    private:
	    double  value;              //!< Value member
};

#endif

