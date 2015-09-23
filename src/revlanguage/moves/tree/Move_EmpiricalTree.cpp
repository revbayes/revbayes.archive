#include "Move_EmpiricalTree.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "EmpiricalTreeProposal.h"
#include "ModelVector.h"
#include "MetropolisHastingsMove.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;


Move_EmpiricalTree::Move_EmpiricalTree() : Move()
{
    
}


/** Clone object */
Move_EmpiricalTree* Move_EmpiricalTree::clone(void) const
{
    
    return new Move_EmpiricalTree(*this);
}


void Move_EmpiricalTree::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new empirical tree move
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tmp = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    
    RevBayesCore::Proposal *p = new RevBayesCore::EmpiricalTreeProposal( t );
    value = new RevBayesCore::MetropolisHastingsMove(p, w, false);
    
}


/** Get Rev type of object */
const std::string& Move_EmpiricalTree::getClassType(void)
{
    static std::string revType = "Move_EmpiricalTree";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_EmpiricalTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_EmpiricalTree::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "tree"   , Tree::getClassTypeSpec() , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_EmpiricalTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_EmpiricalTree::printValue(std::ostream &o) const
{
    
    o << "Move_EmpiricalTree(";
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


/** Set a member variable */
void Move_EmpiricalTree::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}

