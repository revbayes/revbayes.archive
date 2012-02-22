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
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
        void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);

    private:
    
        // Arguments
        RbVariablePtr               value;
    
        // function return value
        retType                     retValue;
};

#endif

#include "DAGNode.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"



/** Clone object */
template <typename valType, typename retType>
Func__uminus<valType, retType>* Func__uminus<valType, retType>::clone( void ) const {

    return new Func__uminus( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType, typename retType>
const RbLanguageObject& Func__uminus<valType, retType>::executeFunction( void ) {

    const valType& val = static_cast<valType&> ( value->getValue() );
             retValue = -( val );

    return retValue;
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func__uminus<valType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "value", valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType>
const std::string& Func__uminus<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__uminus<" + firstValType().getType() + "," + secondValType().getType() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uminus<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uminus<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec& Func__uminus<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename retType>
void Func__uminus<firstValType, retType>::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "value" ) {
        value = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

