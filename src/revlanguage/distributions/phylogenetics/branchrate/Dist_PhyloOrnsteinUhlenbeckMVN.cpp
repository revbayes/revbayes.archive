#include "Dist_PhyloOrnsteinUhlenbeckMVN.h"
#include "PhyloOrnsteinUhlenbeckProcessMVN.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTree.h"

using namespace RevLanguage;


Dist_PhyloOrnsteinUhlenbeckMVN::Dist_PhyloOrnsteinUhlenbeckMVN() : TypedDistribution< ContinuousCharacterData >()
{
    
}


Dist_PhyloOrnsteinUhlenbeckMVN::~Dist_PhyloOrnsteinUhlenbeckMVN()
{
    
}



Dist_PhyloOrnsteinUhlenbeckMVN* Dist_PhyloOrnsteinUhlenbeckMVN::clone( void ) const
{
    
    return new Dist_PhyloOrnsteinUhlenbeckMVN(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >* Dist_PhyloOrnsteinUhlenbeckMVN::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    size_t n_nodes = tau->getValue().getNumberOfNodes();
    
    RevBayesCore::PhyloOrnsteinUhlenbeckProcessMVN *dist = new RevBayesCore::PhyloOrnsteinUhlenbeckProcessMVN(tau, n);
    
    
    // set the clock rates
    if ( branchRates->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* br = static_cast<const ModelVector<RealPos> &>( branchRates->getRevObject() ).getDagNode();
        
        // sanity check
        size_t n_rates = br->getValue().size();
        if ( (n_nodes-1) != n_rates )
        {
            throw RbException( "The number of clock rates does not match the number of branches" );
        }
        
        dist->setBranchRate( br );
        
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* br = static_cast<const RealPos &>( branchRates->getRevObject() ).getDagNode();
        
        dist->setBranchRate( br );
    }
    
    // set the clock rates
    if ( site_rates->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* sr = static_cast<const ModelVector<RealPos> &>( site_rates->getRevObject() ).getDagNode();
        dist->setSiteRate( sr );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* sr = static_cast<const RealPos &>( site_rates->getRevObject() ).getDagNode();
        dist->setSiteRate( sr );
    }
    
    // set alpha
    if ( alpha->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* a = static_cast<const ModelVector<RealPos> &>( alpha->getRevObject() ).getDagNode();
        dist->setAlpha( a );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* a = static_cast<const RealPos &>( alpha->getRevObject() ).getDagNode();
        dist->setAlpha( a );
    }
    
    // set theta
    if ( theta->getRevObject().isType( ModelVector<Real>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* t = static_cast<const ModelVector<Real> &>( theta->getRevObject() ).getDagNode();
        dist->setTheta( t );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* t = static_cast<const Real &>( theta->getRevObject() ).getDagNode();
        dist->setTheta( t );
    }
    
    // set sigma
    if ( sigma->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* s = static_cast<const ModelVector<RealPos> &>( sigma->getRevObject() ).getDagNode();
        dist->setSigma( s );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* s = static_cast<const RealPos &>( sigma->getRevObject() ).getDagNode();
        dist->setSigma( s );
    }
    
    // set the clock rates
    if ( rootStates->getRevObject().isType( ModelVector<Real>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* rs = static_cast<const ModelVector<Real> &>( rootStates->getRevObject() ).getDagNode();
        dist->setRootState( rs );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* rs = static_cast<const Real &>( rootStates->getRevObject() ).getDagNode();
        dist->setRootState( rs );
    }
    
    return dist;
}



/* Get Rev type of object */
const std::string& Dist_PhyloOrnsteinUhlenbeckMVN::getClassType(void)
{
    
    static std::string revType = "Dist_PhyloOrnsteinUhlenbeckMVN";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_PhyloOrnsteinUhlenbeckMVN::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_PhyloOrnsteinUhlenbeckMVN::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloOrnsteinUhlenbeckMVN";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_PhyloOrnsteinUhlenbeckMVN::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        dist_member_rules.push_back( new ArgumentRule( "tree" , Tree::getClassTypeSpec(), "The tree along which the character evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "branchRates" , branchRateTypes, "The rate of evolution along a branch.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> siteRateTypes;
        siteRateTypes.push_back( RealPos::getClassTypeSpec() );
        siteRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        RealPos *defaultSiteRates = new RealPos(1.0);
        dist_member_rules.push_back( new ArgumentRule( "site_rates" , siteRateTypes, "The rate of evolution per site.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );
        
        std::vector<TypeSpec> alphaTypes;
        alphaTypes.push_back( RealPos::getClassTypeSpec() );
        alphaTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "alpha" , alphaTypes, "The rate of attraction/selection (per branch).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> thetaTypes;
        thetaTypes.push_back( Real::getClassTypeSpec() );
        thetaTypes.push_back( ModelVector<Real>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "theta" , thetaTypes, "The optimum value (per branch).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> sigmaTypes;
        sigmaTypes.push_back( RealPos::getClassTypeSpec() );
        sigmaTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "sigma" , sigmaTypes, "The rate of random drift (per branch).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> rootStateTypes;
        rootStateTypes.push_back( Real::getClassTypeSpec() );
        rootStateTypes.push_back( ModelVector<Real>::getClassTypeSpec() );
        Real *defaultRootStates = new Real(0.0);
        dist_member_rules.push_back( new ArgumentRule( "rootStates" , rootStateTypes, "The vector of root states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultRootStates ) );
        
        dist_member_rules.push_back( new ArgumentRule( "nSites"         ,  Natural::getClassTypeSpec(), "The number of sites which is used for the initialized (random draw) from this distribution.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );
        
        rules_set = true;
    }
    
    return dist_member_rules;
}


const TypeSpec& Dist_PhyloOrnsteinUhlenbeckMVN::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_PhyloOrnsteinUhlenbeckMVN::printValue(std::ostream& o) const
{
    
    o << "PhyloOrnsteinUhlenbeckProcess(tree=";
    if ( tree != NULL )
    {
        o << tree->getName();
    }
    else
    {
        o << "?";
    }
    o << ", branchRates=";
    if ( branchRates != NULL )
    {
        o << branchRates->getName();
    }
    else
    {
        o << "?";
    }
    o << ", site_rates=";
    if ( site_rates != NULL )
    {
        o << site_rates->getName();
    }
    else
    {
        o << "?";
    }
    o << ", nSites=";
    if ( nSites != NULL )
    {
        o << nSites->getName();
    }
    else
    {
        o << "?";
    }
    o << ", nSites=";
    if ( nSites != NULL )
    {
        o << nSites->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
    
}


/** Set a member variable */
void Dist_PhyloOrnsteinUhlenbeckMVN::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "branchRates" )
    {
        branchRates = var;
    }
    else if ( name == "site_rates" )
    {
        site_rates = var;
    }
    else if ( name == "alpha" )
    {
        alpha = var;
    }
    else if ( name == "theta" )
    {
        theta = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "rootStates" )
    {
        rootStates = var;
    }
    else if ( name == "nSites" )
    {
        nSites = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
    
}
