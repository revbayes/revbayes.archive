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
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_constructRootedTripletDistribution::Func_constructRootedTripletDistribution( void ) : TypedFunction<RootedTripletDistribution>( ) {
    
}


/** Clone object */
Func_constructRootedTripletDistribution* Func_constructRootedTripletDistribution::clone( void ) const {
    
    return new Func_constructRootedTripletDistribution( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RootedTripletDistribution >* Func_constructRootedTripletDistribution::createFunction( void ) const
{
    
    RevBayesCore::RootedTripletDistributionFunction* f = NULL;
    
    
    if ( this->args[1].getVariable()->getRequiredTypeSpec().isDerivedOf( ModelVector< RlString >::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RbVector< std::string > >* sn = static_cast<const ModelVector< RlString > &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
        f = new RevBayesCore::RootedTripletDistributionFunction( sn );
    }
    else if ( this->args[1].getVariable()->getRequiredTypeSpec().isDerivedOf( ModelVector< Taxon >::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RbVector< RevBayesCore::Taxon > >* t = static_cast<const ModelVector< Taxon > &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
        f = new RevBayesCore::RootedTripletDistributionFunction( t );
    }

    if ( this->args[0].getVariable()->getRequiredTypeSpec().isDerivedOf( ModelVector< TimeTree >::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RbVector< RevBayesCore::Tree > >* gTrees = static_cast<const ModelVector< TimeTree > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
        f->setTrees(gTrees);
    }
    else if ( this->args[0].getVariable()->getRequiredTypeSpec().isDerivedOf( ModelVector< BranchLengthTree >::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RbVector< RevBayesCore::Tree > >* gTrees = static_cast<const ModelVector< BranchLengthTree > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
        f->setTrees(gTrees);
    }

    if ( this->args[2].getVariable()->getRequiredTypeSpec().isDerivedOf( RlBoolean::getClassTypeSpec() ) )
    {
        
        // Sebastian: Currently unused
//        RevBayesCore::TypedDagNode< RevBayesCore::Boolean >* t = static_cast<const RlBoolean &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
        // TODO: Bastien, this isn't working (Sebastian)
//        f->setRecordBranchLengths( t );
    }

    
    return f;
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


        argumentRules.push_back( new ArgumentRule( "geneTrees",         Tree::getClassTypeSpec(),                       ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "speciesNames",      ModelVector< RlString >::getClassTypeSpec(),    ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "keepBranchLengths", RlBoolean::getClassTypeSpec(),                  ArgumentRule::BY_CONSTANT_REFERENCE ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_constructRootedTripletDistribution::getClassType(void)
{
    
    static std::string revType = "Func_constructRootedTripletDistribution";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_constructRootedTripletDistribution::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_constructRootedTripletDistribution::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
