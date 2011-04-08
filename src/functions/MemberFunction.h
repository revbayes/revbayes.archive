/**
 * @file
 * This file contains the declaration of MemberFunction, which is used
 * for member functions (methods) of complex objects with methods.
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
class VectorInteger;
class VectorString;

class MemberFunction :  public RbFunction {

    public:
                                    MemberFunction(const TypeSpec retType, const ArgumentRules& argRules);      //!< Constructor

        // Basic utility functions
        std::string                 briefInfo(void) const;                  //!< Brief info about object
        MemberFunction*             clone(void) const;                      //!< Clone the object
    	const VectorString&         getClass(void) const;                   //!< Get class vector
        std::string                 richInfo(void) const;                   //!< Complete info about object

        // Regular functions
        DAGNode*                    execute(void);                          //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;           //!< Get argument rules
        const TypeSpec              getReturnType(void) const;              //!< Get type of return value

	protected:
        const ArgumentRules&        argumentRules;                          //!< Argument rules (different for different member functions)
        const TypeSpec              returnType;                             //!< Return type (different for different member functions)
};

#endif

