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
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef MemberObject_H
#define MemberObject_H

#include "MemberEnvironment.h"
#include "MethodTable.h"
#include "RbLanguageObject.h"

#include <set>
#include <string>

class ArgumentFrame;
class ArgumentRule;
class DAGNode;
class MethodTable;
class VectorString;

typedef std::vector<RbPtr<ArgumentRule> >   MemberRules;                                                                       //!< Member rules type def, for convenience


const std::string MemberObject_name = "Member Object";

class MemberObject: public RbLanguageObject {

    public:
        virtual                            ~MemberObject(void) {}                                                               //!< Destructor

        // Basic utility functions you have to override
        virtual MemberObject*               clone(void) const = 0;                                                              //!< Clone object
        virtual const VectorString&         getClass(void) const;                                                               //!< Get class vector
        
        // Basic utility functions you may want to override
        virtual void                        printValue(std::ostream& o) const;                                                  //!< Print value for user
        virtual std::string                 richInfo(void) const;                                                               //!< Complete info

        // Basic utility functions you do not have to override
        bool                                isConstant(void) const;                                                             //!< Is the object, including all member variables and elements, constant?

        // Member variable functions you do not have to override
        const RbPtr<MemberEnvironment>      getMembers(void) const;                                                             //!< Get members
        RbPtr<MemberEnvironment>            getMembersPtr(void);                                                                //!< Get members
        const TypeSpec                      getMemberTypeSpec(const std::string& name) const;                                   //!< Get type spec for a member variable
        const RbPtr<RbLanguageObject>       getMemberValue(const std::string& name) const;                                      //!< Get member value (const)
        const RbPtr<DAGNode>                getMemberDagNode(const std::string& name) const;                                    //!< Get member variable 
        RbPtr<DAGNode>                      getMemberDagNodePtr(const std::string& name);                                       //!< Get member variable (non-const ptr)
        bool                                hasMember(const std::string& name) const;                                           //!< Has this object a member with name

        // Member variable functions you may wish to override
        virtual const RbPtr<MemberRules>    getMemberRules(void) const;                                                         //!< Get member rules
        virtual void                        setMemberDagNode(const std::string& name, RbPtr<DAGNode> var);                      //!< Set member variable
        virtual void                        setMemberVariable(const std::string& name, RbPtr<Variable> var);                    //!< Set member variable

        // Member method functions
        RbPtr<RbLanguageObject>             executeMethod(const std::string& name, const std::vector<RbPtr<Argument> >& args);  //!< Direct call of member method
        virtual RbPtr<RbLanguageObject>     executeOperation(const std::string& name, Environment& args);                       //!< Override to map member methods to internal functions
        virtual const RbPtr<MethodTable>    getMethods(void) const;                                                             //!< Get member methods

        virtual bool                        supportsIndex(void) const { return false; }                                         //!< Does object support index operator?

    protected:
                                            MemberObject(const RbPtr<MemberRules> memberRules);                                       //!< Standard constructor
                                            MemberObject(void){}                                                                //!< Default constructor; no members or methods
                                            MemberObject(const MemberObject &m);                                                //!< Copy constructor
 
        // Members is the variable frame that stores member variables
        RbPtr<MemberEnvironment>            members;                                                                                //!< Member variables
};

#endif

