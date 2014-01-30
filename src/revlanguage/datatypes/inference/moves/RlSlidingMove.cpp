#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlSlidingMove.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

SlidingMove::SlidingMove() : Move() {
    
}


/** Clone object */
SlidingMove* SlidingMove::clone(void) const {
    
	return new SlidingMove(*this);
}


void SlidingMove::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double d = static_cast<const Real &>( delta->getValue() ).getValue();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    bool t = static_cast<const RlBoolean &>( tune->getValue() ).getValue();
    RevBayesCore::TypedDagNode<double>* tmp = static_cast<const Real &>( x->getValue() ).getValueNode();
    RevBayesCore::ContinuousStochasticNode *n = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmp );
    value = new RevBayesCore::SlidingMove(n, d, t, w);
    
}


/** Get class name of object */
const std::string& SlidingMove::getClassName(void) { 
    
    static std::string rbClassName = "Sliding move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SlidingMove::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& SlidingMove::getMemberRules(void) const {
    
    static MemberRules slidingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        slidingMoveMemberRules.push_back( new ArgumentRule( "x", false, Real::getClassTypeSpec() ) );
        slidingMoveMemberRules.push_back( new ArgumentRule( "delta", true, Real::getClassTypeSpec() , new Real(1.0) ) );
        slidingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        slidingMoveMemberRules.insert( slidingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        rulesSet = true;
    }
    
    return slidingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& SlidingMove::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}




/** Get type spec */
void SlidingMove::printValue(std::ostream &o) const {
    
    o << "SlidingMove";
    
}


/** Set a member variable */
void SlidingMove::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
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
        Move::setConstMemberVariable(name, var);
    }
    
}
