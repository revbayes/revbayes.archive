/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_rlvector, which is used to create
 * value vectors of RevLanguage only vectors. See the different 
 * vectors for more information.
 *
 * @brief Declaration and implementation of Func_rlvector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-01 14:55:05 +0200 (Fri, 01 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_rlvector.h 1603 2012-06-01 12:55:05Z hoehna $
 */

#ifndef Func_rlvector_H
#define Func_rlvector_H

#include "RbFunction.h"
#include <map>
#include <string>

template <typename valType>
class Func_rlvector :  public RbFunction {
    
public:
    Func_rlvector();
    
    // Basic utility functions
    Func_rlvector*              clone(void) const;                                          //!< Clone the object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
    
protected:
    RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);  //!< Execute function
    
};


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "RlOnlyVector.h"



template <typename valType>
Func_rlvector<valType>::Func_rlvector() : RbFunction() {
    
}

/* Clone object */
template <typename valType>
Func_rlvector<valType>* Func_rlvector<valType>::clone( void ) const {
    
    return new Func_rlvector( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType>
RbPtr<RbLanguageObject> Func_rlvector<valType>::executeFunction( const std::vector<const RbObject*>& args ) {
    
    RlOnlyVector<valType> *theVector = new RlOnlyVector<valType>();
    for ( size_t i = 0; i < args.size(); i++ )
        theVector->push_back( static_cast<const valType &>( *args[i] ) );
    
    return theVector;
}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func_rlvector<valType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        argumentRules.push_back( new ArgumentRule( "", true, valType::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     valType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
template <typename valType>
const std::string& Func_rlvector<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_vector<" + valType::getClassTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename valType>
const TypeSpec& Func_rlvector<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( "Func_vector", new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename valType>
const TypeSpec& Func_rlvector<valType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType>
const TypeSpec& Func_rlvector<valType>::getReturnType( void ) const {
    
    return RlOnlyVector<valType>::getClassTypeSpec();
}


#endif


