/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__uplus, which is used to perform
 * the unary plus operation.
 *
 * @brief Declaration and implementation of Func__uplus
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__uplus_H
#define Func__uplus_H

#include "RbFunction.h"

#include <string>

class DAGNode;
class VectorString;

template <typename valType, typename retType>
class Func__uplus :  public RbFunction {

    public:
        // Basic utility functions
        Func__uplus*                clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	RbLanguageObject*           execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

};

#endif

#include "RbLanguageObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


/** Clone object */
template <typename valType, typename retType>
Func__uplus<valType, retType>* Func__uplus<valType, retType>::clone( void ) const {

    return new Func__uplus( *this );
}


/** Execute function: We simply return a copy of the value */
template <typename valType, typename retType>
RbLanguageObject* Func__uplus<valType, retType>::execute( void ) {

    const valType*  val = static_cast<const valType*> ( args[0].getValue() );

    return val->clone();
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func__uplus<valType, retType>::getArgumentRules( void ) const {

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
template <typename valType, typename retType>
const VectorString& Func__uplus<valType, retType>::getClass( void ) const {

    static std::string  rbName  = "Func__uplus<" + valType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec Func__uplus<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}

