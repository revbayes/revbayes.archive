#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "Dist_bdpTopology.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_bdpTopology::Dist_bdpTopology() : BirthDeathProcess()
{
	
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_bdpTopology* Dist_bdpTopology::clone( void ) const
{
	return new Dist_bdpTopology(*this);
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
RevBayesCore::ConstantRateBirthDeathProcess* Dist_bdpTopology::createDistribution( void ) const
{
	
	// get the parameters
	
	// the root age
	RevBayesCore::TypedDagNode<double>* ra = static_cast<const RealPos &>( rootAge->getRevObject() ).getDagNode();
	
	// speciation rate
	RevBayesCore::TypedDagNode<double>* s       = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
	// extinction rate
	RevBayesCore::TypedDagNode<double>* e       = static_cast<const RealPos &>( mu->getRevObject() ).getDagNode();
	// sampling probability
	RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    // sampling mixture proportion
    RevBayesCore::TypedDagNode<double>* mp       = static_cast<const Probability &>( samplingMixtureProportion->getRevObject() ).getDagNode();
	// sampling strategy
	const std::string &strategy                 = static_cast<const RlString &>( samplingStrategy->getRevObject() ).getValue();
    // incompletely sampled clades
    std::vector<RevBayesCore::Clade> inc_clades;
    if ( incomplete_clades->getRevObject() != RevNullObject::getInstance() )
    {
        inc_clades = static_cast<const ModelVector<Clade> &>( incomplete_clades->getRevObject() ).getValue();
    }
    // condition
	const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
	// taxon names
	const std::vector<RevBayesCore::Taxon> &t   = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getDagNode()->getValue();

    // create the internal distribution object
	RevBayesCore::ConstantRateBirthDeathProcess*   d = new RevBayesCore::ConstantRateBirthDeathProcess(ra, s, e, r, mp, strategy, inc_clades, cond, t);
	
	return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_bdpTopology::getClassType( void )
{
	
	static std::string revType = "Dist_bdpTopologyTopology";
	
	return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_bdpTopology::getClassTypeSpec( void )
{
	
	static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( BirthDeathProcess::getClassTypeSpec() ) );
	
	return revTypeSpec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_bdpTopology::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "BDPTopology";
    
    return d_name;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the constant-rate birth-death process are:
 * (1) the speciation rate lambda which must be a positive real.
 * (2) the extinction rate mu that must be a positive real.
 * (3) all member rules specified by BirthDeathProcess.
 *
 * \return The member rules.
 */
const MemberRules& Dist_bdpTopology::getParameterRules(void) const
{
	
	static MemberRules memberRules;
	static bool rulesSet = false;
	
	if ( !rulesSet )
	{
		
		memberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec(), "The constant speciation rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
		memberRules.push_back( new ArgumentRule( "mu"    , RealPos::getClassTypeSpec(), "The constant extinction rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
		
		// add the rules from the base class
		const MemberRules &parentRules = BirthDeathProcess::getParameterRules();
		memberRules.insert( memberRules.end(), parentRules.begin(), parentRules.end());
		
		rulesSet = true;
	}
	
	return memberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_bdpTopology::getTypeSpec( void ) const
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
void Dist_bdpTopology::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
	
	if ( name == "lambda" )
	{
		lambda = var;
	}
	else if ( name == "mu" )
	{
		mu = var;
	}
	else {
		BirthDeathProcess::setConstParameter(name, var);
	}
	
}
