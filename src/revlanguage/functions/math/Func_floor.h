/**
 * @file
 * This file contains the declaration of the RevLanguage floor function, which
 * is used to created deterministic variable associated with the floor function.
 * This is the standard arithmetic flooring of real numbers.
 *
 * @brief Declaration and implementation of Func_exp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_floor_H
#define Func_floor_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType, typename retType>
    class Func_floor :  public Function {
        
    public:
        Func_floor( void );
        
        // Basic utility functions
        Func_floor*                                     clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevPtr<Variable>                                execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}


#include "FloorFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename valType, typename retType>
RevLanguage::Func_floor<valType, retType>::Func_floor( void ) : Function( ) {
    
}


/** Clone object */
template <typename valType, typename retType>
RevLanguage::Func_floor<valType, retType>* RevLanguage::Func_floor<valType, retType>::clone( void ) const {
    
    return new Func_floor<valType, retType>( *this );
}


template <typename valType, typename retType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Func_floor<valType, retType>::execute() {
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const valType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::FloorFunction* f = new RevBayesCore::FloorFunction( arg );
    
    DeterministicNode<int> *detNode = new DeterministicNode<int>("", f, this->clone());
    
    retType* value = new retType( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
template <typename valType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func_floor<valType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "x", valType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename valType, typename retType>
const std::string& RevLanguage::Func_floor<valType, retType>::getClassType(void) { 

    static std::string revType = "Func_floor<" + valType::getClassType() + "," + retType::getClassType() + ">";
    
	return revType; 
}

/* Get class type spec describing type of object */
template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_floor<valType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_floor<valType, retType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = retType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_floor<valType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif
