#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_uniformTopology.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlString.h"
#include "StochasticNode.h"
#include "UniformTopologyDistribution.h"
#include "Clade.h"
#include "RlClade.h"
#include "RlTaxon.h"

using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_uniformTopology::Dist_uniformTopology() : TypedDistribution<BranchLengthTree>()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Dist_uniformTopology* Dist_uniformTopology::clone( void ) const 
{

    return new Dist_uniformTopology(*this);
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
RevBayesCore::UniformTopologyDistribution* Dist_uniformTopology::createDistribution( void ) const 
{
    // get the parameters
    const std::vector<std::string> &names = static_cast<const ModelVector<RlString> &>( taxonNames->getRevObject() ).getDagNode()->getValue();
    size_t n = names.size();
    
    // get the taxa to simulate either from a vector of rev taxon objects or a vector of names
    std::vector<RevBayesCore::Taxon> t;
    if ( taxa != NULL && taxa->getRevObject() != RevNullObject::getInstance() )
    {
        // rev taxon objects
        t = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    }
    else if ( taxonNames != NULL && taxonNames->getRevObject() != RevNullObject::getInstance() )
    {
        // taxon names
        const std::vector<std::string> &names = static_cast<const ModelVector<RlString> &>( taxonNames->getRevObject() ).getDagNode()->getValue();
        for (size_t i = 0; i < names.size(); ++i)
        {
            t.push_back( RevBayesCore::Taxon( names[i] ) );
        }
    }


	if ( constraints != NULL && constraints->getRevObject() != RevNullObject::getInstance())
    {
		const std::vector<RevBayesCore::Clade> &c   = static_cast<const ModelVector<Clade> &>( constraints->getRevObject() ).getValue();
		RevBayesCore::UniformTopologyDistribution*   d = new RevBayesCore::UniformTopologyDistribution(size_t(n), t, c);
		return d;
	}
    else
    {
        std::vector<RevBayesCore::Clade> c;
		RevBayesCore::UniformTopologyDistribution*   d = new RevBayesCore::UniformTopologyDistribution(size_t(n), t, c);
		return d;
	}    
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_uniformTopology::getClassType(void) 
{ 
    
    static std::string revType = "Dist_uniformTopology";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_uniformTopology::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<BranchLengthTree>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
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
const MemberRules& Dist_uniformTopology::getParameterRules(void) const 
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        memberRules.push_back( new ArgumentRule( "names"  , ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "taxa"  , ModelVector<Taxon>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "constraints", ModelVector<Clade>::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
		
        rulesSet = true;
    }
    
    return memberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_uniformTopology::getTypeSpec( void ) const 
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
void Dist_uniformTopology::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) 
{
    
    if ( name == "names" ) 
    {
        taxonNames = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
	else if ( name == "constraints" ) 
    {
        constraints = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}
