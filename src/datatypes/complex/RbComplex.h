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

#include "RbFunction.h"
#include "RbObject.h"
//#include "StringVector.h"


class RbComplex : public RbObject {

    public:
        static const StringVector   rbClass;            //!< Static class attribute

            virtual ~RbComplex();       //!< Delete member variables 

        // Basic utility functions
        //virtual bool                equals(const RbComplex* o) const;       //!< Equals comparison
		virtual void                print(std::ostream& o) const;
		virtual void                printValue(std::ostream& o) const;

        // Regular functions
        virtual bool                addMember(const std::string& name, RbObject* v);
        virtual void                deleteMember(const std::string& name); 
        virtual const RbObject*     getMember(const std::string& name) const; //!< Return member variable
        virtual const RbFunction*   getMethod(const std::string& name) const; //!< Return method (member function)
        virtual void                setMember(const std::string& name, RbObject* val);   //!< Set member variable
        virtual const std::map<std::string, RbFunction*>&  getMethods(void) const { return methods; }

    protected:
                    RbComplex();        //!< Make it impossible to create objects of this class

        std::map<std::string, RbObject*>       members;    //!< Member variables
        std::map<std::string, RbFunction*>     methods;    //!< Member variables
};

#endif
