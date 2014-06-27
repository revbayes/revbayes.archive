/**
 * @file
 * This file contains the declaration of the RevLanguage round function, which
 * is used to created deterministic variable associated with the round function.
 * This is the standard arithmetic rounding of real numbers.
 *
 * @brief Declaration and implementation of Func_exp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_round_H
#define Func_round_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType, typename retType>
    class Func_round :  public Function {
        
    public:
        Func_round( void );
        
        // Basic utility functions
        Func_round*                                     clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}


#include "DeterministicNode.h"
#include "RoundFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename valType, typename retType>
RevLanguage::Func_round<valType, retType>::Func_round( void ) : Function( ) {
    
}


/** Clone object */
template <typename valType, typename retType>
RevLanguage::Func_round<valType, retType>* RevLanguage::Func_round<valType, retType>::clone( void ) const {
    
    return new Func_round<valType, retType>( *this );
}


template <typename valType, typename retType>
RevLanguage::RevObject* RevLanguage::Func_round<valType, retType>::execute() {
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const valType &>( this->args[0].getVariable()->getValue() ).getValueNode();
    RevBayesCore::RoundFunction* f = new RevBayesCore::RoundFunction( arg );
    RevBayesCore::DeterministicNode<int> *detNode = new RevBayesCore::DeterministicNode<int>("", f);
    
    retType* value = new retType( detNode );
    
    return value;
}


/* Get argument rules */
template <typename valType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func_round<valType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "x", true, valType::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename valType, typename retType>
const std::string& RevLanguage::Func_round<valType, retType>::getClassName(void) { 
    
    static std::string rbClassName = "round";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_round<valType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( valType::getClassTypeSpec() ), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_round<valType, retType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = retType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_round<valType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif
