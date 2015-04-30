/**
 * @file
 * This file contains the declaration and implementation of Dist_empiricalTree, which
 * is the Rev language wrapper for the EmpiricalTreeDistribution 
 * class. The distribution is constructed from an input tree trace.
 * 
 *
 * @brief Declaration and implementation of the Dist_empiricalTree class
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0
 *
 */

#ifndef Dist_empiricalTree_H
#define Dist_empiricalTree_H

#include "EmpiricalTreeDistribution.h"
#include "BranchLengthTree.h"
#include "TimeTree.h"
#include "RlBranchLengthTree.h"
#include "RlTimeTree.h"
#include "RlTypedDistribution.h"
#include "TreeTrace.h"
#include "RlTreeTrace.h"

namespace RevLanguage {
    
	template <typename treeType>
	class Dist_empiricalTree :  public TypedDistribution<treeType> {
        
    public:
		Dist_empiricalTree( void );                //!< Constructor
        virtual ~Dist_empiricalTree();             //!< Virtual destructor
        
        // Basic utility functions
        Dist_empiricalTree<treeType>*                                         clone(void) const;                                                              //!< Clone the object
        static const std::string&                                             getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                                getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                       getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                                                    getParameterRules(void) const;                                                     //!< Get member rules (const)
        
        
        // overridden function from TypedDistribution
		RevBayesCore::EmpiricalTreeDistribution<typename treeType::valueType>*            createDistribution(void) const;
		
    protected:
        
        void                                                                  setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                                             trace;
        RevPtr<const RevVariable>                                             burnin;
    };
	
	
	template <>
	inline RevBayesCore::EmpiricalTreeDistribution<RevBayesCore::BranchLengthTree>* Dist_empiricalTree<BranchLengthTree>::createDistribution( void ) const {
		
		// get tree trace
		const TreeTrace<BranchLengthTree>& rev_tt = static_cast<const TreeTrace<BranchLengthTree>&>( trace->getRevObject() );
		const RevBayesCore::TreeTrace<RevBayesCore::BranchLengthTree>& tt = static_cast<const RevBayesCore::TreeTrace<RevBayesCore::BranchLengthTree>&>( rev_tt.getValue() );
		
		// get burnin
		int b = -1;
		if ( burnin != NULL && burnin->getRevObject() != RevNullObject::getInstance()) {
			b = static_cast<const Natural &>( burnin->getRevObject() ).getDagNode()->getValue();
		} 
		
		// make distrubtion	
		RevBayesCore::EmpiricalTreeDistribution<RevBayesCore::BranchLengthTree>* d = new RevBayesCore::EmpiricalTreeDistribution<RevBayesCore::BranchLengthTree>( tt, b );
		
		return d;
	}
	
	template <>
	inline RevBayesCore::EmpiricalTreeDistribution<RevBayesCore::TimeTree>* Dist_empiricalTree<TimeTree>::createDistribution( void ) const {
		
		// get tree trace
		const TreeTrace<TimeTree>& rev_tt = static_cast<const TreeTrace<TimeTree>&>( trace->getRevObject() );
		const RevBayesCore::TreeTrace<RevBayesCore::TimeTree>& tt = static_cast<const RevBayesCore::TreeTrace<RevBayesCore::TimeTree>&>( rev_tt.getValue() );
		
		// get burnin
		int b = -1;
		if ( burnin != NULL && burnin->getRevObject() != RevNullObject::getInstance()) {
			b = static_cast<const Natural &>( burnin->getRevObject() ).getDagNode()->getValue();
		} 
		
		// make distrubtion	
		RevBayesCore::EmpiricalTreeDistribution<RevBayesCore::TimeTree>*   d = new RevBayesCore::EmpiricalTreeDistribution<RevBayesCore::TimeTree>( tt, b );
		
		return d;
	}
	
}



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_empiricalTree.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlString.h"
#include "StochasticNode.h"
#include "EmpiricalTreeDistribution.h"
#include "RlTreeTrace.h"

using namespace RevLanguage;


template <typename treeType>
Dist_empiricalTree<treeType>::Dist_empiricalTree() : TypedDistribution<treeType>() {
    
}


template <typename treeType>
Dist_empiricalTree<treeType>::~Dist_empiricalTree<treeType>() {
	
}


template <typename treeType>
Dist_empiricalTree<treeType>* Dist_empiricalTree<treeType>::clone( void ) const {
	
    return new Dist_empiricalTree( *this );
}


/* Get Rev type of object */
template <typename treeType>
const std::string& Dist_empiricalTree<treeType>::getClassType(void) {
    
    static std::string revType = "Dist_empiricalTree";
    
	return revType;
}


/* Get class type spec describing type of object.  */
template <typename treeType>
const TypeSpec& Dist_empiricalTree<treeType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<treeType>::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Return member rules */
template <typename treeType>
const MemberRules& Dist_empiricalTree<treeType>::getParameterRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
		
        memberRules.push_back( new ArgumentRule( "burnin", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
		memberRules.push_back( new ArgumentRule( "treetrace", TreeTrace<treeType>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


template <typename treeType>
const TypeSpec& Dist_empiricalTree<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Set a member variable */
template <typename treeType>
void Dist_empiricalTree<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "treetrace" ) {
        trace = var;
    }
    else if ( name == "burnin" ) {
        burnin = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}

#endif