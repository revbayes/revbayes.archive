#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "RlMultispeciesCoalescentConstantPopulationProcess.h"
#include "MultispeciesCoalescent.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
MultispeciesCoalescentConstantPopulationProcess::MultispeciesCoalescentConstantPopulationProcess() : TypedDistribution<TimeTree>() 
{
    
}


/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& MultispeciesCoalescentConstantPopulationProcess::getClassName(void) 
{ 
    
    static std::string rbClassName = "Multispecies Coalescent Constant Population Process";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& MultispeciesCoalescentConstantPopulationProcess::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the homogeneous birth-death process are:
 * (1) Species tree.
 * (2) Population size.
 * (3) Gene name to species name map.
 * (4) the number of taxa.
 *
 * \return The member rules.
 */
const MemberRules& MultispeciesCoalescentConstantPopulationProcess::getMemberRules(void) const 
{
    
    static MemberRules distMultiSpeCoalConstPopMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "speciesTree", true, TimeTree::getClassTypeSpec() ) );
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "Ne"  , true, RealPos::getClassTypeSpec() ) );
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "taxons"  , true, Vector<Taxon>::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return distMultiSpeCoalConstPopMemberRules;
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
void MultispeciesCoalescentConstantPopulationProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) 
{
    
    if ( name == "speciesTree" ) 
    {
        speciesTree = var;
    }
    else if ( name == "Ne" ) 
    {
        Ne = var;
    }
    else if ( name == "taxons" ) 
    {
        taxons = var;
    }
    else 
    {
        TypedDistribution<TimeTree>::setConstMemberVariable(name, var);
    }
    
}
