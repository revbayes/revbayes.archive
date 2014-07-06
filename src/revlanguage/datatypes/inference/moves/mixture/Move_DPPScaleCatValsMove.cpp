#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "DPPScaleCatValsMove.h"
#include "Move_DPPScaleCatValsMove.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


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
    RevBayesCore::TypedDagNode<std::vector<double> >* tmp = static_cast<const Vector<RealPos> &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< std::vector<double> > *sn = static_cast<RevBayesCore::StochasticNode<std::vector<double> > *>( tmp );
    
    value = new RevBayesCore::DPPScaleCatValsMove(sn, l, w);
}


/** Get class name of object */
const std::string& Move_DPPScaleCatValsMove::getClassName(void) { 
    
    static std::string rbClassName = "Move_DPPScaleCatValsMove";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_DPPScaleCatValsMove::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_DPPScaleCatValsMove::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Vector<RealPos>::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
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
