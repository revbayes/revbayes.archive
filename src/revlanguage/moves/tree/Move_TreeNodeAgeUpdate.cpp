#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_TreeNodeAgeUpdate.h"
#include "TreeNodeAgeUpdateProposal.h"
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
Move_TreeNodeAgeUpdate::Move_TreeNodeAgeUpdate() : Move()
{
    
    // add method for call "addGeneTreeVariable" as a function
    ArgumentRules* addGeneTreeArgRules = new ArgumentRules();
    addGeneTreeArgRules->push_back( new ArgumentRule( "geneTree" , TimeTree::getClassTypeSpec(), "A gene tree.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
    methods.addFunction( new MemberProcedure( "addGeneTreeVariable", RlUtils::Void, addGeneTreeArgRules) );
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the move.
 */
Move_TreeNodeAgeUpdate* Move_TreeNodeAgeUpdate::clone(void) const
{
    
    return new Move_TreeNodeAgeUpdate(*this);
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
void Move_TreeNodeAgeUpdate::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *st = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    
    RevBayesCore::Proposal *p = new RevBayesCore::TreeNodeAgeUpdateProposal(st);
    value = new RevBayesCore::MetropolisHastingsMove(p,w);
    
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Move_TreeNodeAgeUpdate::getClassType(void)
{
    
    static std::string rev_type = "Move_TreeNodeAgeUpdate";
    
    return rev_type;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Move_TreeNodeAgeUpdate::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_TreeNodeAgeUpdate::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "TreeNodeAgeSlide";
    
    return c_name;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the scale move are:
 * (1) the variable which must be a time-tree.
 *
 * \return The member rules.
 */
const MemberRules& Move_TreeNodeAgeUpdate::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        memberRules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec(), "A (time-) tree on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC    ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rules_set = true;
    }
    
    return memberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Move_TreeNodeAgeUpdate::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}



/**
 * Print the value for the user.
 */
void Move_TreeNodeAgeUpdate::printValue(std::ostream &o) const
{
    
    o << "SpeciesNodeTimeSlideUniform(";
    if (tree != NULL)
    {
        o << tree->getName();
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
void Move_TreeNodeAgeUpdate::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
    
}




