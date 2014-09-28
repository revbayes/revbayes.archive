#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "DPPScaleCatValsMove.h"
#include "ModelVector.h"
#include "Move_DPPScaleCatValsMove.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_DPPScaleCatValsMove::Move_DPPScaleCatValsMove() : Move() {
    
}


/** Clone object */
Move_DPPScaleCatValsMove* Move_DPPScaleCatValsMove::clone(void) const {
    
	return new Move_DPPScaleCatValsMove(*this);
}


void Move_DPPScaleCatValsMove::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new vector-scale move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tmp = static_cast<const ModelVector<RealPos> &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< RevBayesCore::RbVector<double> > *sn = static_cast<RevBayesCore::StochasticNode< RevBayesCore::RbVector<double> > *>( tmp );
    
    value = new RevBayesCore::DPPScaleCatValsMove(sn, l, w);
}


/** Get Rev type of object */
const std::string& Move_DPPScaleCatValsMove::getClassType(void) { 
    
    static std::string revType = "Move_DPPScaleCatValsMove";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_DPPScaleCatValsMove::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_DPPScaleCatValsMove::getMemberRules(void) const {
    
    static MemberRules dppMove;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        dppMove.push_back( new ArgumentRule( "x"     , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        dppMove.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new Real(1.0) ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        dppMove.insert( dppMove.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return dppMove;
}

/** Get type spec */
const TypeSpec& Move_DPPScaleCatValsMove::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_DPPScaleCatValsMove::printValue(std::ostream &o) const {
    
    o << "Move_DPPScaleCatValsMove(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_DPPScaleCatValsMove::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
