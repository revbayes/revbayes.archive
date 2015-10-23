#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "Move_WeightedNodeTimeSlide.h"
#include "Natural.h"
#include "NodeTimeSlideWeightedProposal.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_WeightedNodeTimeSlide::Move_WeightedNodeTimeSlide() : Move()
{
    
}


/** Clone object */
Move_WeightedNodeTimeSlide* Move_WeightedNodeTimeSlide::clone(void) const
{
    
	return new Move_WeightedNodeTimeSlide(*this);
}


void Move_WeightedNodeTimeSlide::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    int b = static_cast<const Natural &>( blocks->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    
    RevBayesCore::Proposal *p = new RevBayesCore::NodeTimeSlideWeightedProposal(t, size_t(b));
    value = new RevBayesCore::MetropolisHastingsMove(p, w, false);

}


/** Get Rev type of object */
const std::string& Move_WeightedNodeTimeSlide::getClassType(void) { 
    
    static std::string revType = "Move_WeightedNodeTimeSlide";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_WeightedNodeTimeSlide::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_WeightedNodeTimeSlide::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "tree"  , TimeTree::getClassTypeSpec(), "The tree on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
        memberRules.push_back( new ArgumentRule( "blocks", Natural::getClassTypeSpec() , "The number of bocks into which the branch will be broken.", ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new Natural(8) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_WeightedNodeTimeSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_WeightedNodeTimeSlide::printValue(std::ostream &o) const {
    
    o << "Move_WeightedNodeTimeSlide(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_WeightedNodeTimeSlide::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "blocks" ) {
        blocks = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}
