#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_ElementScale.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "ElementScaleProposal.h"


using namespace RevLanguage;

Move_ElementScale::Move_ElementScale() : Move()
{
    
}


/** Clone object */
Move_ElementScale* Move_ElementScale::clone(void) const
{
    
    return new Move_ElementScale(*this);
}


void Move_ElementScale::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new vector-scale move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmp = static_cast<const ModelVector<RealPos> &>( x->getRevObject() ).getDagNode();
    std::set<const RevBayesCore::DagNode*> p = tmp->getParents();
    std::vector< RevBayesCore::StochasticNode<double> *> n;
    for (std::set<const RevBayesCore::DagNode*>::const_iterator it = p.begin(); it != p.end(); ++it)
    {
        const RevBayesCore::StochasticNode<double> *theNode = dynamic_cast< const RevBayesCore::StochasticNode<double>* >( *it );
        if ( theNode != NULL )
        {
            n.push_back( const_cast< RevBayesCore::StochasticNode<double>* >( theNode ) );
        }
        else
        {
            throw RbException("Could not create a mvElementScale because the node isn't a vector of stochastic nodes.");
        }
    }
    
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *prop = new RevBayesCore::ElementScaleProposal(n,l);
    value = new RevBayesCore::MetropolisHastingsMove(prop,w,t);
    
}


/** Get Rev type of object */
const std::string& Move_ElementScale::getClassType(void)
{
    
    static std::string revType = "Move_ElementScale";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_ElementScale::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_ElementScale::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        moveMemberRules.push_back( new ArgumentRule( "x"     , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::DETERMINISTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_ElementScale::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_ElementScale::printValue(std::ostream &o) const
{
    
    o << "Move_VectorScale(";
    if (x != NULL)
    {
        o << x->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_ElementScale::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
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
        Move::setConstParameter(name, var);
    }
}
