/**
 * @file
 * This file contains the declaration of RbInternal, which is
 * the RevBayes abstract base class for internal language objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbInternal_H
#define RbInternal_H

#include <ostream>
#include <string>

class VectorString;

class RbInternal {

    public:
		virtual                    ~RbInternal(void) {}                     //! Virtual destructor

        // Basic utility functions you have to override
        virtual RbInternal*         clone(void) const = 0;                  //!< Clone object
        virtual const VectorString& getClass(void) const;                   //!< Get class vector
        virtual std::string         toString(void) const = 0;               //!< Print object to string

        // Basic utility functions you may want to override
        virtual void                printValue(std::ostream& o) const {}    //!< Print value for user

        // Basic utility functions you do not have to override
        const std::string&          getType(void) const;                    //!< Get type
        bool                        isType(const std::string& type) const;  //!< Is the object of type?
        void                        print(std::ostream& o) const;           //!< Print object

   protected:
		                            RbInternal(void) {}                     //!< No objects of this class
};

#endif

