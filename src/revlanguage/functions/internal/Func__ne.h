/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_ne, which is used to to compare to values ( a > b).
 *
 * @brief Declaration and implementation of Func__ne
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-12 10:25:58 +0200 (Tue, 12 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_vector.h 1626 2012-06-12 08:25:58Z hoehna $
 */

#ifndef Func__ne_H
#define Func__ne_H

#include "RlBoolean.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    template <typename leftValType, typename rightValType>
    class Func__ne : public TypedFunction<RlBoolean> {
        
    public:
        Func__ne();
        
        // Basic utility functions
        Func__ne*                               clone(void) const;                                          //!< Clone the object
        static const std::string&               getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction<bool>*      createFunction(void) const;                                 //!< Create a function object
        const ArgumentRules&                    getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}


#include "ArgumentRule.h"
#include "NotEqualFunction.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"



template <typename leftValType, typename rightValType>
RevLanguage::Func__ne<leftValType,rightValType>::Func__ne() : TypedFunction<RlBoolean>() {
    
}

/* Clone object */
template <typename leftValType, typename rightValType>
RevLanguage::Func__ne<leftValType,rightValType>* RevLanguage::Func__ne<leftValType,rightValType>::clone( void ) const {
    
    return new Func__ne<leftValType,rightValType>( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename leftValType, typename rightValType>
RevBayesCore::TypedFunction<bool>* RevLanguage::Func__ne<leftValType,rightValType>::createFunction( void ) const
{
    
    const RevBayesCore::TypedDagNode<typename leftValType::valueType>* leftVal = static_cast<const leftValType &>( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::TypedDagNode<typename rightValType::valueType>* rightVal = static_cast<const rightValType &>( args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::NotEqualFunction<typename leftValType::valueType, typename rightValType::valueType> *func = new RevBayesCore::NotEqualFunction<typename leftValType::valueType, typename rightValType::valueType>( leftVal, rightVal );

    return func;
}


/** Get argument rules */
template <typename leftValType, typename rightValType>
const RevLanguage::ArgumentRules& RevLanguage::Func__ne<leftValType,rightValType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "", leftValType::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "", rightValType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename leftValType, typename rightValType>
const std::string& RevLanguage::Func__ne<leftValType,rightValType>::getClassType(void) { 
    
    static std::string revType = "Func__ne<" + leftValType::getClassType() + "," + rightValType::getClassType() + ">";
    
	return revType; 
}


/** Get class type spec describing type of object */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__ne<leftValType,rightValType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__ne<leftValType,rightValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif


