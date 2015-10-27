#include "Dist_phyloCTMC.h"

#include "PhyloCTMCSiteHomogeneous.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "PhyloCTMCSiteHomogeneousRestriction.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RevNullObject.h"
#include "RlAminoAcidState.h"
#include "RlBoolean.h"
#include "RlDnaState.h"
#include "RlRateGenerator.h"
#include "RlRnaState.h"
#include "RlString.h"
#include "RlTree.h"
#include "StandardState.h"
#include "RlSimplex.h"
#include "PomoState.h"
#include "NaturalNumbersState.h"

using namespace RevLanguage;

Dist_phyloCTMC::Dist_phyloCTMC() : TypedDistribution< AbstractHomologousDiscreteCharacterData >()
{

}


Dist_phyloCTMC::~Dist_phyloCTMC()
{

}



Dist_phyloCTMC* Dist_phyloCTMC::clone( void ) const
{

    return new Dist_phyloCTMC(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData >* Dist_phyloCTMC::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    bool ambig = static_cast<const RlBoolean &>( treatAmbiguousAsGap->getRevObject() ).getDagNode();
    size_t nNodes = tau->getValue().getNumberOfNodes();
    const std::string& code = static_cast<const RlString &>( coding->getRevObject() ).getValue();

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

    if ( dt == "DNA" )
    {
        RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::DnaState> *dist =
        new RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::DnaState>(tau, true, n, ambig);

        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );

        // set the probability for invariant site (by default this pInv=0.0)
        dist->setPInv( pInvNode );

        // set the clock rates
        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

            // sanity check
            size_t nRates = clockRates->getValue().size();
            if ( (nNodes-1) != nRates )
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

            // sanity check
            if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }

            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }
    else if ( dt == "RNA" )
    {
        RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::RnaState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneousNucleotide<RevBayesCore::RnaState>(tau, true, n, ambig);

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
            // sanity check
            if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }
    else if ( dt == "AA" || dt == "Protein" )
    {
        RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::AminoAcidState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::AminoAcidState>(tau, 20, true, n, ambig);

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
            // sanity check
            if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }
    else if ( dt == "Pomo" )
    {

        // we get the number of states from the rate matrix (we don't know, because Pomo is flexible about its rates)
        // set the rate matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }

        RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::PomoState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::PomoState>(tau, nChars, true, n, ambig);

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
            // sanity check
            if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }
    else if ( dt == "Standard" )
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
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }

        RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::StandardState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::StandardState>(tau, nChars, true, n, ambig);

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
            // sanity check
            if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }
    else if ( dt == "NaturalNumbers" )
    {
        // we get the number of states from the rates matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
            RevBayesCore::g_MAX_NAT_NUM_STATES = nChars;
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
            RevBayesCore::g_MAX_NAT_NUM_STATES = nChars;
        }

        RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::NaturalNumbersState> *dist = new RevBayesCore::PhyloCTMCSiteHomogeneous<RevBayesCore::NaturalNumbersState>(tau, nChars, true, n, ambig);

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
            // sanity check
            if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }
    else if ( dt == "Restriction" )
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
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }

        // sanity check
        if ( nChars != 2 )
        {
            throw RbException( "Only binary characters allowed for type=Restriction" );
        }

        // split the coding option on "|"
        std::string s = code;
        std::vector<std::string> tokens;

        size_t pos = 0;
        while ((pos = s.find("|")) != std::string::npos) {
            tokens.push_back(s.substr(0, pos));
            s.erase(0, pos + 1);
        }
        tokens.push_back(s);

        // set the flags for each token
        int cd = RevBayesCore::RestrictionAscertainmentBias::ALL;
        for(size_t i = 0; i < tokens.size(); i++)
        {
            if(tokens[i] == "noabsencesites")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOABSENCESITES;
            }
            else if(tokens[i] == "nopresencesites")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOPRESENCESITES;
            }
            else if(tokens[i] == "informative")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::INFORMATIVE;
            }
            else if(tokens[i] == "variable")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::VARIABLE;
            }
            else if(tokens[i] == "nosingletongains")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOSINGLETONPRESENCE;
            }
            else if(tokens[i] == "nosingletonlosses")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOSINGLETONABSENCE;
            }
            else if(tokens[i] == "nosingletons")
            {
                cd |= RevBayesCore::RestrictionAscertainmentBias::NOSINGLETONS;
            }
            else if(tokens[i] != "all")
            {
                std::stringstream ss;
                ss << "Unrecognized coding option \"" << tokens[i] << "\"";
                throw RbException(ss.str());
            }
        }

        RevBayesCore::PhyloCTMCSiteHomogeneousRestriction *dist = new RevBayesCore::PhyloCTMCSiteHomogeneousRestriction(tau, true, n, ambig, RevBayesCore::RestrictionAscertainmentBias::Coding(cd));

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
            // sanity check
            if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
            }


            dist->setRateMatrix( rm );
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }

        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }



    return d;
}



/* Get Rev type of object */
const std::string& Dist_phyloCTMC::getClassType(void)
{

    static std::string revType = "Dist_phyloCTMC";

    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_phyloCTMC::getClassTypeSpec(void)
{

    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );

    return revTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_phyloCTMC::getParameterRules(void) const
{

    static MemberRules distMemberRules;
    static bool rulesSet = false;

    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "tree", Tree::getClassTypeSpec(), "The tree along which the process evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( RateGenerator::getClassTypeSpec() );
        rateMatrixTypes.push_back( ModelVector<RateGenerator>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "Q", rateMatrixTypes, "The global or branch-specific rate matrices.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        // optional argument for the root frequencies
        distMemberRules.push_back( new ArgumentRule( "rootFrequencies", Simplex::getClassTypeSpec(), "The root specific frequencies of the characters, if applicable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );

        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates", branchRateTypes, "The global or branch-specific rate multipliers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        ModelVector<RealPos> *defaultSiteRates = new ModelVector<RealPos>();
        distMemberRules.push_back( new ArgumentRule( "siteRates", ModelVector<RealPos>::getClassTypeSpec(), "The rate categories for the sites.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );
        distMemberRules.push_back( new ArgumentRule( "pInv", Probability::getClassTypeSpec(), "The probability of a site being invariant.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(0.0) ) );

        distMemberRules.push_back( new ArgumentRule( "nSites", Natural::getClassTypeSpec(), "The number of sites, used for simulation.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );

        std::vector<std::string> options;
        options.push_back( "DNA" );
        options.push_back( "RNA" );
        options.push_back( "AA" );
        options.push_back( "Pomo" );
        options.push_back( "Protein" );
        options.push_back( "Standard" );
        options.push_back( "NaturalNumbers" );
        options.push_back( "Restriction" );
        distMemberRules.push_back( new OptionRule( "type", new RlString("DNA"), options, "The data type, used for simulation and initialization." ) );

        distMemberRules.push_back( new ArgumentRule( "treatAmbiguousAsGap", RlBoolean::getClassTypeSpec(), "Should we treat ambiguous characters as gaps/missing?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );

        distMemberRules.push_back( new ArgumentRule("coding", RlString::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("all") ) );

        rulesSet = true;
    }

    return distMemberRules;
}


const TypeSpec& Dist_phyloCTMC::getTypeSpec( void ) const
{

    static TypeSpec ts = getClassTypeSpec();

    return ts;
}


/** Print value for user */
void Dist_phyloCTMC::printValue(std::ostream& o) const
{

    o << "Character-State-Evolution-Along-Tree Process(tree=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    o << ", Q=";
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
void Dist_phyloCTMC::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "Q" )
    {
        q = var;
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
    else if ( name == "type" )
    {
        type = var;
    }
    else if ( name == "treatAmbiguousAsGap" )
    {
        treatAmbiguousAsGap = var;
    }
    else if ( name == "coding" )
    {
        coding = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }

}

