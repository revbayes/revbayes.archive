//////
//////  Move_NodeCharacterHistoryRejectionSample.cpp
//////  rb_mlandis
//////
//////  Created by Michael Landis on 7/5/14.
//////  Copyright (c) 2014 Michael Landis. All rights reserved.
//////
////
////
////
////#include "ArgumentRule.h"
////#include "ArgumentRules.h"
////#include "BiogeographicTreeHistoryCtmc.h"
////#include "BiogeographyNodeRejectionSampleProposal.h"
////#include "ContinuousStochasticNode.h"
////#include "Dist_phyloCTMCDASequence.h"
////#include "MetropolisHastingsMove.h"
////#include "Move_NodeCharacterHistoryRejectionSample.h"
////#include "NodeRejectionSampleProposal.h"
////#include "OptionRule.h"
////#include "Probability.h"
////#include "RateGeneratorSequence_Biogeography.h"
////#include "RbException.h"
////#include "Real.h"
////#include "RealPos.h"
////#include "RevObject.h"
////#include "RlAbstractHomologousDiscreteCharacterData.h"
////#include "RlBoolean.h"
////#include "RlRateGeneratorSequence.h"
////#include "RlString.h"
////#include "RlTimeTree.h"
////#include "ScaleProposal.h"
////#include "TypedDagNode.h"
////#include "TypeSpec.h"
////
////#include "StandardState.h"
////
////
////using namespace RevLanguage;
////
/////**
//// * Default constructor.
//// *
//// * The default constructor does nothing except allocating the object.
//// */
////Move_NodeCharacterHistoryRejectionSample::Move_NodeCharacterHistoryRejectionSample() : Move()
////{
////    
//    value = new RevBayesCore::MetropolisHastingsMove(p,w,false);
////    value = new RevBayesCore::PathRejectionSampleMove<RevBayesCore::StandardState, RevBayesCore::Tree>(ctmc_sn, tree_sn, qmap_dn, new RevBayesCore::BiogeographyNodeRejectionSampleProposal<RevBayesCore::StandardState,RevBayesCore::Tree>(ctmc_sn, tree_sn, qmap_dn, d), d, false, w);
//
//}
//
//
///**
// * Get Rev type of object
// *
// * \return The class' name.
// */
//const std::string& Move_NodeCharacterHistoryRejectionSample::getClassType(void)
//{
//    
//    static std::string rev_type = "Move_NodeCharacterHistoryRejectionSample";
//    
//	return rev_type;
//}
//
//
///**
// * Get class type spec describing type of an object from this class (static).
// *
// * \return TypeSpec of this class.
// */
//const TypeSpec& Move_NodeCharacterHistoryRejectionSample::getClassTypeSpec(void)
//{
//    
//    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
//    
//	return rev_type_spec;
//}
//
//
///**
// * Get the Rev name for the constructor function.
// *
// * \return Rev name of constructor function.
// */
//std::string Move_NodeCharacterHistoryRejectionSample::getMoveName( void ) const
//{
//    // create a constructor function name variable that is the same for all instance of this class
//    std::string c_name = "NodeCharacterHistoryRejectionSample";
//    
//    return c_name;
//}
//
//
///**
// * Get the member rules used to create the constructor of this object.
// *
// * The member rules of the scale move are:
// * (1) the variable which must be a positive real.
// * (2) the tuning parameter lambda that defines the size of the proposal (positive real)
// * (3) a flag whether auto-tuning should be used.
// *
// * \return The member rules.
// */
//const MemberRules& Move_NodeCharacterHistoryRejectionSample::getParameterRules(void) const
//{
//    
//    static MemberRules nodeChrsmove_member_rules;
//    static bool rules_set = false;
//    
//    if ( !rules_set )
//    {
//        
//        nodeChrsmove_member_rules.push_back( new ArgumentRule( "ctmc", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
//        nodeChrsmove_member_rules.push_back( new ArgumentRule( "qmap", RateMap::getClassTypeSpec()                      , "", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
//        nodeChrsmove_member_rules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec()                     , "", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
//        nodeChrsmove_member_rules.push_back( new ArgumentRule( "lambda", Probability::getClassTypeSpec()                , "", ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Probability(1.0) ) );
////        nodeChrsmove_member_rules.push_back( new ArgumentRule( "type", true, RlString::getClassTypeSpec(), new RlString("std") ) );
//        std::vector<std::string> options;
//        options.push_back( "std" );
//        options.push_back( "biogeo" );
//        nodeChrsmove_member_rules.push_back( new OptionRule( "type", new RlString("std"), options, "" ) );
//        
//        /* Inherit weight from Move, put it after variable */
//        const MemberRules& inheritedRules = Move::getParameterRules();
//        nodeChrsmove_member_rules.insert( nodeChrsmove_member_rules.end(), inheritedRules.begin(), inheritedRules.end() );
//        
//        rules_set = true;
//    }
//    
//    return nodeChrsmove_member_rules;
//}
//
//
///**
// * Get type-specification on this object (non-static).
// *
// * \return The type spec of this object.
// */
//const TypeSpec& Move_NodeCharacterHistoryRejectionSample::getTypeSpec( void ) const
//{
//    
//    static TypeSpec type_spec = getClassTypeSpec();
//    
//    return type_spec;
//}
//
//
//void Move_NodeCharacterHistoryRejectionSample::printValue(std::ostream &o) const {
//    
//    o << "CharacterHistoryRejectionSample(";
//    if (qmap != NULL)
//    {
//        o << qmap->getName();
//    }
//    else
//    {
//        o << "?";
//    }
//    o << ")";
//    
//}
//
//
///**
// * Set a member variable.
// *
// * Sets a member variable with the given name and store the pointer to the variable.
// * The value of the variable might still change but this function needs to be called again if the pointer to
// * the variable changes. The current values will be used to create the distribution object.
// *
// * \param[in]    name     Name of the member variable.
// * \param[in]    var      Pointer to the variable.
// */
//
//void Move_NodeCharacterHistoryRejectionSample::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
//{
//    
//    if ( name == "ctmc" )
//    {
//        ctmc = var;
//    }
//    else if ( name == "tree" )
//    {
//        tree = var;
//    }
//    else if ( name == "qmap" )
//    {
//        qmap = var;
//    }
//    else if ( name == "lambda" )
//    {
//        lambda = var;
//    }
//    else if ( name == "type" )
//    {
//        type = var;
//    }
//    else
//    {
//        Move::setConstParameter(name, var);
//    }
//    
//}
//
//
