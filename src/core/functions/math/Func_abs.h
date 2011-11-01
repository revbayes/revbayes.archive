/**
 * @file
 * This file contains the declaration of Func_abs, which 
 * calculates the absolute value of x.
 *
 * @brief Declaration of Func_abs
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_abs_H
#define Func_abs_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_abs_name = "Absolut (|x|) function";

class Func_abs :  public RbFunction {
    
public:
    Func_abs(void);                                                                         //!< default constructor
    Func_abs(const Func_abs &x);                                                            //!< copy constructor
    
    // Basic utility functions
    Func_abs*                   clone(void) const;                                          //!< Clone the object
    const VectorString&         getClass(void) const;                                       //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    RbPtr<RbLanguageObject>     execute(void);                                              //!< Execute function
    RbPtr<const ArgumentRules>  getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
    
private:
    static const TypeSpec       typeSpec;  
    static const TypeSpec       returnTypeSpec;
};

#endif

