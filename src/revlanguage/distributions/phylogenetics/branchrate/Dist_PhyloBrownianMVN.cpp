#include "Dist_PhyloBrownianMVN.h"
#include "PhyloBrownianProcessMVN.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTree.h"

using namespace RevLanguage;


Dist_PhyloBrownianMVN::Dist_PhyloBrownianMVN() : TypedDistribution< ContinuousCharacterData >()
{
    
}


Dist_PhyloBrownianMVN::~Dist_PhyloBrownianMVN()
{
    
}



Dist_PhyloBrownianMVN* Dist_PhyloBrownianMVN::clone( void ) const
{
    
    return new Dist_PhyloBrownianMVN(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >* Dist_PhyloBrownianMVN::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    size_t nNodes = tau->getValue().getNumberOfNodes();
    
    
    RevBayesCore::TypedDagNode< double >*                           homBranchRatesNode = NULL;
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >*   hetBranchRatesNode = NULL;
    // set the clock rates
    if ( branchRates->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        hetBranchRatesNode = static_cast<const ModelVector<RealPos> &>( branchRates->getRevObject() ).getDagNode();
        
        // sanity check
        size_t nRates = hetBranchRatesNode->getValue().size();
        if ( (nNodes-1) != nRates )
        {
            throw RbException( "The number of clock rates does not match the number of branches" );
        }
        
    }
    else
    {
        homBranchRatesNode = static_cast<const RealPos &>( branchRates->getRevObject() ).getDagNode();
    }
    
    RevBayesCore::TypedDagNode< double >*                           homSiteRatesNode = NULL;
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >*   hetSiteRatesNode = NULL;
    // set the clock rates
    if ( siteRates->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        hetSiteRatesNode = static_cast<const ModelVector<RealPos> &>( siteRates->getRevObject() ).getDagNode();
    }
    else
    {
        homSiteRatesNode = static_cast<const RealPos &>( siteRates->getRevObject() ).getDagNode();
    }
    
    RevBayesCore::TypedDagNode< double >*                           homRootStatesNode = NULL;
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >*   hetRootStatesNode = NULL;
    // set the clock rates
    if ( rootStates->getRevObject().isType( ModelVector<Real>::getClassTypeSpec() ) )
    {
        hetRootStatesNode = static_cast<const ModelVector<Real> &>( rootStates->getRevObject() ).getDagNode();
    }
    else
    {
        homRootStatesNode = static_cast<const Real &>( rootStates->getRevObject() ).getDagNode();
    }
    
    RevBayesCore::PhyloBrownianProcessMVN *dist = new RevBayesCore::PhyloBrownianProcessMVN(tau, homBranchRatesNode, hetBranchRatesNode, homSiteRatesNode, hetSiteRatesNode, homRootStatesNode, hetRootStatesNode, n);
    
    return dist;
}



/* Get Rev type of object */
const std::string& Dist_PhyloBrownianMVN::getClassType(void)
{
    
    static std::string revType = "Dist_PhyloBrownianMVN";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_PhyloBrownianMVN::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_PhyloBrownianMVN::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloBrownianMVN";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_PhyloBrownianMVN::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "tree" , Tree::getClassTypeSpec(), "The tree along which the character evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates" , branchRateTypes, "The rate of evolution along a branch.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> siteRateTypes;
        siteRateTypes.push_back( RealPos::getClassTypeSpec() );
        siteRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        RealPos *defaultSiteRates = new RealPos(1.0);
        distMemberRules.push_back( new ArgumentRule( "siteRates" , siteRateTypes, "The rate of evolution per site.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );
        
        std::vector<TypeSpec> rootStateTypes;
        rootStateTypes.push_back( Real::getClassTypeSpec() );
        rootStateTypes.push_back( ModelVector<Real>::getClassTypeSpec() );
        Real *defaultRootStates = new Real(0.0);
        distMemberRules.push_back( new ArgumentRule( "rootStates" , rootStateTypes, "The vector of root states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultRootStates ) );
        
        distMemberRules.push_back( new ArgumentRule( "nSites"         ,  Natural::getClassTypeSpec(), "The number of sites which is used for the initialized (random draw) from this distribution.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_PhyloBrownianMVN::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_PhyloBrownianMVN::printValue(std::ostream& o) const
{
    
    o << "PhyloBrownianProcess(tree=";
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
    o << ", siteRates=";
    if ( siteRates != NULL )
    {
        o << siteRates->getName();
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
void Dist_PhyloBrownianMVN::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "branchRates" )
    {
        branchRates = var;
    }
    else if ( name == "siteRates" )
    {
        siteRates = var;
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
