/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__uminus, which is used to perform
 * the unary minus operation.
 *
 * @brief Declaration and implementation of Func__uminus
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__uminus_H
#define Func__uminus_H

#include "RbFunction.h"

#include <string>

class RbLanguageObject;
class VectorString;

template <typename valType, typename retType>
class Func__uminus :  public RbFunction {

    public:
        // Basic utility functions
        Func__uminus*               clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
    	RbLanguageObject*           execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

#include "DAGNode.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


// Definition of the static type spec member
template <typename firstValType, typename retType>
const TypeSpec Func__uminus<firstValType, retType>::typeSpec("Func__uminus", new TypeSpec(firstValType().getType() + "," + retType().getType()));


/** Clone object */
template <typename valType, typename retType>
Func__uminus<valType, retType>* Func__uminus<valType, retType>::clone( void ) const {

    return new Func__uminus( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType, typename retType>
RbLanguageObject* Func__uminus<valType, retType>::execute( void ) {

    const valType*  val = static_cast<const valType*> ( args[0].getValue() );
    retType         res = -( *val );

    return res.clone();
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func__uminus<valType, retType>::getArgumentRules( void ) const {

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
const VectorString& Func__uminus<valType, retType>::getClass( void ) const {

    static std::string  rbName  = "Func__uminus<" + valType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec Func__uminus<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}


/** Get return spec */
template <typename firstValType, typename retType>
const TypeSpec& Func__uminus<firstValType, retType>::getTypeSpec( void ) const {
    
    return typeSpec;
}

