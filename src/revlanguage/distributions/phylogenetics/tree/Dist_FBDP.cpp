#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "Dist_FBDP.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "PiecewiseConstantFossilizedBirthDeathProcess.h"
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
Dist_FBDP::Dist_FBDP() : BirthDeathProcess()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_FBDP* Dist_FBDP::clone( void ) const
{
    return new Dist_FBDP(*this);
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
RevBayesCore::AbstractBirthDeathProcess* Dist_FBDP::createDistribution( void ) const
{
    
    // get the parameters
    
    // the start age
    RevBayesCore::TypedDagNode<double>* sa       = static_cast<const RealPos &>( startAge->getRevObject() ).getDagNode();

    // the start condition
    bool uo = ( startCondition == "originAge" ? true : false );
    
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
    }

    bool piecewise = piecewiseLambda || piecewiseMu || piecewisePsi;

    if ( piecewise && (intervals == NULL || intervals->getRevObject() == RevNullObject::getInstance() ) )
    {
        throw(RbException("No time intervals provided for piecewise constant fossilized birth death process"));
    }


    RevBayesCore::AbstractBirthDeathProcess* d;

    if ( piecewise )
    {
        // speciation rate
        RevBayesCore::DagNode* l;
        // extinction rate
        RevBayesCore::DagNode* m;
        // fossilization rate
        RevBayesCore::DagNode* p;

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
        }
        else
        {
            p = static_cast<const RealPos &>( psi->getRevObject() ).getDagNode();
        }

        // sampling probability
        RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();

        // rate change times
        RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* rt       = static_cast<const ModelVector<RealPos> &>( intervals->getRevObject() ).getDagNode();

        d = new RevBayesCore::PiecewiseConstantFossilizedBirthDeathProcess(sa, l, m, p, r, rt, uo, cond, t);
    }
    else
    {
        // speciation rate
        RevBayesCore::TypedDagNode<double>* l       = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
        // extinction rate
        RevBayesCore::TypedDagNode<double>* m       = static_cast<const RealPos &>( mu->getRevObject() ).getDagNode();
        // fossilization rate
        RevBayesCore::TypedDagNode<double>* p       = static_cast<const RealPos &>( psi->getRevObject() ).getDagNode();
        // sampling probability
        RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();

        d = new RevBayesCore::ConstantRateFossilizedBirthDeathProcess(sa, l, m, p, r, uo, cond, t);
    }

    return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_FBDP::getClassType( void )
{
    
    static std::string rev_type = "Dist_FBDP";
    
    return rev_type;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_FBDP::getClassTypeSpec( void )
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( BirthDeathProcess::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_FBDP::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "FBDP" );
    
    return a_names;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_FBDP::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "FossilBirthDeath";
    
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
const MemberRules& Dist_FBDP::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {

        std::vector<std::string> aliases;
        aliases.push_back("rootAge");
        aliases.push_back("originAge");
        dist_member_rules.push_back( new ArgumentRule( aliases,   RealPos::getClassTypeSpec(), "The start age of the process, either the root age or the origin time.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        std::vector<TypeSpec> paramTypes;
        paramTypes.push_back( RealPos::getClassTypeSpec() );
        paramTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "lambda",  paramTypes, "The speciation rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist_member_rules.push_back( new ArgumentRule( "mu",      paramTypes, "The extinction rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        dist_member_rules.push_back( new ArgumentRule( "psi",     paramTypes, "The fossil sampling rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        dist_member_rules.push_back( new ArgumentRule( "rho",     Probability::getClassTypeSpec(), "The extant taxon sampling fraction.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        dist_member_rules.push_back( new ArgumentRule( "intervals",   ModelVector<RealPos>::getClassTypeSpec(), "The rate interval change times of the piecewise constant process.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );

        // add the rules from the base class
        const MemberRules &parentRules = BirthDeathProcess::getParameterRules();
        for (size_t i = 0; i < parentRules.size(); i++)
        {
            if ( parentRules[i].getArgumentLabel() != "rootAge" && parentRules[i].getArgumentLabel() != "rho" )
            {
                ArgumentRule* tmp = parentRules[i].clone();
                dist_member_rules.push_back( tmp );
            }
        }
        
        rules_set = true;
    }
    
    return dist_member_rules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_FBDP::getTypeSpec( void ) const
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
void Dist_FBDP::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    if (name == "rootAge" || name == "originAge")
    {
        startCondition = name;
        startAge = var;
    }
    else if ( name == "lambda" )
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
    else if ( name == "intervals" )
    {
        intervals = var;
    }
    else
    {
        BirthDeathProcess::setConstParameter(name, var);
    }
    
}
