/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__and, which is used to perform
 * the logical and operation.
 *
 * @brief Declaration and implementation of Func__and
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__and_H
#define Func__and_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType>
class Func__and :  public RbFunction {

    public:
        // Basic utility functions
        Func__and*                  clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	DAGNode*                    execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

};

#endif


#include "Boolean.h"
#include "DAGNode.h"
#include "Integer.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


/** Clone object */
template <typename firstValType, typename secondValType>
Func__and<firstValType, secondValType>* Func__and<firstValType, secondValType>::clone( void ) const {

    return new Func__and( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
DAGNode* Func__and<Integer,Integer>::execute( void ) {

    bool val1 = static_cast<const firstValType*> ( args[0].getValue() );
    bool val2 = static_cast<const secondValType*>( args[1].getValue() );
    return new ConstantNode( new Boolean( *val1 && *val2 ) );
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__and<firstValType, secondValType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", firstValType() .getTypeSpec() );
        argumentRules.push_back( new ValueRule( "", secondValType().getTypeSpec() );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType>
const VectorString& Func__and<firstValType, secondValType>::getClass( void ) const {

    static std::string  rbName  = "Func__and<" + firstValType().getType() + "," + secondValType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec Func__and<firstValType, secondValType>::getReturnType( void ) const {

    return TypeSpec( Boolean_name );
}

