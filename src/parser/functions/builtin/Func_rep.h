/**
 * @file
 * This file contains the declaration of Func_rep, which is used
 * to replicate a variable n times.
 *
 * @brief Declaration of Func_rep
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 10:23:29 -0700 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_rep.h 1411 2012-04-20 17:23:29Z hoehna $
 */

#ifndef Func_rep_H
#define Func_rep_H

#include "RbFunction.h"
#include "RlVector.h"

#include <string>


template<class valueType>
class Func_rep :  public RbFunction {
    
public:
    Func_rep( void );
    
    // Basic utility functions
    Func_rep*                   clone(void) const;                                          //!< Clone the object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
    
protected:
    RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);  //!< Execute function
    
};


#include "ConstArgumentRule.h"
#include "Func_rep.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"



/** default constructor */
template <typename valueType>
Func_rep<valueType>::Func_rep( void ) : RbFunction( ) {
    
}


/** Clone object */
template <typename valueType>
Func_rep<valueType>* Func_rep<valueType>::clone( void ) const {
    
    return new Func_rep<valueType>( *this );
}


/** Execute function */
template <typename valueType>
RbPtr<RbLanguageObject> Func_rep<valueType>::executeFunction( const std::vector<const RbObject*>& args ) {
    
    const RbLanguageObject *var = static_cast<const RbLanguageObject*>( args[0] );
    int times = static_cast<const Natural*>( args[1] )->getValue();
    
    RlVector<valueType> *repValues = new RlVector<valueType>();
    for ( int i = 0; i < times; i++ ) {
        repValues->push_back( var->clone() );
    }
    
    return RbPtr<RbLanguageObject>( repValues );
}


/** Get argument rules */
template <typename valueType>
const ArgumentRules& Func_rep<valueType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) {
        
        argumentRules.push_back( new ConstArgumentRule( "x",        valueType::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "times",    Natural::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
template <typename valueType>
const std::string& Func_rep<valueType>::getClassName(void) { 
    
    static std::string rbClassName = "Replicate function<" + valueType::getClassName() + ">";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename valueType>
const TypeSpec& Func_rep<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
template <typename valueType>
const TypeSpec& Func_rep<valueType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valueType>
const TypeSpec& Func_rep<valueType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlVector<valueType>::getClassTypeSpec();
    return returnTypeSpec;
}


#endif

