/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the RevBayes abstract base class for all objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbObject_H
#define RbObject_H

#include <string>
#include <ostream>

/* Forward declarations */
// class RbDumpState;
class StringVector;
class Environment;

class RbObject {

    public:
            virtual ~RbObject() {}          //! Virtual destructor because of virtual functions

        static const StringVector   rbClass;                //!< Static class attribute

        // Basic utility functions
        virtual const StringVector& getClass() const;                   //!< Get class
        virtual RbObject*           clone() const = 0;                  //!< Clone object
        virtual RbObject*           clone(Environment& env) const { return clone(); }   //!< Clone to environment
        virtual bool                equals(const RbObject* o) const = 0;//!< Equals comparison
        virtual void                print(std::ostream& o) const;       //!< Print complete object info
        virtual void                printValue(std::ostream& o) const {}//!< Print value (for user)

        //TODO I do not use this at the moment but keep it if others want to use it -- Fredrik
        virtual std::string         toString(void) const { return std::string(); }  //!< General info on object

        // Type conversion
        virtual bool                isConvertibleTo(const std::string& type) const; //! Is convertible to type?
        virtual RbObject*           convertTo(const std::string& type) const;       //! Convert to type

        // Pointer-based comparison -- throw not supported error by default
        virtual bool                lessThan(const RbObject* o) const;  //!< Less than

        // Pointer-based arithmetic -- throw not supported error by default
        virtual RbObject*           add(const RbObject* o) const;       //!< Addition
        virtual RbObject*           subtract(const RbObject* o) const;  //!< Subtraction
        virtual RbObject*           multiply(const RbObject* o) const;  //!< Multiplication
        virtual RbObject*           divide(const RbObject* o) const;    //!< Division
        virtual RbObject*           raiseTo(const RbObject* o) const;   //!< Power

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
     // virtual void                dump(std::ostream& o) = 0;              //!< Dump to ostream c
     // virtual void                resurrect(const RbDumpState& x) = 0;    //!< Resurrect from dumped state

    protected:
            RbObject() {}                   //!< Make it impossible to create objects
};

#endif
