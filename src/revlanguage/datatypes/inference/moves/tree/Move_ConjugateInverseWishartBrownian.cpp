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
#include "RlMultivariatePhyloProcess.h"
#include "RealSymmetricMatrix.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Natural.h"


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
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix> *sig = static_cast<const RealSymmetricMatrix &>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::MultivariatePhyloProcess> *proc = static_cast<const MultivariatePhyloProcess &>( process->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double> *kap = static_cast<const RealPos &>( kappa->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int> *deg = static_cast<const Natural &>( df->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::PrecisionMatrix> *s = static_cast<RevBayesCore::StochasticNode<RevBayesCore::PrecisionMatrix> *>( sig );
    RevBayesCore::StochasticNode<RevBayesCore::MultivariatePhyloProcess> *p = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MultivariatePhyloProcess> *>( proc );
    RevBayesCore::StochasticNode<double> *k = static_cast<RevBayesCore::StochasticNode<double> *>( kap );
    RevBayesCore::StochasticNode<int> *d = static_cast<RevBayesCore::StochasticNode<int> *>( deg );
       
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
const MemberRules& Move_ConjugateInverseWishartBrownian::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "sigma", false, RealSymmetricMatrix::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "process", false, MultivariatePhyloProcess::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "kappa", false, RealPos::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "df", false, Natural::getClassTypeSpec() ) );        

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
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
    if (process != NULL) {
        o << process->getName();
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
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_ConjugateInverseWishartBrownian::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
    else {
        Move::setConstMemberVariable(name, var);
    }
}
