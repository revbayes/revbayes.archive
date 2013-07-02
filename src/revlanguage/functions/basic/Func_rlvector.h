/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_vector, which is used to create
 * value vectors of RevLanguage objects only.
 *
 * @brief Declaration and implementation of Func_rlvector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-12 10:25:58 +0200 (Tue, 12 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_vector.h 1626 2012-06-12 08:25:58Z hoehna $
 */

#ifndef Func_rlvector_H
#define Func_rlvector_H

#include "Function.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    template <typename valType>
    class Func_rlvector :  public Function {
        
    public:
        Func_rlvector();
        
        // Basic utility functions
        Func_rlvector*              clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RbLanguageObject*           execute(void);                                              //!< Execute function
        
    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorRbPointer.h"



template <typename valType>
RevLanguage::Func_rlvector<valType>::Func_rlvector() : Function() {
    
}

/* Clone object */
template <typename valType>
RevLanguage::Func_rlvector<valType>* RevLanguage::Func_rlvector<valType>::clone( void ) const {
    
    return new Func_rlvector( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType>
RevLanguage::RbLanguageObject* RevLanguage::Func_rlvector<valType>::execute( void ) {
    
    std::vector<typename valType::valueType*> params;
    for ( size_t i = 0; i < args.size(); i++ ) {
        const valType &val = static_cast<const valType &>( args[i].getVariable()->getValue() );
        params.push_back( val.getValue().clone() );
    }
    
    VectorRbPointer<valType> *theVector = new VectorRbPointer<valType>( params );
        
    return theVector;
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_rlvector<valType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "", true, valType::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     valType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
template <typename valType>
const std::string& RevLanguage::Func_rlvector<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_rlvector<" + valType::getClassTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rlvector<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( "Func_rlvector", new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rlvector<valType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rlvector<valType>::getReturnType( void ) const {
    
    return Vector<valType>::getClassTypeSpec();
}


#endif


