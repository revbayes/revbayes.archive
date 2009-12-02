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

//class Environment;
//class RbDumpState;
class StringVector;

class RbObject {

    public:
        static const StringVector   rbClass;            //!< Static class attribute

        virtual ~RbObject(void) { }                                                           //! Virtual destructor because of virtual functions

        // Basic utility functions
        virtual std::string         briefInfo(void) const { return "unknown object"; }        //!< Brief info about object
        virtual RbObject*           clone(void) const = 0;                                    //!< Clone object
        virtual bool                equals(const RbObject* o) const = 0;                      //!< Equals comparison
        virtual const StringVector& getClass(void) const=0;                                   //!< Get class
        bool                        isType(const std::string t) const;                        //!< Is the object of type t?
        void                        print(std::ostream& o) const;                         //!< Print complete object info
        virtual void                printValue(std::ostream& o) const = 0;                    //!< Print value (for user)
        virtual std::string         toString(void) const = 0;                                 //!< General info on object

        // Type conversion
        virtual RbObject*           convertTo(const std::string& type) const { return NULL; }        //! Convert to type
        virtual bool                isConvertibleTo(const std::string& type) const { return false; } //! Is convertible to type?
        
        // overloaded operators
        virtual RbObject&			operator=(const RbObject& o) = 0;

        // Dump and resurrect
        // TODO I am commenting these out for now, they need to be implemented later -- Fredrik
        // virtual void                dump(std::ostream& o) = 0;                             //!< Dump to ostream c
        // virtual void                resurrect(const RbDumpState& x) = 0;                   //!< Resurrect from dumped state

    protected:
        RbObject(void) { }                                                                    //!< Make it impossible to create objects
};

#endif
