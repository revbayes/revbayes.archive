/**
 * @file
 * This file contains the declaration of Func_resize, which
 * is used to return a matrix/vector of new dimensions.
 *
 * @brief Declaration of Func_resize
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_resize_H
#define Func_resize_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_resize_name = "Resize function";

template <typename valType, typename firstValType, typename secondValType> 
class Func_resize :  public RbFunction {
	
public:
	// Basic utility functions
	Func_resize*                clone(void) const;                                  //!< Clone the object
	const VectorString&         getClass(void) const;                               //!< Get class vector
  const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
	
	// Regular functions
  RbPtr<const ArgumentRules>  getArgumentRules(void) const;                       //!< Get argument rules
	const TypeSpec&             getReturnType(void) const;                          //!< Get type of return value
  
protected:
	RbPtr<RbLanguageObject>     executeFunction( void);                              //!< Execute operation
  
private:
  static const TypeSpec       typeSpec;
};

#endif

#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"


// Definition of the static type spec member
template <typename valType, typename firstValType, typename secondValType> 
const TypeSpec Func_resize<valType, firstValType, secondValType>::typeSpec("Func_resize", new TypeSpec(valType().getType()));


/** Clone object */
template <typename valType, typename firstValType, typename secondValType> 
Func_resize<valType, firstValType, secondValType>* Func_resize<valType, firstValType, secondValType>::clone( void ) const {
  
  return new Func_resize( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType, typename firstValType, typename secondValType> 
RbPtr<RbLanguageObject> Func_resize<valType, firstValType, secondValType>::executeFunction( void ) {

  //The identity of the object to resize
  RbPtr<valType> val( static_cast<valType*>( (*args)[0]->getValue()->clone() ) );

  // get the information from the arguments for reading the file
  Integer nrows ( static_cast<Integer*>( (RbObject*)(*args)[1]->getValue() ) );
  Integer ncols ( static_cast<Integer*>( (RbObject*)(*args)[2]->getValue() ) );

  val->resize(nrows, ncols);
  return RbPtr<RbLanguageObject>( (valType*)val );
  
}


/** Get argument rules */
template <typename valType, typename firstValType, typename secondValType> 
RbPtr<const ArgumentRules> Func_resize<valType, firstValType, secondValType>::getArgumentRules( void ) const {
  
  static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
  static bool          rulesSet = false;
  
  if ( !rulesSet ) 
  {
    argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", valType() .getTypeSpec() ) ) );
    argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "nrows", firstValType().getTypeSpec() ) ) );
    argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "ncols", secondValType().getTypeSpec() ) ) );
    rulesSet = true;
  }
  
  return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
template <typename valType, typename firstValType, typename secondValType> 
const VectorString& Func_resize<valType, firstValType, secondValType>::getClass( void ) const {
  
  static std::string  rbName  = "Func_resize<" + valType().getType() + ">"; 
  static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
  
  return rbClass;
}


/** Get return type */
template <typename valType, typename firstValType, typename secondValType> 
const TypeSpec& Func_resize<valType, firstValType, secondValType>::getReturnType( void ) const {
	
  return valType().getTypeSpec();
}


/** Get return type */
template <typename valType, typename firstValType, typename secondValType> 
const TypeSpec& Func_resize<valType, firstValType, secondValType>::getTypeSpec( void ) const {
  
  return typeSpec;
}


