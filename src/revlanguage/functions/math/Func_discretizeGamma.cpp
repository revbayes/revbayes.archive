//
//  Func_discretizeGamma.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_discretizeGamma.h"
#include "NormalizeVectorFunction.h"
#include "ModelVector.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

#include "DiscretizeGammaFunction.h"


using namespace RevLanguage;

Func_discretizeGamma::Func_discretizeGamma() : Function() {
    
}

/* Clone object */
Func_discretizeGamma* Func_discretizeGamma::clone( void ) const {
    
    return new Func_discretizeGamma( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RevPtr<Variable> Func_discretizeGamma::execute( void ) {
    
    RevBayesCore::TypedDagNode<double>* shape = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* rate = static_cast<const Real &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int>* numCats = static_cast<const Integer &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    bool median = static_cast<const RlBoolean &>( args[3].getVariable()->getRevObject() ).getValue();

    RevBayesCore::DiscretizeGammaFunction *func = new RevBayesCore::DiscretizeGammaFunction( shape, rate, numCats, median );
    
    DeterministicNode<std::vector<double> > *detNode = new DeterministicNode<std::vector<double> >("", func, this->clone());
    
    ModelVector<RealPos> *discGammaVector = new ModelVector<RealPos>( detNode );
    
    return new Variable( discGammaVector );
}


/** Get argument rules */
const ArgumentRules& Func_discretizeGamma::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "shape", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "rate", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "num_cats", true, Integer::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "median", true, RlBoolean::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_discretizeGamma::getClassName(void) { 
    
    static std::string rbClassName = "Func_discretizeGamma";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_discretizeGamma::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func_discretizeGamma::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_discretizeGamma::getReturnType( void ) const {
    
    return ModelVector<RealPos>::getClassTypeSpec();
}
