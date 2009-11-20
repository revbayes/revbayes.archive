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

class RbInt : public RbObject {

    public:

            RbInt(const int v);                         //!< Constructor from int

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        virtual RbInt*              clone() const { return new RbInt(*this); }  //!< Clone object
        virtual bool                equals(const RbObject* obj) const;          //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }        //!< Get class
        virtual void                print(std::ostream& o) const;               //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;          //!< Print value (for user)

        // Type conversion
        virtual RbObject*           convertTo(const std::string& type) const;       //!< Convert to type
                                    operator int() { return value; }               //!< Type conversion to int

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     // virtual void                dump(std::ostream& o) = 0;              //!< Dump to ostream c
     // virtual void                resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state

    private:
	    int     value;              //!< Value member
};

#endif

