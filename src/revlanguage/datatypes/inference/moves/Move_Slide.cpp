#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "Move_Slide.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_Slide::Move_Slide() : Move() {
    
}


/** Clone object */
Move_Slide* Move_Slide::clone(void) const {
    
	return new Move_Slide(*this);
}


void Move_Slide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<double>* tmp = static_cast<const Real &>( x->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *n = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmp );
    value = new RevBayesCore::SlidingMove(n, d, t, w);
    
}


/** Get Rev type of object */
const std::string& Move_Slide::getClassType(void) { 
    
    static std::string revType = "Move_Slide";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_Slide::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_Slide::getParameterRules(void) const {
    
    static MemberRules slidingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        slidingMoveMemberRules.push_back( new ArgumentRule( "x"    , Real::getClassTypeSpec()     , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        slidingMoveMemberRules.push_back( new ArgumentRule( "delta", RealPos::getClassTypeSpec()  , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RealPos(1.0) ) );
        slidingMoveMemberRules.push_back( new ArgumentRule( "tune" , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        slidingMoveMemberRules.insert( slidingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        rulesSet = true;
    }
    
    return slidingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_Slide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}




/** Get type spec */
void Move_Slide::printValue(std::ostream &o) const {
    
    o << "Move_Slide";
    
}


/** Set a member variable */
void Move_Slide::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) 
    {
        x = var;
    }
    else if ( name == "delta" ) 
    {
        delta = var;
    }
    else if ( name == "tune" ) 
    {
        tune = var;
    }
    else 
    {
        Move::setConstParameter(name, var);
    }
    
}
