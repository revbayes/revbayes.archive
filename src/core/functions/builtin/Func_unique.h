/**
 * @file
 * This file contains the declaration of Func_unique, which
 * is used to count the number of unique values in a vector.
 *
 * @brief Declaration of Func_unique
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_unique_H
#define Func_unique_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_unique_name = "Unclamp function";

class Func_unique :  public RbFunction {
	
public:
	// Basic utility functions
	Func_unique*                clone(void) const;                                  //!< Clone the object
	const VectorString&         getClass(void) const;                               //!< Get class vector
	
	// Regular functions
	bool                        addAsChildOfArguments(void) { return false; }       //!< We do not wish that this function is added as a child of the arguments
	RbLanguageObject*           execute(void);                                      //!< Execute operation
	const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
	const TypeSpec              getReturnType(void) const;                          //!< Get type of return value
	
};

#endif

