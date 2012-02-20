/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__gt, which is used to determine
 * whether the first variable is greater than the second.
 *
 * @brief Declaration and implementof Func__gt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__gt_H
#define Func__gt_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType>
class Func__gt :  public RbFunction {

    public:
        // Basic utility functions
        Func__gt*                   clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
        void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);

    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;
    
        // Arguments
        RbVariablePtr               first;
        RbVariablePtr               second;
    
        // function return value
        RbBoolean                   retValue;
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
const TypeSpec Func__gt<firstValType, secondValType>::typeSpec("Func__gt", new TypeSpec(firstValType().getType() + "," + secondValType().getType()));
template <typename firstValType, typename secondValType>
const TypeSpec Func__gt<firstValType, secondValType>::returnTypeSpec(RbBoolean_name);


/** Clone object */
template <typename firstValType, typename secondValType>
Func__gt<firstValType, secondValType>* Func__gt<firstValType, secondValType>::clone( void ) const {

    return new Func__gt( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
const RbLanguageObject& Func__gt<firstValType,secondValType>::executeFunction( void ) {

    const firstValType&  val1 = static_cast<firstValType&> ( first->getValue()  );
    const secondValType& val2 = static_cast<secondValType&>( second->getValue() );
    retValue.setValue( val1 > val2 );
    
    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__gt<firstValType, secondValType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {
        argumentRules.push_back( new ValueRule( "first", firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "second", secondValType().getTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType>
const VectorString& Func__gt<firstValType, secondValType>::getClass( void ) const {

    static std::string  rbName  = "Func__gt<" + firstValType().getType() + "," + secondValType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__gt<firstValType, secondValType>::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get return spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__gt<firstValType, secondValType>::getTypeSpec( void ) const {
    
    return typeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType>
void Func__gt<firstValType, secondValType>::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "first" ) {
        first = var;
    }
    else if ( name == "second" ) {
        second = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

