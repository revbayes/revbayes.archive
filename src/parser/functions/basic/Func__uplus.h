/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__uplus, which is used to perform
 * the unary plus operation.
 *
 * @brief Declaration and implementation of Func__uplus
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__uplus_H
#define Func__uplus_H

#include "RbFunction.h"

#include <string>

class DAGNode;

template <typename valType, typename retType>
class Func__uplus :  public RbFunction {

public:
                                    Func__uplus( void );
    
        // Basic utility functions
        Func__uplus*                clone(void) const;                                          //!< Clone the object
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
#include "RbLanguageObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"



/** default constructor */
template <typename firstValType, typename retType>
Func__uplus<firstValType, retType>::Func__uplus( void ) : RbFunction( ) {

}


/** Clone object */
template <typename valType, typename retType>
Func__uplus<valType, retType>* Func__uplus<valType, retType>::clone( void ) const {

    return new Func__uplus( *this );
}


/** Execute function: We simply return a copy of the value */
template <typename valType, typename retType>
RbPTr<RbLanguageObject> Func__uplus<valType, retType>::executeFunction(const std::vector<const RbObject *> &args) {

    const valType& val = static_cast<const valType&> ( *args[0] );

    return RbPtr<RbLanguageObject>( val.clone() );
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func__uplus<valType, retType>::getArgumentRules( void ) const {

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
template <typename firstValType, typename secondValType>
const std::string& Func__uplus<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__uplus<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uplus<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uplus<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec& Func__uplus<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}

