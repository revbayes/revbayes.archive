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
#include <typeinfo>

class DAGNode;
class VectorString;

template <typename valType, typename retType>
class Func_vector :  public RbFunction {

    public:
        // Basic utility functions
        Func_vector*                clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        bool                        addAsChildOfArguments(void) { return false; }               //!< We do not wish that this function is added as a child of the arguments
    	RbLanguageObject*           execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


// Definition of the static type spec member
template <typename valType, typename retType>
const TypeSpec Func_vector<valType, retType>::typeSpec("Func_vector", new TypeSpec(valType().getType() + "," + retType().getType()));


/** Clone object */
template <typename valType, typename retType>
Func_vector<valType, retType>* Func_vector<valType, retType>::clone( void ) const {

    return new Func_vector( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType, typename retType>
RbLanguageObject* Func_vector<valType, retType>::execute( void ) {

    retType* tempVec = new retType();
    for ( size_t i = 0; i < args.size(); i++ )
        tempVec->push_back( *( static_cast<const valType*>( args[i].getValue() ) ) );

    return tempVec;
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func_vector<valType, retType>::getArgumentRules( void ) const {

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
const VectorString& Func_vector<valType, retType>::getClass( void ) const {

    static std::string  rbName  = "Func_vector<" + valType().getType() + "," + retType().getType() + ">"; 
    static VectorString rbClass = VectorString( rbName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec Func_vector<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec& Func_vector<valType, retType>::getTypeSpec( void ) const {
    
    return typeSpec;
}

