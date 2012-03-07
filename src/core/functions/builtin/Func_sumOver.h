/**
 * @file
 * This file contains the declaration of Func_sumOver, which
 * is used to flag a stochastic node for summing over.
 *
 * @brief Declaration of Func_sumOver
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_sumOver.h 1247 2012-02-25 09:17:07Z hoehna $
 */

#ifndef Func_sumOver_H
#define Func_sumOver_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_sumOver_name = "sumOver function";

class Func_sumOver :  public RbFunction {
    
public:
    // Basic utility functions
    Func_sumOver*               clone(void) const;                                  //!< Clone the object
    static const std::string&   getClassName(void);                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                          //!< Get type of return value
    
protected:
    const RbLanguageObject&     executeFunction(void);                              //!< Execute operation
    void                        setArgumentVariable(const std::string& name, const Variable* var);
    
private:
    
    // arguments
    RbConstVariablePtr          variable;
    RbConstVariablePtr          value;
    
};

#endif

