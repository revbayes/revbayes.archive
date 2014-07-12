#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_divDepYuleProcess.h"
#include "DiversityDependentPureBirthProcess.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Taxon.h"

using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_divDepYuleProcess::Dist_divDepYuleProcess() : TypedDistribution<TimeTree>() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process. 
 */
Dist_divDepYuleProcess* Dist_divDepYuleProcess::clone( void ) const 
{
    return new Dist_divDepYuleProcess(*this);
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
RevBayesCore::DiversityDependentPureBirthProcess* Dist_divDepYuleProcess::createDistribution( void ) const 
{
    
    // get the parameters
    
    // the origin
    RevBayesCore::TypedDagNode<double>* o       = static_cast<const RealPos &>( origin->getRevObject() ).getDagNode();
    // speciation rate
    RevBayesCore::TypedDagNode<double>* s       = static_cast<const RealPos &>( initialLambda->getRevObject() ).getDagNode();
    // extinction rate
    RevBayesCore::TypedDagNode<int>* k          = static_cast<const Natural &>( capacity->getRevObject() ).getDagNode();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    // taxon names
    const std::vector<std::string> &names       = static_cast<const ModelVector<RlString> &>( taxonNames->getRevObject() ).getDagNode()->getValue();
    // clade constraints
    const std::vector<RevBayesCore::Clade> &c   = static_cast<const ModelVector<Clade> &>( constraints->getRevObject() ).getValue();
    
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i) 
    {
        taxa.push_back( RevBayesCore::Taxon( names[i] ) );
    }
    
    // create the internal distribution object
    RevBayesCore::DiversityDependentPureBirthProcess*   d = new RevBayesCore::DiversityDependentPureBirthProcess(o, s, k, cond, taxa, c);
    
    return d;
}



/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& Dist_divDepYuleProcess::getClassType( void ) 
{ 
    
    static std::string rbClassName = "Dist_divDepYuleProcess";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_divDepYuleProcess::getClassTypeSpec( void ) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
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
const MemberRules& Dist_divDepYuleProcess::getMemberRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambda"  , true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "capacity", true, Natural::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "origin", true, RealPos::getClassTypeSpec() ) );
        std::vector<RlString> optionsCondition;
        optionsCondition.push_back( RlString("time") );
        optionsCondition.push_back( RlString("survival") );
        optionsCondition.push_back( RlString("nTaxa") );
        distcBirthDeathMemberRules.push_back( new OptionRule( "condition", new RlString("survival"), optionsCondition ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "nTaxa"  , true, Natural::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , true, ModelVector<RlString>::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints"  , true, ModelVector<Clade>::getClassTypeSpec(), new ModelVector<Clade>() ) );

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
const TypeSpec& Dist_divDepYuleProcess::getTypeSpec( void ) const 
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
void Dist_divDepYuleProcess::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "lambda" ) 
    {
        initialLambda = var;
    }
    else if ( name == "capacity" ) 
    {
        capacity = var;
    }
    else if ( name == "origin" ) 
    {
        origin = var;
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
    else {
        TypedDistribution<TimeTree>::setConstMemberVariable(name, var);
    }
    
}
