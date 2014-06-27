/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_uminus, which is used to take the negative value of a variable.
 *
 * @brief Declaration and implementation of Func_add
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef Func_uminus_H
#define Func_uminus_H

#include "Function.h"

#include <map>
#include <string>

namespace RevLanguage {

template <typename firstValType, typename retType>
class Func_uminus :  public Function {
    
public:
    Func_uminus( void );
    
    // Basic utility functions
    Func_uminus*                                    clone(void) const;                                                              //!< Clone the object
    static const std::string&                       getClassName(void);                                                             //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
    const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
    
    // Function functions you have to override
    RevObject*                                      execute(void);                                                                  //!< Execute function
    const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
    const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
    
private:
    
};
    
}

#include "DeterministicNode.h"
#include "TypedDagNode.h"
#include "UnaryMinus.h"

/** default constructor */
template <typename firstValType, typename retType>
RevLanguage::Func_uminus<firstValType, retType>::Func_uminus( void ) : Function( ) {
    
}


/** Clone object */
template <typename firstValType, typename retType>
RevLanguage::Func_uminus<firstValType, retType>* RevLanguage::Func_uminus<firstValType, retType>::clone( void ) const {
    
    return new Func_uminus( *this );
}


template <typename firstValType, typename retType>
RevLanguage::RevObject* RevLanguage::Func_uminus<firstValType, retType>::execute() {
    
    RevBayesCore::TypedDagNode<typename firstValType::valueType>* firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getValue() ).getValueNode();
    RevBayesCore::UnaryMinus<typename firstValType::valueType> *func = new RevBayesCore::UnaryMinus<typename firstValType::valueType>(firstArg);
    RevBayesCore::DeterministicNode<typename retType::valueType> *detNode = new RevBayesCore::DeterministicNode<typename retType::valueType>("", func);
    retType* value = new retType( detNode );
    
    return value;
}


/* Get argument rules */
template <typename firstValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func_uminus<firstValType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "value", true, firstValType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename retType>
const std::string& RevLanguage::Func_uminus<firstValType, retType>::getClassName(void) { 
    
    static std::string rbClassName = "Unary Minus<" + firstValType::getClassName() + "," + retType::getClassName() + ">";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename firstValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_uminus<firstValType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
template <typename firstValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_uminus<firstValType, retType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = retType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename firstValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_uminus<firstValType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
