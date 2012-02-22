/**
 * @file
 * This file contains the declaration of Func_exp, which 
 * calculates 'e' to the power of 'b'.
 *
 * @brief Declaration of Func_exp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_exp_H
#define Func_exp_H

#include "RbFunction.h"
#include "RealPos.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_exp_name = "Exponential function";

class Func_exp :  public RbFunction {
    
public:
    Func_exp(void);                                                                         //!< default constructor
    
    // Basic utility functions
    Func_exp*                   clone(void) const;                                          //!< Clone the object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

protected:
    const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
    void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);

private:
    RealPos                     value;
    
    // Arguments
    RbVariablePtr               x;
    
};

#endif

