#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "SubtreePruneRegraftProposal.h"
#include "RevObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "MetropolisHastingsMove.h"
#include "Move_SPRNonclock.h"
#include "RlTopology.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_SPRNonclock::Move_SPRNonclock() : Move()
{
    
}


/** Clone object */
Move_SPRNonclock* Move_SPRNonclock::clone(void) const
{
    
	return new Move_SPRNonclock(*this);
}


void Move_SPRNonclock::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::Topology> *tmp = static_cast<const Topology &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::Topology> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Topology> *>( tmp );

    RevBayesCore::Proposal *p = new RevBayesCore::SubtreePruneRegraftProposal(t);
    value = new RevBayesCore::MetropolisHastingsMove(p,w);
    
}


/** Get class name of object */
const std::string& Move_SPRNonclock::getClassName(void)
{
    
    static std::string rbClassName = "Move_SPR";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_SPRNonclock::getClassTypeSpec(void)
{
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& Move_SPRNonclock::getParameterRules(void) const {
    
    static MemberRules SPRMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
    
        SPRMemberRules.push_back( new ArgumentRule( "tree", Topology::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        SPRMemberRules.insert( SPRMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return SPRMemberRules;
}

/** Get type spec */
const TypeSpec& Move_SPRNonclock::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_SPRNonclock::printValue(std::ostream &o) const {
    
    o << "SPR(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_SPRNonclock::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}
