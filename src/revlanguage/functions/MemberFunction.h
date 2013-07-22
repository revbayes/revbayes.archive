/**
 * @file
 * This file contains the declaration of MemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular Function objects
 * implementing the member functions.
 *
 * @brief Declaration of MemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-15 18:59:11 +0200 (Tue, 15 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: MemberFunction.h 1544 2012-05-15 16:59:11Z hoehna $
 */

#ifndef MemberFunction_H
#define MemberFunction_H

#include "RlFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevLanguage {

class ArgumentRule;
class RbLanguageObject;

class MemberFunction :  public Function {
    
public:
    MemberFunction(const TypeSpec retType, ArgumentRules* argRules);                                                //!< Constructor
    
    // Basic utility functions
    MemberFunction*                     clone(void) const;                                                          //!< Clone the object
    static const std::string&           getClassName(void);                                                         //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
    
    // Regular functions   
    virtual RbLanguageObject*           execute(void);                                                              //!< Execute function
    const ArgumentRules&                getArgumentRules(void) const;                                               //!< Get argument rules
    const TypeSpec&                     getReturnType(void) const;                                                  //!< Get type of return value
    void                                setMemberObject(RbLanguageObject* obj);                                     //!< Set the member object to which this function belongs
    void                                setMethodName(const std::string& name);                                     //!< Set name of member method
        
    
protected:
    const ArgumentRules*                argumentRules;                                                              //!< Argument rules (different for different member functions)
    std::string                         funcName;                                                                   //!< Name of member method
    RbLanguageObject*                   object;                                                                     //!< The member object to which this function belongs (we do not own the member object because of cyclic ownership)
    const TypeSpec                      returnType;                                                                 //!< Return type (different for different member functions)
    
};
    
}

#endif

