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

class StringVector;

class RbComplex : public RbObject {

    public:
		virtual                     ~RbComplex();                                           //!< Delete member variables 

        // Basic utility functions
        //virtual bool                equals(const RbComplex* o) const;             //!< Equals comparison
		virtual std::string         toString() const;                               //!< Complete info about object
		virtual void                printValue(std::ostream& o) const;
		virtual const StringVector& getClass(void) const;                           //!< Get class

        // Member variable functions
        virtual bool                addMember(const std::string& name, const std::string& type, RbObject* v);
        virtual void                deleteMember(const std::string& name);          //!< Delete member variable
        virtual const RbObject*     getMember(const std::string& name) const;       //!< Get member variable
        virtual std::string         getMemberType(const std::string& name) const;   //!< Get required type of var
        virtual void                setMember(const std::string& name, RbObject* val);  //!< Set member variable

        // Member method functions
        virtual RbObject*             executeMethod(const std::string& funcId);
        virtual RbObject*             executeMethod(const std::string& funcId, std::vector<Argument>& args);
        bool                          existsMethod(const std::string& funcId, std::vector<Argument>& args) const;
        //std::vector<FunctionDescr>  getMethodDescr(const std::string& funcId) const;      //!< Get method descr
        //FunctionTable*              getMethodTable(void) const { return methodTable; }  //!< Get method table
        virtual void                  setArguments(const std::string& funcId, std::vector<Argument>& args);

    protected:
            RbComplex();        //!< Make it impossible to create objects of this class

        std::map<std::string, RbObject*>       members;         //!< Member variables
        std::map<std::string, std::string>     type;            //!< Member types
        //FunctionTable*                       methodTable;     //!< Table of member functions
};

#endif
