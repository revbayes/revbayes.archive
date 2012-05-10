/**
 * @file
 * This file contains the declaration of Func_type, which is used
 * to print info about the type of a variable.
 *
 * @brief Declaration of Func_type
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_type_H
#define Func_type_H

#include "RbFunction.h"
#include "RbString.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;

class Func_type :  public RbFunction {
    
public:
    // Basic utility functions
    Func_type*                  clone(void) const;                                          //!< Clone the object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
    
protected:
    RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);  //!< Execute operation
        
};

#endif

