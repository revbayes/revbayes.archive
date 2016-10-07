#include "AddRemoveFossilProposal.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "Move_AddRemoveFossil.h"
#include "Probability.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_AddRemoveFossil::Move_AddRemoveFossil() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_AddRemoveFossil* Move_AddRemoveFossil::clone(void) const
{
    
    return new Move_AddRemoveFossil(*this);
}


void Move_AddRemoveFossil::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new move
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    
    RevBayesCore::TypedDagNode<double> *o = static_cast<const RealPos &>( origin->getRevObject() ).getDagNode();
    
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    
    double pr = static_cast<const RealPos &>( probRemove->getRevObject() ).getValue();
    double ps = static_cast<const RealPos &>( probAncestor->getRevObject() ).getValue();

    RevBayesCore::Proposal *p = new RevBayesCore::AddRemoveFossilProposal( t, o, pr, ps );
    value = new RevBayesCore::MetropolisHastingsMove(p,w,false);
}


/** Get Rev type of object */
const std::string& Move_AddRemoveFossil::getClassType(void)
{
    
    static std::string revType = "Move_AddRemoveFossil";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_AddRemoveFossil::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_AddRemoveFossil::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "AddRemoveFossil";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Move_AddRemoveFossil::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        memberRules.push_back( new ArgumentRule( "tree"  , TimeTree::getClassTypeSpec(), "The tree on which this moves operates on.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "origin", RealPos::getClassTypeSpec() , "The variable for the origin of the process giving a maximum age for the new fossil attachement time.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "probRemove", Probability::getClassTypeSpec(), "The probability of proposing to remove a fossil.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability( 0.5 ) ) );
        memberRules.push_back( new ArgumentRule( "probAncestor", Probability::getClassTypeSpec(), "The probability of proposing to add a fossil as a sampled ancestor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability( 0.5 ) ) );


        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rules_set = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_AddRemoveFossil::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}



/** Get type spec */
void Move_AddRemoveFossil::printValue(std::ostream &o) const
{
    
    o << "Move_AddRemoveFossil(";
    if (tree != NULL)
    {
        o << tree->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_AddRemoveFossil::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "origin" )
    {
        origin = var;
    }
    else if ( name == "probRemove" )
    {
        probRemove = var;
    }
    else if ( name == "probAncestor" )
    {
        probAncestor = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}
