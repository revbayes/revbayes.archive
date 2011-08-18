/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__unot, which is used to perform
 * the unary RbBoolean not (!) operation.
 *
 * @brief Declaration and implementation of Func__unot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__unot_H
#define Func__unot_H

#include "RbFunction.h"

#include <string>

class DAGNode;
class VectorString;

template <typename valType>
class Func__unot :  public RbFunction {

    public:
        // Basic utility functions
        Func__unot*                 clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	RbLanguageObject*           execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

};

#endif

#include "RbBoolean.h"
#include "DAGNode.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


/** Clone object */
template <typename valType>
Func__unot<valType>* Func__unot<valType>::clone( void ) const {

    return new Func__unot( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType>
RbLanguageObject* Func__unot<valType>::execute( void ) {

    const valType*  val = static_cast<const valType*> ( args[0].getValue() );
    RbBoolean         res = ! (*val);

    return res.clone();
}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func__unot<valType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", valType() .getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename valType>
const VectorString& Func__unot<valType>::getClass( void ) const {

    static std::string  rbName  = "Func__unot<" + valType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType>
const TypeSpec Func__unot<valType>::getReturnType( void ) const {

    return TypeSpec( RbBoolean_name );
}

