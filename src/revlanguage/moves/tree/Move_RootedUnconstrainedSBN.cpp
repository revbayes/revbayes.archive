#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ModelVector.h"
#include "RootedUnconstrainedSBNProposal.h"
#include "RevObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "MetropolisHastingsMove.h"
#include "Move_RootedUnconstrainedSBN.h"
#include "RlBranchLengthTree.h"
#include "RlSBNParameters.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_RootedUnconstrainedSBN::Move_RootedUnconstrainedSBN() : Move()
{

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_RootedUnconstrainedSBN* Move_RootedUnconstrainedSBN::clone(void) const
{

	return new Move_RootedUnconstrainedSBN(*this);
}


void Move_RootedUnconstrainedSBN::constructInternalObject( void )
{
    // we free the memory first
    delete value;

    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const BranchLengthTree &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );

    const RevBayesCore::SBNParameters &sbn = static_cast<const SBNParameters &>(parameters->getRevObject()).getValue();

    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmp2 = static_cast<const ModelVector<RealPos> &>( branchLengths->getRevObject() ).getDagNode();
    std::vector<const RevBayesCore::DagNode*> p = tmp2->getParents();
    std::vector< RevBayesCore::StochasticNode<double> *> n;
    for (std::vector<const RevBayesCore::DagNode*>::const_iterator it = p.begin(); it != p.end(); ++it)
    {
        const RevBayesCore::StochasticNode<double> *the_node = dynamic_cast< const RevBayesCore::StochasticNode<double>* >( *it );
        if ( the_node != NULL )
        {
            n.push_back( const_cast< RevBayesCore::StochasticNode<double>* >( the_node ) );
        }
        else
        {
            throw RbException("Could not create a mvRootedUnconstrainedSBN because the node isn't a vector of stochastic nodes.");
        }
    }

    RevBayesCore::Proposal *s = new RevBayesCore::RootedUnconstrainedSBNProposal(t,n,sbn);
    value = new RevBayesCore::MetropolisHastingsMove(s,w);

}


/** Get class name of object */
const std::string& Move_RootedUnconstrainedSBN::getClassName(void)
{

    static std::string rbClassName = "Move_RootedUnconstrainedSBN";

	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_RootedUnconstrainedSBN::getClassTypeSpec(void)
{

    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );

	return rbClass;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_RootedUnconstrainedSBN::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "RootedUnconstrainedSBN";

    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Move_RootedUnconstrainedSBN::getParameterRules(void) const
{

    static MemberRules RUSBNMemberRules;
    static bool rules_set = false;

    if ( !rules_set )
    {

        RUSBNMemberRules.push_back( new ArgumentRule( "tree",          BranchLengthTree::getClassTypeSpec(),     "The tree variable this move operates on.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        RUSBNMemberRules.push_back( new ArgumentRule( "branchLengths", ModelVector<RealPos>::getClassTypeSpec(), "The tree's branch lengths.",               ArgumentRule::BY_REFERENCE, ArgumentRule::DETERMINISTIC ) );
        RUSBNMemberRules.push_back( new ArgumentRule( "parameters",    SBNParameters::getClassTypeSpec() ,       "The parameters of the SBN",                ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        RUSBNMemberRules.insert( RUSBNMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );

        rules_set = true;
    }

    return RUSBNMemberRules;
}

/** Get type spec */
const TypeSpec& Move_RootedUnconstrainedSBN::getTypeSpec( void ) const
{

    static TypeSpec type_spec = getClassTypeSpec();

    return type_spec;
}



/** Get type spec */
void Move_RootedUnconstrainedSBN::printValue(std::ostream &o) const
{

    o << "RootedUnconstrainedSBN(";
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
void Move_RootedUnconstrainedSBN::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "branchLengths" )
    {
        branchLengths = var;
    }
    else if ( name == "parameters" )
    {
        parameters = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}
