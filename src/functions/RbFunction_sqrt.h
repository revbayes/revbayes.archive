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
#include "RbReal.h"
#include "RbStandardFxn.h"
#include <iostream>
#include <string>


/** This is the class for the sqrt() function, which takes a single
 *  vector (or scalar) of reals or ints.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFxn
 */
class RbFunction_sqrt :  public RbStandardFxn {

    public:
            RbFunction_sqrt();                              //!< Default constructor, allocate workspace
            RbFunction_sqrt(const RbFunction_sqrt& s);      //!< Copy constructor
	        ~RbFunction_sqrt();                             //!< Destructor, delete workspace

#pragma mark Parser help functions
        static const ArgumentRule   argRules[];             //!< The argument rules
        RbFunction_sqrt*            copy() const;           //!< Return copy
        const ArgumentRule*         getArgRules() const;    //!< Get argument rules
        const std::string&          getDataType() const;    //!< Get data type of result

#pragma mark Regular functions
        RbDataType*                 execute();              //!< Get result
        void                        print(std::ostream& c=std::cout) const;     //!< Print this object

    protected:
        RbReal*                     resultVec;              //!< Workspace for result
};

#endif
