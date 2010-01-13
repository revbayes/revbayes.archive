/**
 * @file
 * This file contains the declaration of RbFunction_rbeta, the
 * dnorm() function.
 *
 * @brief Declaration of RbFunction_rbeta
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

#ifndef RbFunction_rbeta_H
#define RbFunction_rbeta_H

#include "RbFunction.h"
#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class RbDouble;
class RbObject;

/** This is the class for the dbeta() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_rbeta :  public RbFunction {

    public:
									RbFunction_rbeta(void);                                         //!< Default constructor, allocate workspace
                                    RbFunction_rbeta(const RbFunction_rbeta& s);                    //!< Copy constructor
								   ~RbFunction_rbeta(void);                                         //!< Destructor, delete workspace

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

