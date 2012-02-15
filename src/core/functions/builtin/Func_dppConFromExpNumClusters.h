/**
 * @file
 * This file contains the declaration of Func_dppConFromExpNumClusters, which 
 * calculates expected value of the DPP-concentration parameter from
 * an expected number of clusters (tables).
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

#ifndef Func_dppConFromExpNumClusters_H
#define Func_dppConFromExpNumClusters_H

#include "RbFunction.h"
#include "RealPos.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_dppConFromExpNumClusters_name = "DPP Concentration Parameter Calculator function";

class Func_dppConFromExpNumClusters :  public RbFunction {
	
public:
	// Basic utility functions
	Func_dppConFromExpNumClusters*  clone(void) const;                                          //!< Clone the object
	const VectorString&             getClass(void) const;                                       //!< Get class vector
    const TypeSpec&                 getTypeSpec(void) const;                                    //!< Get language type of the object
	
	// Regular functions
    const ArgumentRules&            getArgumentRules(void) const;                               //!< Get argument rules
	const TypeSpec&                 getReturnType(void) const;                                  //!< Get type of return value

protected:
    const RbLanguageObject&         executeFunction(void);                                      //!< Execute function

private:
    static const TypeSpec           typeSpec;	
    static const TypeSpec           returnTypeSpec;

    // memberfunction return values
    RealPos                         concentration;
};

#endif

