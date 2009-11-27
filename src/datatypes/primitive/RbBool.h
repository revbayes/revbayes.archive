/**
 * @file
 * This file contains the declaration of RbBool, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Declaration of RbBool
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

#ifndef RbBool_H
#define RbBool_H

#include <iostream>

#include "RbPrimitive.h"
#include "StringVector.h"

class RbDumpState;
class RbObject;

class RbBool : public RbPrimitive {

public:

        static const StringVector   rbClass;            //!< Static class attribute

	RbBool(const bool v);                                            //!< Constructor from double
	RbBool(const RbBool& s);                                         //!< Copy constructor
	virtual ~RbBool();

        // Basic utility functions
        RbObject*           clone() const;                              //!< Clone object
        bool                equals(const RbObject* obj) const;          //!< Equals comparison
        const StringVector& getClass() const { return rbClass; }        //!< Get class
        void                print(std::ostream& o) const;               //!< Print complete object info
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< General info on object

        // Type conversion
        bool                isConvertible(const std::string& type) const;
        RbObject*           convertTo(const std::string& type) const;       //!< Convert to type
                                    operator int() const { return value; }               //!< Type conversion to int

        // overloaded operators
        RbObject&           operator=(const RbObject& obj);
        RbBool&             operator=(const RbBool& obj);

        // Pointer-based comparison -- throw not supported error by default
        bool                lessThan(const RbObject* o) const;                        //!< Less than

        // Pointer-based arithmetic -- throw not supported error by default
        RbObject*           add(const RbObject* o) const;                             //!< Addition
        RbObject*           subtract(const RbObject* o) const;                        //!< Subtraction
        RbObject*           multiply(const RbObject* o) const;                        //!< Multiplication
        RbObject*           divide(const RbObject* o) const;                          //!< Division
        RbObject*           raiseTo(const RbObject* o) const;
        

    // implemented abstract/virtual functions from base classes
	void       dump(std::ostream& c);                              //!< Dump to ostream c
	void       resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state

	// memeber functions
	void       setValue(bool v);
	bool        getValue(void) const;

private:
	bool        value;      //!< value member
};

#endif

