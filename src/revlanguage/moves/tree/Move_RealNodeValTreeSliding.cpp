/* 
 * File:   Move_RealNodeValTreeSliding.cpp
 * Author: nl
 * 
 * Created on 26 juillet 2014, 13:31
 */

#include "Move_RealNodeValTreeSliding.h"

#include "ArgumentRules.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "RlRealNodeValTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_RealNodeValTreeSliding::Move_RealNodeValTreeSliding() : Move() {
    
}


/** Clone object */
Move_RealNodeValTreeSliding* Move_RealNodeValTreeSliding::clone(void) const {
    
	return new Move_RealNodeValTreeSliding(*this);
}


void Move_RealNodeValTreeSliding::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::RealNodeContainer> *tmp = static_cast<const RealNodeValTree &>( process->getRevObject() ).getDagNode();
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tuning->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::RealNodeContainer> *p = static_cast<RevBayesCore::StochasticNode<RevBayesCore::RealNodeContainer> *>( tmp );
    value = new RevBayesCore::RealNodeContainerSlidingMove(p, l, t, w);

}


/** Get class name of object */
const std::string& Move_RealNodeValTreeSliding::getClassType(void) { 
    
    static std::string revClassType = "Move_RealNodeValTreeSliding";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_RealNodeValTreeSliding::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_RealNodeValTreeSliding::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "process", RealNodeValTree::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "lambda" , RealPos::getClassTypeSpec()        , ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RealPos( 1.0 ) ) );
        memberRules.push_back( new ArgumentRule( "tune"   , RlBoolean::getClassTypeSpec()      , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_RealNodeValTreeSliding::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_RealNodeValTreeSliding::printValue(std::ostream &o) const {
    
    o << "Move_RealNodeValTreeSliding(";
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
void Move_RealNodeValTreeSliding::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
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

