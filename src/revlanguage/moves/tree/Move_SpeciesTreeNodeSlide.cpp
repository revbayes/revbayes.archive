#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_SpeciesTreeNodeSlide.h"
#include "SpeciesNodeTimeSlideUniformProposal.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Move_SpeciesTreeNodeSlide::Move_SpeciesTreeNodeSlide() : Move()
{
    
    // add method for call "addGeneTreeVariable" as a function
    ArgumentRules* addGeneTreeArgRules = new ArgumentRules();
    addGeneTreeArgRules->push_back( new ArgumentRule( "geneTree" , TimeTree::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
    methods.addFunction("addGeneTreeVariable",  new MemberProcedure( RlUtils::Void, addGeneTreeArgRules) );
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the move.
 */
Move_SpeciesTreeNodeSlide* Move_SpeciesTreeNodeSlide::clone(void) const
{
    
    return new Move_SpeciesTreeNodeSlide(*this);
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
void Move_SpeciesTreeNodeSlide::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tmp = static_cast<const TimeTree &>( speciesTree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *st = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    
    std::vector< RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *> gt;
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<RevBayesCore::TimeTree> >* tmp_gt = static_cast<const ModelVector<TimeTree> &>( geneTrees->getRevObject() ).getDagNode();
    std::set<const RevBayesCore::DagNode*> parents = tmp_gt->getParents();
    for (std::set<const RevBayesCore::DagNode*>::const_iterator it = parents.begin(); it != parents.end(); ++it)
    {
        const RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *theNode = dynamic_cast< const RevBayesCore::StochasticNode<RevBayesCore::TimeTree>* >( *it );
        if ( theNode != NULL )
        {
            gt.push_back( const_cast< RevBayesCore::StochasticNode<RevBayesCore::TimeTree>* >( theNode ) );
        }
        else
        {
            throw RbException("Could not create a mvSpeciesNodeTimeSlideUniform because the node isn't a vector of stochastic nodes.");
        }
    }
    
    //    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tmp_gt = static_cast<const TimeTree &>( geneTrees->getRevObject() ).getDagNode();
    //    gt.push_back( static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp_gt ) );
    
    
    RevBayesCore::Proposal *p = new RevBayesCore::SpeciesNodeTimeSlideUniformProposal(st,gt);
    value = new RevBayesCore::MetropolisHastingsMove(p,w);
    
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Move_SpeciesTreeNodeSlide::getClassType(void)
{
    
    static std::string revType = "Move_SpeciesTreeNodeSlide";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Move_SpeciesTreeNodeSlide::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the scale move are:
 * (1) the variable which must be a time-tree.
 *
 * \return The member rules.
 */
const MemberRules& Move_SpeciesTreeNodeSlide::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ArgumentRule( "speciesTree", TimeTree::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
        //        memberRules.push_back( new ArgumentRule( "geneTrees"  , TimeTree::getClassTypeSpec()             , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "geneTrees"  , ModelVector<TimeTree>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::DETERMINISTIC ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Move_SpeciesTreeNodeSlide::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevPtr<RevVariable> Move_SpeciesTreeNodeSlide::executeMethod(const std::string& name, const std::vector<Argument>& args, bool &found)
{
    
    if ( name == "addGeneTreeVariable" )
    {
        found = true;
        
        RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tmp = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *gt = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
        
        RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
        RevBayesCore::SpeciesNodeTimeSlideUniformProposal &p = static_cast<RevBayesCore::SpeciesNodeTimeSlideUniformProposal&>( m->getProposal() );
        p.addGeneTree( gt );
        
        return NULL;
    }
    else if ( name == "removeGeneTreeVariable" )
    {
        found = true;
        
        RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tmp = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *gt = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
        
        RevBayesCore::MetropolisHastingsMove *m = static_cast<RevBayesCore::MetropolisHastingsMove*>(this->value);
        RevBayesCore::SpeciesNodeTimeSlideUniformProposal &p = static_cast<RevBayesCore::SpeciesNodeTimeSlideUniformProposal&>( m->getProposal() );
        p.removeGeneTree( gt );
        
        return NULL;
    }
    
    return Move::executeMethod( name, args, found );
}



/**
 * Print the value for the user.
 */
void Move_SpeciesTreeNodeSlide::printValue(std::ostream &o) const
{
    
    o << "SpeciesNodeTimeSlideUniform(";
    if (speciesTree != NULL)
    {
        o << speciesTree->getName();
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
void Move_SpeciesTreeNodeSlide::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "speciesTree" )
    {
        speciesTree = var;
    }
    else if ( name == "geneTrees" )
    {
        geneTrees = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
    
}




