/**
 * @file
 * This file contains the declaration of Func_power, which 
 * calculates 'a' to the power of 'b'.
 *
 * @brief Declaration of Func_power
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_power_H
#define Func_power_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_power_name = "Power function";

class Func_power :  public RbFunction {
    
public:
    // Basic utility functions
    Func_power*                 clone(void) const;                                          //!< Clone the object
    const VectorString&         getClass(void) const;                                       //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    RbPtr<const ArgumentRules>  getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

protected:
    RbPtr<RbLanguageObject>     executeFunction(void);                                              //!< Execute function

private:
    static const TypeSpec       typeSpec;
    static const TypeSpec       returnTypeSpec;
};

#endif

