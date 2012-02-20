/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__add, which is used to add two
 * variables.
 *
 * @brief Declaration and implementation of Func__add
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__add_H
#define Func__add_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

const std::string Func__add_name = "Addition";

template <typename firstValType, typename secondValType, typename retType>
class Func__add :  public RbFunction {

    public:
        // Basic utility functions
        Func__add*                  clone(void) const;                                          //!< Clone the object
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

        // Arguments
        RbVariablePtr               first;
        RbVariablePtr               second;
    
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
const TypeSpec Func__add<firstValType, secondValType, retType>::typeSpec(Func__add_name, new TypeSpec(firstValType().getType() + "," + secondValType().getType() + "," + retType().getType() ) );

/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
Func__add<firstValType, secondValType, retType>* Func__add<firstValType, secondValType, retType>::clone( void ) const {

    return new Func__add( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename firstValType, typename secondValType, typename retType>
const RbLanguageObject& Func__add<firstValType,secondValType,retType>::executeFunction( void ) {

    const firstValType&  val1       = static_cast<firstValType&> ( first->getValue() );
    const secondValType& val2       = static_cast<secondValType&>( second->getValue() );
                         retValue   = val1 + val2;

    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const ArgumentRules& Func__add<firstValType, secondValType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "first", firstValType().getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "second", secondValType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const VectorString& Func__add<firstValType, secondValType, retType>::getClass( void ) const {

    static std::string  rbName  = "Func__add<" + firstValType().getType() + "," + secondValType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec& Func__add<firstValType, secondValType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}

/** Get return spec */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec& Func__add<firstValType, secondValType, retType>::getTypeSpec( void ) const {
    
    return typeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType, typename retType>
void Func__add<firstValType, secondValType, retType>::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
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

