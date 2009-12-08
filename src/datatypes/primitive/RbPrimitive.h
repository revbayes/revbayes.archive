/**
 * @file
 * This file contains the declaration of RbPrimitive, which is
 * a collective holder for primitive types in the RevBayes
 * language.
 *
 * @brief Declaration of RbPrimitive
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @since 2009-11-23, version 1.0
 *
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbPrimitive_H
#define RbPrimitive_H

#include "RbObject.h"

class StringVector;


/**
 * @todo Strictly speaking, the class is not necessary and it may add significant overhead
 *       for the important primitive types. Only three classes are ever going to inherit from
 *       this class so there is no need to protect programmers from deriving inappropriate primitive
 *       types in the future. Therefore, there is little value to add pure virtual functions here to
 *       enforce a common interface. Also, I can't come up with any situations in the language
 *       in which it might be useful to group the primitive types. I still like grouping the
 *       primitive types in a separate folder and describing them as a group in the language
 *       description, but this can be done without adding this class.  -- Fredrik
 */
class RbPrimitive : public RbObject {

    public:
        virtual                     ~RbPrimitive() {}       //! Virtual destructor because of virtual functions

        // Basic utility functions
        RbObject*                   clone(void) const = 0;                      //!< Clone object
        bool                        equals(const RbObject* obj) const = 0;      //!< Equals comparison
        virtual const StringVector& getClass(void) const;                       //!< Get class vector
        void                        printValue(std::ostream& o) const = 0;      //!< Print value (for user)
        std::string                 toString(void) const = 0;                   //!< Complete info about object

    protected:
                                    RbPrimitive() : RbObject() {}               //!< Impossible to create objects
};

#endif

