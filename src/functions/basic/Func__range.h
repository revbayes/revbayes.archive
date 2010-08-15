/**
 * @file
 * This file contains the declaration of Func__range, which is used
 * to look up a variable.
 *
 * @brief Declaration of Func__range
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__range_H
#define Func__range_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class StringVector;

class Func__range :  public RbFunction {

    public:
        // Basic utility functions
        Func__range*                clone(void) const;                      //!< Clone the object
    	const StringVector&         getClass(void) const;                   //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;           //!< Get argument rules
        int                         getReturnDim(void) const;               //!< Get dim of return value
        const std::string&          getReturnType(void) const;              //!< Get type of return value

        // New design
        RbObject*                   getValue(void);                         //!< Get function value

	protected:
        RbObject*                   convertTo(const std::string& type) const;        //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;  //!< Is convertible to type and dim?
        const RbObject*             executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation
};

#endif

