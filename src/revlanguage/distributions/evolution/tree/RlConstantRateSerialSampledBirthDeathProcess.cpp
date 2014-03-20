#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateSerialSampledBirthDeathProcess.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlConstantRateSerialSampledBirthDeathProcess.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Taxon.h"
#include "Vector.h"

using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
ConstantRateSerialSampledBirthDeathProcess::ConstantRateSerialSampledBirthDeathProcess() : TypedDistribution<TimeTree>() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the model. 
 */
ConstantRateSerialSampledBirthDeathProcess* ConstantRateSerialSampledBirthDeathProcess::clone( void ) const 
{
    return new ConstantRateSerialSampledBirthDeathProcess(*this);
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
RevBayesCore::ConstantRateSerialSampledBirthDeathProcess* ConstantRateSerialSampledBirthDeathProcess::createDistribution( void ) const 
{
    
    // get the parameters
    
    // the origin
    RevBayesCore::TypedDagNode<double>* o       = static_cast<const RealPos &>( origin->getValue() ).getValueNode();
    // speciation rate
    RevBayesCore::TypedDagNode<double>* s       = static_cast<const RealPos &>( lambda->getValue() ).getValueNode();
    // extinction rate
    RevBayesCore::TypedDagNode<double>* e       = static_cast<const RealPos &>( mu->getValue() ).getValueNode();
    // sampling rate
    RevBayesCore::TypedDagNode<double>* p       = static_cast<const RealPos &>( psi->getValue() ).getValueNode();
    // sampling probability
    RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getValue() ).getValueNode();
    // time between now and most recent sample
    double tLastSample                          = static_cast<const RealPos &>( tLast->getValue() ).getValue();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getValue() ).getValue();
    // taxon names
    const std::vector<std::string> &names       = static_cast<const Vector<RlString> &>( taxonNames->getValue() ).getValueNode()->getValue();
    // clade constraints
    const std::vector<RevBayesCore::Clade> &c   = static_cast<const Vector<Clade> &>( constraints->getValue() ).getValue();
    
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i) 
    {
        taxa.push_back( RevBayesCore::Taxon( names[i] ) );
    }
    
    // create the internal distribution object
    RevBayesCore::ConstantRateSerialSampledBirthDeathProcess*   d = new RevBayesCore::ConstantRateSerialSampledBirthDeathProcess(o, s, e, p, r, tLastSample, cond, taxa, c);
    
    return d;
}



/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& ConstantRateSerialSampledBirthDeathProcess::getClassName( void ) 
{ 
    
    static std::string rbClassName = "DivDependentPureBirthProcess";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& ConstantRateSerialSampledBirthDeathProcess::getClassTypeSpec( void ) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the diversity-dependent pure-birth process are:
 * (1) the initial speciation rate lambda which must be a positive real.
 * (2) the carrying capacity that must be a natural number.
 * (3) all member rules specified by BirthDeathProcess.
 *
 * \return The member rules.
 */
const MemberRules& ConstantRateSerialSampledBirthDeathProcess::getMemberRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "origin", true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambda"  , true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "mu", true, RealPos::getClassTypeSpec(), new RealPos(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "psi", true, RealPos::getClassTypeSpec(), new RealPos(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho", true, Probability::getClassTypeSpec(), new Probability(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "timeSinceLastSample", true, RealPos::getClassTypeSpec(), new RealPos(0.0) ) );
        Vector<RlString> optionsCondition;
        optionsCondition.push_back( RlString("time") );
        optionsCondition.push_back( RlString("survival") );
        optionsCondition.push_back( RlString("nTaxa") );
        distcBirthDeathMemberRules.push_back( new OptionRule( "condition", new RlString("survival"), optionsCondition ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , true, Vector<RlString>::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints"  , true, Vector<Clade>::getClassTypeSpec(), new Vector<Clade>() ) );
        
        // add the rules from the base class
        const MemberRules &parentRules = TypedDistribution<TimeTree>::getMemberRules();
        distcBirthDeathMemberRules.insert(distcBirthDeathMemberRules.end(), parentRules.begin(), parentRules.end());
        
        rulesSet = true;
    }
    
    return distcBirthDeathMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& ConstantRateSerialSampledBirthDeathProcess::getTypeSpec( void ) const 
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
void ConstantRateSerialSampledBirthDeathProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) 
{
    
    if ( name == "lambda" ) 
    {
        lambda = var;
    }
    else if ( name == "mu" ) 
    {
        mu = var;
    }
    else if ( name == "psi" ) 
    {
        psi = var;
    }
    else if ( name == "rho" ) 
    {
        rho = var;
    }
    else if ( name == "timeSinceLastSample" ) 
    {
        tLast = var;
    }
    else if ( name == "origin" ) 
    {
        origin = var;
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
    else {
        TypedDistribution<TimeTree>::setConstMemberVariable(name, var);
    }
    
}
