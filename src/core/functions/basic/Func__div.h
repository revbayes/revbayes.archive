/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__div, which is used to divide
 * two variables.
 *
 * @brief Declaration and implementation of Func__div
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id:$
 */

#ifndef Func__div_H
#define Func__div_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType, typename retType>
class Func__div :  public RbFunction {

    public:
        // Basic utility functions
        Func__div*                  clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	DAGNode*                    execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

};

#endif

#include "ContainerNode.h"
#include "DAGNode.h"
#include "Integer.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "TypeSpec.h"
#include "ValueContainer.h"
#include "ValueRule.h"
#include "VectorString.h"


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
Func__div<firstValType, secondValType, retType>* Func__div<firstValType, secondValType, retType>::clone( void ) const {

    return new Func__div( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename firstValType, typename secondValType, typename retType>
DAGNode* Func__div<firstValType,secondValType,retType>::execute( void ) {

    const firstValType*  val1 = static_cast<const firstValType*> ( args[0].getValue() );
    const secondValType* val2 = static_cast<const secondValType*>( args[1].getValue() );
    retType              quot = *val1 / *val2;
    
    return quot.clone()->wrapIntoVariable();
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const ArgumentRules& Func__div<firstValType, secondValType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "", secondValType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const VectorString& Func__div<firstValType, secondValType, retType>::getClass(void) const {

    static std::string  rbName  = "Func__div<" + firstValType().getType() + "," + secondValType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec Func__div<firstValType, secondValType, retType>::getReturnType(void) const {

    return retType().getTypeSpec();
}

