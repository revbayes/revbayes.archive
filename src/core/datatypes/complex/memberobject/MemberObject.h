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

#include "Environment.h"
#include "MethodTable.h"
#include "RbLanguageObject.h"

#include <set>
#include <string>

class ArgumentFrame;
class ArgumentRule;
class DAGNode;
class MethodTable;
class VectorString;

class MemberObject: public RbLanguageObject {

    public:
        virtual                                            ~MemberObject(void);                                                                 //!< Destructor

        // Basic utility functions you have to override
        virtual MemberObject*                               clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                           getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&                              getClassTypeSpec(void);                                                             //!< Get class type spec
        
        // Basic utility functions you may want to override
        virtual void                                        printValue(std::ostream& o) const;                                                  //!< Print value for user

        // Basic utility functions you do not have to override
        bool                                                isConstant(void) const;                                                             //!< Is the object, including all member variables and elements, constant?

        // Member variable functions you have to override
        virtual const Variable*                             getMember(const std::string& name) const;                                           //!< Get member variable 
        virtual Variable*                                   getMember(const std::string& name);                                                 //!< Get member variable (non-const node)
        virtual const MemberRules&                          getMemberRules(void) const;                                                         //!< Get member rules (const)
        virtual bool                                        hasMember(const std::string& name) const;                                           //!< Has this object a member with name

        // Member variable functions you do not have to override
        virtual const std::map<std::string, RbVariablePtr>& getMembers(void) const;                                                             //!< Get members
        virtual std::map<std::string, RbVariablePtr>&       getMembers(void);                                                                   //!< Get members
        void                                                setMember(const std::string& name, Variable* var);                                   //!< Set member variable
//        const TypeSpec&                     getMemberTypeSpec(const std::string& name) const;                                   //!< Get type spec for a member variable

        // Member method functions
        const RbLanguageObject&                             executeMethod(const std::string& name, const std::vector<Argument>& args);          //!< Direct call of member method
        virtual const RbLanguageObject&                     executeOperation(const std::string& name, const std::vector<Argument>& args);                       //!< Override to map member methods to internal functions
        virtual const MethodTable&                          getMethods(void) const;                                                             //!< Get member methods (const)

        virtual bool                                        supportsIndex(void) const { return false; }                                         //!< Does object support index operator?

    protected:
                                                            MemberObject(const MemberRules& memberRules);                                       //!< Standard constructor
                                                            MemberObject(void);                                                                 //!< Default constructor; no members or methods
                                                            MemberObject(const MemberObject &m);                                                //!< Copy constructor
    
        MemberObject&                                       operator=(const MemberObject& m);
 
        virtual void                                        setMemberVariable(const std::string& name, Variable* var);                          //!< Set member variable
        virtual const RbLanguageObject&                     executeOperationSimple(const std::string& name, const std::vector<Argument>& args); //!< Override to map member methods to internal functions

        // Members is the variable frame that stores member variables
        std::map<std::string, RbVariablePtr>                members;                                                                            //!< Member variables
};

#endif

