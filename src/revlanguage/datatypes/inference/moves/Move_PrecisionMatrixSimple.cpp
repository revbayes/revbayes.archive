//
//  Move_PrecisionMatrixSimple.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "Move_PrecisionMatrixSimple.h"


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "PrecisionMatrix.h"
#include "PrecisionMatrixSimpleMove.h"
#include "RealSymmetricMatrix.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Move_PrecisionMatrixSimple::Move_PrecisionMatrixSimple() : Move() {
    
}

/** Clone object */
Move_PrecisionMatrixSimple* Move_PrecisionMatrixSimple::clone(void) const {
    
	return new Move_PrecisionMatrixSimple(*this);
}


void Move_PrecisionMatrixSimple::constructInternalObject( void ) {
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
const std::string& Move_PrecisionMatrixSimple::getClassName(void) {
    
    static std::string rbClassName = "Move_VectorSingleElement";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_PrecisionMatrixSimple::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& Move_PrecisionMatrixSimple::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, RealSymmetricMatrix::getClassTypeSpec() ) );
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
const TypeSpec& Move_PrecisionMatrixSimple::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_PrecisionMatrixSimple::printValue(std::ostream &o) const {
    
    o << "Move_PrecisionMatrixSimple(";
    if (mat != NULL) {
        o << mat->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_PrecisionMatrixSimple::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
