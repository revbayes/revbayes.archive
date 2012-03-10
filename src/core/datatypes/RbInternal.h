/**
 * @file
 * This file contains the declaration of RbInternal, which is
 * the RevBayes abstract base class for internal language objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id$
 */

#ifndef RbInternal_H
#define RbInternal_H

#include "RbObject.h"

#include <ostream>
#include <string>

class RbInternal : public RbObject { 

    public:
		virtual                        ~RbInternal(void) {}                     //! Virtual destructor

        // Basic utility functions you have to override
        virtual RbInternal*             clone(void) const = 0;                  //!< Clone object
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec

    
        // Basic utility functions you can overwrite
        virtual void                    printValue(std::ostream& o) const = 0;  //!< Print value for user

   protected:
                                        RbInternal(void) {}                     //!< No objects of this class
};

#endif

