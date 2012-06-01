/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__gt, which is used to determine
 * whether the first variable is greater than the second.
 *
 * @brief Declaration and implementof Func__gt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__gt_H
#define Func__gt_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <map>
#include <string>

class DAGNode;

template <typename firstValType, typename secondValType>
class Func__gt :  public RbFunction {

    public:
                                    Func__gt( void );
    
        // Basic utility functions
        Func__gt*                   clone(void) const;                                          //!< Clone the object
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

#include "ArgumentRule.h"
#include "RbBoolean.h"
#include "DAGNode.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "TypeSpec.h"



/** default constructor */
template <typename firstValType, typename secondValType>
Func__gt<firstValType, secondValType>::Func__gt( void ) : RbFunction( ) {

}


/** Clone object */
template <typename firstValType, typename secondValType>
Func__gt<firstValType, secondValType>* Func__gt<firstValType, secondValType>::clone( void ) const {

    return new Func__gt( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
RbPtr<RbLanguageObject> Func__gt<firstValType,secondValType>::executeFunction(const std::vector<const RbObject *> &args) {

    const firstValType&  val1 = static_cast<const firstValType&> ( *args[0]  );
    const secondValType& val2 = static_cast<const secondValType&>( *args[1] );
    
    return RbPtr<RbLanguageObject>( new RbBoolean( val1 > val2 ) );
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__gt<firstValType, secondValType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {
        argumentRules.push_back( new ArgumentRule( "first",  true, firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "second", true, secondValType().getTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType>
const std::string& Func__gt<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__gt<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__gt<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__gt<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__gt<firstValType, secondValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}

