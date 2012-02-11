/**
 * @file
 * This file contains the declaration of Func_dppServe, which 
 * creates a vector of parameter classes and draws the value for each
 * from the base distribution G_0
 *
 * @brief Implementation of Func_dppServe
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */


#ifndef Func_dppServe_H
#define Func_dppServe_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_dppServe_name = "DPP function that returns a vector of parameters for each unique category";

class Func_dppServe :  public RbFunction {
	
public:
	// Basic utility functions
	Func_dppServe*	                clone(void) const;                                          //!< Clone the object
	const VectorString&             getClass(void) const;                                       //!< Get class vector
    const TypeSpec&                 getTypeSpec(void) const;                                    //!< Get language type of the object
	
	// Regular functions
    const ArgumentRules&            getArgumentRules(void) const;                               //!< Get argument rules
	const TypeSpec&                 getReturnType(void) const;                                  //!< Get type of return value

protected:
	RbLanguageObject*               executeFunction(void);                                              //!< Execute function
    
private:
    static const TypeSpec           typeSpec;	
    static const TypeSpec           returnTypeSpec;
};

#endif

