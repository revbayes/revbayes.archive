#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "DiversityDependentPureBirthProcess.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlDiversityDependentPureBirthProcess.h"
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
DiversityDependentPureBirthProcess::DiversityDependentPureBirthProcess() : BirthDeathProcess() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the model. 
 */
DiversityDependentPureBirthProcess* DiversityDependentPureBirthProcess::clone( void ) const 
{
    return new DiversityDependentPureBirthProcess(*this);
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
RevBayesCore::DiversityDependentPureBirthProcess* DiversityDependentPureBirthProcess::createDistribution( void ) const 
{
    
    // get the parameters
    
    // the origin
    RevBayesCore::TypedDagNode<double>* o       = static_cast<const RealPos &>( origin->getValue() ).getValueNode();
    // speciation rate
    RevBayesCore::TypedDagNode<double>* s       = static_cast<const RealPos &>( initialLambda->getValue() ).getValueNode();
    // extinction rate
    RevBayesCore::TypedDagNode<int>* k          = static_cast<const Natural &>( capacity->getValue() ).getValueNode();
    // sampling probability
    RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getValue() ).getValueNode();
    // sampling strategy
    const std::string &strategy                 = static_cast<const RlString &>( samplingStrategy->getValue() ).getValue();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getValue() ).getValue();
    // number of taxa
    int n                                       = static_cast<const Natural &>( numTaxa->getValue() ).getValue();
    // taxon names
    const std::vector<std::string> &names       = static_cast<const Vector<RlString> &>( taxonNames->getValue() ).getValueNode()->getValue();
    // clade constraints
    const std::vector<RevBayesCore::Clade> &c   = static_cast<const Vector<Clade> &>( constraints->getValue() ).getValue();
    
    // create the internal distribution object
    RevBayesCore::DiversityDependentPureBirthProcess*   d = new RevBayesCore::DiversityDependentPureBirthProcess(o, s, k, r, strategy, cond, n, names, c);
    
    return d;
}



/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& DiversityDependentPureBirthProcess::getClassName( void ) 
{ 
    
    static std::string rbClassName = "diversity-depdenent pure-birth Process";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& DiversityDependentPureBirthProcess::getClassTypeSpec( void ) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( BirthDeathProcess::getClassTypeSpec() ) );
    
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
const MemberRules& DiversityDependentPureBirthProcess::getMemberRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambda"  , true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "capacity", true, Natural::getClassTypeSpec() ) );
        
        // add the rules from the base class
        const MemberRules &parentRules = BirthDeathProcess::getMemberRules();
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
const TypeSpec& DiversityDependentPureBirthProcess::getTypeSpec( void ) const 
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
void DiversityDependentPureBirthProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) 
{
    
    if ( name == "lambda" ) 
    {
        initialLambda = var;
    }
    else if ( name == "capacity" ) 
    {
        capacity = var;
    }
    else {
        BirthDeathProcess::setConstMemberVariable(name, var);
    }
    
}
