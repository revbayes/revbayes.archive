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
class VectorString;

template <typename valType>
class Func_simplex :  public RbFunction {

    public:
        // Basic utility functions
        Func_simplex*               clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
    	RbPtr<RbLanguageObject>     execute(void);                                              //!< Execute function
        RbPtr<const ArgumentRules>  getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        bool                        throws(void) const;                                         //!< One variant needs to throw
    
    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;
};

#endif


#include "Ellipsis.h"
#include "Integer.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "Simplex.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


// Definition of the static type spec member
template <typename valType>
const TypeSpec Func_simplex<valType>::typeSpec("Func_simplex", new TypeSpec(valType().getType()));
template <typename valType>
const TypeSpec Func_simplex<valType>::returnTypeSpec(Simplex_name);


/** Clone object */
template <typename valType>
Func_simplex<valType>* Func_simplex<valType>::clone( void ) const {

    return new Func_simplex( *this );
}


/** Execute function: Simplex <- ( Integer ) */
template <>
RbPtr<RbLanguageObject> Func_simplex<Integer>::execute( void ) {

    int size = static_cast<const Integer*>( (const RbLanguageObject*)(*args)[0]->getValue() )->getValue();

    if ( size < 2 )
        throw RbException( "Simplex size must be at least 2" );

    RbPtr<RbLanguageObject> temp( new Simplex( size ) );
    return temp;
}


/** Execute function: Simplex <- ( VectorRealPos ) */
template <>
RbPtr<RbLanguageObject> Func_simplex<VectorRealPos>::execute( void ) {

    const RbPtr<VectorRealPos> tempVec( static_cast<VectorRealPos*>( (RbLanguageObject*)(*args)[0]->getValue() ) );

    RbPtr<RbLanguageObject> temp( new Simplex( *tempVec ) );

    return temp;
}


/** Execute function: Simplex <- ( RealPos, RealPos, ... ) */
template <>
RbPtr<RbLanguageObject> Func_simplex<RealPos>::execute( void ) {

    VectorReal  tempVec;
    for ( size_t i = 0; i < args->size(); i++ )
        tempVec.push_back( *( static_cast<const RealPos*>( (const RbLanguageObject*)(*args)[i]->getValue() )->clone() ) );

    // Normalization is done by the Simplex constructor
    return RbPtr<RbLanguageObject>( new Simplex( tempVec ) );
}


/** Get argument rules for general case */
template <typename valType>
RbPtr<const ArgumentRules> Func_simplex<valType>::getArgumentRules( void ) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", valType().getTypeSpec() ) ) );
        rulesSet = true;
        }

    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get argument rules for: Simplex <- ( RealPos, RealPos, ... ) */
template <>
RbPtr<const ArgumentRules> Func_simplex<RealPos>::getArgumentRules( void ) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", RealPos_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", RealPos_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new Ellipsis (     RealPos_name ) ) );
        rulesSet = true;
        }

    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
template <typename valType>
const VectorString& Func_simplex<valType>::getClass( void ) const {

    static std::string  rbName  = "Func_simplex<" + valType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType>
const TypeSpec& Func_simplex<valType>::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get return type */
template <typename valType>
const TypeSpec& Func_simplex<valType>::getTypeSpec( void ) const {
    
    return typeSpec;
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

