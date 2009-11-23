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
class RbFunction;

class RbComplex : public RbObject {

    public:

        // Basic utility functions
        virtual bool                equals(const RbComplex* o) const;       //!< Equals comparison
		virtual void                print(std::ostream& o) const;
		virtual void                printValue(std::ostream& o) const;

        // Regular functions
        virtual bool                addMember(const std::string& name, RbObject* v);
        virtual void                deleteMember(const std::string& name); 
        virtual const RbObject*     getMember(const std::string& name) const; //!< Return member variable
        virtual const RbFunction*   getMethod(const std::string& name) const; //!< Return method (member function)
        virtual bool                setMember(const std::string& name, RbObject* val);   //!< Set member variable

    protected:
                    RbComplex();        //!< Make it impossible to create objects of this class
            virtual ~RbComplex();       //!< Delete member variables 

        std::map<const std::string, RbObject*>       members;    //!< Member variables
        std::map<const std::string, RbFunction*>     methods;    //!< Member variables
};

#endif
