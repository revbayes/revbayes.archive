#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ModelVector.h"
#include "Move_VectorScale.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorScaleMove.h"


using namespace RevLanguage;

Move_VectorScale::Move_VectorScale() : Move() {
    
}


/** Clone object */
Move_VectorScale* Move_VectorScale::clone(void) const {
    
	return new Move_VectorScale(*this);
}


void Move_VectorScale::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new vector-scale move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmp = static_cast<const ModelVector<RealPos> &>( x->getRevObject() ).getDagNode();
    std::set<const RevBayesCore::DagNode*> p = tmp->getParents();
    std::vector< RevBayesCore::StochasticNode<double> *> n;
    for (std::set<const RevBayesCore::DagNode*>::const_iterator it = p.begin(); it != p.end(); ++it) 
    {
        const RevBayesCore::StochasticNode<double> *theNode = static_cast< const RevBayesCore::StochasticNode<double>* >( *it );
        n.push_back( const_cast< RevBayesCore::StochasticNode<double>* >( theNode ) );
    }
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::VectorScaleMove(n, l, t, w);
}


/** Get Rev type of object */
const std::string& Move_VectorScale::getClassType(void) { 
    
    static std::string revType = "Move_VectorScale";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_VectorScale::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_VectorScale::getMemberRules(void) const {
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        moveMemberRules.push_back( new ArgumentRule( "x"     , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_VectorScale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_VectorScale::printValue(std::ostream &o) const {
    
    o << "Move_VectorScale(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_VectorScale::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
