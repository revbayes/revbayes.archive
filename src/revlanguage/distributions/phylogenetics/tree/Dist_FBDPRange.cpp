#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_FBDPRange.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "PiecewiseConstantFossilizedBirthDeathRangeProcess.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"

using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_FBDPRange::Dist_FBDPRange() : TypedDistribution<MatrixReal>()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_FBDPRange* Dist_FBDPRange::clone( void ) const
{
    return new Dist_FBDPRange(*this);
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
RevBayesCore::PiecewiseConstantFossilizedBirthDeathRangeProcess* Dist_FBDPRange::createDistribution( void ) const
{
    
    // get the parameters
    
    // sampling condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    
    // get the taxa to simulate either from a vector of rev taxon objects or a vector of names
    std::vector<RevBayesCore::Taxon> t = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    
    bool piecewiseLambda = false;
    bool piecewiseMu = false;
    bool piecewisePsi = false;

    if( lambda->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        piecewiseLambda = true;
    }

    if( mu->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        piecewiseMu = true;
    }

    if( psi->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        piecewisePsi = true;

        if( !fossil_counts->getRevObject().isType( ModelVector<Integer>::getClassTypeSpec() ) )
        {
            throw(RbException("Heterogeneous fossil sampling rates provided, but not fossil counts"));
        }
    }

    bool piecewise = piecewiseLambda || piecewiseMu || piecewisePsi;

    if ( piecewise && (timeline == NULL || timeline->getRevObject() == RevNullObject::getInstance() ) )
    {
        throw(RbException("No time intervals provided for piecewise constant fossilized birth death process"));
    }

    // speciation rate
    RevBayesCore::DagNode* l;
    // extinction rate
    RevBayesCore::DagNode* m;
    // fossilization rate
    RevBayesCore::DagNode* p;
    // fossil counts
    RevBayesCore::DagNode* c;

    if(piecewiseLambda)
    {
        l = static_cast<const ModelVector<RealPos> &>( lambda->getRevObject() ).getDagNode();
    }
    else
    {
        l = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    }
    // extinction rate
    if(piecewiseMu)
    {
        m = static_cast<const ModelVector<RealPos> &>( mu->getRevObject() ).getDagNode();
    }
    else
    {
        m = static_cast<const RealPos &>( mu->getRevObject() ).getDagNode();
    }
    // fossilization rate
    if(piecewisePsi)
    {
        p = static_cast<const ModelVector<RealPos> &>( psi->getRevObject() ).getDagNode();
        c = static_cast<const ModelVector<Integer> &>( fossil_counts->getRevObject() ).getDagNode();
    }
    else
    {
        p = static_cast<const RealPos &>( psi->getRevObject() ).getDagNode();
        c = static_cast<const Integer &>( fossil_counts->getRevObject() ).getDagNode();
    }

    // sampling probability
    RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();

    // rate change times
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* rt       = static_cast<const ModelVector<RealPos> &>( timeline->getRevObject() ).getDagNode();

    RevBayesCore::PiecewiseConstantFossilizedBirthDeathRangeProcess* d = new RevBayesCore::PiecewiseConstantFossilizedBirthDeathRangeProcess(l, m, p, c, r, rt, cond, t);

    return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_FBDPRange::getClassType( void )
{
    
    static std::string rev_type = "Dist_FBDPRange";
    
    return rev_type;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_FBDPRange::getClassTypeSpec( void )
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<MatrixReal>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_FBDPRange::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "FBDPRange" );
    
    return a_names;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_FBDPRange::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "FossilizedBirthDeathRange";
    
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
const MemberRules& Dist_FBDPRange::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {

        std::vector<TypeSpec> paramTypes;
        paramTypes.push_back( RealPos::getClassTypeSpec() );
        paramTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "lambda",  paramTypes, "The speciation rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist_member_rules.push_back( new ArgumentRule( "mu",      paramTypes, "The extinction rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        dist_member_rules.push_back( new ArgumentRule( "psi",     paramTypes, "The fossil sampling rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        dist_member_rules.push_back( new ArgumentRule( "rho",     Probability::getClassTypeSpec(), "The extant taxon sampling fraction.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        dist_member_rules.push_back( new ArgumentRule( "timeline",   ModelVector<RealPos>::getClassTypeSpec(), "The rate interval change times of the piecewise constant process.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );

        std::vector<TypeSpec> intTypes;
        intTypes.push_back( Integer::getClassTypeSpec() );
        intTypes.push_back( ModelVector<Integer>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "k",   intTypes, "The total number of fossil observations (in each time interval).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "survival" );
        dist_member_rules.push_back( new OptionRule( "condition", new RlString("time"), optionsCondition, "The condition of the process." ) );
        dist_member_rules.push_back( new ArgumentRule( "taxa"  , ModelVector<Taxon>::getClassTypeSpec(), "The taxa with stratigraphic ranges used for initialization.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return dist_member_rules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_FBDPRange::getTypeSpec( void ) const
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
void Dist_FBDPRange::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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
    else if ( name == "timeline" )
    {
        timeline = var;
    }
    else if ( name == "k" )
    {
        fossil_counts = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
    else if ( name == "condition" )
    {
        condition = var;
    }
    else
    {
        TypedDistribution<MatrixReal>::setConstParameter(name, var);
    }
    
}
