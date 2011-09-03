/**
 * @file
 * This file contains the declaration of Func_structure, which is used
 * to print info about the structure of a variable.
 *
 * @brief Declaration of Func_structure
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_structure_H
#define Func_structure_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_structure_name = "Struct function";

class Func_structure :  public RbFunction {

    public:
        // Basic utility functions
        Func_structure*             clone(void) const;                                  //!< Clone the object
        const VectorString&         getClass(void) const;                               //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object

        // Regular functions
        bool                        addAsChildOfArguments(void) { return false; }       //!< We do not wish that this function is added as a child of the arguments
        RbLanguageObject*           execute(void);                                      //!< Execute operation
        const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                          //!< Get type of return value
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

