#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "ContinuousStochasticNode.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_SliderUpDown.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "SlideUpDownProposal.h"


using namespace RevLanguage;

Move_SliderUpDown::Move_SliderUpDown() : Move()
{
    
}

/** Clone object */
Move_SliderUpDown* Move_SliderUpDown::clone(void) const
{
    
	return new Move_SliderUpDown(*this);
}


void Move_SliderUpDown::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double sf = static_cast<const RealPos &>( slideFactor->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<double>* tmpsv1 = static_cast<const Real &>( slideUp->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *sv1 = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmpsv1 );
    
    RevBayesCore::TypedDagNode<double>* tmpsv2 = static_cast<const Real &>( slideDown->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *sv2 = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmpsv2 );
    
    
    bool tv = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *p = new RevBayesCore::SlideUpDownProposal(sv1, sv2, sf);
    value = new RevBayesCore::MetropolisHastingsMove(p,w,tv);
    
}


/** Get Rev type of object */
const std::string& Move_SliderUpDown::getClassType(void) {
    
    static std::string revType = "Move_SliderUpDown";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_SliderUpDown::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_SliderUpDown::getParameterRules(void) const
{
    
    static MemberRules mixingStepMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        mixingStepMemberRules.push_back( new ArgumentRule( "value_1", Real::getClassTypeSpec()     , "The variable to slide up.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "value_2", Real::getClassTypeSpec()     , "The variable to slide down.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "slide"  , RealPos::getClassTypeSpec()  , "The window size parameter.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0) ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "tune"   , RlBoolean::getClassTypeSpec(), "Should we tune the window size during burnin?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        mixingStepMemberRules.insert( mixingStepMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return mixingStepMemberRules;
}

/** Get type spec */
const TypeSpec& Move_SliderUpDown::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_SliderUpDown::printValue(std::ostream &o) const {
    
    o << "Move_SliderUpDown(";
    if (slideUp != NULL) {
        o << slideUp->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_SliderUpDown::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "value_1" ) {
        slideUp = var;
    }
    else if ( name == "value_2" ) {
        slideDown = var;
    }
    else if ( name == "slide" ) {
        slideFactor = var;
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
