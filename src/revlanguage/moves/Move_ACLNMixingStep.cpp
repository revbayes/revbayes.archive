
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "ContinuousStochasticNode.h"
#include "ModelVector.h"
#include "Move_ACLNMixingStep.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "RateAgeACLNMixingMove.h"


using namespace RevLanguage;

Move_ACLNMixingStep::Move_ACLNMixingStep() : Move() {
    
}

/** Clone object */
Move_ACLNMixingStep* Move_ACLNMixingStep::clone(void) const {
    
	return new Move_ACLNMixingStep(*this);
}


void Move_ACLNMixingStep::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double ep = static_cast<const RealPos &>( epsilon->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmpNR = static_cast<const ModelVector<RealPos> &>( nodeRates->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *nr = static_cast<RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *>( tmpNR );
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmpTre = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *tre = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmpTre );
    RevBayesCore::TypedDagNode<double>* tmpRR = static_cast<const Real &>( rootRate->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *rr = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmpRR );
    bool tv = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
	std::vector<RevBayesCore::DagNode*> treeAndRates;
	treeAndRates.push_back( tre );
	treeAndRates.push_back(nr);
	treeAndRates.push_back(rr);
    
    
    value = new RevBayesCore::RateAgeACLNMixingMove(treeAndRates, ep, tv, w);
}


/** Get Rev type of object */
const std::string& Move_ACLNMixingStep::getClassType(void) { 
    
    static std::string revType = "Move_ACLNMixingStep";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_ACLNMixingStep::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_ACLNMixingStep::getParameterRules(void) const
{
    
    static MemberRules mixingStepMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        mixingStepMemberRules.push_back( new ArgumentRule( "tree"     , TimeTree::getClassTypeSpec()            , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "nodeRates", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "rootRate" , RealPos::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "epsilon"  , RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new Real(1.0) ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "tune"     , RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RlBoolean( false ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        mixingStepMemberRules.insert( mixingStepMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return mixingStepMemberRules;
}

/** Get type spec */
const TypeSpec& Move_ACLNMixingStep::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_ACLNMixingStep::printValue(std::ostream &o) const {
    
    o << "Move_ACLNMixingStep(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_ACLNMixingStep::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "nodeRates" ) {
        nodeRates = var;
    }
    else if ( name == "rootRate" ) {
        rootRate = var;
    }
    else if ( name == "epsilon" ) {
        epsilon = var;
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
