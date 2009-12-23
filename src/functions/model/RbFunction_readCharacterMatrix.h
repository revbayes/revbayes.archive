/**
 * @file
 * This file contains the declaration of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Declaration of RbFunction_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_readCharacterMatrix_H
#define RbFunction_readCharacterMatrix_H

#include "ArgumentRule.h"
#include "RbDouble.h"
#include "RbFunction.h"
#include "RbObject.h"
#include <iostream>
#include <string>
#include <vector>

class RbDumpState;
class DAGNode;

/** This is the class for the sqrt() function, which takes a single
 *  scalar real or int.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFunction
 */
class RbFunction_readCharacterMatrix :  public RbFunction {

    public:

            RbFunction_readCharacterMatrix(void);                              //!< Default constructor, allocate workspace
            RbFunction_readCharacterMatrix(const RbFunction_readCharacterMatrix& s);      //!< Copy constructor
	        ~RbFunction_readCharacterMatrix();                             //!< Destructor, delete workspace


	        RbObject*                   clone(void) const ;                                             //!< clone this object
	        const StringVector&         getClass(void) const;                                           //!< Get class
	        std::string                 toString(void) const;                                           //!< General info on object

	        const RbObject*             executeOperation(const std::vector<RbObjectWrapper*>& arguments) const; //!< Get result
	        const ArgumentRule**        getArgumentRules(void) const;                                   //!< Get the number of argument rules
	        const std::string           getReturnType(void) const;

    protected:
};

#endif
