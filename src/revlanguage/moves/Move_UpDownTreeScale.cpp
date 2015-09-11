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
    
    // add member methods
    
    // first, the argument rules
    ArgumentRules* addTreeArgRules                  = new ArgumentRules();
    ArgumentRules* addScalarArgRules                = new ArgumentRules();
    ArgumentRules* addModelVectorArgRules           = new ArgumentRules();
    ArgumentRules* addCompositeVectorArgRules       = new ArgumentRules();
    ArgumentRules* removeTreeArgRules               = new ArgumentRules();
    ArgumentRules* removeScalarArgRules             = new ArgumentRules();
    ArgumentRules* removeModelVectorArgRules        = new ArgumentRules();
    ArgumentRules* removeCompositeVectorArgRules    = new ArgumentRules();
    
    
    // next, set the specific arguments
    addTreeArgRules->push_back(                 new ArgumentRule( "tree", TimeTree::getClassTypeSpec(),             ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
    addTreeArgRules->push_back(                 new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    addScalarArgRules->push_back(               new ArgumentRule( "var" , Real::getClassTypeSpec(),                 ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
    addScalarArgRules->push_back(               new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    addModelVectorArgRules->push_back(          new ArgumentRule( "var" , ModelVector<Real>::getClassTypeSpec(),    ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
    addModelVectorArgRules->push_back(          new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    addCompositeVectorArgRules->push_back(      new ArgumentRule( "var" , ModelVector<Real>::getClassTypeSpec(),    ArgumentRule::BY_REFERENCE, ArgumentRule::DETERMINISTIC ) );
    addCompositeVectorArgRules->push_back(      new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    removeTreeArgRules->push_back(              new ArgumentRule( "tree", TimeTree::getClassTypeSpec(),             ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
    removeTreeArgRules->push_back(              new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    removeScalarArgRules->push_back(            new ArgumentRule( "var" , Real::getClassTypeSpec(),                 ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
    removeScalarArgRules->push_back(            new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    removeModelVectorArgRules->push_back(       new ArgumentRule( "var" , ModelVector<Real>::getClassTypeSpec(),    ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
    removeModelVectorArgRules->push_back(       new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    removeCompositeVectorArgRules->push_back(   new ArgumentRule( "var" , ModelVector<Real>::getClassTypeSpec(),    ArgumentRule::BY_REFERENCE, ArgumentRule::DETERMINISTIC ) );
    removeCompositeVectorArgRules->push_back(   new ArgumentRule( "up"  , RlBoolean::getClassTypeSpec(),            ArgumentRule::BY_VALUE                                  ) );
    
    
    // finally, create the member
    methods.addFunction("addVariable",          new MemberProcedure( RlUtils::Void, addTreeArgRules) );
    methods.addFunction("addVariable",          new MemberProcedure( RlUtils::Void, addScalarArgRules) );
    methods.addFunction("addVariable",          new MemberProcedure( RlUtils::Void, addModelVectorArgRules) );
    methods.addFunction("addVariable",          new MemberProcedure( RlUtils::Void, addCompositeVectorArgRules) );
    methods.addFunction("removeVariable",       new MemberProcedure( RlUtils::Void, removeTreeArgRules) );
    methods.addFunction("removeVariable",       new MemberProcedure( RlUtils::Void, removeScalarArgRules) );
    methods.addFunction("removeVariable",       new MemberProcedure( RlUtils::Void, removeModelVectorArgRules) );
    methods.addFunction("removeVariable",       new MemberProcedure( RlUtils::Void, removeCompositeVectorArgRules) );
    
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
    
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    // finally create the internal move object
    RevBayesCore::UpDownScaleProposal *prop = new RevBayesCore::UpDownScaleProposal(l);
    
    value = new RevBayesCore::MetropolisHastingsMove(prop,w,t);
    
}


RevPtr<RevVariable> Move_UpDownTreeScale::executeMethod(const std::string& name, const std::vector<Argument>& args, bool &found)
{
    
    if ( name == "addVariable" )
    {
        found = true;
        
        TimeTree* uTree = static_cast<TimeTree *>( &args[0].getVariable()->getRevObject() );
        Real* uReal = static_cast<Real *>( &args[0].getVariable()->getRevObject() );
        ModelVector<Real>* uVector = static_cast<ModelVector<Real> *>( &args[0].getVariable()->getRevObject() );
        bool up = static_cast<RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();
        
        if ( uTree != NULL )
        {
            RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::TimeTree> * >( uTree->getDagNode() );
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            prop.addVariable( node, up );
        }
        else if ( uReal != NULL )
        {
            RevBayesCore::StochasticNode<double> *node = static_cast< RevBayesCore::StochasticNode<double> * >( uReal->getDagNode() );
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            prop.addVariable( node, up );
        }
        else if ( uVector != NULL && uVector->getDagNode()->isStochastic() == true )
        {
            RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > * >( uVector->getDagNode() );
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            prop.addVariable( node, up );
        }
        else if ( uVector != NULL && uVector->getDagNode()->isStochastic() == false )
        {
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            
            RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* deterministic_vector = uVector->getDagNode();
            std::set<const RevBayesCore::DagNode*> parents = deterministic_vector->getParents();
            for (std::set<const RevBayesCore::DagNode*>::const_iterator it = parents.begin(); it != parents.end(); ++it)
            {
                const RevBayesCore::StochasticNode<double> *the_node = dynamic_cast< const RevBayesCore::StochasticNode<double>* >( *it );
                if ( the_node != NULL )
                {
                    prop.addVariable( const_cast<RevBayesCore::StochasticNode<double> *>( the_node ), up );
                }
                else
                {
                    throw RbException("Could not remove the node isn't a vector of stochastic nodes.");
                }
            }
            
        }
        else
        {
            throw RbException("A problem occured when trying to add " + args[0].getVariable()->getName() + " to the move.");
        }
        
        return NULL;
    }
    else if ( name == "removeVariable" )
    {
        found = true;
        
        TimeTree* uTree = static_cast<TimeTree *>( &args[0].getVariable()->getRevObject() );
        Real* uReal = static_cast<Real *>( &args[0].getVariable()->getRevObject() );
        ModelVector<Real>* uVector = static_cast<ModelVector<Real> *>( &args[0].getVariable()->getRevObject() );
        bool up = static_cast<RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();
        
        if ( uTree != NULL )
        {
            RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::TimeTree> * >( uTree->getDagNode() );
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            prop.removeVariable( node, up );
        }
        else if ( uReal != NULL )
        {
            RevBayesCore::StochasticNode<double> *node = static_cast< RevBayesCore::StochasticNode<double> * >( uReal->getDagNode() );
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            prop.removeVariable( node, up );
        }
        else if ( uVector != NULL && uVector->getDagNode()->isStochastic() == true )
        {
            RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *node = static_cast< RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > * >( uVector->getDagNode() );
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            prop.removeVariable( node, up );
        }
        else if ( uVector != NULL && uVector->getDagNode()->isStochastic() == false )
        {
            RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
            RevBayesCore::UpDownScaleProposal &prop = static_cast<RevBayesCore::UpDownScaleProposal&>( m->getProposal() );
            
            RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* deterministic_vector = uVector->getDagNode();
            std::set<const RevBayesCore::DagNode*> parents = deterministic_vector->getParents();
            for (std::set<const RevBayesCore::DagNode*>::const_iterator it = parents.begin(); it != parents.end(); ++it)
            {
                const RevBayesCore::StochasticNode<double> *the_node = dynamic_cast< const RevBayesCore::StochasticNode<double>* >( *it );
                if ( the_node != NULL )
                {
                    prop.removeVariable( const_cast<RevBayesCore::StochasticNode<double> *>( the_node ), up );
                }
                else
                {
                    throw RbException("Could not remove the node isn't a vector of stochastic nodes.");
                }
            }

        }
        else
        {
            throw RbException("A problem occured when trying to add " + args[0].getVariable()->getName() + " to the move.");
        }
        
        return NULL;
    }
    
    return Move::executeMethod( name, args, found );
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
//        moveMemberRules.push_back( new ArgumentRule( "upScalar"    , Real::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
//        moveMemberRules.push_back( new ArgumentRule( "upVector"    , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
//        moveMemberRules.push_back( new ArgumentRule( "upTree"      , TimeTree::getClassTypeSpec()         , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
//        moveMemberRules.push_back( new ArgumentRule( "downScalar"  , Real::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
//        moveMemberRules.push_back( new ArgumentRule( "downVector"  , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
//        moveMemberRules.push_back( new ArgumentRule( "downTree"    , TimeTree::getClassTypeSpec()         , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC, NULL ) );
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
    
    o << "Move_UpDownTreeScale(?)";
    
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
    
    if ( name == "lambda" )
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
