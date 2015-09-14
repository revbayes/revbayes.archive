#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_UpDownTreeScale.h"
#include "UpDownScaleProposal.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Move_UpDownTreeScale::Move_UpDownTreeScale() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
Move_UpDownTreeScale* Move_UpDownTreeScale::clone(void) const
{
    
    return new Move_UpDownTreeScale(*this);
}


/**
 * Create a new internal move object.
 *
 * This function simply dynamically allocates a new internal move object that is
 * associated with the variable (DAG-node). The internal move object is created by calling its
 * constructor and passing the move-parameters (the variable and other parameters) as arguments of the
 * constructor. The move constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
void Move_UpDownTreeScale::constructInternalObject( void )
{
    
    // we free the memory first
    delete value;
    
    // now allocate a new up-down-scale move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    
//    WorkspaceVector<RevObject>& u = static_cast<WorkspaceVector<RevObject> &>( up->getRevObject() );
//    WorkspaceVector<RevObject>& d = static_cast<WorkspaceVector<RevObject> &>( down->getRevObject() );
    
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    // finally create the internal move object
    RevBayesCore::UpDownScaleProposal *prop = new RevBayesCore::UpDownScaleProposal(l);
    
    Real& uReal = static_cast<Real &>( upScalar->getRevObject() );
    
    if ( uReal != RevNullObject::getInstance() )
    {
        RevBayesCore::StochasticNode<double> *node = static_cast< RevBayesCore::StochasticNode<double> * >( uReal.getDagNode() );
        prop->addUpVariable( node );
    }
    
    TimeTree& uTree = static_cast<TimeTree &>( upTree->getRevObject() );
    
    if ( uTree != RevNullObject::getInstance() )
    {
        RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::TimeTree> * >( uTree.getDagNode() );
        prop->addUpVariable( node );
    }
    
    
    ModelVector<Real>& uVector = static_cast<ModelVector<Real> &>( upVector->getRevObject() );
    
    if ( uVector != RevNullObject::getInstance() )
    {
        RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > * >( uVector.getDagNode() );
        prop->addUpVariable( node );
    }
    
    Real& dReal = static_cast<Real &>( downScalar->getRevObject() );
    
    if ( dReal != RevNullObject::getInstance() )
    {
        RevBayesCore::StochasticNode<double> *node = static_cast< RevBayesCore::StochasticNode<double> * >( dReal.getDagNode() );
        prop->addDownVariable( node );
    }
    
    TimeTree& dTree = static_cast<TimeTree &>( downTree->getRevObject() );
    
    if ( dTree != RevNullObject::getInstance() )
    {
        RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::TimeTree> * >( dTree.getDagNode() );
        prop->addDownVariable( node );
    }
    
    
    ModelVector<Real>& dVector = static_cast<ModelVector<Real> &>( downVector->getRevObject() );
    
    if ( dVector != RevNullObject::getInstance() )
    {
        RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > * >( dVector.getDagNode() );
        prop->addDownVariable( node );
    }
    
    
    value = new RevBayesCore::MetropolisHastingsMove(prop,w,t);
    
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Move_UpDownTreeScale::getClassType(void)
{
    
    static std::string revType = "Move_UpDownTreeScale";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Move_UpDownTreeScale::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the scale move are:
 * (1) the variable which must be an integer.
 *
 * \return The member rules.
 */
const MemberRules& Move_UpDownTreeScale::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "upScalar"    , Real::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
        moveMemberRules.push_back( new ArgumentRule( "upVector"    , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
        moveMemberRules.push_back( new ArgumentRule( "upTree"      , TimeTree::getClassTypeSpec()         , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
        moveMemberRules.push_back( new ArgumentRule( "downScalar"  , Real::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
        moveMemberRules.push_back( new ArgumentRule( "downVector"  , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
        moveMemberRules.push_back( new ArgumentRule( "downTree"    , TimeTree::getClassTypeSpec()         , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
        moveMemberRules.push_back( new ArgumentRule( "lambda"      , RealPos::getClassTypeSpec()          , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RealPos(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"        , RlBoolean::getClassTypeSpec()        , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Move_UpDownTreeScale::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



void Move_UpDownTreeScale::printValue(std::ostream &o) const
{
    
    o << "Move_UpDownTreeScale(";
    if (upScalar != NULL)
    {
        o << upScalar->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
    
}


/**
 * Set a member variable.
 *
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
void Move_UpDownTreeScale::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "upScalar" )
    {
        upScalar = var;
    }
    else if ( name == "upVector" )
    {
        upVector = var;
    }
    else if ( name == "upTree" )
    {
        upTree = var;
    }
    else if ( name == "downScalar" )
    {
        downScalar = var;
    }
    else if ( name == "downVector" )
    {
        downVector = var;
    }
    else if ( name == "downTree" )
    {
        downTree = var;
    }
    else if ( name == "lambda" )
    {
        lambda = var;
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
