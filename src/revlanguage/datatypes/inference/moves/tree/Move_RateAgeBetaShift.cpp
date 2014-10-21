#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ModelVector.h"
#include "Move_RateAgeBetaShift.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorFunction.h"


using namespace RevLanguage;

Move_RateAgeBetaShift::Move_RateAgeBetaShift() : Move() {
    
}


/** Clone object */
Move_RateAgeBetaShift* Move_RateAgeBetaShift::clone(void) const {
    
	return new Move_RateAgeBetaShift(*this);
}


void Move_RateAgeBetaShift::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    bool at = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tmpRates = static_cast<const ModelVector<RealPos> &>( rates->getRevObject() ).getDagNode();
    std::vector< RevBayesCore::StochasticNode<double> *> rates;
    RevBayesCore::DeterministicNode< RevBayesCore::RbVector<double> >*dnode = static_cast< RevBayesCore::DeterministicNode< RevBayesCore::RbVector<double> > *>( tmpRates );
    const std::vector<const RevBayesCore::TypedDagNode<double>* >& pars = static_cast< const RevBayesCore::VectorFunction<double> &>( dnode->getFunction() ).getParams();

    for (size_t i = 0; i < pars.size(); ++i)
    {
        rates.push_back( const_cast<RevBayesCore::StochasticNode<double>* >(static_cast<const RevBayesCore::StochasticNode<double>* >( pars[i] ) ) );
    }
    
    value = new RevBayesCore::RateAgeBetaShift(t, rates, d, at, w);
}


/** Get Rev type of object */
const std::string& Move_RateAgeBetaShift::getClassType(void) { 
    
    static std::string revType = "Move_RateAgeBetaShift";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_RateAgeBetaShift::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_RateAgeBetaShift::getParameterRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        nniMemberRules.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec()            , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        nniMemberRules.push_back( new ArgumentRule( "rates", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        nniMemberRules.push_back( new ArgumentRule( "delta", RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0) ) );
        nniMemberRules.push_back( new ArgumentRule( "tune" , RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
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
void Move_RateAgeBetaShift::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        Move::setConstParameter(name, var);
    }
}
