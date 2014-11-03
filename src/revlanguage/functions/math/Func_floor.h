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

#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType, typename retType>
    class Func_floor : public TypedFunction<retType> {
        
    public:
        Func_floor( void );
        
        // Basic utility functions
        Func_floor*                                                             clone(void) const;                                          //!< Clone the object
        static const std::string&                                               getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                                                  getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&                                                         getTypeSpec(void) const;                                    //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< typename retType::valueType>*              createFunction(void) const;                                 //!< Create a function object
        const ArgumentRules&                                                    getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}


#include "FloorFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename valType, typename retType>
RevLanguage::Func_floor<valType, retType>::Func_floor( void ) : TypedFunction<retType>( ) {
    
}


/** Clone object */
template <typename valType, typename retType>
RevLanguage::Func_floor<valType, retType>* RevLanguage::Func_floor<valType, retType>::clone( void ) const {
    
    return new Func_floor<valType, retType>( *this );
}


template <typename valType, typename retType>
RevBayesCore::TypedFunction< typename retType::valueType >* RevLanguage::Func_floor<valType, retType>::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const valType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::FloorFunction* f = new RevBayesCore::FloorFunction( arg );
    
    return f;
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


template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_floor<valType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif
