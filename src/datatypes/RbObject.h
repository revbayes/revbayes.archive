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
        virtual std::string         toString(void) const = 0;                                       //!< Complete info about object

        // Basic utility functions you may want to override
        virtual std::string         briefInfo(void) const;                                          //!< Brief info about object
        virtual RbObject*           convertTo(const std::string& type, int dim=0) const;                            //! Convert to type and dim
        virtual bool                isConvertibleTo(const std::string& type, int dim=0, bool once=false) const;     //! Is convertible to type and dim?
        virtual bool                isMutableTo(VectorInteger& index, const RbObject* newValue, std::string mutantType) const;  //! Is mutable to new value
        virtual RbObject*           mutateTo(const VectorInteger& index, RbObject* newValue) const;                 //! Mutate to new value
        
        // Subscript dimensions: override if object supports subscripting
        virtual int                 getDim(void) const { return 0; }                                //!< Get subscript dimensions

        // Basic utility functions you do not have to override
        const std::string&          getType(void) const;                                            //!< Get type
        bool                        isType(const std::string& type) const;                          //!< Is the object of class type?
        void                        print(std::ostream& o) const;                                   //!< Print complete object info

   protected:
		                            RbObject(void) {}                                               //!< No objects of this class
};

std::ostream& operator<<(std::ostream& o, const RbObject& x);                                       //!< Overloaded output operator

#endif
