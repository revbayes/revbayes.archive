/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__uminus, which is used to take the negative value of a variable.
 *
 * @brief Declaration and implementation of Func__add
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef Func_uminus_H
#define Func_uminus_H

#include "TypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {

template <typename firstValType, typename retType>
class Func__uminus : public TypedFunction<retType> {
    
public:
    Func__uminus( void );
    
    // Basic utility functions
    Func__uminus*                                   clone(void) const;                                                              //!< Clone the object
    static const std::string&                       getClassType(void);                                                             //!< Get Rev type
    static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
    const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
    
    // Function functions you have to override
    RevBayesCore::TypedFunction<typename retType::valueType>*     createFunction(void) const;                                       //!< Create a function object
    const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
    
};
    
}

#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "UnaryMinus.h"

/** default constructor */
template <typename firstValType, typename retType>
RevLanguage::Func__uminus<firstValType, retType>::Func__uminus( void ) : TypedFunction<retType>( ) {
    
}


/** Clone object */
template <typename firstValType, typename retType>
RevLanguage::Func__uminus<firstValType, retType>* RevLanguage::Func__uminus<firstValType, retType>::clone( void ) const {
    
    return new Func__uminus( *this );
}


template <typename firstValType, typename retType>
RevBayesCore::TypedFunction<typename retType::valueType>* RevLanguage::Func__uminus<firstValType, retType>::createFunction(void) const
{
    
    RevBayesCore::TypedDagNode<typename firstValType::valueType>* firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::UnaryMinus<typename firstValType::valueType> *func = new RevBayesCore::UnaryMinus<typename firstValType::valueType>(firstArg);

    return func;
}


/* Get argument rules */
template <typename firstValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func__uminus<firstValType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "value", firstValType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename retType>
const std::string& RevLanguage::Func__uminus<firstValType, retType>::getClassType(void) { 
    
    static std::string revType = "Func__uminus<" + firstValType::getClassType() + "," + retType::getClassType() + ">";
    
	return revType; 
}

/* Get class type spec describing type of object */
template <typename firstValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__uminus<firstValType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


template <typename firstValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__uminus<firstValType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
