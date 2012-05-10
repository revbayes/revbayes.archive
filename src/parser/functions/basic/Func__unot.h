/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__unot, which is used to perform
 * the unary RbBoolean not (!) operation.
 *
 * @brief Declaration and implementation of Func__unot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__unot_H
#define Func__unot_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <string>

class DAGNode;

template <typename valType>
class Func__unot :  public RbFunction {

    public:
                                    Func__unot( void );
    
        // Basic utility functions
        Func__unot*                 clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);   //!< Execute function

};

#endif

#include "ConstArgumentRule.h"
#include "RbBoolean.h"
#include "DAGNode.h"
#include "RbUtil.h"
#include "TypeSpec.h"



/** default constructor */
template <typename valType>
Func__unot<valType>::Func__unot( void ) : RbFunction( ) {

}


/** Clone object */
template <typename valType>
Func__unot<valType>* Func__unot<valType>::clone( void ) const {

    return new Func__unot( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType>
RbPtr<RbLanguageObject> Func__unot<valType>::executeFunction(const std::vector<const RbObject *> &args) {

    const valType& val = static_cast<const valType&> ( *args[0] );

    return RbPtr<RbLanguageObject>( new RbBoolean( !val ) );
}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func__unot<valType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ConstArgumentRule( "value", valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType>
const std::string& Func__unot<firstValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__unot<" + firstValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType>
const TypeSpec& Func__unot<firstValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType>
const TypeSpec& Func__unot<firstValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType>
const TypeSpec& Func__unot<firstValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}

