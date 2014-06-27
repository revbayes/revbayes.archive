#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Move_RateAgeBetaShift.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorFunction.h"


using namespace RevLanguage;

Move_RateAgeBetaShift::Move_RateAgeBetaShift() : Move() {
    
}


/** Clone object */
Move_RateAgeBetaShift* Move_RateAgeBetaShift::clone(void) const {
    
	return new Move_RateAgeBetaShift(*this);
}


void Move_RateAgeBetaShift::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    bool at = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    RevBayesCore::TypedDagNode<std::vector<double> >* tmpRates = static_cast<const Vector<RealPos> &>( rates->getRevObject() ).getDagNode();
    std::vector< RevBayesCore::StochasticNode<double> *> rates;
    RevBayesCore::DeterministicNode<std::vector<double> >*dnode = static_cast< RevBayesCore::DeterministicNode<std::vector<double> > *>( tmpRates );
    const std::vector<const RevBayesCore::TypedDagNode<double>* >& pars = static_cast< const RevBayesCore::VectorFunction<double> &>( dnode->getFunction() ).getParams();

    for (size_t i = 0; i < pars.size(); ++i)
    {
        rates.push_back( const_cast<RevBayesCore::StochasticNode<double>* >(static_cast<const RevBayesCore::StochasticNode<double>* >( pars[i] ) ) );
    }
    
    value = new RevBayesCore::RateAgeBetaShift(t, rates, d, at, w);
}


/** Get class name of object */
const std::string& Move_RateAgeBetaShift::getClassName(void) { 
    
    static std::string rbClassName = "Move_Move_RateAgeBetaShift";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_RateAgeBetaShift::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_RateAgeBetaShift::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "tree", false, TimeTree::getClassTypeSpec() ) );
        nniMemberRules.push_back( new ArgumentRule( "rates", false, Vector<RealPos>::getClassTypeSpec() ) );
        nniMemberRules.push_back( new ArgumentRule( "delta", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        nniMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& Move_RateAgeBetaShift::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_RateAgeBetaShift::printValue(std::ostream &o) const {
    
    o << "Move_RateAgeBetaShift(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_RateAgeBetaShift::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "rates" ) {
        rates = var;
    }
    else if ( name == "delta" ) {
        delta = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
