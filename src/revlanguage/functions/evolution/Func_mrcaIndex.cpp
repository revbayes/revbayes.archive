//
//  Func_mrcaIndex.cpp
//  RevLanguage
//
//  Created by Michael Landis on 8/19/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "Func_mrcaIndex.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "MrcaIndexStatistic.h"
#include "Topology.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mrcaIndex::Func_mrcaIndex( void ) : Function( ) {
    
}


/** Clone object */
Func_mrcaIndex* Func_mrcaIndex::clone( void ) const {
    
    return new Func_mrcaIndex( *this );
}


RevPtr<Variable> Func_mrcaIndex::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::Clade& c = static_cast<const Clade &>( this->args[1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::MrcaIndexStatistic* f = new RevBayesCore::MrcaIndexStatistic( tau, c );
    
    DeterministicNode<int> *detNode = new DeterministicNode<int>("", f, this->clone());
    
    Natural* value = new Natural( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_mrcaIndex::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()   , ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_mrcaIndex::getClassType(void) {
    
    static std::string revType = "Func_mrcaIndex";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_mrcaIndex::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_mrcaIndex::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_mrcaIndex::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
