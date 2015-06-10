/**
 * @file
 * This file contains the declaration and implementation of the RevLanguage phyloDiversity function
 * which calculates the sum of all branches on the tree needed to connect the sampled taxa.
 *
 * @brief Declaration and implementation of Func_phyloDiversity
 *
 * (c) Copyright 2009- under GPL version 3
 * @author Will Freyman
 * @license GPL version 3
 * @version 1.0
 *
 */


#ifndef Func_phyloDiversity_H
#define Func_phyloDiversity_H


#include "RealPos.h"
#include "RlTypedFunction.h"


namespace RevLanguage {
    
    template <class treeType>
    class Func_phyloDiversity : public TypedFunction<RealPos> {
        
    public:
        Func_phyloDiversity( void );
        
        // Basic utility functions
        Func_phyloDiversity*                                                clone(void) const;                                                              //!< Clone the object
        static const std::string&                                           getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                              getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                     getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<double>*                                createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                                getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}


#include "Clade.h"
#include "Func_phyloDiversity.h"
#include "PhyloDiversityFunction.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlTypedFunction.h"


using namespace RevLanguage;


template <class treeType>
Func_phyloDiversity<treeType>::Func_phyloDiversity( void ) : TypedFunction<RealPos>( ) {
    
}


template <class treeType>
Func_phyloDiversity<treeType>* Func_phyloDiversity<treeType>::clone( void ) const {
    
    return new Func_phyloDiversity<treeType>( *this );
}


template <class treeType>
RevBayesCore::TypedFunction<double>* Func_phyloDiversity<treeType>::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>(  this->args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::Clade& sample                             = static_cast<const Clade &>(     this->args[1].getVariable()->getRevObject() ).getValue();
    bool includeRoot                                              = static_cast<const RlBoolean &>( this->args[2].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::PhyloDiversityFunction<typename treeType::valueType>* f = new RevBayesCore::PhyloDiversityFunction<typename treeType::valueType>( tau, sample, includeRoot );
    return f;
}


template <class treeType>
const ArgumentRules& Func_phyloDiversity<treeType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree",        treeType::getClassTypeSpec(),  ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "sample",      Clade::getClassTypeSpec(),     ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "includeRoot", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


template <class treeType>
const std::string& Func_phyloDiversity<treeType>::getClassType(void) {
    
    static std::string revType = "Func_phyloDiversity";
    
    return revType;
}


template <class treeType>
const TypeSpec& Func_phyloDiversity<treeType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


template <class treeType>
const TypeSpec& Func_phyloDiversity<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif
