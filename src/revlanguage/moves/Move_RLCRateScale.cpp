#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BranchRateJumpProcess.h"
#include "ModelVector.h"
#include "Move_RLCRateScale.h"
#include "RbException.h"
#include "RevObject.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlcRateScaleMove.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Move_RLCRateScale::Move_RLCRateScale() : Move() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the move. 
 */
Move_RLCRateScale* Move_RLCRateScale::clone(void) const 
{
    
	return new Move_RLCRateScale(*this);
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
void Move_RLCRateScale::constructInternalObject( void ) 
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double w                                                    = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    double l                                                    = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    bool t                                                      = static_cast<const RlBoolean &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> > *tmp      = static_cast<const ModelVector<RealPos> &>( v->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< RevBayesCore::RbVector<double> > *n      = static_cast<RevBayesCore::StochasticNode< RevBayesCore::RbVector<double> > *>( tmp );
    
    // just to make sure also test that the stochastic node has the correct distribution
    RevBayesCore::BranchRateJumpProcess *brj = dynamic_cast< RevBayesCore::BranchRateJumpProcess* >( &n->getDistribution() );
    if ( brj == NULL )
    {
        throw RbException("The SwitchRateJumpMove can only operate on a random variable drawn from a BranchRateJumpProcess.");
    }
    
    // finally create the internal move object
    value = new RevBayesCore::RlcRateScaleMove(n, l, t, w);
    
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Move_RLCRateScale::getClassType(void) 
{ 
    
    static std::string revType = "Move_RLCRateScale";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Move_RLCRateScale::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the scale move are:
 * (1) the variable which must be a vector of positive reals.
 * (2) the weight how often the move is used.
 *
 * \return The member rules.
 */
const MemberRules& Move_RLCRateScale::getParameterRules(void) const 
{
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        scalingMoveMemberRules.push_back( new ArgumentRule( "x"     , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Move_RLCRateScale::getTypeSpec( void ) const 
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



void Move_RLCRateScale::printValue(std::ostream &o) const {
    
    o << "Move_RLCRateScale(";
    if (v != NULL) 
    {
        o << v->getName();
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
void Move_RLCRateScale::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "x" ) 
    {
        v = var;
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
