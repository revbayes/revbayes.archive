/**
 * @file
 * This file contains the declaration of Func_sin, which 
 * calculates the sin of x.
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

#ifndef Func_sin_H
#define Func_sin_H

#include "RbFunction.h"
#include "Real.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;


class Func_sin :  public RbFunction {
    
public:
    // Basic utility functions
    Func_sin*                   clone(void) const;                                          //!< Clone the object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

protected:
    const RbLanguageObject&     executeFunction(void);                                              //!< Execute function
    void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);

private:
    Real                        value;
    
    // Arguments
    RbVariablePtr               x;
    
};

#endif

