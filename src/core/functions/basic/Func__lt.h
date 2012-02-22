/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__lt, which is used to determine
 * whether the first variable is less than the second.
 *
 * @brief Declaration and implementof Func__lt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__lt_H
#define Func__lt_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType>
class Func__lt :  public RbFunction {

    public:
        // Basic utility functions
        Func__lt*                   clone(void) const;                                          //!< Clone the object
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



/** Clone object */
template <typename firstValType, typename secondValType>
Func__lt<firstValType, secondValType>* Func__lt<firstValType, secondValType>::clone( void ) const {

    return new Func__lt( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
const RbLanguageObject& Func__lt<firstValType,secondValType>::executeFunction( void ) {

    const firstValType&  val1 = static_cast<firstValType&> ( first->getValue()  );
    const secondValType& val2 = static_cast<secondValType&>( second->getValue() );
    retValue.setValue( val1 < val2 );
    
    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__lt<firstValType, secondValType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {
        argumentRules.push_back( new ValueRule( "first", firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "second", secondValType().getTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType>
const std::string& Func__lt<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__lt<" + firstValType().getType() + "," + secondValType().getType() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__lt<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__lt<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__lt<firstValType, secondValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType>
void Func__lt<firstValType, secondValType>::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
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

