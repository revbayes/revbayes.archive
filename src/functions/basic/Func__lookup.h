/**
 * @file
 * This file contains the declaration of Func__lookup, which is used
 * to look up a variable.
 *
 * @brief Declaration of Func__lookup
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__lookup_H
#define Func__lookup_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class StringVector;

class Func__lookup :  public RbFunction {

    public:
                                    Func__lookup(const std::string& type, int dim); //!< Constructor
                                    Func__lookup(const Func__lookup& x);            //!< Copy constructor

        // Assignment operator
        Func__lookup&               operator=(const Func__lookup& x);     //!< Assignment operator

        // Basic utility functions
        Func__lookup*               clone(void) const;                      //!< Clone the object
    	bool                        equals(const RbObject* obj) const;      //!< Equals comparison
    	const StringVector&         getClass(void) const;                   //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;           //!< Get argument rules
        int                         getReturnDim(void) const;               //!< Get dim of return value
        const std::string&          getReturnType(void) const;              //!< Get type of return value
        DAGNode*                    getVariable(void) const;                //!< Get pointer to variable (for changing variable)

	protected:
        RbObject*                   executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation

        ArgumentRules               argRules;                               //!< The argument rules
};

#endif

