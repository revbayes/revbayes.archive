/* 
 * File:   Move_BrownianInverseWishartSliding.cpp
 * Author: nl
 * 
 * Created on 23 juillet 2014, 17:44
 */

#include "Move_BrownianInverseWishartSliding.h"

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RevObject.h"
#include "RlMultivariatePhyloProcess.h"
#include "RealSymmetricMatrix.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Natural.h"


using namespace RevLanguage;

Move_BrownianInverseWishartSliding::Move_BrownianInverseWishartSliding() : Move() {
    
}


/** Clone object */
Move_BrownianInverseWishartSliding* Move_BrownianInverseWishartSliding::clone(void) const {
    
	return new Move_BrownianInverseWishartSliding(*this);
}


void Move_BrownianInverseWishartSliding::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::MultivariatePhyloProcess> *proc = static_cast<const MultivariatePhyloProcess &>( process->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix> *sig = static_cast<const RealSymmetricMatrix &>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double> *kap = static_cast<const RealPos &>( kappa->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int> *deg = static_cast<const Natural &>( df->getRevObject() ).getDagNode();
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tuning->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::MultivariatePhyloProcess> *p = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MultivariatePhyloProcess> *>( proc );
    RevBayesCore::StochasticNode<RevBayesCore::PrecisionMatrix> *s = static_cast<RevBayesCore::StochasticNode<RevBayesCore::PrecisionMatrix> *>( sig );
    RevBayesCore::StochasticNode<double> *k = static_cast<RevBayesCore::StochasticNode<double> *>( kap );
    RevBayesCore::StochasticNode<int> *d = static_cast<RevBayesCore::StochasticNode<int> *>( deg );

    std::vector<RevBayesCore::DagNode*> n;
    
    n.push_back(p);
    n.push_back(s);
    n.push_back(k);
    n.push_back(d);
    
 //       rates.push_back(const_cast<RevBayesCore::StochasticNode<double>*> (static_cast<const RevBayesCore::StochasticNode<double>*> (pars[i])));
   
    value = new RevBayesCore::BrownianInverseWishartSlidingMove(n, l, t, w);
//    value = new RevBayesCore::BrownianInverseWishartSlidingMove(p, s, k, l, t, w);
    
}


/** Get class name of object */
const std::string& Move_BrownianInverseWishartSliding::getClassType(void) { 
    
    static std::string revClassType = "Move_BrownianInverseWishartSliding";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_BrownianInverseWishartSliding::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_BrownianInverseWishartSliding::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "process", false, MultivariatePhyloProcess::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "sigma", false, RealSymmetricMatrix::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "kappa", false, RealPos::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "df", false, Natural::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec(), new RealPos( 1.0 ) ) );
        nniMemberRules.push_back( new ArgumentRule( "tune", true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& Move_BrownianInverseWishartSliding::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_BrownianInverseWishartSliding::printValue(std::ostream &o) const {
    
    o << "Move_BrownianInverseWishartSliding(";
    if (process != NULL) {
        o << process->getName();
    }
    else {
        o << "?";
    }
    if (sigma != NULL) {
        o << sigma->getName();
    }
    else {
        o << "?";
    }
    if (kappa != NULL) {
        o << kappa->getName();
    }
    else {
        o << "?";
    }
    if (df != NULL) {
        o << df->getName();
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
void Move_BrownianInverseWishartSliding::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "process" ) {
        process = var;
    }
    else if ( name == "sigma" ) {
        sigma = var;
    }
    else if ( name == "kappa" ) {
        kappa = var;
    }
    else if ( name == "df" ) {
        df = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "tune" ) {
        tuning = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
