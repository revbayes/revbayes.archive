/**
 * @file
 * This file contains the declaration of ObjectSlot, which is
 * used to describe and initialize a slot for a function
 * argument or a class member variable.
 *
 * @brief Declaration of ObjectSlot
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

#ifndef ObjectSlot_H
#define ObjectSlot_H

#include <ostream>
#include <sstream>

#include "RbObject.h"

// Forward declaration
#include "StringVector.h"


class ObjectSlot : public RbObject {

    public:

            ObjectSlot(const std::string valType, RbObject* defaultVal=NULL);   //!< Constructor
            ~ObjectSlot() { delete value; }            //! Destructor should delete value

        static const StringVector   rbClass;           //!< Static class attribute

        // Basic utility functions
        std::string         briefInfo() const;                          //!< Brief info about the object
        ObjectSlot*         clone() const { return new ObjectSlot(*this); } //!< Clone object
        bool                equals(const RbObject* obj) const;          //!< Equals comparison
        const StringVector& getClass() const { return rbClass; }        //!< Get class
        void                print(std::ostream& o) const;               //!< Print complete object info
        void                printValue(std::ostream& o) const;          //!< Print value (for user)

        // Regular functions
        std::string         getType() const { return type; }            //!< Get type
        const RbObject*     getValue() const { return value; }          //!< Get value
        void                setValue(RbObject* val);                    //!< Set value

    private:
	    std::string     type;           //!< Required type of the object
        RbObject*       value;          //!< Value, initialized with default for the slot (NULL if no default)
};

#endif

