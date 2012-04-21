/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__and, which is used to perform
 * the logical and operation.
 *
 * @brief Declaration and implementation of Func__and
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__and_H
#define Func__and_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <string>

template <typename firstValType, typename secondValType>
class Func__and :  public RbFunction {

    public:
        Func__and( void );
    
        // Basic utility functions
        Func__and*                  clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(const std::vector<const RbObject*>& args);   //!< Execute function

    private:
    
        // function return value
        RbBoolean                   retValue;
};

#endif


#include "RbBoolean.h"
#include "DAGNode.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "TypeSpec.h"
#include "ValueRule.h"


/** default constructor */
template <typename firstValType, typename secondValType>
Func__and<firstValType, secondValType>::Func__and( void ) : RbFunction( ) {
    
}


/** Clone object */
template <typename firstValType, typename secondValType>
Func__and<firstValType, secondValType>* Func__and<firstValType, secondValType>::clone( void ) const {

    return new Func__and( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
const RbLanguageObject& Func__and<firstValType,secondValType>::executeFunction( const std::vector<const RbObject*>& args ) {

    const firstValType&  val1 = static_cast<const firstValType&> ( *args[0]  );
    const secondValType& val2 = static_cast<const secondValType&>( *args[1] );
    retValue.setValue( val1 && val2 );
    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__and<firstValType, secondValType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "first", firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "second", secondValType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType>
const std::string& Func__and<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__and<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__and<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__and<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__and<firstValType, secondValType>::getReturnType( void ) const {

    static TypeSpec returnTypeSpec = RbBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}

