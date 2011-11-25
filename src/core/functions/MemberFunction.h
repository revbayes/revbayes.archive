/**
 * @file
 * This file contains the declaration of MemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular RbFunction objects
 * implementing the member functions. Note that the first argument passed
 * in a member function call is a pointer to the MemberNode of the calling
 * object (like a this pointer).
 *
 * @brief Declaration of MemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef MemberFunction_H
#define MemberFunction_H

#include "RbFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class MemberObject;
class VectorInteger;
class VectorString;

const std::string MemberFunction_name = "Member function";

class MemberFunction :  public RbFunction {

public:
    MemberFunction(const TypeSpec retType, const RbPtr<ArgumentRules> argRules);                            //!< Constructor
    virtual ~MemberFunction(void);                                                                          //!< Destructor

    // Basic utility functions
    std::string                 briefInfo(void) const;                                                      //!< Brief info about object
    MemberFunction*             clone(void) const;                                                          //!< Clone the object
    const VectorString&         getClass(void) const;                                                       //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                                                    //!< Get language type of the object
    std::string                 richInfo(void) const;                                                       //!< Complete info about object

    // Regular functions   
    RbPtr<DAGNode>              execute(void);                                                              //!< Execute function
    RbPtr<const ArgumentRules>  getArgumentRules(void) const;                                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                                  //!< Get type of return value
    void                        setMemberObject(RbPtr<MemberObject> obj);                                   //!< Set the member object to which this function belongs
    void                        setMethodName(const std::string& name) { funcName = name; }                 //!< Set name of member method


private:
    const RbPtr<ArgumentRules>  argumentRules;                                                              //!< Argument rules (different for different member functions)
    std::string                 funcName;                                                                   //!< Name of member method
    MemberObject*               object;                                                                     //!< The member object to which this function belongs (we do not own the member object because of cyclic ownership)
    const TypeSpec              returnType;                                                                 //!< Return type (different for different member functions)
    
    static const TypeSpec       typeSpec;  
};

#endif

