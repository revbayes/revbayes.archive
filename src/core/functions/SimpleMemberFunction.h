/**
 * @file
 * This file contains the declaration of SimpleMemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular RbFunction objects
 * implementing the member functions.
 *
 * @brief Declaration of SimpleMemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef SimpleMemberFunction_H
#define SimpleMemberFunction_H

#include "RbFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class MemberObject;

class SimpleMemberFunction :  public RbFunction {

public:
    SimpleMemberFunction(const TypeSpec retType, ArgumentRules* argRules);                                                //!< Constructor

    // Basic utility functions
    SimpleMemberFunction*               clone(void) const;                                                          //!< Clone the object
    static const std::string&           getClassName(void);                                                         //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object

    // Regular functions   
    const ArgumentRules&                getArgumentRules(void) const;                                               //!< Get argument rules
    const TypeSpec&                     getReturnType(void) const;                                                  //!< Get type of return value
    void                                setArgumentVariable(const std::string& name, const RbPtr<const RbLanguageObject> &var);     
    void                                setMemberObject(const RbPtr<MemberObject>& obj);                            //!< Set the member object to which this function belongs
    void                                setMethodName(const std::string& name) { funcName = name; }                 //!< Set name of member method

protected:
    virtual RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);                  //!< Execute the function. This is the function one has to overwrite for single return values.


private:
    const ArgumentRules*                argumentRules;                                                              //!< Argument rules (different for different member functions)
    std::string                         funcName;                                                                   //!< Name of member method
    MemberObject*                       object;                                                                     //!< The member object to which this function belongs (we do not own the member object because of cyclic ownership)
    const TypeSpec                      returnType;                                                                 //!< Return type (different for different member functions)
    
};

#endif

