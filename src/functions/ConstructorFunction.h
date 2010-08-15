/**
 * @file
 * This file contains the declaration of ConstructorFunction, which is used
 * for functions that construct member objects.
 *
 * @brief Declaration of ConstructorFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef ConstructorFunction_H
#define ConstructorFunction_H

#include "RbFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class DAGNode;
class MemberObject;
class StringVector;

class ConstructorFunction :  public RbFunction {

    public:
                                    ConstructorFunction(MemberObject* obj);     //!< Object constructor

        // Basic utility functions
        ConstructorFunction*        clone(void) const;                      //!< Clone the object
    	bool                        equals(const RbObject* obj) const;      //!< Equals comparison
    	const StringVector&         getClass(void) const;                   //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;           //!< Get argument rules
        const std::string&          getReturnType(void) const;              //!< Get type of return value

        // Test new design
        RbObject*                   getValue(void);                         //!< Get new result object

	protected:
        const RbObject*             executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation

        MemberObject*               templateObject;                         //!< The template object
};

#endif

