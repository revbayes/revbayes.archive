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

#include "MemberFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class MemberObject;

class SimpleMemberFunction :  public MemberFunction {

public:
    SimpleMemberFunction(const TypeSpec retType, ArgumentRules* argRules);                                          //!< Constructor

    // Basic utility functions
    SimpleMemberFunction*               clone(void) const;                                                          //!< Clone the object
    static const std::string&           getClassName(void);                                                         //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
//    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object

    // Regular functions   
    virtual RlValue<RbLanguageObject>   execute(void);                                                              //!< Execute function

protected:
    virtual RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);                  //!< Execute the function. This is the function one has to overwrite for single return values.
    
};

#endif

