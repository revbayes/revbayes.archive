//
//  Func_constructRootedTripletDistribution.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_constructRootedTripletDistribution.h"
#include "ModelVector.h"
#include "RealPos.h"
#include "RlBranchLengthTree.h" 
#include "RlDeterministicNode.h"
#include "RlRootedTripletDistribution.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTaxon.h"
#include "RootedTripletDistributionFunction.h"
#include "Topology.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_constructRootedTripletDistribution::Func_constructRootedTripletDistribution( void ) : Function( ) {
    
}


/** Clone object */
Func_constructRootedTripletDistribution* Func_constructRootedTripletDistribution::clone( void ) const {
    
    return new Func_constructRootedTripletDistribution( *this );
}


RevPtr<Variable> Func_constructRootedTripletDistribution::execute() {
    
    RevBayesCore::RootedTripletDistributionFunction* f ;
    if ( this->args[1].getVariable()->getRevObjectTypeSpec().isDerivedOf( ModelVector< RlString >::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<std::vector< std::string > >* sn = static_cast<const ModelVector< RlString > &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
        f = new RevBayesCore::RootedTripletDistributionFunction( sn );
    }
    else if ( this->args[1].getVariable()->getRevObjectTypeSpec().isDerivedOf( ModelVector< Taxon >::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<std::vector< RevBayesCore::Taxon > >* t = static_cast<const ModelVector< Taxon > &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
        f = new RevBayesCore::RootedTripletDistributionFunction( t );
    }
    
    if ( this->args[0].getVariable()->getRevObjectTypeSpec().isDerivedOf( ModelVector< TimeTree >::getClassTypeSpec() ) )
    {
       RevBayesCore::TypedDagNode<std::vector< RevBayesCore::TimeTree > >* gTrees = static_cast<const ModelVector< TimeTree > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
        f->setTrees(gTrees);
    }
    else if ( this->args[0].getVariable()->getRevObjectTypeSpec().isDerivedOf( ModelVector< BranchLengthTree >::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<std::vector< RevBayesCore::BranchLengthTree > >* gTrees = static_cast<const ModelVector< BranchLengthTree > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
        f->setTrees(gTrees);
    }

    
    DeterministicNode<RevBayesCore::RootedTripletDistribution> *detNode = new DeterministicNode<RevBayesCore::RootedTripletDistribution>("", f, this->clone());
    
    RootedTripletDistribution* value = new RootedTripletDistribution( detNode->getValue() );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_constructRootedTripletDistribution::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        std::vector<TypeSpec> treeTypes;
        treeTypes.push_back( ModelVector< TimeTree >::getClassTypeSpec() );
        treeTypes.push_back( ModelVector< BranchLengthTree >::getClassTypeSpec() );


        argumentRules.push_back( new ArgumentRule( "geneTrees", treeTypes , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "speciesNames", ModelVector< RlString >::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_constructRootedTripletDistribution::getClassType(void) {
    
    static std::string revType = "Func_constructRootedTripletDistribution";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_constructRootedTripletDistribution::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_constructRootedTripletDistribution::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RootedTripletDistribution::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_constructRootedTripletDistribution::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
