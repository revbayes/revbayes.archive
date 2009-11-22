/**
 * @file
 * This file contains the declaration of RbComplex, which is
 * the RevBayes abstract base class for complex objects.
 *
 * @brief Declaration of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: RbComplex.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbComplex_H
#define RbComplex_H

#include <string>
#include <ostream>
#include <map>

#include "RbObject.h"

/* Forward declarations */
// class RbDumpState;
class ArgumentRule;
class Environment;
class FunctionDef;
class ObjectSlot;
class StringVector;

class RbComplex : public RbObject {

    public:

        static const StringVector   rbClass;                //!< Static class attribute
        static const ArgumentRule   memberTemplate[];       //!< Static member template
        static const FunctionDef    methodDefs[];           //!< Static method definitions

        // Basic utility functions
        virtual RbComplex*          clone() const { return new RbComplex(*this); }  //!< Clone object
        virtual const StringVector& getClass() const;                       //!< Get class
        virtual bool                equals(const RbComplex* o) const;       //!< Equals comparison
        virtual void                print(std::ostream& o) const;           //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;      //!< Print value (for user)

        // Regular functions
        virtual const RbObject*     getMember(const std::string& name) const; //!< Return member variable
        virtual const RbFunction*   getMethod(const std::string& name) const; //!< Return method (member function)
        const ObjectSlot*           getSlot(const std::string& name);       //!< Return member variable slot
        virtual bool                setMember(const std::string& name, RbObject* val);   //!< Set member variable

    protected:
                    RbComplex();        //!< Make it impossible to create objects of this class
            virtual ~RbComplex();       //!< Delete member variables 

        std::map<const std::string, ObjectSlot>     members;    //!< Member variables
};

#endif
