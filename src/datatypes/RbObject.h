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

class RbObject {

    public:
        virtual                    ~RbObject(void) {}                                               //! Virtual destructor

        // Basic utility functions you have to override (also getClass()!)
        virtual RbObject*           clone(void) const = 0;                                          //!< Clone object
        virtual bool                equals(const RbObject* x) const = 0;                            //!< Equals comparison
        virtual const VectorString& getClass(void) const;                                           //!< Get class vector
        virtual void                printValue(std::ostream& o) const = 0;                          //!< Print value for user
        virtual std::string         richInfo(void) const = 0;                                       //!< Complete info about object

        // Basic utility functions you may want to override
        virtual std::string         briefInfo(void) const;                                              //!< Brief info about object
        virtual RbObject*           convertTo(const TypeSpec& typeSpec) const;                          //! Convert to language object of type typeSpec
        virtual bool                isConvertibleTo(const TypeSpec& typeSpec, bool once=false) const;   //! Is convertible to language object of type typeSpec?
        
        // Subscript dimensions: override if object supports subscripting
        virtual int                 getDim(void) const { return 0; }                                //!< Get subscript dimensions

        // Basic utility functions you do not have to override
        const std::string&          getType(void) const;                                            //!< Get type of object
        const TypeSpec              getTypeSpec(void) const;                                        //!< Get language type of the object
        bool                        isType(const std::string& type) const;                          //!< Is the object of type (ignoring dimensions and element types, which influence language type)?
        bool                        isType(const TypeSpec& typeSpec) const;                         //!< Does the language type of the object fit type specification typeSpec?
        void                        print(std::ostream& o) const;                                   //!< Print complete object info

   protected:
		                            RbObject(void) {}                                               //!< No objects of this class
};


/* Global functions using the class */
std::ostream&   operator<<(std::ostream& o, const RbObject& x);         //!< Overloaded output operator

#endif
