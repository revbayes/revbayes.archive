#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_CharacterDependentFossilizedBirthDeathProcess.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RateGenerator.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlDistributionMemberFunction.h"
#include "RlRateGenerator.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StateDependentSpeciationExtinctionProcess.h"
#include "StochasticNode.h"

using namespace RevLanguage;


Dist_CharacterDependentFossilizedBirthDeathProcess::Dist_CharacterDependentFossilizedBirthDeathProcess() : Dist_CharacterDependentBirthDeathProcess()
{
    
}


Dist_CharacterDependentFossilizedBirthDeathProcess::~Dist_CharacterDependentFossilizedBirthDeathProcess()
{
    
}



Dist_CharacterDependentFossilizedBirthDeathProcess* Dist_CharacterDependentFossilizedBirthDeathProcess::clone( void ) const
{
    
    return new Dist_CharacterDependentFossilizedBirthDeathProcess( *this );
}


RevBayesCore::TypedDistribution<RevBayesCore::Tree>* Dist_CharacterDependentFossilizedBirthDeathProcess::createDistribution( void ) const
{
    RevBayesCore::StateDependentSpeciationExtinctionProcess*   d = static_cast<RevBayesCore::StateDependentSpeciationExtinctionProcess*>(Dist_CharacterDependentBirthDeathProcess::createDistribution());
    
    if( fossilization_rates->getRevObject() != RevNullObject::getInstance() )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* fo  = static_cast<const ModelVector<RealPos> &>( fossilization_rates->getRevObject() ).getDagNode();
        d->setFossilizationRates( fo );
    }

    return d;
}



/* Get Rev type of object */
const std::string& Dist_CharacterDependentFossilizedBirthDeathProcess::getClassType(void)
{
    
    static std::string rev_type = "Dist_CharacterDependentFossilizedBirthDeathProcess";
    
    return rev_type;
}


/* Get class type spec describing type of object. TODO: Check if the correct parent is TypedDistribution or Distribution */
const TypeSpec& Dist_CharacterDependentFossilizedBirthDeathProcess::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Dist_CharacterDependentBirthDeathProcess::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_CharacterDependentFossilizedBirthDeathProcess::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "CDFBDP";
    
    return d_name;
}



/* Return member rules */
const MemberRules& Dist_CharacterDependentFossilizedBirthDeathProcess::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        memberRules = Dist_CharacterDependentBirthDeathProcess::getParameterRules();
        
        memberRules.push_back( new ArgumentRule( "psi"       , ModelVector<RealPos>::getClassTypeSpec()          , "The vector of fossil sampling rates."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY, NULL ) );

        rules_set = true;
    }
    
    return memberRules;
}


/** Set a member variable */
void Dist_CharacterDependentFossilizedBirthDeathProcess::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "psi" )
    {
        fossilization_rates = var;
    }
    else
    {
        Dist_CharacterDependentBirthDeathProcess::setConstParameter(name, var);
    }
}

