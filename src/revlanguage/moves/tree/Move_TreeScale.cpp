#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "Move_TreeScale.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TreeScaleProposal.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_TreeScale::Move_TreeScale() : Move() {
    
}


/** Clone object */
Move_TreeScale* Move_TreeScale::clone(void) const
{
    
	return new Move_TreeScale(*this);
}


void Move_TreeScale::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new tree scale move
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    
    RevBayesCore::StochasticNode<double> *ra = NULL;
    if ( rootAge != NULL && rootAge->getRevObject() != RevNullObject::getInstance() )
    {
        RevBayesCore::TypedDagNode<double> *tmp = static_cast<const RealPos &>( rootAge->getRevObject() ).getDagNode();
        ra = static_cast<RevBayesCore::StochasticNode<double> *>( tmp );
    }
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    double l = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    bool tune = static_cast<const RlBoolean &>( tuning->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *p = new RevBayesCore::TreeScaleProposal(t, ra, l);
    value = new RevBayesCore::MetropolisHastingsMove(p, w, tune);
}


/** Get Rev type of object */
const std::string& Move_TreeScale::getClassType(void)
{
    
    static std::string revType = "Move_TreeScale";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_TreeScale::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules */
const MemberRules& Move_TreeScale::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "tree"   , TimeTree::getClassTypeSpec() , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "rootAge", RealPos::getClassTypeSpec()  , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "delta"  , RealPos::getClassTypeSpec()  , ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RealPos( 1.0 ) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"   , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_TreeScale::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_TreeScale::printValue(std::ostream &o) const
{
    
    o << "Move_TreeScale(";
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
void Move_TreeScale::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "rootAge" )
    {
        rootAge = var;
    }
    else if ( name == "delta" )
    {
        delta = var;
    }
    else if ( name == "tune" )
    {
        tuning = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }

}
