/**
 * @file
 * This file contains the declaration of the RevLanguage gtr function, which
 * is used to created deterministic variable associated with the gtr function.
 *
 * @brief Declaration and implementation of Func_gtr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_dppNumTablesStatistic_H
#define Func_dppNumTablesStatistic_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType>
    class Func_dppNumTablesStatistic :  public Function {
        
    public:
        Func_dppNumTablesStatistic( void );
        
        // Basic utility functions
        Func_dppNumTablesStatistic*                     clone(void) const;                                                              //!< Clone the object
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
#include "DppNumTablesStatistic.h"
#include "RealPos.h"
#include "Integer.h"
#include "TypedDagNode.h"
#include "Vector.h"

//using namespace RevLanguage;

/** default constructor */
template <typename valType>
RevLanguage::Func_dppNumTablesStatistic<valType>::Func_dppNumTablesStatistic( void ) : Function( ) {
    
}


/** Clone object */
template <typename valType>
RevLanguage::Func_dppNumTablesStatistic<valType>* RevLanguage::Func_dppNumTablesStatistic<valType>::clone( void ) const {
    
    return new RevLanguage::Func_dppNumTablesStatistic<valType>( *this );
}

template <typename valType>
RevLanguage::RevObject* RevLanguage::Func_dppNumTablesStatistic<valType>::execute() {
    
    RevBayesCore::TypedDagNode< std::vector<typename valType::valueType> >* dppVec = static_cast<const std::vector<typename valType::valueType> >( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::DppNumTablesStatistic<typename valType::valueType>* f = new RevBayesCore::DppNumTablesStatistic<typename valType::valueType>( dppVec );
    RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
    
    RealPos* value = new RealPos( detNode );
    
    return value;
}


/* Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_dppNumTablesStatistic<valType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "dppVector", true, Vector<valType>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename valType>
const std::string& RevLanguage::Func_dppNumTablesStatistic<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_dppNumTablesStatistic";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_dppNumTablesStatistic<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_dppNumTablesStatistic<valType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_dppNumTablesStatistic<valType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
