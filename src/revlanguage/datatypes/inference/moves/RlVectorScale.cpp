#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "Natural.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RlVectorScale.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorScaleMove.h"


using namespace RevLanguage;

VectorScale::VectorScale() : Move() {
    
}


/** Clone object */
VectorScale* VectorScale::clone(void) const {
    
	return new VectorScale(*this);
}


void VectorScale::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double l = static_cast<const RealPos &>( lambda->getValue() ).getValue();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    RevBayesCore::TypedDagNode<std::vector<double> >* tmp = static_cast<const Vector<RealPos> &>( x->getValue() ).getValueNode();
    RevBayesCore::StochasticNode<std::vector<double> > *n = static_cast<RevBayesCore::StochasticNode<std::vector<double> > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getValue() ).getValue();
    value = new RevBayesCore::VectorScaleMove(n, l, t, w);
}


/** Get class name of object */
const std::string& VectorScale::getClassName(void) { 
    
    static std::string rbClassName = "VectorScale";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& VectorScale::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& VectorScale::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Vector<RealPos>::getClassTypeSpec() ) );
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
const TypeSpec& VectorScale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void VectorScale::printValue(std::ostream &o) const {
    
    o << "VectorScale(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void VectorScale::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
