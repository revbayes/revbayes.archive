//
//  Move_FossilSafeSlide.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/9/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "FossilSafeSlideMove.h"
#include "ModelVector.h"
#include "Move_FossilSafeSlide.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorFunction.h"


using namespace RevLanguage;

Move_FossilSafeSlide::Move_FossilSafeSlide() : Move() {
    
}


/** Clone object */
Move_FossilSafeSlide* Move_FossilSafeSlide::clone(void) const {
    
	return new Move_FossilSafeSlide(*this);
}


void Move_FossilSafeSlide::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move

    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    bool at = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    std::vector<double> ta       = static_cast<const ModelVector<RealPos> &>( tipAges->getRevObject() ).getDagNode()->getValue();

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *t_tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t     = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( t_tmp );
    
    RevBayesCore::TypedDagNode<double>* n_tmp = static_cast<const RealPos &>( scale->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *n = static_cast<RevBayesCore::ContinuousStochasticNode *>( n_tmp );
    
    std::vector<RevBayesCore::DagNode*> nd;
    nd.push_back(t);
    nd.push_back(n);
    
    value = new RevBayesCore::FossilSafeSlideMove(nd, ta, d, at, w);
}


/** Get Rev type of object */
const std::string& Move_FossilSafeSlide::getClassType(void) {
    
    static std::string revType = "Move_FossilSafeSlide";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_FossilSafeSlide::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_FossilSafeSlide::getParameterRules(void) const {
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "scale",   RealPos::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "tree",    TimeTree::getClassTypeSpec()            , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "tipAges", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        moveMemberRules.push_back( new ArgumentRule( "delta",   RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune" ,   RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_FossilSafeSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_FossilSafeSlide::printValue(std::ostream &o) const {
    
    o << "Move_FossilSafeSlide(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_FossilSafeSlide::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "scale" ) {
        scale = var;
    }
    else if ( name == "tipAges" ) {
        tipAges = var;
    }
    else if ( name == "delta" ) {
        delta = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}