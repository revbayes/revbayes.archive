/**
 * @file
 * This file contains the declaration of RbLanguageObject, which is
 * the RevBayes abstract base class for all language objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-07-27
 *
 * $Id$
 */

#ifndef RbLanguageObject_H
#define RbLanguageObject_H

#include "DagNode.h"
#include "RbPtr.h"

#include <ostream>
#include <string>
#include <vector>


namespace RevLanguage {

class Argument;
class ArgumentRule;
class ArgumentRules;
class MethodTable;
class TypeSpec;
class Variable;

typedef ArgumentRules MemberRules;                                                                                                      //!< Member rules type def, for convenience



class RbLanguageObject {
    
public:
    virtual                            ~RbLanguageObject(void) {}                                                                       //!< Virtual destructor
    
    // overloaded operators
    bool                                operator==(const RbLanguageObject& x) const { return &x == this; }
    bool                                operator!=(const RbLanguageObject& x) const { return !operator==(x); }
    bool                                operator<(const RbLanguageObject& x) const { return false; }
    
    // Basic utility functions you have to override (also getClassTypeSpec()!)
    virtual RbLanguageObject*           clone(void) const = 0;                                                                          //!< Clone object
    static const std::string&           getClassName(void);                                                                             //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                                         //!< Get class type spec
    virtual const TypeSpec&             getTypeSpec(void) const = 0;                                                                    //!< Get the type spec of the instance
    virtual void                        printValue(std::ostream& o) const = 0;                                                          //!< Print value for user
    
    // Basic utility functions you may want to override
    virtual void                        constructInternalObject(void);                                                                  //!< Objects using the ConstructorFunction should overwrite this function for proper initializiation of the internal objects.
    virtual RbLanguageObject*           convertTo(const TypeSpec& type) const;                                                          //!< Convert to type
    virtual RevBayesCore::DagNode*      getValueNode(void) const;
    virtual bool                        isConvertibleTo(const TypeSpec& type) const;                                                    //!< Is convertible to type?

    // functions wrapper objects of RB core objects you might want to overwrite
    virtual bool                        isConstant(void) const;                                                                         //!< Is this variable and the internally stored deterministic node constant?
    virtual void                        makeConstantValue();                                                                            //!< Convert the stored variable to a constant variable (if applicable)
    virtual void                        replaceVariable(RbLanguageObject *newVar);                                                      //!< Replace the internal DAG node
    virtual void                        setName(const std::string &n);                                                                  //!< Set the name of the variable (if applicable)
  
    // Member variable functions you may want to override
    virtual RbLanguageObject*           getMember(const std::string& name) const;                                                       //!< Get member variable 
    virtual bool                        hasMember(const std::string& name) const;                                                       //!< Has this object a member with name

    
    // Member variable functions you may want to override
    virtual const MemberRules&          getMemberRules(void) const;                                                                     //!< Get member rules
    virtual void                        setConstMember(const std::string& name, const RbPtr<const Variable> &var);                      //!< Set member variable
    virtual void                        setMember(const std::string& name, const RbPtr<Variable> &var);                                 //!< Set member variable
    
    // Member method functions
    virtual RbLanguageObject*           executeMethod(const std::string& name, const std::vector<Argument>& args);                      //!< Override to map member methods to internal functions
    virtual const MethodTable&          getMethods(void) const;                                                                         //!< Get member methods (const)

    
    // Basic utility functions you should not have to override
    const std::string&                  getType(void) const;                                                                            //!< Get the type specification as a string
    bool                                isTypeSpec(const TypeSpec& typeSpec) const;                                                     //!< Does the language type of the object fit type specification typeSpec?

protected:
    RbLanguageObject(void) {}                                                                                                           //!< No objects of this class
    RbLanguageObject(const MemberRules& memberRules);                                                                                   //!< Standard constructor

    virtual void                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
    virtual void                        setMemberVariable(const std::string& name, const RbPtr<Variable> &var);                         //!< Set member variable

    
};
    
}

// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RevLanguage::RbLanguageObject& x);                                             //!< Overloaded output operator



#endif
