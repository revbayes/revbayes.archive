#include "Dist_phyloCTMCClado.h"
#include "PhyloCTMCClado.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RateMatrix.h"
#include "RevNullObject.h"
#include "RlMatrixReal.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "RlTree.h"
#include "StandardState.h"
#include "PomoState.h"
#include "NaturalNumbersState.h"

using namespace RevLanguage;

Dist_phyloCTMCClado::Dist_phyloCTMCClado() : TypedDistribution< AbstractHomologousDiscreteCharacterData >()
{
    
}

Dist_phyloCTMCClado::~Dist_phyloCTMCClado()
{
    
}


Dist_phyloCTMCClado* Dist_phyloCTMCClado::clone( void ) const
{
    
    return new Dist_phyloCTMCClado(*this);
}

RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData >* Dist_phyloCTMCClado::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    bool ambig = static_cast<const RlBoolean &>( treatAmbiguousAsGap->getRevObject() ).getValue();
    
    size_t nNodes = tau->getValue().getNumberOfNodes();
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* siteRatesNode = NULL;
    if ( siteRates != NULL && siteRates->getRevObject() != RevNullObject::getInstance() )
    {
        siteRatesNode = static_cast<const ModelVector<RealPos> &>( siteRates->getRevObject() ).getDagNode();
    }
    RevBayesCore::TypedDagNode< double >* pInvNode = NULL;
    if ( pInv != NULL && pInv->getRevObject() != RevNullObject::getInstance() )
    {
        pInvNode = static_cast<const Probability &>( pInv->getRevObject() ).getDagNode();
    }
    
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData > *d = NULL;
    const RevBayesCore::TypedDagNode< RevBayesCore::RbVector< double > > *rf = NULL;
    if ( rootFrequencies->getRevObject() != RevNullObject::getInstance() )
    {
        rf = static_cast<const Simplex &>( rootFrequencies->getRevObject() ).getDagNode();
    }
    
    if ( dt == "NaturalNumbers" )
    {
        // we get the number of states from the rates matrix
        // set the rate matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }
        RevBayesCore::g_MAX_NAT_NUM_STATES = nChars;
        
        RevBayesCore::PhyloCTMCClado<RevBayesCore::NaturalNumbersState> *dist = new RevBayesCore::PhyloCTMCClado<RevBayesCore::NaturalNumbersState>(tau, nChars, true, n, ambig);
        
        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );
        
        // set the probability for invariant site (by default this pInv=0.0)
        dist->setPInv( pInvNode );
        
        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != clockRates->getValue().size() )
            {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }
            
            dist->setClockRate( clockRates );
        }
        else
        {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }
        
        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != rm->getValue().size() )
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            
            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        // set the clado probs
        if ( cladoProbs->getRevObject().isType( ModelVector<MatrixReal>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::MatrixReal> >* cp = static_cast<const ModelVector<MatrixReal> &>( cladoProbs->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != cp->getValue().size() )
            {
                throw RbException( "The number of cladogenesis probability matrices does not match the number of branches" );
            }
            dist->setCladogenesisMatrix( cp );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* cp = static_cast<const MatrixReal &>( cladoProbs->getRevObject() ).getDagNode();
            dist->setCladogenesisMatrix( cp );
        }
        
        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }
        
        d = dist;    }
    
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_phyloCTMCClado::getClassType(void) {
    
    static std::string revType = "Dist_phyloCTMCClado";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_phyloCTMCClado::getClassTypeSpec(void) {
    
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
std::string Dist_phyloCTMCClado::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloCTMCClado";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_phyloCTMCClado::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        // epoch model requires time tree
        distMemberRules.push_back( new ArgumentRule( "tree"           , Tree::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        // epoch model requires vector of Q
        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( RateGenerator::getClassTypeSpec() );
        rateMatrixTypes.push_back( ModelVector<RateGenerator>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "Q"              , rateMatrixTypes, "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        // clado model accepts a single or vector of cladogenesis probs
        std::vector<TypeSpec> cladoMtxTypes;
        cladoMtxTypes.push_back( MatrixReal::getClassTypeSpec() );
        cladoMtxTypes.push_back( ModelVector<MatrixReal>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "cladoProbs"              , cladoMtxTypes, "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::DETERMINISTIC ) );
        
        // optional argument for the root frequencies
        distMemberRules.push_back( new ArgumentRule( "rootFrequencies", Simplex::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        // branch rates
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates"    , branchRateTypes, "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        
        ModelVector<RealPos> *defaultSiteRates = new ModelVector<RealPos>();
        distMemberRules.push_back( new ArgumentRule( "siteRates"      , ModelVector<RealPos>::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );
        distMemberRules.push_back( new ArgumentRule( "pInv"           , Probability::getClassTypeSpec()         , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(0.0) ) );
        
        distMemberRules.push_back( new ArgumentRule( "nSites"         , Natural::getClassTypeSpec()             , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );
        
        
        std::vector<std::string> options;
        options.push_back( "DNA" );
        options.push_back( "RNA" );
        options.push_back( "AA" );
        options.push_back( "Pomo" );
        options.push_back( "Protein" );
        options.push_back( "Standard" );
        options.push_back( "NaturalNumbers" );
        distMemberRules.push_back( new OptionRule( "type", new RlString("NaturalNumbers"), options, "" ) );
        
        distMemberRules.push_back( new ArgumentRule( "treatAmbiguousAsGap", RlBoolean::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}

const TypeSpec& Dist_phyloCTMCClado::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_phyloCTMCClado::printValue(std::ostream& o) const
{
    
    o << "Character-State-Evolution-Along-Tree-With-Cladogenesis Process(tree=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    o << ", Q=";
    o << ", cladoProbs=";
    if ( q != NULL ) {
        o << q->getName();
    } else {
        o << "?";
    }
    o << ", branchRates=";
    if ( rate != NULL ) {
        o << rate->getName();
    } else {
        o << "?";
    }
    o << ", siteRates=";
    if ( siteRates != NULL ) {
        o << siteRates->getName();
    } else {
        o << "?";
    }
    o << ", pInv=";
    if ( pInv != NULL ) {
        o << pInv->getName();
    } else {
        o << "?";
    }
    o << ", nSites=";
    if ( nSites != NULL ) {
        o << nSites->getName();
    } else {
        o << "?";
    }
    o << ")";
    
}


/** Set a member variable */
void Dist_phyloCTMCClado::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "Q" )
    {
        q = var;
    }
    else if ( name == "cladoProbs" )
    {
        cladoProbs = var;
    }
    else if ( name == "rootFrequencies" )
    {
        rootFrequencies = var;
    }
    else if ( name == "branchRates" )
    {
        rate = var;
    }
    else if ( name == "siteRates" )
    {
        siteRates = var;
    }
    else if ( name == "pInv" )
    {
        pInv = var;
    }
    else if ( name == "nSites" )
    {
        nSites = var;
    }
    else if ( name == "nStates" )
    {
        nStates = var;
    }
    else if ( name == "type" )
    {
        type = var;
    }
    else if ( name == "treatAmbiguousAsGap" )
    {
        treatAmbiguousAsGap = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
    
}


