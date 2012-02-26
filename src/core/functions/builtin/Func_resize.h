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
    static const std::string&   getClassName(void);                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
	
	// Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
	const TypeSpec&             getReturnType(void) const;                          //!< Get type of return value
  
protected:
    void                        clearArguments(void);                               //!< Clear the arguments of this class
	const RbLanguageObject&     executeFunction( void);                              //!< Execute operation
    void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);
  
private:
    void                        resizeVector(AbstractVector &vec, Environment *args, unsigned int numArg);
  
    // arguments
    RbVariablePtr               container;
    std::vector<RbVariablePtr>  sizes;
    
    // function return value
    AbstractVector*             retValue;
};

#endif

#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"


/** Clear the arguments. We empty the list of elements to print. Then give the call back to the base class. */
void Func_resize::clearArguments(void) {
    // just empty the elements list, the super smart pointers will take care of the rest
    sizes.clear();
    
    // we can also set the other elements to NULL
    container = NULL;
}

/** Clone object */
Func_resize* Func_resize::clone( void ) const {
  
  return new Func_resize( *this );
}

void Func_resize::resizeVector(AbstractVector &vec, Environment *args, unsigned int numArg) { 
    unsigned int nrows = ( static_cast<Natural&>( (*args)[numArg].getValue() ) ).getValue();
    for (unsigned int j = 0 ; j < vec.size() ; j ++) {
        if (vec.getElement(j).isTypeSpec(Vector::getClassTypeSpec() )) { //if element j already a vector
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
    retValue = static_cast<AbstractVector*>( container->getValue().clone() );

    if (nargs == 2) {
      //Resizing a vector
      unsigned int nrows = ( static_cast<Natural&>( sizes[0]->getValue() ) ).getValue();
      retValue->resize(nrows);
    }
    else {
      //Resizing a matrix of nargs-1 dimensions
        int nrows = ( static_cast<Natural&>( sizes[0]->getValue() ) ).getValue();
        
        if (!retValue->isTypeSpec(Vector::getClassTypeSpec())) {
            retValue = static_cast<AbstractVector*>( retValue->convertTo(TypeSpec(Vector::getClassName(), new TypeSpec( AbstractVector::getClassTypeSpec() ), new TypeSpec(RbLanguageObject::getClassTypeSpec()) ) ) );
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
      argumentRules.push_back( new ValueRule( "x", AbstractVector::getClassTypeSpec() ) );
      argumentRules.push_back( new ValueRule( "size", Natural::getClassTypeSpec() ) );
      argumentRules.push_back( new Ellipsis( Natural::getClassTypeSpec() ) );
    rulesSet = true;
  }
  
  return argumentRules;
}


/** Get class name of object */
const std::string& Func_resize::getClassName(void) { 
    
    static std::string rbClassName = "Resize function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_resize::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_resize::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_resize::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = AbstractVector::getClassTypeSpec();
    return returnTypeSpec;
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


