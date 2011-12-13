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
const TypeSpec Func_resize::typeSpec(Func_resize_name);


/** Clone object */
Func_resize* Func_resize::clone( void ) const {
  
  return new Func_resize( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
RbPtr<RbLanguageObject> Func_resize::executeFunction( void ) {

    unsigned int nargs = args->size();
  //The identity of the object to resize
  RbPtr<Vector> val( static_cast<Vector*>( (*args)[0]->getValue()->clone() ) );

  // get the information from the arguments for reading the file
  int nrows = ( static_cast<Natural*>( (RbObject*)(*args)[1]->getValue() ) )->getValue();
//  Integer ncols ( static_cast<Integer*>( (RbObject*)(*args)[2]->getValue() ) );

  val->resize(nrows);
  return RbPtr<RbLanguageObject>( val );
  
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_resize::getArgumentRules( void ) const {
  
  static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
  static bool          rulesSet = false;
  
  if ( !rulesSet ) 
  {
    argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", TypeSpec(Vector_name, RbPtr<TypeSpec>(new TypeSpec(RbLanguageObject_name) ) ) ) ) );
    argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", Natural_name ) ) );
    argumentRules->push_back( RbPtr<ArgumentRule>( new Ellipsis( Natural_name ) ) );
    rulesSet = true;
  }
  
  return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_resize::getClass( void ) const {
  
  static std::string  rbName  = "Func_resize"; 
  static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
  
  return rbClass;
}


/** Get return type */
const TypeSpec& Func_resize::getReturnType( void ) const {
	static TypeSpec rt = TypeSpec(Vector_name, RbPtr<TypeSpec>(new TypeSpec(RbLanguageObject_name) ) );
  return rt;
}


/** Get type spec */
const TypeSpec& Func_resize::getTypeSpec( void ) const {
  
  return typeSpec;
}


