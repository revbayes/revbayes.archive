/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_vector, which is used to create
 * value vectors.
 *
 * @brief Declaration and implementation of Func_vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_vector_H
#define Func_vector_H

#include "RbFunction.h"
#include <map>
#include <string>

template <typename valType>
class Func_vector :  public RbFunction {

    public:
        Func_vector();
    
        // Basic utility functions
        Func_vector*                clone(void) const;                                          //!< Clone the object
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



template <typename valType>
Func_vector<valType>::Func_vector() : RbFunction() {
    
}

/* Clone object */
template <typename valType>
Func_vector<valType>* Func_vector<valType>::clone( void ) const {
    
    return new Func_vector( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType>
RbPtr<RbLanguageObject> Func_vector<valType>::executeFunction( const std::vector<const RbObject*>& args ) {
    
    RlVector<valType> *theVector = new RlVector<valType>();
    for ( size_t i = 0; i < args.size(); i++ )
        theVector->push_back( *static_cast<const valType*>( args[i] ) );
    
    return theVector;
}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func_vector<valType>::getArgumentRules( void ) const {
    
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
const std::string& Func_vector<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_vector<" + valType::getClassTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename valType>
const TypeSpec& Func_vector<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( "Func_vector", new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename valType>
const TypeSpec& Func_vector<valType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType>
const TypeSpec& Func_vector<valType>::getReturnType( void ) const {
    
    return RlVector<valType>::getClassTypeSpec();
}


#endif


