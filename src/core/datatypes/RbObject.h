/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the RevBayes abstract base class for all language objects.
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

#ifndef RbObject_H
#define RbObject_H

#include <ostream>
#include <string>


class DAGNode;
class TypeSpec;
class VectorInteger;
class VectorString;
class XmlDocument;
class XmlElement;

class RbObject {

    public:
        virtual                    ~RbObject(void);                                                         //!< Virtual destructor

        // Basic utility functions you have to override (also getClass()!)
        virtual RbObject*           clone(void) const = 0;                                                  //!< Clone object
        virtual const XmlElement*   encode(XmlDocument* doc, const std::string& name);                      //!< Function to encode this object into an XML string
        virtual const VectorString& getClass(void) const;                                                   //!< Get class vector

        // Basic utility functions you may want to override
        virtual bool                allowsVariableInsertion(void) const { return false; }                   //!< Do we allow variable to be inserted in this object (only appicable for some container, e.g. DagNodeContainer)
        virtual RbObject*           convertTo(const std::string& type) const;                               //!< Convert to type
        virtual RbObject*           getElement(size_t index);                                               //!< Get element or subcontainer
        virtual const TypeSpec      getTypeSpec(void) const;                                                //!< Get language type of the object

        virtual bool                isConvertibleTo(const std::string& type, bool once) const;              //!< Is convertible to type?
        virtual bool                isType(const std::string& type) const;                                  //!< Is the object of type?
        virtual bool                isTypeSpec(const TypeSpec& typeSpec) const;                             //!< Does the language type of the object fit type specification typeSpec?
    

        // Basic utility functions you should not have to override
        RbObject*                   convertTo(const TypeSpec& typeSpec) const;                              //!< Convert to language object of type typeSpec
        size_t                      getNumberOfReferences(void) { return numReferences; }                   //!< Get the number how often this instance is referenced by other objects
        const std::string&          getType(void) const;                                                    //!< Get type of object
        bool                        isConvertibleTo(const TypeSpec& typeSpec, bool once) const;             //!< Is convertible to type?
        bool                        isUnreferenced(void) { return numReferences == 0; }                     //!< Test whether other objetcs still hold references to this object
//        bool                        isUnreferenced(void) { return false; }                                  //!< Test whether other objetcs still hold references to this object
        void                        release(void) { numReferences--; }                                      //!< release the object
        void                        retain(void) { numReferences++; }                                       //!< retain the object

    protected:
                                    RbObject(void) { numReferences = 0; }                                   //!< No objects of this class
    
    private:
        size_t                      numReferences;
};
#endif
