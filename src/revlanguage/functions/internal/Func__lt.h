/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_lt, which is used to to compare to values ( a < b).
 *
 * @brief Declaration and implementation of Func__lt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-12 10:25:58 +0200 (Tue, 12 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_vector.h 1626 2012-06-12 08:25:58Z hoehna $
 */

#ifndef Func__lt_H
#define Func__lt_H

#include "Function.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    template <typename leftValType, typename rightValType>
    class Func__lt : public Function {
        
    public:
        Func__lt();
        
        // Basic utility functions
        Func__lt*                   clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevPtr<Variable>            execute(void);                                              //!< Execute function
        
    };
    
}


#include "ArgumentRule.h"
#include "LessThanFunction.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"



template <typename leftValType, typename rightValType>
RevLanguage::Func__lt<leftValType,rightValType>::Func__lt() : Function() {
    
}

/* Clone object */
template <typename leftValType, typename rightValType>
RevLanguage::Func__lt<leftValType,rightValType>* RevLanguage::Func__lt<leftValType,rightValType>::clone( void ) const {
    
    return new Func__lt<leftValType,rightValType>( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename leftValType, typename rightValType>
RevLanguage::RevPtr<Variable> RevLanguage::Func__lt<leftValType,rightValType>::execute( void ) {
    
    const RevBayesCore::TypedDagNode<typename leftValType::valueType>* leftVal = static_cast<const leftValType &>( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::TypedDagNode<typename rightValType::valueType>* rightVal = static_cast<const rightValType &>( args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::LessThanFunction<typename leftValType::valueType, typename rightValType::valueType> *func = new RevBayesCore::LessThanFunction<typename leftValType::valueType, typename rightValType::valueType>( leftVal, rightVal );

    DeterministicNode<bool> *detNode = new DeterministicNode<bool>("", func, this->clone());
    
    RlBoolean *theBool = new RlBoolean( detNode );
    
    return new Variable( theBool );
}


/** Get argument rules */
template <typename leftValType, typename rightValType>
const RevLanguage::ArgumentRules& RevLanguage::Func__lt<leftValType,rightValType>::getArgumentRules( void ) const {
    
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
const std::string& RevLanguage::Func__lt<leftValType,rightValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__lt<" + leftValType::getClassTypeSpec() + "," + rightValType::getClassTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__lt<leftValType,rightValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__lt<leftValType,rightValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename leftValType, typename rightValType>
const RevLanguage::TypeSpec& RevLanguage::Func__lt<leftValType,rightValType>::getReturnType( void ) const {
    
    return RlBoolean::getClassTypeSpec();
}


#endif


