/**
 * @file
 * This file contains the declaration of the RevLanguage pairwise distances function, which
 * is used to create a deterministic variable associated with the pairwise distances matrix.
 *
 * @brief Declaration and implementation of Func_treePairwiseDistances
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_treePairwiseDistances.h 1406 2015-07-23 02:06:14Z Boussau $
 */


#ifndef Func_treePairwiseDistances_H
#define Func_treePairwiseDistances_H

#include "RlDistanceMatrix.h"
#include "RlTypedFunction.h"

#include <string>


namespace RevLanguage {

	template <class treeType>
    class Func_treePairwiseDistances : public TypedFunction< RlDistanceMatrix > {
        
    public:
        Func_treePairwiseDistances( void );
        
        // Basic utility functions
        Func_treePairwiseDistances*                     					clone(void) const;                                                              //!< Clone the object
        static const std::string&                       					getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          					getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 					getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::DistanceMatrix >*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                            					getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}


//
//  Func_treePairwiseDistances.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_treePairwiseDistances.h"
#include "DistanceMatrix.h"
#include "ModelVector.h"
#include "RlTimeTree.h"
#include "RlBranchLengthTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "Topology.h"
#include "TreePairwiseDistances.h"
#include "TypedDagNode.h"

using namespace RevLanguage;


/** default constructor */
template <class treeType>
Func_treePairwiseDistances<treeType>::Func_treePairwiseDistances( void ) : TypedFunction< RlDistanceMatrix >( )
{
	
}


/** Clone object */
template <class treeType>
Func_treePairwiseDistances< treeType > * Func_treePairwiseDistances<treeType>::clone( void ) const {
	
	return new Func_treePairwiseDistances< treeType >( *this );
}


template <class treeType>
RevBayesCore::TypedFunction< RevBayesCore::DistanceMatrix >* Func_treePairwiseDistances<treeType>::createFunction( void ) const
{
	
	// RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
	
	RevBayesCore::TypedDagNode < typename treeType::valueType >* tau = static_cast<const treeType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
	
	RevBayesCore::TreePairwiseDistances < typename treeType::valueType > * f = new RevBayesCore::TreePairwiseDistances < typename treeType::valueType > ( tau );
	
/*	if ( this->args[0].getVariable()->getRequiredTypeSpec().isDerivedOf( TimeTree::getClassTypeSpec() ) )
	{
		RevBayesCore::TypedDagNode < RevBayesCore::TimeTree >* tau = static_cast<const treeType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
		f = new RevBayesCore::TreePairwiseDistances < RevBayesCore::TimeTree > ( tau );
	}
	else if ( this->args[0].getVariable()->getRequiredTypeSpec().isDerivedOf( BranchLengthTree::getClassTypeSpec() ) )
	{
		RevBayesCore::TypedDagNode< RevBayesCore::BranchLengthTree >* tau = static_cast<const BranchLengthTree &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
		f = new RevBayesCore::TreePairwiseDistances< RevBayesCore::BranchLengthTree > ( tau );
	}*/
	
	return f;
}


/* Get argument rules */
template <class treeType>
const ArgumentRules& Func_treePairwiseDistances<treeType>::getArgumentRules( void ) const {
	
	static ArgumentRules argumentRules = ArgumentRules();
	static bool          rulesSet = false;
	
	if ( !rulesSet )
	{
		
		/*std::vector<TypeSpec> treeTypes;
		treeTypes.push_back( TimeTree::getClassTypeSpec() );
		treeTypes.push_back( BranchLengthTree::getClassTypeSpec() );*/
		
		
		argumentRules.push_back( new ArgumentRule( "tree", treeType::getClassTypeSpec(),  ArgumentRule::BY_CONSTANT_REFERENCE ) );
		
		rulesSet = true;
	}
	
	return argumentRules;
}


template <class treeType>
const std::string& Func_treePairwiseDistances<treeType>::getClassType(void) {
	
	static std::string revType = "Func_treePairwiseDistances<" + treeType::getClassType() + ">";
	
	return revType;
}

/* Get class type spec describing type of object */
template <class treeType>
const TypeSpec& Func_treePairwiseDistances<treeType>::getClassTypeSpec(void) {
	
	static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<treeType>::getClassTypeSpec() ) );
	
	return revTypeSpec;
}


template <class treeType>
const TypeSpec& Func_treePairwiseDistances<treeType>::getTypeSpec( void ) const {
	
	static TypeSpec typeSpec = getClassTypeSpec();
	
	return typeSpec;
}


#endif
