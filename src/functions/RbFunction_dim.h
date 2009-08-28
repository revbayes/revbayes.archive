/**
 * @file
 * This file contains the declaration of RbFunction_dim, the
 * REvBayes dim() function.
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

#ifndef RbFunction_dim_H
#define RbFunction_dim_H

#include "RbDataType.h"
#include "RbFunction.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/** This is the class for the dim() function, which can be used to access the
 *  dim attribute of language objects (dim(x)) or set it (dim(x)<-)
 *
 *  @package    functions
 *  @implements RbAccessor
 */
class RbFunction_dim :  public RbAccessor {

    public:
            RbFunction_dim() : RbAccessor() {}  //!< Constructor uses default argument rules
	        virtual ~RbFunction_dim() {}        //!< Destructor, does nothing

#pragma mark Public functions
        RbDataType* execute();                                  //!< Get result
        RbDataType  getDataType() { return RbVector(); }        //!< Get data type for type checking
        void        print(ostream &c) const;                    //!< Print this object
        bool        setAttribute(RbDataType *newValue);         //!< Set attribute  
};

#endif
