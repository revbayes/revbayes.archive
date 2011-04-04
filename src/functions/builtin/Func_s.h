/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_s, which is used to create
 * simplex objects.
 *
 * @brief Declaration and implementation of Func_s
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_s_H
#define Func_s_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename valType>
class Func_s :  public RbFunction {

    public:
        // Basic utility functions
        Func_s*                     clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	DAGNode*                    executeFunction(void);                                      //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
        bool                        throws(void) const;                                         //!< One variant needs to throw

};

#endif


#include "Ellipsis.h"
#include "Integer.h"
#include "MemberNode.h"
#include "RbNames.h"
#include "RealPos.h"
#include "Simplex.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


/** Clone object */
template <typename valType>
Func_s<valType>* Func_s<valType>::clone( void ) const {

    return new Func_s( *this );
}


/** Execute function: Simplex <- ( Integer ) */
template <>
DAGNode* Func_s<Integer>::executeFunction( void ) {

    int size = static_cast<const Integer*>( args[0]->getValue() )->getValue();

    if ( size < 2 )
        throw RbException( "Simplex size must be at least 2" );

    Simplex* temp = new Simplex( *( ) );
    return new MemberNode( temp );
}


/** Execute function: Simplex <- ( VectorRealPos ) */
template <>
DAGNode* Func_s<VectorRealPos>::executeFunction( void ) {

    const VectorRealPos* tempVec = static_cast<const VectorRealPos*>( args[0]->getValue() );

    Simplex* temp = new Simplex( *tempVec );

    return new MemberNode( temp );
}


/** Execute function: Simplex <- ( RealPos, RealPos, ... ) */
template <>
DAGNode* Func_s<RealPos>::executeFunction( void ) {

    VectorReal  tempVec;
    for ( size_t i = 0; i < args.size(); i++ )
        tempVec.push_back( static_cast<const RealPos*>( args[i].getValue() )->clone() );

    // Normalization is done by the Simplex constructor
    return new MemberNode( new Simplex( tempVec ) );
}


/** Get argument rules for general case */
template <typename valType>
const ArgumentRules& Func_s<valType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get argument rules for: Simplex <- ( RealPos, RealPos, ... ) */
template <>
const ArgumentRules& Func_s<RealPos>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", RealPos_name ) );
        argumentRules.push_back( new ValueRule( "", RealPos_name ) );
        argumentRules.push_back( new Ellipsis (     RealPos_name ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename valType>
const VectorString& Func_s<valType>::getClass( void ) const {

    static std::string  rbName  = "Func_s<" + valType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType>
const TypeSpec Func_s<valType>::getReturnType( void ) const {

    return TypeSpec( Simplex_name );
}


/** Function does not throw in general case */
template <typename valType>
bool Func_s<valType>::throws( void ) const {

    return false;
}


/** Function does throw occasionally in Simplex <- ( Integer ) case */
template <>
bool Func_s<Integer>::throws( void ) const {

    return true;
}

