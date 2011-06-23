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


class TypeSpec;
class VectorInteger;
class VectorString;
class XmlDocument;
class XmlElement;

class RbObject {

    public:
        virtual                    ~RbObject(void) {}                                                       //!< Virtual destructor

        // Basic utility functions you have to override (also getClass()!)
        virtual RbObject*           clone(void) const = 0;                                                  //!< Clone object
        virtual const VectorString& getClass(void) const;                                                   //!< Get class vector
        virtual void                printValue(std::ostream& o) const = 0;                                  //!< Print value for user
        virtual std::string         richInfo(void) const = 0;                                               //!< Complete info about object

        virtual const XmlElement*   encode(XmlDocument* doc, const std::string& name);                      //!< function to encode this object into an XML string

        // Basic utility functions you may want to override
        virtual std::string         briefInfo(void) const;                                                  //!< Brief info about object
        virtual RbObject*           convertTo(const std::string& type, size_t dim) const;                   //!< Convert to type and dim
        virtual const TypeSpec      getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual bool                isConstant(void) const { return true; }                                 //!< Is value a constant or does it include variables?
        virtual bool                isConvertibleTo(const std::string& type, size_t dim, bool once) const;  //!< Is convertible to type and dim?
        virtual bool                supportsSubscripting(void) const { return false; }                      //!< Supports subscripting, as in operator[] 

        // Dimensions of object: override only if object is a container with dimensions exposed to language / parser
        virtual int                 getDim(void) const { return 0; }                                        //!< Get container dimensions

        // Basic utility functions you should not have to override
        RbObject*                   convertTo(const TypeSpec& typeSpec) const;                              //!< Convert to language object of type typeSpec
        const std::string&          getType(void) const;                                                    //!< Get type of object
        bool                        isConvertibleTo(const TypeSpec& typeSpec, bool once) const;             //!< Is convertible to type and dim?
        bool                        isType(const std::string& type) const;                                  //!< Is the object of type?
        bool                        isTypeSpec(const TypeSpec& typeSpec) const;                             //!< Does the language type of the object fit type specification typeSpec?
        void                        print(std::ostream& o) const;                                           //!< Print complete object info

   protected:
		                            RbObject(void) {}                                                   //!< No objects of this class
};


        // Global functions using the class
        std::ostream&               operator<<(std::ostream& o, const RbObject& x);                     //!< Overloaded output operator

#endif
