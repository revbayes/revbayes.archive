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
    const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
	const TypeSpec&             getReturnType(void) const;                          //!< Get type of return value
  
protected:
	const RbLanguageObject&     executeFunction( void);                              //!< Execute operation
    void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);
  
private:
    void                        resizeVector(Container &vec, Environment *args, unsigned int numArg);
    static const TypeSpec       typeSpec;
  
    // arguments
    RbVariablePtr               container;
    std::vector<RbVariablePtr>  sizes;
    
    // function return value
    Container*                  retValue;
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

void Func_resize::resizeVector(Container &vec, Environment *args, unsigned int numArg) { 
    unsigned int nrows = ( static_cast<Natural&>( (*args)[numArg].getValue() ) ).getValue();
    for (unsigned int j = 0 ; j < vec.size() ; j ++) {
        if (vec.getElement(j).isTypeSpec(Vector_name )) { //if element j already a vector
            static_cast<Vector&>( vec.getElement(j)).resize(nrows);
        }
        else {
            Vector* v2 = new Vector(vec.getElement(j).getTypeSpec() );
            v2->push_back(vec.getElement(j).clone());
            v2->resize(nrows);
            vec.setElement(j, v2);
        }
        if (args->size() != numArg +1) { //if we still have dimensions to populate
            resizeVector((static_cast<Vector&>( vec.getElement(j))), args, numArg+1);
        }
        
    }
}

/** Execute function: We rely on operator overloading to provide the necessary functionality */
const RbLanguageObject& Func_resize::executeFunction( void ) {

    unsigned long nargs = args.size();
    //The identity of the object to resize
    retValue = static_cast<Container*>( container->getValue().clone() );

    if (nargs == 2) {
      //Resizing a vector
      unsigned int nrows = ( static_cast<Natural&>( sizes[0]->getValue() ) ).getValue();
      retValue->resize(nrows);
    }
    else {
      //Resizing a matrix of nargs-1 dimensions
        int nrows = ( static_cast<Natural&>( sizes[0]->getValue() ) ).getValue();
        
        if (!retValue->isType(Vector_name)) {
            retValue = static_cast<Container*>( retValue->convertTo(TypeSpec(Vector_name, new TypeSpec(RbLanguageObject_name) ) ) );
        }
        
        retValue->resize(nrows);
        // TODO: We need to fix this!
//        resizeVector(*retValue, args, 2);
    }
  return *retValue;
  
}


/** Get argument rules */
const ArgumentRules& Func_resize::getArgumentRules( void ) const {
  
  static ArgumentRules argumentRules = ArgumentRules();
  static bool          rulesSet = false;
  
  if ( !rulesSet ) 
  {
    argumentRules.push_back( new ValueRule( "x", TypeSpec(Container_name) ) );
    argumentRules.push_back( new ValueRule( "size", Natural_name ) );
    argumentRules.push_back( new Ellipsis( Natural_name ) );
    rulesSet = true;
  }
  
  return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_resize::getClass( void ) const {
  
  static std::string  rbName  = "Func_resize"; 
  static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
  
  return rbClass;
}


/** Get return type */
const TypeSpec& Func_resize::getReturnType( void ) const {
	static TypeSpec rt = TypeSpec(Vector_name, new TypeSpec(RbLanguageObject_name) );
  return rt;
}


/** Get type spec */
const TypeSpec& Func_resize::getTypeSpec( void ) const {
  
  return typeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_resize::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "x" ) {
        container = var;
    }
    else if ( name == "size" || name == "") {
        sizes.push_back(var);
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}


