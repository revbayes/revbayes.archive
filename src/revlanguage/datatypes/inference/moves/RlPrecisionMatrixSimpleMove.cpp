//
//  RlPrecisionMatrixSimpleMove.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "RlPrecisionMatrixSimpleMove.h"


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "ContinuousStochasticNode.h"
#include "Natural.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "Vector.h"
#include "PrecisionMatrix.h"
#include "RealSymmetricMatrix.h"
#include "RlPrecisionMatrixSimpleMove.h"
#include "PrecisionMatrixSimpleMove.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

PrecisionMatrixSimpleMove::PrecisionMatrixSimpleMove() : Move() {
    
}

/** Clone object */
PrecisionMatrixSimpleMove* PrecisionMatrixSimpleMove::clone(void) const {
    
	return new PrecisionMatrixSimpleMove(*this);
}


void PrecisionMatrixSimpleMove::constructInternalObject( void ) {
    // we free the memory first
    delete value;
  
    // now allocate a new wishart simple move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix>* tmp = static_cast<const RealSymmetricMatrix &>( mat->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::PrecisionMatrix > *matrix = static_cast<RevBayesCore::StochasticNode<RevBayesCore::PrecisionMatrix > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::PrecisionMatrixMove(matrix, l, t, w);
        
}


/** Get class name of object */
const std::string& PrecisionMatrixSimpleMove::getClassName(void) {
    
    static std::string rbClassName = "Move_VectorSingleElement";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& PrecisionMatrixSimpleMove::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& PrecisionMatrixSimpleMove::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Vector<Real>::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& PrecisionMatrixSimpleMove::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PrecisionMatrixSimpleMove::printValue(std::ostream &o) const {
    
    o << "PrecisionMatrixSimpleMove(";
    if (mat != NULL) {
        o << mat->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void PrecisionMatrixSimpleMove::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        mat = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "weight" ) {
        weight = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
