/**
 * @file
 * This file contains the declaration of ObjectSlot, which is
 * a slot for a variable. It contains a specification of the
 * declared (required) type and dimensions of the object, in
 * addition to the current value.
 *
 * @brief Declaration of ObjectSlot
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef ObjectSlot_H
#define ObjectSlot_H

#include <ostream>

#include "RbObject.h"


// Forward declaration
class StringVector;


class ObjectSlot {

    public:
            ObjectSlot(const std::string valType, int elementDim);  //!< Constructor with NULL init
            ObjectSlot(RbObject* initVal);                          //!< Constructor with object init
            ~ObjectSlot() { delete value; }                         //!< Destructor should delete value

        // Basic utility function
        void                print(std::ostream& o) const;               //!< Print complete object info

        // Regular functions
        int                 getDim() const { return dim; }              //!< Get number of dimensions
        const std::string&  getType() const { return type; }            //!< Get type
        std::string         getTypeDescr() const;                       //!< Get type description with dimensions
        const RbObject*     getValue() const { return value; }          //!< Get value
        void                setValue(RbObject* val);                    //!< Set value

    private:
	    std::string         type;           //!< Declared type of the object
        int                 dim;            //!< Declared dimension of the object
        RbObject*           value;          //!< Current value held by the slot
};

#endif

