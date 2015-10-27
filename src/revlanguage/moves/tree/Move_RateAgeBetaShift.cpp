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
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    bool at = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tmpRates = static_cast<const ModelVector<RealPos> &>( rates->getRevObject() ).getDagNode();
    std::vector< RevBayesCore::StochasticNode<double> *> rates;
    RevBayesCore::DeterministicNode< RevBayesCore::RbVector<double> >*dnode = static_cast< RevBayesCore::DeterministicNode< RevBayesCore::RbVector<double> > *>( tmpRates );

    RevBayesCore::VectorFunction<double>* funcVec = dynamic_cast<RevBayesCore::VectorFunction<double>*>( &dnode->getFunction() );
    if ( funcVec == NULL )
    {
        throw RbException("Wrong argument type for the rates vector. We expect a vector of iid elements.");
    }
    const std::vector<const RevBayesCore::TypedDagNode<double>* >& pars = funcVec->getVectorParameters();

    for (std::vector<const RevBayesCore::TypedDagNode<double>* >::const_iterator it = pars.begin(); it != pars.end(); ++it)
    {
        rates.push_back( const_cast<RevBayesCore::StochasticNode<double>* >(static_cast<const RevBayesCore::StochasticNode<double>* >( *it ) ) );
    }
    
    value = new RevBayesCore::RateAgeBetaShift(t, rates, d, at, w);
}


/** Get Rev type of object */
const std::string& Move_RateAgeBetaShift::getClassType(void)
{
    
    static std::string revType = "Move_RateAgeBetaShift";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_RateAgeBetaShift::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_RateAgeBetaShift::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        moveMemberRules.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec()            , "The tree on which this move operates on.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "rates", ModelVector<RealPos>::getClassTypeSpec(), "The vector of per-branch rates (from a relaxed clock).", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC)  );
        moveMemberRules.push_back( new ArgumentRule( "delta", RealPos::getClassTypeSpec()             , "The concentration of the move on the previous age.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune" , RlBoolean::getClassTypeSpec()           , "Should we tune this move during burnin?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
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
void Move_RateAgeBetaShift::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
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
