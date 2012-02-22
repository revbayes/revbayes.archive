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
const RbLanguageObject& Func__uplus<valType, retType>::executeFunction( void ) {

    const valType& val = static_cast<valType&> ( value->getValue() );

    return val;
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func__uplus<valType, retType>::getArgumentRules( void ) const {

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
const std::string& Func__uplus<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__uplus<" + firstValType().getType() + "," + secondValType().getType() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uplus<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uplus<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec& Func__uplus<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename retType>
void Func__uplus<firstValType, retType>::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "value" ) {
        value = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

