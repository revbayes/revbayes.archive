/**
 * @file
 * This file contains the declaration of MemberObject, an abstract base
 * class for complex objects with member variables and methods.
 *
 * @todo For now, we do not allow function overloading. Overriding is manual.
 *
 * @brief Declaration of MemberObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef MemberObject_H
#define MemberObject_H

#include "Argument.h"
#include "RbLanguageObject.h"

class MethodTable;

#include <set>
#include <string>

class ArgumentRule;

class MemberObject: public RbLanguageObject {

    public:

        // Basic utility functions you have to override
        virtual MemberObject*                               clone(void) const = 0;                                                                          //!< Clone object
        static const std::string&                           getClassName(void);                                                                             //!< Get class name
        static const TypeSpec&                              getClassTypeSpec(void);                                                                         //!< Get class type spec
        
        // Basic utility functions you may want to override
        virtual void                                        printValue(std::ostream& o) const;                                                              //!< Print value for user

        // Member variable functions you have to override
        virtual RbPtr<RbLanguageObject>                     getMember(const std::string& name) const;                                                       //!< Get member variable 
        virtual const MemberRules&                          getMemberRules(void) const;                                                                     //!< Get member rules (const)
        virtual bool                                        hasMember(const std::string& name) const;                                                       //!< Has this object a member with name

        // Member variable functions you may want to override
        virtual void                                        setConstMember(const std::string& name, const RbPtr<const Variable> &var);                      //!< Set member variable
        virtual void                                        setMember(const std::string& name, const RbPtr<Variable> &var);                                 //!< Set member variable

        // Member method functions
        virtual RbPtr<RbLanguageObject>                     executeMethod(const std::string& name, const std::vector<RlValue<const RbObject> >& args);      //!< Override to map member methods to internal functions
        virtual RbPtr<RbLanguageObject>                     executeSimpleMethod(const std::string& name, const std::vector<const RbObject *>& args);        //!< Override to map member methods to internal functions
        virtual const MethodTable&                          getMethods(void) const;                                                                         //!< Get member methods (const)

    protected:
                                                            MemberObject(const MemberRules& memberRules);                                                   //!< Standard constructor
                                                            MemberObject(void);                                                                             //!< Default constructor; no members or methods
 
        virtual void                                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        virtual void                                        setMemberVariable(const std::string& name, const RbPtr<Variable> &var);                         //!< Set member variable
        virtual void                                        setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var);        //!< Set member variable

};

#endif

