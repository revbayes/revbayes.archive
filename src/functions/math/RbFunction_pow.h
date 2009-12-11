/**
 * @file
 * This file contains the declaration of RbFunction_pow, the
 * sqrt() function.
 *
 * @brief Declaration of RbFunction_pow
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_pow_H
#define RbFunction_pow_H

#include "RbFunction.h"
#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class RbDouble;
class RbObject;
class StringVector;

/** This is the class for the pow() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_pow :  public RbFunction {

    public:
                                    RbFunction_pow(void);                                           //!< Default constructor, allocate workspace
                                    RbFunction_pow(const RbFunction_ln& s);                         //!< Copy constructor
                                   ~RbFunction_pow(void);                                           //!< Destructor, delete workspace

        RbObject*                   clone(void) const ;                                             //!< clone this object
        const StringVector&         getClass(void) const;                                           //!< Get class
        std::string					toString(void) const;                                           //!< General info on object

        const RbObject*             executeOperation(const std::vector<DAGNode*>& arguments) const; //!< Get result
		const ArgumentRule**        getArgumentRules(void) const;                                   //!< Get the number of argument rules
        const std::string           getReturnType(void) const;                                      //!< Get return type

    protected:
        RbDouble*					value;                                                          //!< Workspace for result
};

#endif
