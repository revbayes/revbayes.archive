/*
 * File:   Move_GraphFlipChord.cpp
 * Author: nl
 *
 * Created on 13 juillet 2014, 18:13
 */

#include "Move_GraphFlipChord.h"

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "MatrixReal.h"
#include "GraphFlipChordProposal.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Probability.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlMatrixReal.h"
#include "RlMatrixRealSymmetric.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_GraphFlipChord::Move_GraphFlipChord() : Move()
{
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_GraphFlipChord* Move_GraphFlipChord::clone(void) const
{
    
    return new Move_GraphFlipChord(*this);
}


void Move_GraphFlipChord::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double l = static_cast<const Probability &>( prob->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    const RevBayesCore::RbVector<long> &e = static_cast<const ModelVector<Natural> &>( vertices->getRevObject() ).getValue();
    
    
    RevBayesCore::Proposal *p = NULL;
    
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal >* tmp = static_cast<const MatrixRealSymmetric &>( v->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::MatrixReal > *n = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MatrixReal> *>( tmp );
    
    bool symm = v->getRevObject().isType( MatrixRealSymmetric::getClassTypeSpec() );
    p = new RevBayesCore::GraphFlipChordProposal( n, e, l, symm );
    
    value = new RevBayesCore::MetropolisHastingsMove(p,w,t);
    
}


/** Get class name of object */
const std::string& Move_GraphFlipChord::getClassType(void) {
    
    static std::string revClassType = "Move_GraphFlipChord";
    
    return revClassType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_GraphFlipChord::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revClassTypeSpec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_GraphFlipChord::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "GraphFlipChord";
    
    return c_name;
}



/** Return member rules (no members) */
const MemberRules& Move_GraphFlipChord::getParameterRules(void) const
{
    
    static MemberRules move_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        std::vector<TypeSpec> matTypes;
        matTypes.push_back( MatrixRealSymmetric::getClassTypeSpec() );
        move_member_rules.push_back( new ArgumentRule( "x"     , matTypes, "The variable on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        move_member_rules.push_back( new ArgumentRule( "p", Probability::getClassTypeSpec()   , "The probability of flipping each edge in edges.", ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Probability(1.0) ) );
        move_member_rules.push_back( new ArgumentRule( "vertices" , ModelVector<Natural>::getClassTypeSpec(), "A vector of vertices to target with this proposal. An empty vector is interpretted as the full list of vertices.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, new ModelVector<Natural>() ) );
        
        move_member_rules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec() , "Should we tune the scaling factor during burnin?", ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        move_member_rules.insert( move_member_rules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rules_set = true;
    }
    
    return move_member_rules;
}

/** Get type spec */
const TypeSpec& Move_GraphFlipChord::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get type spec */
void Move_GraphFlipChord::printValue(std::ostream &o) const
{
    
    o << "Move_GraphFlipChord(";
    if (v != NULL)
    {
        o << v->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_GraphFlipChord::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "x" )
    {
        v = var;
    }
    else if ( name == "vertices" )
    {
        vertices = var;
    }
    else if ( name == "p" )
    {
        prob = var;
    }
    else if ( name == "weight" ) {
        weight = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}

