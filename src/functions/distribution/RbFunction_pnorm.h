/**
 * @file
 * This file contains the declaration of RbFunction_pnorm, the
 * dnorm() function.
 *
 * @brief Declaration of RbFunction_pnorm
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

#ifndef RbFunction_pnorm_H
#define RbFunction_pnorm_H

#include "RbFunction.h"

#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class RbDouble;
class RbObject;

/** This is the class for the dnorm() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_pnorm :  public RbFunction {

    public:
									RbFunction_pnorm(void);                                            //!< Default constructor, allocate workspace
                                    RbFunction_pnorm(const RbFunction_pnorm& s);                       //!< Copy constructor
								   ~RbFunction_pnorm(void);                                            //!< Destructor, delete workspace

        RbObject*                   clone(void) const ;                                                //!< clone this object
        const StringVector&         getClass(void) const;                                              //!< Get class
        std::string                 toString(void) const;                                              //!< General info on object

        const RbObject*             executeOperation(const std::vector<DAGNode*>& args) const; //!< Get result
		const ArgumentRule**        getArgumentRules(void) const;                                      //!< Get the number of argument rules
        const std::string           getReturnType(void) const;                                         //!< Get return type

    protected:
        RbDouble*                   value;                                                             //!< Workspace for result
};

#endif


