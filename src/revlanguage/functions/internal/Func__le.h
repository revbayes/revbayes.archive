/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_le, which is used to to compare to values ( a <= b).
 *
 * @brief Declaration and implementation of Func__le
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-12 10:25:58 +0200 (Tue, 12 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_vector.h 1626 2012-06-12 08:25:58Z hoehna $
 */

#ifndef Func__le_H
#define Func__le_H

#include "Function.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    template <typename leftValType, typename rightValType>
    class Func__le : public Function {
        
    public:
        Func__le();
        
        // Basic utility functions
        Func__le*                   clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevObject*                  execute(void);                                              //!< Execute function
        
    };
    
}


#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "LessEqualFunction.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"



template <typename leftValType, typename rightValType>
RevLanguage::Func__le<leftValType,rightValType>::Func__le() : Function() {
    
}

/* Clone object */
template <typename leftValType, typename rightValType>
RevLanguage::Func__le<leftValType,rightValType>* RevLanguage::Func__le<leftValType,rightValType>::clone( void ) const {
    
    return new Func__le<leftValType,rightValType>( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename leftValType, typename rightValType>
RevLanguage::RevObject* RevLanguage::Func__le<leftValType,rightValType>::execute( void ) {
    
    const RevBayesCore::TypedDagNode<typename leftValType::valueType>* leftVal = static_cast<const leftValType &>( args[0].getVariable()->getValue() ).getValueNode();
    const RevBayesCore::TypedDagNode<typename rightValType::valueType>* rightVal = static_cast<const rightValType &>( args[1].getVariable()->getValue() ).getValueNode();
    
    RevBayesCore::LessEqualFunction<typename leftValType::valueType, typename rightValType::valueType> *func = new RevBayesCore::LessEqualFunction<typename leftValType::valueType, typename rightValType::valueType>( leftVal, rightVal );
    RevBayesCore::TypedDagNode<bool> *detNode = new RevBayesCore::DeterministicNode<bool>("", func);
    RlBoolean *theBool = new RlBoolean( detNode );
    
    return theBool;
    
}


/** Get argument rules */
template <typename leftValType, typename rightValType>
const RevLanguage::ArgumentRules& RevLanguage::Func__le<leftValType,rightValType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "", true, leftValType::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "", true, rightValType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
template <typename leftValType, typename rightValType>
const std::string& RevLanguage::Func__le<leftValType,rightValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__le<" + leftValType::getClassTypeSpec() + "," + rightValType::getClassTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__le<leftValType,rightValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( "Func__le", new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__le<leftValType,rightValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__le<leftValType,rightValType>::getReturnType( void ) const {
    
    return RlBoolean::getClassTypeSpec();
}


#endif


