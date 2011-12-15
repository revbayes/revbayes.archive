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
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbLanguageObject_H
#define RbLanguageObject_H

#include "RbObject.h"
#include "RbPtr.h"

#include <ostream>
#include <string>

class ArgumentRule;
class DAGNode;
class TypeSpec;
class Vector;
class VectorInteger;
class VectorString;
class XmlDocument;
class XmlElement;

const std::string RbLanguageObject_name = "language object";

typedef std::vector<RbPtr<ArgumentRule> >   MemberRules;                                                                       //!< Member rules type def, for convenience

class RbLanguageObject : public RbObject {
    
public:
    virtual                            ~RbLanguageObject(void) {}                                               //!< Virtual destructor
    
    // Basic utility functions you have to override (also getClass()!)
    virtual RbLanguageObject*           clone(void) const = 0;                                                  //!< Clone object
    virtual RbPtr<XmlElement>           encode(RbPtr<XmlDocument> doc, const std::string& name);                //!< Function to encode this object into an XML string
    virtual const VectorString&         getClass(void) const;                                                   //!< Get class vector
    virtual void                        printValue(std::ostream& o) const = 0;                                  //!< Print value for user
    
    // Basic utility functions you may want to override
    virtual RbObject*                   convertTo(const TypeSpec& type) const;                                  //!< Convert to type
    virtual RbPtr<const MemberRules>    getMemberRules(void) const;                                             //!< Get member rules
    virtual void                        initialize(const RbPtr<Vector>& attributes);                            //!< Initialize this object with the values inside the vector
    virtual bool                        isConstant(void) const { return true; }                                 //!< Is value a constant or does it include variables?
    virtual bool                        isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type and dim?
    virtual bool                        supportsIndex(void) const { return false; }                             //!< Supports indexing, as in operator[]
        
    // Basic utility functions you should not have to orverwrite
    void                                print(std::ostream& o) const;                                           //!< Print complete object info
    
protected:
    RbLanguageObject(void) {}                                                                               //!< No objects of this class
};

// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RbLanguageObject& x);                 //!< Overloaded output operator



#endif
