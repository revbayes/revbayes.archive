/**
 * @file
 * This file contains the declaration of RbAccessor, the
 * abstract base class for REvBayes accessor functions.
 *
 * @brief Declaration of RbFunction_dim
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef RbAccessor_H
#define RbAccessor_H

#include "RbDataType.h"
#include "RbFunction.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/** This is the insterface class for accessor functions, which can be used
 *  both to access attributes and to set them.
 *
 *  @package    functions
 *  @interface  RbAccessor
 */
class RbAccessor :  public RbFunction {

    public:
            RbAccessor() : RbFunction() {}  //!< Constructor
	        virtual ~RbAccessor() {}        //!< Destructor, does nothing

#pragma mark Parser info
        static ArgumentRule* argumentRules; //!< The argument rules; they are the same for all accessor functions
        ArgumentRule*        getArgumentRules() { return argumentRules; } //!< Get argument rules

#pragma mark Public functions
        virtual RbDataType* execute() = 0;                              //!< Get result (access attribute)
        virtual RbDataType  getDataType() = 0;                          //!< Get data type for type checking
        virtual void        print(ostream &c) const = 0;                //!< Print this object
        virtual bool        setAttribute(RbDataType *newValue) = 0;     //!< Set attribute
};

#endif
