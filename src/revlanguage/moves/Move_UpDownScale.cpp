#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_UpDownScale.h"
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
Move_UpDownScale::Move_UpDownScale() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
Move_UpDownScale* Move_UpDownScale::clone(void) const
{
    
    return new Move_UpDownScale(*this);
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
void Move_UpDownScale::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new up-down-scale move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();

    WorkspaceVector<RevObject>& u = static_cast<WorkspaceVector<RevObject> &>( up->getRevObject() );
    WorkspaceVector<RevObject>& d = static_cast<WorkspaceVector<RevObject> &>( down->getRevObject() );
    
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    // finally create the internal move object
    RevBayesCore::UpDownScaleProposal *prop = new RevBayesCore::UpDownScaleProposal(l);
    
    for ( size_t i=0; i<u.size(); ++i)
    {
        RevObject &tmpRevObj = u[i];
    
        // first we check if this variable is a scalar variable
        Real * mdlScalar = dynamic_cast< Real * >( &tmpRevObj );
        if ( mdlScalar != NULL )
        {
            RevBayesCore::TypedDagNode<double>* tmp = mdlScalar->getDagNode();
            RevBayesCore::StochasticNode<double> *n = dynamic_cast<RevBayesCore::StochasticNode<double> *>( tmp );
            
            if ( n == NULL )
            {
                throw RbException("Cannot add a non-stochastic DAG node to the up-down-scale move.");
            }
            
            prop->addUpVariable( n );
        }
        
        // second we check if this variable is a vector variable
        ModelVector<Real> * mdlVector = dynamic_cast< ModelVector<Real> * >( &tmpRevObj );
        if ( mdlVector != NULL )
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmp = mdlVector->getDagNode();
            RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *n = dynamic_cast<RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *>( tmp );
            
            if ( n == NULL )
            {
                throw RbException("Cannot add a non-stochastic DAG node to the up-down-scale move.");
            }
            
            prop->addUpVariable( n );
        }
        
        
        // third we check if this variable is a tree variable
        TimeTree * mdlTree = dynamic_cast< TimeTree * >( &tmpRevObj );
        if ( mdlTree != NULL )
        {
            RevBayesCore::TypedDagNode<RevBayesCore::TimeTree >* tmp = mdlTree->getDagNode();
            RevBayesCore::StochasticNode<RevBayesCore::TimeTree > *n = dynamic_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree > *>( tmp );
            
            if ( n == NULL )
            {
                throw RbException("Cannot add a non-stochastic DAG node to the up-down-scale move.");
            }
            
            prop->addUpVariable( n );
        }
        
    }
    
    // now the down variables
    for ( size_t i=0; i<d.size(); ++i)
    {
        RevObject &tmpRevObj = d[i];
        
        // first we check if this variable is a scalar variable
        Real * mdlScalar = dynamic_cast< Real * >( &tmpRevObj );
        if ( mdlScalar != NULL )
        {
            RevBayesCore::TypedDagNode<double>* tmp = mdlScalar->getDagNode();
            RevBayesCore::StochasticNode<double> *n = dynamic_cast<RevBayesCore::StochasticNode<double> *>( tmp );
            
            if ( n == NULL )
            {
                throw RbException("Cannot add a non-stochastic DAG node to the up-down-scale move.");
            }
            
            prop->addDownVariable( n );
        }
        
        // second we check if this variable is a vector variable
        ModelVector<Real> * mdlVector = dynamic_cast< ModelVector<Real> * >( &tmpRevObj );
        if ( mdlVector != NULL )
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmp = mdlVector->getDagNode();
            RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *n = dynamic_cast<RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *>( tmp );
            
            if ( n == NULL )
            {
                throw RbException("Cannot add a non-stochastic DAG node to the up-down-scale move.");
            }
            
            prop->addDownVariable( n );
        }
        
        
        // third we check if this variable is a tree variable
        TimeTree * mdlTree = dynamic_cast< TimeTree * >( &tmpRevObj );
        if ( mdlTree != NULL )
        {
            RevBayesCore::TypedDagNode<RevBayesCore::TimeTree >* tmp = mdlTree->getDagNode();
            RevBayesCore::StochasticNode<RevBayesCore::TimeTree > *n = dynamic_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree > *>( tmp );
            
            if ( n == NULL )
            {
                throw RbException("Cannot add a non-stochastic DAG node to the up-down-scale move.");
            }
            
            prop->addDownVariable( n );
        }
        
    }
    
    value = new RevBayesCore::MetropolisHastingsMove(prop,w,t);
    
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Move_UpDownScale::getClassType(void)
{
    
    static std::string revType = "Move_UpDownScale";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Move_UpDownScale::getClassTypeSpec(void)
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
const MemberRules& Move_UpDownScale::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "up"  , WorkspaceVector<RevObject>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        moveMemberRules.push_back( new ArgumentRule( "down", WorkspaceVector<RevObject>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        moveMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
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
const TypeSpec& Move_UpDownScale::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



void Move_UpDownScale::printValue(std::ostream &o) const
{
    
    o << "Move_UpDownScale(";
    if (up != NULL)
    {
        o << up->getName();
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
void Move_UpDownScale::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "up" )
    {
        up = var;
    }
    else if ( name == "down" )
    {
        down = var;
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
