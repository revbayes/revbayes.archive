/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__mul, which is used to multiply
 * two variables.
 *
 * @brief Declaration and implementation of Func__mul
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__mul_H
#define Func__mul_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType, typename retType>
class Func__mul :  public RbFunction {

    public:
        // Basic utility functions
        Func__mul*                  clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	RbLanguageObject*           execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

};

#endif

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
template <typename firstValType, typename secondValType, typename retType>
Func__mul<firstValType, secondValType, retType>* Func__mul<firstValType, secondValType, retType>::clone( void ) const {

    return new Func__mul( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename firstValType, typename secondValType, typename retType>
RbLanguageObject* Func__mul<firstValType,secondValType,retType>::execute( void ) {

    const firstValType*  val1 = static_cast<const firstValType*> ( args[0].getValue() );
    const secondValType* val2 = static_cast<const secondValType*>( args[1].getValue() );

    retType              prod = *val1 * *val2;
        
    return prod.clone();
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const ArgumentRules& Func__mul<firstValType, secondValType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "", secondValType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const VectorString& Func__mul<firstValType, secondValType, retType>::getClass(void) const {

    static std::string  rbName  = "Func__mul<" + firstValType().getType() + "," + secondValType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec Func__mul<firstValType, secondValType, retType>::getReturnType(void) const {

    return retType().getTypeSpec();
}

