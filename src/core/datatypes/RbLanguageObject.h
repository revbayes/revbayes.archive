/**
 * @file
 * This file contains the declaration of RbLanguageObject, which is
 * the RevBayes abstract base class for all language objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-07-27
 *
 * $Id$
 */

#ifndef RbLanguageObject_H
#define RbLanguageObject_H

#include "RbObject.h"

#include <ostream>
#include <string>

class ArgumentRule;
class ArgumentRules;
class DAGNode;
class TypeSpec;
class Vector;
class VectorInteger;
class VectorString;
class XmlDocument;
class XmlElement;

typedef ArgumentRules MemberRules;                                                                       //!< Member rules type def, for convenience

class RbLanguageObject : public RbObject {
    
public:
    virtual                            ~RbLanguageObject(void) {}                                               //!< Virtual destructor
    
    // Basic utility functions you have to override (also getClassTypeSpec()!)
    virtual RbLanguageObject*           clone(void) const = 0;                                                  //!< Clone object
    virtual XmlElement*                 encode(XmlDocument* doc, const std::string& name);                      //!< Function to encode this object into an XML string
    static const std::string&           getClassName(void);                                                     //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                 //!< Get class type spec
    virtual const TypeSpec&             getTypeSpec(void) const = 0;                                            //!< Get the type spec of the instance
    virtual void                        printValue(std::ostream& o) const = 0;                                  //!< Print value for user
    
    // Basic utility functions you may want to override
    virtual RbObject*                   convertTo(const TypeSpec& type) const;                                  //!< Convert to type
    virtual const MemberRules&          getMemberRules(void) const;                                             //!< Get member rules
    virtual void                        initialize(const Vector& attributes);                                   //!< Initialize this object with the values inside the vector
    virtual bool                        isConstant(void) const { return true; }                                 //!< Is value a constant or does it include variables?
    virtual bool                        isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type and dim?
    virtual bool                        supportsIndex(void) const { return false; }                             //!< Supports indexing, as in operator[]
        
    
protected:
    RbLanguageObject(void) {}                                                                               //!< No objects of this class
};

// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RbLanguageObject& x);                 //!< Overloaded output operator



#endif
