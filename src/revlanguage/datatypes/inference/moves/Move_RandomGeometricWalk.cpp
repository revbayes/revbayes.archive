#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Integer.h"
#include "Move_RandomGeometricWalk.h"
#include "Probability.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Move_RandomGeometricWalk::Move_RandomGeometricWalk() : Move() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Move_RandomGeometricWalk* Move_RandomGeometricWalk::clone(void) const 
{
    
	return new Move_RandomGeometricWalk(*this);
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
void Move_RandomGeometricWalk::constructInternalObject( void ) 
{
    // we free the memory first
    delete value;
    
    // now allocate a new random-geometric-walk move
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<int>* tmp = static_cast<const Integer &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<int> *n = static_cast<RevBayesCore::StochasticNode<int> *>( tmp );
    double q = static_cast<const Probability &>( p->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();

    // finally create the internal move object
    value = new RevBayesCore::RandomGeometricWalkMove(n, q, t, w);
    
}


/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& Move_RandomGeometricWalk::getClassName(void) 
{ 
    
    static std::string rbClassName = "Move_RandomGeometricWalk";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Move_RandomGeometricWalk::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the scale move are:
 * (1) the variable which must be an integer.
 *
 * \return The member rules.
 */
const MemberRules& Move_RandomGeometricWalk::getMemberRules(void) const 
{
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Integer::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "p", true, Probability::getClassTypeSpec(), new Real(0.5) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune", true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
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
const TypeSpec& Move_RandomGeometricWalk::getTypeSpec( void ) const 
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



void Move_RandomGeometricWalk::printValue(std::ostream &o) const {
    
    o << "Move_RandomGeometricWalk(";
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
void Move_RandomGeometricWalk::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "x" ) 
    {
        x = var;
    }
    else if ( name == "p" ) 
    {
        p = var;
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
