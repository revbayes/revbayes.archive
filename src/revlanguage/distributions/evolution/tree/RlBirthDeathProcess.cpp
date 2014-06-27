#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "RlBirthDeathProcess.h"
#include "BirthDeathProcess.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
BirthDeathProcess::BirthDeathProcess() : TypedDistribution<TimeTree>() 
{
    
}


/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& BirthDeathProcess::getClassName(void) 
{ 
    
    static std::string rbClassName = "Dist_BDP";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& BirthDeathProcess::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the homogeneous birth-death process are:
 * (1) time of the process since the origin.
 * (2) the sampling probability.
 * (3) the sampling strategy.
 * (4) the condition.
 * (5) the number of taxa.
 * (6) the taxon names.
 * (7) the clade constraints.
 *
 * \return The member rules.
 */
const MemberRules& BirthDeathProcess::getMemberRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "origin", true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho"  , true, Probability::getClassTypeSpec(), new Probability(1.0) ) );
        Vector<RlString> optionsStrategy;
        optionsStrategy.push_back( RlString("uniform") );
        optionsStrategy.push_back( RlString("diversified") );
        distcBirthDeathMemberRules.push_back( new OptionRule( "samplingStrategy", new RlString("uniform"), optionsStrategy ) );
        Vector<RlString> optionsCondition;
        optionsCondition.push_back( RlString("time") );
        optionsCondition.push_back( RlString("survival") );
        optionsCondition.push_back( RlString("nTaxa") );
        distcBirthDeathMemberRules.push_back( new OptionRule( "condition", new RlString("survival"), optionsCondition ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "nTaxa"  , true, Natural::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , true, Vector<RlString>::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints"  , true, Vector<Clade>::getClassTypeSpec(), new Vector<Clade>() ) );
        
        rulesSet = true;
    }
    
    return distcBirthDeathMemberRules;
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
void BirthDeathProcess::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "origin" ) 
    {
        origin = var;
    }
    else if ( name == "rho" ) 
    {
        rho = var;
    }
    else if ( name == "samplingStrategy" ) 
    {
        samplingStrategy = var;
    }
    else if ( name == "nTaxa" ) 
    {
        numTaxa = var;
    }
    else if ( name == "names" ) 
    {
        taxonNames = var;
    }
    else if ( name == "constraints" ) 
    {
        constraints = var;
    }
    else if ( name == "condition" ) 
    {
        condition = var;
    }
    else 
    {
        TypedDistribution<TimeTree>::setConstMemberVariable(name, var);
    }
    
}
