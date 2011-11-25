/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__ge, which is used to determine
 * whether the first variable is greater than or equal to
 * the second.
 *
 * @brief Declaration and implementof Func__ge
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__ge_H
#define Func__ge_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType>
class Func__ge :  public RbFunction {

    public:
        // Basic utility functions
        Func__ge*                   clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        RbPtr<const ArgumentRules>  getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        RbPtr<RbLanguageObject>     executeFunction(void);                                      //!< Execute function

    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;
};

#endif

#include "RbBoolean.h"
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
template <typename firstValType, typename secondValType>
const TypeSpec Func__ge<firstValType, secondValType>::typeSpec("Func__ge", new TypeSpec(firstValType().getType() + "," + secondValType().getType()));
template <typename firstValType, typename secondValType>
const TypeSpec Func__ge<firstValType, secondValType>::returnTypeSpec(RbBoolean_name);


/** Clone object */
template <typename firstValType, typename secondValType>
Func__ge<firstValType, secondValType>* Func__ge<firstValType, secondValType>::clone( void ) const {

    return new Func__ge( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
RbPtr<RbLanguageObject> Func__ge<firstValType,secondValType>::executeFunction( void ) {

    const RbPtr<firstValType>  val1( static_cast<firstValType*> ( (RbObject*)(*args)[0]->getValue() ) );
    const RbPtr<secondValType> val2( static_cast<secondValType*>( (RbObject*)(*args)[1]->getValue() ) );
    
    return RbPtr<RbLanguageObject>( new RbBoolean( *val1 >= *val2 ) );
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
RbPtr<const ArgumentRules> Func__ge<firstValType, secondValType>::getArgumentRules(void) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if ( !rulesSet ) {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", firstValType() .getTypeSpec() ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", secondValType().getTypeSpec() ) ) );
        rulesSet = true;
    }

    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType>
const VectorString& Func__ge<firstValType, secondValType>::getClass( void ) const {

    static std::string  rbName  = "Func__ge<" + firstValType().getType() + "," + secondValType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ge<firstValType, secondValType>::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get return spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ge<firstValType, secondValType>::getTypeSpec( void ) const {
    
    return typeSpec;
}

