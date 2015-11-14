//
//  Move_MatrixRealSymmetricSlide.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "Move_MatrixRealSymmetricSlide.h"


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "MetropolisHastingsMove.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "MatrixReal.h"
#include "MatrixRealSymmetricSingleElementSlidingProposal.h"
#include "RlMatrixRealSymmetric.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_MatrixRealSymmetricSlide::Move_MatrixRealSymmetricSlide() : Move()
{
    
}

/** Clone object */
Move_MatrixRealSymmetricSlide* Move_MatrixRealSymmetricSlide::clone(void) const
{
    
	return new Move_MatrixRealSymmetricSlide(*this);
}


void Move_MatrixRealSymmetricSlide::constructInternalObject( void )
{
    // we free the memory first
    delete value;
  
    // now allocate a new wishart simple move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* tmp = static_cast<const MatrixRealSymmetric &>( mat->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::MatrixReal > *matrix = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MatrixReal > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *p = new RevBayesCore::MatrixRealSymmetricSingleElementSlidingProposal(matrix,l);
    value = new RevBayesCore::MetropolisHastingsMove(p,w,t);
        
}


/** Get class name of object */
const std::string& Move_MatrixRealSymmetricSlide::getClassType(void)
{
    
    static std::string revClassType = "Move_MatrixRealSymmetricSlideMove";
    
	return revClassType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_MatrixRealSymmetricSlide::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_MatrixRealSymmetricSlide::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "mvSymmetricMatrixElementSlide";
    
    return c_name;
}



/** Return member rules (no members) */
const MemberRules& Move_MatrixRealSymmetricSlide::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        moveMemberRules.push_back( new ArgumentRule( "x"     , MatrixRealSymmetric::getClassTypeSpec(), "The matrix variable on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()            , "The sliding window size.", ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec()          , "Should we tune the move during burnin?", ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_MatrixRealSymmetricSlide::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_MatrixRealSymmetricSlide::printValue(std::ostream &o) const
{
    
    o << "Move_MatrixRealSymmetricSlide(";
    if (mat != NULL)
    {
        o << mat->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_MatrixRealSymmetricSlide::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
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
        Move::setConstParameter(name, var);
    }
}
