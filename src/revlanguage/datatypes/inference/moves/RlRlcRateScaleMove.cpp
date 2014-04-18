#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "BranchRateJumpProcess.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlRlcRateScaleMove.h"
#include "RlcRateScaleMove.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
RlcRateScaleMove::RlcRateScaleMove() : Move() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the move. 
 */
RlcRateScaleMove* RlcRateScaleMove::clone(void) const 
{
    
	return new RlcRateScaleMove(*this);
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
void RlcRateScaleMove::constructInternalObject( void ) 
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double w                                                    = static_cast<const RealPos &>( weight->getValue() ).getValue();
    double l                                                    = static_cast<const RealPos &>( weight->getValue() ).getValue();
    bool t                                                      = static_cast<const RlBoolean &>( weight->getValue() ).getValue();
    RevBayesCore::TypedDagNode< std::vector<double> > *tmp      = static_cast<const Vector<RealPos> &>( v->getValue() ).getValueNode();
    RevBayesCore::StochasticNode< std::vector<double> > *n      = static_cast<RevBayesCore::StochasticNode<std::vector<double> > *>( tmp );
    
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
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& RlcRateScaleMove::getClassName(void) 
{ 
    
    static std::string rbClassName = "RlcRateScaleMove";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& RlcRateScaleMove::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
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
const MemberRules& RlcRateScaleMove::getMemberRules(void) const 
{
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Vector<RealPos>::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "lambda", false, RealPos::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune", false, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
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
const TypeSpec& RlcRateScaleMove::getTypeSpec( void ) const 
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



void RlcRateScaleMove::printValue(std::ostream &o) const {
    
    o << "RlcRateScaleMove(";
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
void RlcRateScaleMove::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) 
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
        Move::setConstMemberVariable(name, var);
    }
    
}
