#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBirthDeathProcess.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"

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
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& BirthDeathProcess::getClassType(void) 
{ 
    
    static std::string revType = "BirthDeathProcess";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& BirthDeathProcess::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the homogeneous birth-death process are:
 * (1) time of the process since the origin.
 * (2) time of the process since the rootAge.
 * (3) the sampling probability.
 * (4) the sampling strategy.
 * (5) the condition.
 * (6) the number of taxa.
 * (7) the taxon names.
 * (8) the clade constraints.
 *
 * \return The member rules.
 */
const MemberRules& BirthDeathProcess::getMemberRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "origin" , RealPos::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rootAge", RealPos::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho"    , Probability::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(1.0) ) );
        std::vector<std::string> optionsStrategy;
        optionsStrategy.push_back( "uniform" );
        optionsStrategy.push_back( "diversified" );
        distcBirthDeathMemberRules.push_back( new OptionRule( "samplingStrategy", new RlString("uniform"), optionsStrategy ) );
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "time" );
        optionsCondition.push_back( "survival" );
        optionsCondition.push_back( "nTaxa" );
        distcBirthDeathMemberRules.push_back( new OptionRule( "condition", new RlString("survival"), optionsCondition ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "nTaxa"  , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints", ModelVector<Clade>::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new ModelVector<Clade>() ) );
        
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
    else if ( name == "rootAge" )
    {
        rootAge = var;
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
