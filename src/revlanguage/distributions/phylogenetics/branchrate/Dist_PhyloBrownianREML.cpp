#include "Dist_PhyloBrownianREML.h"
#include "OptionRule.h"
#include "PhyloBrownianProcessREML.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTree.h"

using namespace RevLanguage;

Dist_PhyloBrownianREML::Dist_PhyloBrownianREML() : TypedDistribution< ContinuousCharacterData >()
{
    
}


Dist_PhyloBrownianREML::~Dist_PhyloBrownianREML()
{
    
}



Dist_PhyloBrownianREML* Dist_PhyloBrownianREML::clone( void ) const
{
    
    return new Dist_PhyloBrownianREML(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >* Dist_PhyloBrownianREML::createDistribution( void ) const
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
    
    RevBayesCore::PhyloBrownianProcessREML *dist = new RevBayesCore::PhyloBrownianProcessREML(tau, homBranchRatesNode, hetBranchRatesNode, homSiteRatesNode, hetSiteRatesNode, n);
    
    return dist;
}



/* Get Rev type of object */
const std::string& Dist_PhyloBrownianREML::getClassType(void)
{
    
    static std::string revType = "Dist_PhyloBrownianREML";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_PhyloBrownianREML::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_PhyloBrownianREML::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "tree" , Tree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates" , branchRateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> siteRateTypes;
        siteRateTypes.push_back( RealPos::getClassTypeSpec() );
        siteRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        RealPos *defaultSiteRates = new RealPos(1.0);
        distMemberRules.push_back( new ArgumentRule( "siteRates" , siteRateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );
        
        distMemberRules.push_back( new ArgumentRule( "nSites"         , Natural::getClassTypeSpec()             , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_PhyloBrownianREML::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_PhyloBrownianREML::printValue(std::ostream& o) const
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
    o << ")";
    
}


/** Set a member variable */
void Dist_PhyloBrownianREML::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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
    else if ( name == "nSites" )
    {
        nSites = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
    
}

