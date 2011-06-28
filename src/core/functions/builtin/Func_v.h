/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_v, which is used to create
 * value vectors.
 *
 * @brief Declaration and implementation of Func_v
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id:$
 */

#ifndef Func_v_H
#define Func_v_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename valType, typename retType>
class Func_v :  public RbFunction {

    public:
        // Basic utility functions
        Func_v*                     clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	DAGNode*                    execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

};

#endif

#include "ContainerNode.h"
#include "Ellipsis.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


/** Clone object */
template <typename valType, typename retType>
Func_v<valType, retType>* Func_v<valType, retType>::clone( void ) const {

    return new Func_v( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType, typename retType>
DAGNode* Func_v<valType, retType>::execute( void ) {

    retType*    tempVec = new retType();
    for ( size_t i = 0; i < args.size(); i++ )
        tempVec->push_back( static_cast<const valType*>( args[i].getValue() )->clone() );

    return tempVec->wrapIntoVariable();
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func_v<valType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", valType().getTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename valType, typename retType>
const VectorString& Func_v<valType, retType>::getClass( void ) const {

    static std::string  rbName  = "Func_v<" + valType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec Func_v<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}

