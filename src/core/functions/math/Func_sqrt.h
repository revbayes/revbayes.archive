/**
 * @file
 * This file contains the declaration of Func_sqrt, which 
 * calculates the square root.
 *
 * @brief Declaration of Func_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_sqrt_H
#define Func_sqrt_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_sqrt_name = "Square root function";

class Func_sqrt :  public RbFunction {

    public:
        // Basic utility functions
        Func_sqrt*                  clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        RbLanguageObject*           executeFunction(void);                                      //!< Execute function

    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;
};

#endif

