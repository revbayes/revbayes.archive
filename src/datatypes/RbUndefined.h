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

#ifndef RbUndefined_H
#define RbUndefined_H

#include <iostream>

#include "RbObject.h"

class RbUndefined : public RbObject {

    public:

            RbUndefined(void);                         //!< Constructor from double
            RbUndefined(const RbUndefined& v);

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        RbObject*                  clone() const { return new RbUndefined(*this); }   //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object


        // overloaded operators
        RbObject&                  operator=(const RbObject& o);
        RbUndefined&                  operator=(const RbUndefined& o);

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     // virtual void                dump(std::ostream& o) = 0;              //!< Dump to ostream c
     // virtual void                resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state

    private:
};

#endif

