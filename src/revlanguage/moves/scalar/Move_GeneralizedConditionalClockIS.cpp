#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "StochasticNode.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_GeneralizedConditionalClockIS.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "GeneralizedConditionalClockPseudoIndependenceSampler.h"


using namespace RevLanguage;

Move_GeneralizedConditionalClockIS::Move_GeneralizedConditionalClockIS() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_GeneralizedConditionalClockIS* Move_GeneralizedConditionalClockIS::clone(void) const
{
    
    return new Move_GeneralizedConditionalClockIS(*this);
}


void Move_GeneralizedConditionalClockIS::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
//    RevBayesCore::Proposal *prop = NULL;
    
    // now allocate a new vector-scale move
    double s = static_cast<const RealPos &>( sigma->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<double>* stl = static_cast<const RealPos &>( substitutionTreeLength->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tmp1 = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
//    RevBayesCore::StochasticNode<RevBayesCore::Tree> *tr = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp1 );
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    int    k = static_cast<const Integer &>( numInvariantSites->getRevObject() ).getValue();
    int    N = static_cast<const Integer &>( numSites->getRevObject() ).getValue();
    bool   l = static_cast<const RlBoolean &>( clockIsLogScale->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<double>* tmp = static_cast<const RealPos &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<double> *n = const_cast<RevBayesCore::StochasticNode<double>* >(static_cast<const RevBayesCore::StochasticNode<double>* >( tmp ) );
    RevBayesCore::Proposal *prop = new RevBayesCore::GeneralizedConditionalClockISProposal(n,stl,s,k,N,l);

    // get the tuning
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    value = new RevBayesCore::MetropolisHastingsMove(prop,w,t);
    
}


/** Get Rev type of object */
const std::string& Move_GeneralizedConditionalClockIS::getClassType(void)
{
    
    static std::string rev_type = "Move_GeneralizedConditionalClockIS";
    
    return rev_type;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_GeneralizedConditionalClockIS::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_GeneralizedConditionalClockIS::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "GeneralizedConditionalClockIS";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Move_GeneralizedConditionalClockIS::getParameterRules(void) const
{
    
    static MemberRules move_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        move_member_rules.push_back( new ArgumentRule( "x"                      , Real::getClassTypeSpec()      , "The variable on which the move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        move_member_rules.push_back( new ArgumentRule( "substitutionTreeLength" , RealPos::getClassTypeSpec()  , "The length of the tree in substitutions sum(clock_rate_i*branch_length_i).", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        move_member_rules.push_back( new ArgumentRule( "numInvariantSites"      , Integer::getClassTypeSpec()   , "The number of invariant sites in the alignment.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        move_member_rules.push_back( new ArgumentRule( "numSites"               , Integer::getClassTypeSpec()   , "The number of sites in the alignment.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        move_member_rules.push_back( new ArgumentRule( "clockIsLogScale"        , RlBoolean::getClassTypeSpec() , "Is the clock rate parameter for this move on the log scale?", ArgumentRule::BY_VALUE    , ArgumentRule::ANY ) );
        move_member_rules.push_back( new ArgumentRule( "sigma"                  , RealPos::getClassTypeSpec()   , "The standard deviation of the distribution for the IS.", ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Real(1.0) ) );
        move_member_rules.push_back( new ArgumentRule( "tune"                   , RlBoolean::getClassTypeSpec() , "Should we tune the scaling factor during burnin?", ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inherited_rules = Move::getParameterRules();
        move_member_rules.insert( move_member_rules.end(), inherited_rules.begin(), inherited_rules.end() );
        
        rules_set = true;
    }
    
    return move_member_rules;
}

/** Get type spec */
const TypeSpec& Move_GeneralizedConditionalClockIS::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get type spec */
void Move_GeneralizedConditionalClockIS::printValue(std::ostream &o) const
{
    
    o << "Move_GeneralizedConditionalClockIS(";
    if (x != NULL)
    {
        o << x->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
    
}


/** Set a member variable */
void Move_GeneralizedConditionalClockIS::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "x" )
    {
        x = var;
    }
    else if ( name == "substitutionTreeLength" )
    {
        substitutionTreeLength = var;
    }
    else if ( name == "numInvariantSites" )
    {
        numInvariantSites = var;
    }
    else if ( name == "numSites" )
    {
        numSites = var;
    }
    else if ( name == "clockIsLogScale" )
    {
        clockIsLogScale = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "tune" )
    {
        tune = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
    
}
