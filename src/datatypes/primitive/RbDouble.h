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

#include "RbObject.h"

class RbDouble : public RbObject {

    public:

            RbDouble(const double v);                         //!< Constructor from double

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        virtual RbDouble*           clone() const { return new RbDouble(*this); }   //!< Clone object
        virtual bool                equals(const RbObject* obj) const;              //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }            //!< Get class
        virtual void                print(std::ostream& o) const;                   //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;              //!< Print value (for user)

        // Type conversion
        virtual RbObject*           convertTo(const std::string& type) const;       //!< Convert to type
                                    operator double() { return value; }             //!< Type conversion to duble

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     // virtual void                dump(std::ostream& o) = 0;              //!< Dump to ostream c
     // virtual void                resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state

    private:
	    double  value;              //!< Value member
};

#endif

