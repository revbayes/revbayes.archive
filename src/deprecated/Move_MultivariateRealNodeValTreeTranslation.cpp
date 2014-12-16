/* 
 * File:   Move_MultivariateRealNodeValTreeTranslation.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 19:46
 */

#include "Move_MultivariateRealNodeValTreeTranslation.h"


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "RlMultivariateRealNodeValTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_MultivariateRealNodeValTreeTranslation::Move_MultivariateRealNodeValTreeTranslation() : Move() {
    
}


/** Clone object */
Move_MultivariateRealNodeValTreeTranslation* Move_MultivariateRealNodeValTreeTranslation::clone(void) const {
    
	return new Move_MultivariateRealNodeValTreeTranslation(*this);
}


void Move_MultivariateRealNodeValTreeTranslation::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::MultivariateRealNodeContainer> *tmp = static_cast<const MultivariateRealNodeValTree &>( process->getRevObject() ).getDagNode();
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tuning->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::MultivariateRealNodeContainer> *p = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MultivariateRealNodeContainer> *>( tmp );
    value = new RevBayesCore::MultivariateRealNodeContainerTranslationMove(p, l, t, w);

    // now allocate a new sliding move
    /*
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::NodeTimeSlideUniform(t, w);
    */
    
}


/** Get class name of object */
const std::string& Move_MultivariateRealNodeValTreeTranslation::getClassType(void) { 
    
    static std::string revClassType = "Move_MultivariateRealNodeValTreeTranslation";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_MultivariateRealNodeValTreeTranslation::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_MultivariateRealNodeValTreeTranslation::getParameterRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "process", MultivariateRealNodeValTree::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "lambda" , RealPos::getClassTypeSpec()                    , ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RealPos( 1.0 ) ) );
        memberRules.push_back( new ArgumentRule( "tune"   , RlBoolean::getClassTypeSpec()                  , ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RlBoolean( true ) ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_MultivariateRealNodeValTreeTranslation::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_MultivariateRealNodeValTreeTranslation::printValue(std::ostream &o) const {
    
    o << "Move_MultivariateRealNodeValTreeTranslation(";
    if (process != NULL) {
        o << process->getName();
    }
    else {
        o << "?";
    }
    if (lambda != NULL) {
        o << lambda->getName();
    }
    else {
        o << "?";
    }
    if (tuning != NULL) {
        o << tuning->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_MultivariateRealNodeValTreeTranslation::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "process" ) {
        process = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "tune" ) {
        tuning = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}
