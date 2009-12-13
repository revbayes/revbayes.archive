/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the RevBayes abstract base class for all language objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbObject_H
#define RbObject_H

#include <ostream>
#include <string>

class StringVector;

class RbObject {

    public:
        virtual                     ~RbObject(void) {}                                  //! Virtual destructor

        // Basic utility functions you have to override (also getClass()!)
        virtual RbObject*           clone(void) const = 0;                              //!< Clone object
        virtual bool                equals(const RbObject* x) const = 0;                //!< Equals comparison
        virtual const StringVector& getClass(void) const;                               //!< Get class vector
        virtual void                printValue(std::ostream& o) const = 0;              //!< Print value for user
        virtual std::string         toString(void) const = 0;                           //!< Complete info about object

        // Basic utility functions you may want to override
        virtual std::string         briefInfo(void) const;                              //!< Brief info about object
        virtual RbObject*           convertTo(const std::string& type) const;           //! Convert to type
        virtual bool                isConvertibleTo(const std::string& type) const;     //! Is convertible to type?
        
        // Functions for containers: override if object is a container with elements
        virtual int                 getDim(void) const { return 0; }                    //!< Get subscript dimensions
        virtual const StringVector& getAtomicClass(void) const { return getClass(); }   //!< Get atomic class
        const std::string&          getAtomicType(void) const;                          //!< Get atomic type

        // Basic utility functions you do not have to override
        const std::string&          getType(void) const;                                //!< Get type
        bool                        isType(const std::string& type) const;              //!< Is the object of class type?
        void                        print(std::ostream& o) const;                       //!< Print complete object info

   protected:
		                            RbObject(void) {}                   //!< No objects of this class
};

#endif
