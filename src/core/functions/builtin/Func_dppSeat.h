/**
 * @file
 * This file contains the declaration of Func_dppSeat, which 
 * seats data elemts at "tables".
 * Returns an allocation vector
 *
 * @brief Declaration of Func_dppSeat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_dppSeat_H
#define Func_dppSeat_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_dppSeat_name = "DPP function that returns allocation vector";

class Func_dppSeat :  public RbFunction {
	
public:
	// Basic utility functions
	Func_dppSeat*	                clone(void) const;                                          //!< Clone the object
	const VectorString&             getClass(void) const;                                       //!< Get class vector
    const TypeSpec&                 getTypeSpec(void) const;                                    //!< Get language type of the object
	
	// Regular functions
	RbPtr<RbObject>                 execute(void);                                              //!< Execute function
    RbPtr<const ArgumentRules>      getArgumentRules(void) const;                               //!< Get argument rules
	const TypeSpec&                 getReturnType(void) const;                                  //!< Get type of return value
    
private:
    static const TypeSpec           typeSpec;	
    static const TypeSpec           returnTypeSpec;
};

#endif

