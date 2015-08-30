#ifndef Move_CharacterHistory_H
#define Move_CharacterHistory_H

#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the scaling move.
     *
     * The RevLanguage wrapper of the scaling move simply
     * manages the interactions through the Rev with our core.
     * That is, the internal move object can be constructed and hooked up
     * in a DAG-nove (variable) that it works on.
     * See the ScaleMove.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-28, version 1.0
     *
     */
    template <class treeType>
    class Move_CharacterHistory : public Move {
        
    public:
        
        Move_CharacterHistory(void);                                                                                                                        //!< Default constructor
        
        // Basic utility functions
        virtual Move_CharacterHistory*              clone(void) const;                                                                          //!< Clone object
        void                                        constructInternalObject(void);                                                              //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                                         //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                     //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                                 //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                    //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                          //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);         //!< Set member variable
        
        RevPtr<const RevVariable>                      ctmc;                                                                                       //!< The variable on which the move works
        RevPtr<const RevVariable>                      qmap;                                                                                          //!< The variable on which the move works
        RevPtr<const RevVariable>                      tree;                                                                                          //!< The variable on which the move works
        RevPtr<const RevVariable>                      lambda;                                                                                     //!< The tuning parameter used.
        RevPtr<const RevVariable>                      type;
        RevPtr<const RevVariable>                      graph;
        RevPtr<const RevVariable>                      proposal;
        
    };
    
}

//
//  Move_CharacterHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 7/5/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "BiogeographyNodeRejectionSampleProposal.h"
#include "BiogeographyPathRejectionSampleProposal.h"
#include "PathRejectionSampleMove.h"
#include "NodeRejectionSampleProposal.h"
#include "PathUniformizationSampleProposal.h"
#include "NodeUniformizationSampleProposal.h"
#include "ContinuousStochasticNode.h"
#include "Dist_phyloDACTMC.h"
#include "MetropolisHastingsMove.h"
#include "Move_CharacterHistory.h"
#include "NodeRejectionSampleProposal.h"
#include "Probability.h"
#include "RateMap_Biogeography.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlRateMap.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "ScaleProposal.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

// to be removed once MH moves work
#include "PathRejectionSampleMove.h"
#include "StandardState.h"


using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
template <class treeType>
RevLanguage::Move_CharacterHistory<treeType>::Move_CharacterHistory() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the move.
 */
template <class treeType>
Move_CharacterHistory<treeType>* RevLanguage::Move_CharacterHistory<treeType>::clone(void) const
{
    
	return new Move_CharacterHistory<treeType>(*this);
}


/**
 * Create a new internal move object.
 *
 * This function simply dynamically allocates a new internal move object that is
 * associated with the variable (DAG-node). The internal move object is created by calling its
 * constructor and passing the move-parameters (the variable and other parameters) as arguments of the
 * constructor. The move constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
template <class treeType>
void RevLanguage::Move_CharacterHistory<treeType>::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // move/proposal arguments
    double d        = static_cast<const Probability &>( lambda->getRevObject() ).getValue();
    double w        = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    std::string mt  = static_cast<const RlString &>( type->getRevObject() ).getValue();
    std::string gt  = static_cast<const RlString &>( graph->getRevObject() ).getValue();
    std::string pt  = static_cast<const RlString &>( proposal->getRevObject() ).getValue();
    
    // move/proposal parameters
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData>* ctmc_tdn   = static_cast<const RevLanguage::AbstractDiscreteCharacterData&>( ctmc->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateMap>* qmap_tdn                 = static_cast<const RateMap&>( qmap->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tree_tdn          = static_cast<const treeType&>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::AbstractDiscreteCharacterData>* ctmc_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::AbstractDiscreteCharacterData>* >(ctmc_tdn);
    RevBayesCore::DeterministicNode<RevBayesCore::RateMap>* qmap_dn             = static_cast<RevBayesCore::DeterministicNode<RevBayesCore::RateMap>* >(qmap_tdn);
    RevBayesCore::StochasticNode<typename treeType::valueType>* tree_sn         = static_cast<RevBayesCore::StochasticNode<typename treeType::valueType>* >(tree_tdn);
    
    // finally create the internal move object
    //    RevBayesCore::Proposal *p = NULL;
    //    if (mt == "std")
    //        ;
    //    else if (mt == "biogeo")
    //        p = new RevBayesCore::BiogeographyPathRejectionSampleProposal<RevBayesCore::StandardState, RevBayesCore::TimeTree>(ctmc_sn, tree_sn, qmap_dn, d);
    //
    //    value = new RevBayesCore::MetropolisHastingsMove(p,w,false);
    //    value = new RevBayesCore::PathRejectionSampleMove<RevBayesCore::StandardState, RevBayesCore::TimeTree>(ctmc_sn, tree_sn, qmap_dn, new RevBayesCore::BiogeographyNodeRejectionSampleProposal<RevBayesCore::StandardState,RevBayesCore::TimeTree>(ctmc_sn, tree_sn, qmap_dn, d), d, false, w);
    
    RevBayesCore::Proposal *p = NULL;
    
    if (mt == "Biogeo")
    {
        if (gt == "node")
        {
//            RevBayesCore::Proposal* p2 = new RevBayesCore::BiogeographyPathRejectionSampleProposal<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
            p = new RevBayesCore::BiogeographyNodeRejectionSampleProposal<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
            value = new RevBayesCore::PathRejectionSampleMove<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, p, d, false, w);
            return;
        }
        else if (gt == "branch")
        {
            p = new RevBayesCore::BiogeographyPathRejectionSampleProposal<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
            value = new RevBayesCore::PathRejectionSampleMove<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, p, d, false, w);
            return;
        }
    }
    else if (mt == "DNA")
    {
        if (gt == "node" && pt == "rejection")
            p = new RevBayesCore::NodeRejectionSampleProposal<RevBayesCore::DnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "rejection")
            p = new RevBayesCore::PathRejectionSampleProposal<RevBayesCore::DnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "node" && pt == "uniformization")
            p = new RevBayesCore::NodeUniformizationSampleProposal<RevBayesCore::DnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "uniformization")
            p = new RevBayesCore::PathUniformizationSampleProposal<RevBayesCore::DnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
    }
    else if (mt == "RNA")
    {
        if (gt == "node" && pt == "rejection")
            p = new RevBayesCore::NodeRejectionSampleProposal<RevBayesCore::RnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "rejection")
            p = new RevBayesCore::PathRejectionSampleProposal<RevBayesCore::RnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "node" && pt == "uniformization")
            p = new RevBayesCore::NodeUniformizationSampleProposal<RevBayesCore::RnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "uniformization")
            p = new RevBayesCore::PathUniformizationSampleProposal<RevBayesCore::RnaState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
    }
    else if (mt == "AA" || mt == "Protein")
    {
        if (gt == "node" && pt == "rejection")
            p = new RevBayesCore::NodeRejectionSampleProposal<RevBayesCore::AminoAcidState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "rejection")
            p = new RevBayesCore::PathRejectionSampleProposal<RevBayesCore::AminoAcidState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "node" && pt == "uniformization")
            p = new RevBayesCore::NodeUniformizationSampleProposal<RevBayesCore::AminoAcidState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "uniformization")
            p = new RevBayesCore::PathUniformizationSampleProposal<RevBayesCore::AminoAcidState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
    }
    else if (mt == "Standard")
    {
        if (gt == "node" && pt == "rejection")
            p = new RevBayesCore::NodeRejectionSampleProposal<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "rejection")
            p = new RevBayesCore::PathRejectionSampleProposal<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "node" && pt == "uniformization")
            p = new RevBayesCore::NodeUniformizationSampleProposal<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
        else if (gt == "branch" && pt == "uniformization")
            p = new RevBayesCore::PathUniformizationSampleProposal<RevBayesCore::StandardState, typename treeType::valueType>(ctmc_sn, tree_sn, qmap_dn, d);
    }
    
    value = new RevBayesCore::MetropolisHastingsMove(p,w,false);
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
template <class treeType>
const std::string& RevLanguage::Move_CharacterHistory<treeType>::getClassType(void)
{
    
    static std::string revType = "Move_CharacterHistory"; // <" + treeType::getClassType() + ">";
    
	return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
template <class treeType>
const TypeSpec& RevLanguage::Move_CharacterHistory<treeType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the scale move are:
 * (1) the variable which must be a positive real.
 * (2) the tuning parameter lambda that defines the size of the proposal (positive real)
 * (3) a flag whether auto-tuning should be used.
 *
 * \return The member rules.
 */
template <class treeType>
const MemberRules& RevLanguage::Move_CharacterHistory<treeType>::getParameterRules(void) const
{
    
    static MemberRules nodeChrsMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        nodeChrsMoveMemberRules.push_back( new ArgumentRule( "ctmc", AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        nodeChrsMoveMemberRules.push_back( new ArgumentRule( "qmap", RateMap::getClassTypeSpec()                      , ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        nodeChrsMoveMemberRules.push_back( new ArgumentRule( "tree", treeType::getClassTypeSpec()                     , ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        nodeChrsMoveMemberRules.push_back( new ArgumentRule( "lambda", Probability::getClassTypeSpec()                , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Probability(1.0) ) );
        
        std::vector<std::string> optionsType;
        optionsType.push_back( "Biogeo" );
        optionsType.push_back( "DNA" );
        optionsType.push_back( "RNA" );
        optionsType.push_back( "AA" );
        optionsType.push_back( "Protein" );
        optionsType.push_back( "Standard" );
        nodeChrsMoveMemberRules.push_back( new OptionRule( "type", new RlString("Standard"), optionsType ) );
        
        std::vector<std::string> optionsGraph;
        optionsGraph.push_back( "node" );
        optionsGraph.push_back( "branch" );
        nodeChrsMoveMemberRules.push_back( new OptionRule( "graph", new RlString("node"), optionsGraph ) );
        
        std::vector<std::string> optionsProposal;
        optionsProposal.push_back( "rejection" );
        optionsProposal.push_back( "uniformization" );
        nodeChrsMoveMemberRules.push_back( new OptionRule( "proposal", new RlString("rejection"), optionsProposal ) );
        
        /* Inherit weight from Move, put it after variable */
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        nodeChrsMoveMemberRules.insert( nodeChrsMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return nodeChrsMoveMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
template <class treeType>
const TypeSpec& RevLanguage::Move_CharacterHistory<treeType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

template <class treeType>
void RevLanguage::Move_CharacterHistory<treeType>::printValue(std::ostream &o) const {
    
    o << "CharacterHistoryRejectionSample(";
    if (qmap != NULL)
    {
        o << qmap->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
    
}


/**
 * Set a member variable.
 *
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
template <class treeType>
void RevLanguage::Move_CharacterHistory<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "ctmc" )
    {
        ctmc = var;
    }
    else if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "qmap" )
    {
        qmap = var;
    }
    else if ( name == "lambda" )
    {
        lambda = var;
    }
    else if ( name == "type" )
    {
        type = var;
    }
    else if ( name == "graph" )
    {
        graph = var;
    }
    else if ( name == "proposal" )
    {
        proposal = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
    
}

#endif
