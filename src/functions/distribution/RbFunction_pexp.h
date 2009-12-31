/**
 * @file
 * This file contains the declaration of RbFunction_pexp, the
 * dnorm() function.
 *
 * @brief Declaration of RbFunction_pexp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_pexp_H
#define RbFunction_pexp_H

#include "RbFunction.h"
#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class RbDouble;
class RbObject;

/** This is the class for the pexp() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_pexp :  public RbFunction {

    public:
									RbFunction_pexp(void);                                          //!< Default constructor, allocate workspace
                                    RbFunction_pexp(const RbFunction_pexp& s);                      //!< Copy constructor
								   ~RbFunction_pexp(void);                                          //!< Destructor, delete workspace

        RbObject*                   clone(void) const ;                                             //!< clone this object
        const StringVector&         getClass(void) const;                                           //!< Get class
        std::string                 toString(void) const;                                           //!< General info on object

        const RbObject*             executeOperation(const std::vector<DAGNode*>& arguments) const; //!< Get result
		const ArgumentRule**        getArgumentRules(void) const;                                   //!< Get the number of argument rules
        const std::string           getReturnType(void) const;                                      //!< Get return type

    protected:
        RbDouble*                   value;                                                          //!< Workspace for result
};

#endif



