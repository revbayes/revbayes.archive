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

#include "Argument.h"
#include "RbException.h"
#include "RbObject.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class RbObjectWrapper;
class StringVector;

class RbComplex : public RbObject {

    public:
		virtual                             ~RbComplex() {}                                 //!< Virtual destructor 

        // Basic utility functions you HAVE TO override
        virtual RbObject*                   clone(void) const = 0;                          //!< Clone object
        virtual bool                        equals(const RbComplex* obj) const { throw RbException("Not implemented");} //!< TODO: Change! Equals comparison
		virtual const StringVector&         getClass(void) const;                           //!< Get class vector
		virtual void                        printValue(std::ostream& o) const = 0;          //!< Print value for user
		virtual std::string                 toString() const = 0;                           //!< Complete info about object

        // Member variable functions: override if object contains member variables
        virtual bool                        addMember(const std::string& name, const std::string& type);    //!< Add member variable
        virtual bool                        addMember(const std::string& name, RbObjectWrapper* val);       //!< Add member variable
        virtual void                        deleteMember(const std::string& name);          //!< Delete member variable
        virtual const RbObjectWrapper*      getMember(const std::string& name) const;       //!< Get member variable
        //virtual std::vector<MemberDescr>    getMemberDescr(const std::string& name="") const;  //!< Get member descr 
        virtual void                        setMember(const std::string& name, RbObjectWrapper* val);  //!< Set member variable

        // Member method functions: override if object contains member functions
        virtual RbObject*                   executeMethod(int funcId);          //!< Execute function with preprocessed args
        virtual RbObject*                   executeMethod(const std::string& name, std::vector<Argument>& args);    //!< Execute function
        bool                                existsMethod(const std::string& name, std::vector<Argument>& args) const;   //!< Does method exist?
        //virtual std::vector<FunctionDescr>  getMethodDescr(const std::string& name="") const;      //!< Get method descriptions
        virtual int                         setArguments(const std::string& name, std::vector<Argument>& args); //!< Set arguments and get funcId back 

         // Element access functions: override if object contains elements
        virtual RbObject*                   getElement(const IntVector& index) const;           //!< Get element (a copy)
        virtual int                         getElementDim(void) const { return 0; }             //!< Get dimensions
        virtual const IntVector&            getElementLength(void) const;                       //!< Get length in each dim
        virtual const std::string&          getElementType(void) const;                         //!< Get element type
        virtual void                        setElement(const IntVector& index, RbObject* val);  //!< Set element
        virtual void                        setElementLength(const IntVector& index);           //!< Get length in each dim
        virtual void                        resize(const IntVector& index);                     //!< Get length in each dim

    protected:
                                            RbComplex() : RbObject() {}        //!< No objects of this class
};

#endif
