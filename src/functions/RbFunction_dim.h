/**
 * @file
 * This file contains the declaration of RbFunction_dim, the
 * dim() accessor function.
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

#include "RbAccessorFxn.h"
#include "RbDataType.h"
#include "RbInt.h"

/** This is the class for the dim() function, which can be used to access the
 *  dim attribute of language objects (dim(x)) or set it (dim(x)<-)
 *
 *  @package    functions
 *  @implements RbFunction, RbAccessorFxn
 */
class RbFunction_dim :  public RbAccessorFxn {

    public:
            RbFunction_dim() : RbAccessorFxn() {}   //!< Default constructor
            RbFunction_dim(const RbFunction_dim& s) : RbAccessorFxn(s) {}   //!< Copy constructor
	        ~RbFunction_dim() {}                    //!< Destructor does nothing

#pragma mark Parser help functions
        RbFunction_dim*         copy() const { return new RbFunction_dim(*this); }  //!< Get copy
        const string&           getDataType() const { return RbInt::dataType; }     //!< Get result type

#pragma mark Regular functions
        RbDataType*             execute();                                  //!< Get result
        void                    print(std::ostream& c) const;               //!< Print this object
        bool                    setAttribute(const RbDataType *newValue);   //!< Set attribute
};

#endif
