#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlUniformTopologyDistribution.h"
#include "UniformTopologyDistribution.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlString.h"
#include "RlTopology.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
UniformTopologyDistribution::UniformTopologyDistribution() : TypedDistribution<Topology>() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the model. 
 */
UniformTopologyDistribution* UniformTopologyDistribution::clone( void ) const 
{

    return new UniformTopologyDistribution(*this);
}


/**
 * Create a new internal distribution object.
 *
 * This function simply dynamically allocates a new internal distribution object that can be 
 * associated with the variable. The internal distribution object is created by calling its
 * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the 
 * constructor. The distribution constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
RevBayesCore::UniformTopologyDistribution* UniformTopologyDistribution::createDistribution( void ) const 
{
    // get the parameters
    int n = static_cast<const Natural &>( numTaxa->getRevObject() ).getDagNode()->getValue();
    const std::vector<std::string> &names = static_cast<const Vector<RlString> &>( taxonNames->getRevObject() ).getDagNode()->getValue();
    RevBayesCore::UniformTopologyDistribution*   d = new RevBayesCore::UniformTopologyDistribution(size_t(n), names);
    
    return d;
}


/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& UniformTopologyDistribution::getClassName(void) 
{ 
    
    static std::string rbClassName = "uniform topology distribution";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& UniformTopologyDistribution::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the uniform topology distribution are:
 * (1) the number of taxa.
 * (2) the names of the taxa.
 *
 * \return The member rules.
 */
const MemberRules& UniformTopologyDistribution::getMemberRules(void) const 
{
    
    static MemberRules distUniformTopologyMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distUniformTopologyMemberRules.push_back( new ArgumentRule( "nTaxa"  , true, Natural::getClassTypeSpec() ) );
        distUniformTopologyMemberRules.push_back( new ArgumentRule( "names"  , true, Vector<RlString>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distUniformTopologyMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& UniformTopologyDistribution::getTypeSpec( void ) const 
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
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
void UniformTopologyDistribution::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "nTaxa" ) 
    {
        numTaxa = var;
    }
    else if ( name == "names" ) 
    {
        taxonNames = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
