/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_simplex, which is used to create
 * simplex objects.
 *
 * @brief Declaration and implementation of Func_simplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_simplex_H
#define Func_simplex_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;

template <typename valType>
class Func_simplex :  public RbFunction {

    public:
        // Basic utility functions
        Func_simplex<valType>*      clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        bool                        throws(void) const;                                         //!< One variant needs to throw

    protected:
        RbPtr<RbLanguageObject>     executeFunction(void);                                      //!< Execute function

};

#endif


#include "ConstArgumentRule.h"
#include "Ellipsis.h"
#include "Integer.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RbVector.h"
#include "Simplex.h"
#include "TypeSpec.h"


/** Clone the object */
template <typename valType>
Func_simplex<valType>* Func_simplex<valType>::clone( void ) const {
    return new Func_simplex<valType>( *this );
}


/** Execute function: Simplex <- ( Integer ) */
template <>
RbPtr<RbLanguageObject> Func_simplex<Integer>::executeFunction( void ) {

    int size = static_cast<const Integer&>( *args[0]->getVariable()->getValue() ).getValue();

    if ( size < 2 )
        throw RbException( "Simplex size must be at least 2" );

    return RbPtr<RbLanguageObject>( new Simplex( size ) );
}


/** Execute function: Simplex <- ( VectorRealPos ) */
template <>
RbPtr<RbLanguageObject> Func_simplex<RbVector>::executeFunction( void ) {

    const RbVector& tempVec = static_cast<const RbVector& >( *args[0]->getVariable()->getValue() );

    return RbPtr<RbLanguageObject>( new Simplex( tempVec ) );
}


/** Execute function: Simplex <- ( RealPos, RealPos, ... ) */
template <>
RbPtr<RbLanguageObject> Func_simplex<RealPos>::executeFunction( void ) {

    RbVector  tempVec( RealPos::getClassTypeSpec() );
    for ( size_t i = 0; i < args.size(); i++ )
        tempVec.push_back( static_cast<const RealPos&>( *args[i]->getVariable()->getValue() ).clone() );

    // Normalization is done by the Simplex constructor
    return RbPtr<RbLanguageObject>( new Simplex( tempVec ) );
}


/** Get argument rules for general case */
template <typename valType>
const ArgumentRules& Func_simplex<valType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ConstArgumentRule( "", valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get argument rules for: Simplex <- ( RealPos, RealPos, ... ) */
template <>
const ArgumentRules& Func_simplex<RealPos>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ConstArgumentRule( "", RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "", RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     RealPos::getClassTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType>
const std::string& Func_simplex<firstValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_simplex<" + firstValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType>
const TypeSpec& Func_simplex<firstValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType>
const TypeSpec& Func_simplex<firstValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType>
const TypeSpec& Func_simplex<firstValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Simplex::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** Function does not throw in general case */
template <typename valType>
bool Func_simplex<valType>::throws( void ) const {

    return false;
}


/** Function does throw occasionally in Simplex <- ( Integer ) case */
template <>
bool Func_simplex<Integer>::throws( void ) const {

    return true;
}

