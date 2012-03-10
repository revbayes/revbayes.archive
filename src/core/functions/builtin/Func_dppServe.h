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

#include "RbVector.h"
#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;


class Func_dppServe :  public RbFunction {
	
public:
	// Basic utility functions
	Func_dppServe*	                clone(void) const;                                          //!< Clone the object
    static const std::string&       getClassName(void);                                         //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&                 getTypeSpec(void) const;                                    //!< Get language type of the object
	
	// Regular functions
    const ArgumentRules&            getArgumentRules(void) const;                               //!< Get argument rules
	const TypeSpec&                 getReturnType(void) const;                                  //!< Get type of return value

protected:
	const RbLanguageObject&         executeFunction(void);                                              //!< Execute function
    
private:

    // function return value
    RbVector<Real>                  values;
};

#endif

