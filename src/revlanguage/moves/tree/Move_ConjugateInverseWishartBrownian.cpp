/* 
 * File:   Move_ConjugateInverseWishartBrownian.cpp
 * Author: nl
 * 
 * Created on 23 juillet 2014, 17:44
 */

#include "Move_ConjugateInverseWishartBrownian.h"

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RevObject.h"
#include "RlMultivariateRealNodeValTree.h"
#include "RealSymmetricMatrix.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Natural.h"
#include "TimeTree.h"
#include "RlTimeTree.h"


using namespace RevLanguage;

Move_ConjugateInverseWishartBrownian::Move_ConjugateInverseWishartBrownian() : Move() {
    
}


/** Clone object */
Move_ConjugateInverseWishartBrownian* Move_ConjugateInverseWishartBrownian::clone(void) const {
    
	return new Move_ConjugateInverseWishartBrownian(*this);
}


void Move_ConjugateInverseWishartBrownian::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixRealSymmetric> *sig = static_cast<const RealSymmetricMatrix &>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::MultivariateRealNodeContainer> *proc = static_cast<const MultivariateRealNodeValTree &>( process->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double> *kap = static_cast<const RealPos &>( kappa->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int> *deg = static_cast<const Natural &>( df->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tree = static_cast<const TimeTree &>( tau->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::MatrixRealSymmetric> *s = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MatrixRealSymmetric> *>( sig );
    RevBayesCore::StochasticNode<RevBayesCore::MultivariateRealNodeContainer> *p = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MultivariateRealNodeContainer> *>( proc );
    RevBayesCore::StochasticNode<double> *k = static_cast<RevBayesCore::StochasticNode<double> *>( kap );
    RevBayesCore::StochasticNode<int> *d = static_cast<RevBayesCore::StochasticNode<int> *>( deg );
//    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tree );
       
//    value = new RevBayesCore::ConjugateInverseWishartBrownianMove(s, p, t, k, d, w);
    value = new RevBayesCore::ConjugateInverseWishartBrownianMove(s, p, k, d, w);
    
}

/** Get class name of object */
const std::string& Move_ConjugateInverseWishartBrownian::getClassType(void) { 
    
    static std::string revClassType = "Move_ConjugateInverseWishartBrownian";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_ConjugateInverseWishartBrownian::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}


/** Return member rules (no members) */
const MemberRules& Move_ConjugateInverseWishartBrownian::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "sigma"  , RealSymmetricMatrix::getClassTypeSpec()        , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "process", MultivariateRealNodeValTree::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
//        nniMemberRules.push_back( new ArgumentRule( "tau", false, TimeTree::getClassTypeSpec() ) );        
        memberRules.push_back( new ArgumentRule( "kappa"  , RealPos::getClassTypeSpec()                    , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "df"     , Natural::getClassTypeSpec()                    , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_ConjugateInverseWishartBrownian::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_ConjugateInverseWishartBrownian::printValue(std::ostream &o) const {
    
    o << "Move_ConjugateInverseWishartBrownian(";
    if (sigma != NULL) {
        o << sigma->getName();
    }
    else {
        o << "?";
    }
    o << ",";
    if (process != NULL) {
        o << process->getName();
    }
    else {
        o << "?";
    }
    o << ",";
    if (process != NULL) {
        o << process->getName();
    }
    else {
        o << "?";
    }
    o << ",";
    if (kappa != NULL) {
        o << kappa->getName();
    }
    else {
        o << "?";
    }
    o << ",";
    if (df != NULL) {
        o << df->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_ConjugateInverseWishartBrownian::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
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
//    else if ( name == "tau" ) {
//        tau = var;
//    }
    else {
        Move::setConstParameter(name, var);
    }
}
