/**
 * @file
 * This file contains the declaration of RbDouble, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Declaration of RbDouble
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

#ifndef RbDouble_H
#define RbDouble_H

#include "RbObject.h"

#include <ostream>
#include <string>

class StringVector;

/** @note Some functions are virtual because PosReal is derived from double */
class RbDouble : public RbObject {

    public:
                                    RbDouble(void);                                  //!< Default constructor (0.0)
                                    RbDouble(const double v);                        //!< Construct from double
                                    RbDouble(const int v);                           //!< Construct from int 
                                    RbDouble(const bool v);                          //!< Construct from bool

        // Basic utility functions
        virtual RbObject*           clone(void) const;                               //!< Clone object
        bool                        equals(const RbObject* obj) const;               //!< Equals comparison
        virtual const StringVector& getClass(void) const;                            //!< Get class vector
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

