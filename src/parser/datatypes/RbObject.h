/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the RevBayes abstract base class for all language objects.
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

#ifndef RbObject_H
#define RbObject_H

#include "RbValue.h"
#include "TypeSpec.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include <ostream>
#include <string>



class RbObject {

    public:
        virtual                            ~RbObject(void);                                                         //!< Virtual destructor

        // overloaded operators
        bool                                operator==(const RbObject& x) const { return &x == this; }
        bool                                operator!=(const RbObject& x) const { return !operator==(x); }
        bool                                operator<(const RbObject& x) const { return false; }
    
        // Basic utility functions you have to override (also getClassTypeSpec() and getClassName()!)
        virtual RbObject*                   clone(void) const = 0;                                                  //!< Clone object
        virtual XmlElement*                 encode(XmlDocument* doc, const std::string& name);                      //!< Function to encode this object into an XML string
        static const std::string&           getClassName(void);                                                     //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&             getTypeSpec(void) const = 0;                                            //!< Get language type of the object
        virtual void*                       getLeanValue(std::vector<size_t> &length) const { return NULL; }        //!< Transform the object into a basic element pointer for fast access.
        virtual size_t                      memorySize() const { return sizeof( *this ); }                          //!< Get the size
        virtual void                        setLeanValue(const RbValue<void*> &val);                                //!< Set the value of the object from a basic (lean) element pointer.
        virtual void                        printValue(std::ostream& o) const = 0;                                  //!< Print value for user

        // Basic utility functions you may want to override
        virtual RbObject*                   convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        virtual std::string                 debugInfo(void) const;                                                  //!< Complete info about object for developers (debugging purpose)
        virtual bool                        isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
    

        // Basic utility functions you should not have to override
        size_t                              decrementReferenceCount(void) const;                                    //!< Decrement the reference count for reference counting in smart pointers
        size_t                              getReferenceCount(void) const;                                          //!< Get the reference count for reference counting in smart pointers
        void                                incrementReferenceCount(void) const;                                    //!< Increment the reference count for reference counting in smart pointers
        bool                                isTypeSpec(const TypeSpec& typeSpec) const;                             //!< Does the language type of the object fit type specification typeSpec?

    protected:
        RbObject(void);                                                                                             //!< No objects of this class

    private:
        size_t                              refCount;                                                               //!< The reference count for smart pointers
 
};

// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RbObject& x);                         //!< Overloaded output operator

#endif
