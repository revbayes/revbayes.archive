/**
 * @file
 * This file contains the declaration of Func_unique, which
 * is used to return a vector of unique values in a vector.
 *
 * @brief Declaration of Func_unique
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_unique_H
#define Func_unique_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;

const std::string Func_unique_name = "Unique function";

template <typename valType>
class Func_unique :  public RbFunction {
	
public:
	// Basic utility functions
	Func_unique*                clone(void) const;                                              //!< Clone the object
    static const std::string&   getClassName(void);                                             //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                        //!< Get language type of the object
	
	// Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                                   //!< Get argument rules
	const TypeSpec&             getReturnType(void) const;                                      //!< Get type of return value

protected:
    RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);      //!< Execute operation
    
};

#endif

#include "ConstArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"


/** Clone object */
template <typename valType>
Func_unique<valType>* Func_unique<valType>::clone( void ) const {
    
    return new Func_unique( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType> 
RbPtr<RbLanguageObject> Func_unique<valType>::executeFunction( const std::vector<const RbObject*>& args ) {
    
    valType *retValue = static_cast<const valType*>( args[0] )->clone();    
    if(retValue->size() == 0) 
        return RbPtr<RbLanguageObject>( retValue );
    
    retValue->sort();
    retValue->unique();
    
    return RbPtr<RbLanguageObject>( retValue );

}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func_unique<valType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        argumentRules.push_back( new ConstArgumentRule( "value", valType() .getTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
template <typename firstValType>
const std::string& Func_unique<firstValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_unique<" + firstValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType>
const TypeSpec& Func_unique<firstValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType>
const TypeSpec& Func_unique<firstValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType> 
const TypeSpec& Func_unique<valType>::getReturnType( void ) const {
	
    return valType().getTypeSpec();
}


