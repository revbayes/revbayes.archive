/**
 * @file
 * This file contains the declaration of Real, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Declaration of Real
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

#ifndef Real_H
#define Real_H

#include "RbObject.h"

#include <ostream>
#include <string>

class VectorString;

/** @note Some functions are virtual because RealPos is derived from double */
class Real : public RbObject {

    public:
                                    Real(void);                                  //!< Default constructor (0.0)
                                    Real(const double v);                        //!< Construct from double
                                    Real(const int v);                           //!< Construct from int 
                                    Real(const bool v);                          //!< Construct from bool

        // Basic utility functions
        virtual RbObject*           clone(void) const;                               //!< Clone object
        bool                        equals(const RbObject* obj) const;               //!< Equals comparison
        virtual const VectorString& getClass(void) const;                            //!< Get class vector
        void                        printValue(std::ostream& o) const;               //!< Print value (for user)
        virtual std::string         toString(void) const;                            //!< Complete info about object

        // Type conversion
                                    operator double(void) const { return value; }    //!< Type conversion to double

        // Getters and setters
        virtual void                setValue(double x) { value = x; }                //!< Set value
        double                      getValue(void) const { return value; }           //!< Get value

	protected:
        virtual RbObject*           convertTo(const std::string& type) const;        //!< Convert to type
        virtual bool                isConvertibleTo(const std::string& type) const;  //!< Is convertible to type and dim?
		double                      value;                                           //!< Value member
};

#endif

