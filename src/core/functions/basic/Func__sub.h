/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__sub, which is used to subtract
 * two variables.
 *
 * @brief Declaration and implementation of Func__sub
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__sub_H
#define Func__sub_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType, typename retType>
class Func__sub :  public RbFunction {

    public:
        // Basic utility functions
        Func__sub*                  clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
 
    protected:
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function

    private:
        static const TypeSpec       typeSpec;
    
        // function return value
        retType                     retValue;
};

#endif

#include "DAGNode.h"
#include "Integer.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"



// Definition of the static type spec member
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec Func__sub<firstValType, secondValType, retType>::typeSpec("Func__sub", new TypeSpec(firstValType().getType() + "," + secondValType().getType() + "," + retType().getType()));


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
Func__sub<firstValType, secondValType, retType>* Func__sub<firstValType, secondValType, retType>::clone( void ) const {

    return new Func__sub( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename firstValType, typename secondValType, typename retType>
const RbLanguageObject& Func__sub<firstValType,secondValType,retType>::executeFunction( void ) {

    const firstValType&  val1 = static_cast<firstValType&> ( (*args)[0].getValue() );
    const secondValType& val2 = static_cast<secondValType&>( (*args)[1].getValue() );
    retValue = val1 - val2;
    
    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const ArgumentRules& Func__sub<firstValType, secondValType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
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
const VectorString& Func__sub<firstValType, secondValType, retType>::getClass(void) const {

    static std::string  rbName  = "Func__sub<" + firstValType().getType() + "," + secondValType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec& Func__sub<firstValType, secondValType, retType>::getReturnType(void) const {

    return retType().getTypeSpec();
}

/** Get return spec */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec& Func__sub<firstValType, secondValType, retType>::getTypeSpec( void ) const {
    
    return typeSpec;
}

