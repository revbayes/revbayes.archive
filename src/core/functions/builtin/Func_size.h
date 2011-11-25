/**
 * @file
 * This file contains the declaration of Func_size, which
 * is used to count the number of elements in a vector.
 *
 * @brief Declaration of Func_size
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_size_H
#define Func_size_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_size_name = "Size function";

template <typename valType>
class Func_size :  public RbFunction {
	
public:
	// Basic utility functions
	Func_size*                  clone(void) const;                                  //!< Clone the object
	const VectorString&         getClass(void) const;                               //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
	
	// Regular functions
    RbPtr<const ArgumentRules>  getArgumentRules(void) const;                       //!< Get argument rules
	const TypeSpec&             getReturnType(void) const;                          //!< Get type of return value

protected:
    RbPtr<RbLanguageObject>     executeFunction(void);                              //!< Execute operation

private:
    static const TypeSpec       typeSpec;
    static const TypeSpec       returnTypeSpec;
};

#endif

#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"


// Definition of the static type spec member
template <typename valType>
const TypeSpec Func_size<valType>::typeSpec(Func_size_name, new TypeSpec(valType().getType()));
template <typename valType>
const TypeSpec Func_size<valType>::returnTypeSpec(Natural_name);


/** Clone object */
template <typename valType>
Func_size<valType>* Func_size<valType>::clone( void ) const {
    
    return new Func_size( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType> 
RbPtr<RbLanguageObject> Func_size<valType>::executeFunction( void ) {
    
    const RbPtr<valType> val( static_cast<valType*> ( (RbObject*)(*args)[0]->getValue() ) ) ;
    RbPtr<RbLanguageObject> size( new Natural( val->size() ) ); 
    return size;
    
}


/** Get argument rules */
template <typename valType>
RbPtr<const ArgumentRules> Func_size<valType>::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", valType() .getTypeSpec() ) ) );
        rulesSet = true;
    }
    
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
template <typename valType>
const VectorString& Func_size<valType>::getClass( void ) const {
    
    static std::string  rbName  = "Func_size<" + valType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType> 
const TypeSpec& Func_size<valType>::getReturnType( void ) const {
	
    return returnTypeSpec;
}


/** Get return type */
template <typename valType>
const TypeSpec& Func_size<valType>::getTypeSpec( void ) const {
    
    return typeSpec;
}


