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

#include <set>
#include <string>

class ArgumentRule;

class MemberObject: public RbLanguageObject {

    public:

        // Basic utility functions you have to override
        virtual MemberObject*                               clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                           getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&                              getClassTypeSpec(void);                                                             //!< Get class type spec
        
        // Basic utility functions you may want to override
        virtual void                                        printValue(std::ostream& o) const;                                                  //!< Print value for user

        // Member variable functions you have to override
        virtual RbPtr<RbLanguageObject>                     getMember(const std::string& name) const;                                           //!< Get member variable 
        virtual const MemberRules&                          getMemberRules(void) const;                                                         //!< Get member rules (const)
        virtual bool                                        hasMember(const std::string& name) const;                                           //!< Has this object a member with name

        // Member variable functions you do not have to override
        void                                                setMember(const std::string& name, const RbPtr<const Variable> &var);                            //!< Set member variable

        // Member method functions
        virtual RbPtr<RbLanguageObject>                     executeOperation(const std::string& name, const std::vector<Argument>& args);                       //!< Override to map member methods to internal functions
        virtual const MethodTable&                          getMethods(void) const;                                                             //!< Get member methods (const)

    protected:
                                                            MemberObject(const MemberRules& memberRules);                                       //!< Standard constructor
                                                            MemberObject(void);                                                                 //!< Default constructor; no members or methods
 
        virtual void                                        setMemberVariable(const std::string& name, const RbPtr<RbLanguageObject> &var);     //!< Set member variable
        virtual RbPtr<RbLanguageObject>                     executeOperationSimple(const std::string& name, const std::vector<Argument>& args); //!< Override to map member methods to internal functions

};

#endif

