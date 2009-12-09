/**
 * @file
 * This file contains the declaration of RbFunction_qbeta, the
 * dnorm() function.
 *
 * @brief Declaration of RbFunction_qbeta
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

#ifndef RbFunction_qbeta_H
#define RbFunction_qbeta_H

#include "RbFunction.h"
#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class RbDouble;
class RbObject;

/** This is the class for the qbeta() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_qbeta :  public RbFunction {

    public:
									RbFunction_qbeta(void);                                         //!< Default constructor, allocate workspace
                                    RbFunction_qbeta(const RbFunction_qbeta& s);                    //!< Copy constructor
								   ~RbFunction_qbeta(void);                                         //!< Destructor, delete workspace

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
