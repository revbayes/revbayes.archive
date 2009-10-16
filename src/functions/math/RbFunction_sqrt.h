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

#ifndef RbFunction_sqrt_H
#define RbFunction_sqrt_H

#include "ArgumentRule.h"
#include "RbDataType.h"
#include "RbDouble.h"
#include "AbstractFunction.h"
#include <iostream>
#include <string>


/** This is the class for the sqrt() function, which takes a single
 *  scalar real or int.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFunction
 */
class RbFunction_sqrt :  public AbstractFunction {

    public:
            RbFunction_sqrt();                              //!< Default constructor, allocate workspace
            RbFunction_sqrt(const RbFunction_sqrt& s);      //!< Copy constructor
	        ~RbFunction_sqrt();                             //!< Destructor, delete workspace

#pragma mark Parser help functions
        static const ArgumentRule   argRules[];             //!< The argument rules

        RbObject*                   clone() const;           //!< Return copy
        const ArgumentRule*         getArgumentRules() const;    //!< Get argument rules

#pragma mark Regular functions
        RbDataType*                 execute();              //!< Get result
        void                        print(std::ostream& c) const;     //!< Print this object

    protected:
        RbDouble*                     value;              //!< Workspace for result
};

#endif
