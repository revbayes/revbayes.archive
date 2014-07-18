/**
 * @file
 * This file contains the declaration of Func_fnNormalizedQuantile,
 * which returns a vector of normalized gamma rates for n categories
 * for among-site rate variation
 *
 * @brief Declaration of Func_fnNormalizedQuantile
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Func_fnNormalizedQuantile_H
#define Func_fnNormalizedQuantile_H

#include "RlFunction.h"
#include <map>
#include <string>
#include "TypedDagNode.h"

namespace RevLanguage {
    
    template <typename valType>
    class Func_fnNormalizedQuantile :  public Function {
        
    public:
        Func_fnNormalizedQuantile();
        
        // Basic utility functions
        Func_fnNormalizedQuantile*             clone(void) const;                                          //!< Clone the object
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
#include "Ellipsis.h"
#include "NormalizeVectorFunction.h"
#include "RbUtil.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "QuantileFunction.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "GammaDistribution.h"
#include "VectorFunction.h"
#include "TypedDistribution.h"
#include "ContinuousDistribution.h"
#include "RlContinuousDistribution.h"
#include "RlTypedDistribution.h"

using namespace RevLanguage;

template <typename valType>
Func_fnNormalizedQuantile<valType>::Func_fnNormalizedQuantile() : Function() {
    
}

/* Clone object */
template <typename valType>
Func_fnNormalizedQuantile<valType>* Func_fnNormalizedQuantile<valType>::clone( void ) const {
    
    return new Func_fnNormalizedQuantile( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType>
RevObject* Func_fnNormalizedQuantile<valType>::execute( void ) {
    
    const ContinuousDistribution& rlDistribution    = static_cast<const ContinuousDistribution &>( args[0].getVariable()->getRevObject() );
    RevBayesCore::ContinuousDistribution* dist     = static_cast<RevBayesCore::ContinuousDistribution* >( rlDistribution.createDistribution() );    
    
    int nc = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getValue();
    
    double binWidth = 1.0 / ((double)nc);
    double binMid = binWidth * 0.5;
    std::vector<const RevBayesCore::TypedDagNode<double> *> gRates = std::vector<const RevBayesCore::TypedDagNode<double>* >();
    for(int i=1; i<=nc; i++){
        RevBayesCore::ConstantNode<double> *quant = new RevBayesCore::ConstantNode<double>("", new double((double)i * binWidth - binMid));
        RevBayesCore::DeterministicNode<double> *tmp = new RevBayesCore::DeterministicNode<double>("", new RevBayesCore::QuantileFunction(quant,  dist));
		gRates.push_back( tmp );
    }
    RevBayesCore::DeterministicNode<std::vector<double> > *discRates = new RevBayesCore::DeterministicNode<std::vector<double> >( "", new RevBayesCore::VectorFunction<double>(gRates) );
    RevBayesCore::NormalizeVectorFunction *func = new RevBayesCore::NormalizeVectorFunction( discRates );
    
    DeterministicNode<std::vector<double> > *detNode = new DeterministicNode<std::vector<double> >("", func, this->clone());
    
    Vector<valType> *theNormalizedVector = new Vector<valType>( detNode );
    
    return theNormalizedVector;
}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func_fnNormalizedQuantile<valType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "ContDstribution", true, TypedDistribution<valType>::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "numCategories", true, Integer::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
template <typename valType>
const std::string& Func_fnNormalizedQuantile<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_fnNormalizedQuantile";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& Func_fnNormalizedQuantile<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename valType>
const RevLanguage::TypeSpec& Func_fnNormalizedQuantile<valType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType>
const RevLanguage::TypeSpec& Func_fnNormalizedQuantile<valType>::getReturnType( void ) const {
    
    return Vector<valType>::getClassTypeSpec();
}


#endif


