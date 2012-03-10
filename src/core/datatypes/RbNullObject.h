/**
 * @file
 * This file contains the declaration of RbNullObject, which is
 * the RevBayes class used for NULL references.
 *
 * @brief Declaration of RbNullObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-02-14
 *
 * $Id$
 */

#ifndef RbNullObject_H
#define RbNullObject_H

#include "RbLanguageObject.h"

#include <ostream>
#include <string>

class RbNullObject : public RbLanguageObject {
    
public:
    RbNullObject(void);    
    RbNullObject(const RbNullObject& n);                                                                        //!< Default constructor
    virtual                            ~RbNullObject(void) {}                                                   //!< Virtual destructor
    
    bool                                operator==(void* ptr);                                                  //!< Compare to NULL pointer
    bool                                operator==(const RbNullObject& obj);                                                  //!< Compare to NULL pointer
    bool                                operator==(const RbLanguageObject& obj);                                                  //!< Compare to NULL pointer
    
    static RbNullObject&     getInstance(void)                                           //!< Get the memory manager
    {
        static RbNullObject theInstance = RbNullObject();
        return theInstance;
    }

    
    // Basic utility functions you have to override (also getClassTypeSpec()!)
    RbNullObject*                       clone(void) const;                                                      //!< Clone object
//    virtual XmlElement*                 encode(XmlDocument* doc, const std::string& name);                      //!< Function to encode this object into an XML string
    static const std::string&           getClassName(void);                                                     //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                 //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get the type spec for this instance
    void                                printValue(std::ostream& o) const;                                      //!< Print value for user
        
    
};

// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RbNullObject& x);                 //!< Overloaded output operator



#endif
