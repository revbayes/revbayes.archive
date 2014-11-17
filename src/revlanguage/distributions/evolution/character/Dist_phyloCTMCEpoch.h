#ifndef Dist_phyloCTMCEpoch_H
#define Dist_phyloCTMCEpoch_H

#include "AbstractDiscreteCharacterData.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    class Dist_phyloCTMCEpoch :  public TypedDistribution< AbstractDiscreteCharacterData > {
        
    public:
        Dist_phyloCTMCEpoch( void );
        virtual ~Dist_phyloCTMCEpoch();
        
        // Basic utility functions
        Dist_phyloCTMCEpoch*                            clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                          tree;
        RevPtr<const RevVariable>                          q;
        RevPtr<const RevVariable>                          rate;
        RevPtr<const RevVariable>                          siteRates;
        RevPtr<const RevVariable>                          rootFrequencies;
        RevPtr<const RevVariable>                          pInv;
        RevPtr<const RevVariable>                          nSites;
        RevPtr<const RevVariable>                          nStates;
        RevPtr<const RevVariable>                          type;
        RevPtr<const RevVariable>                          epochTimes;
        RevPtr<const RevVariable>                          epochRates;
        
    };
    
}

#include "PhyloCTMCEpoch.h"
#include "OptionRule.h"
#include "RateMatrix.h"
#include "RevNullObject.h"
#include "RlRateMatrix.h"
#include "RlString.h"
#include "StandardState.h"
#include "PomoState.h"

RevLanguage::Dist_phyloCTMCEpoch::Dist_phyloCTMCEpoch() : TypedDistribution< AbstractDiscreteCharacterData >() {
    
}

RevLanguage::Dist_phyloCTMCEpoch::~Dist_phyloCTMCEpoch() {
    
}


RevLanguage::Dist_phyloCTMCEpoch* RevLanguage::Dist_phyloCTMCEpoch::clone( void ) const {
    
    return new Dist_phyloCTMCEpoch(*this);
}

RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData >* RevLanguage::Dist_phyloCTMCEpoch::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<TimeTree&>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    
    // epoch stuff
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* ecr = static_cast<const ModelVector<RealPos> &>( epochRates->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* et = static_cast<const ModelVector<RealPos> &>( epochTimes->getRevObject() ).getDagNode();
     RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* erm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
    
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
    
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData > *d = NULL;
    const RevBayesCore::TypedDagNode< RevBayesCore::RbVector< double > > *rf = NULL;
    if ( rootFrequencies->getRevObject() != RevNullObject::getInstance() )
    {
        rf = static_cast<const Simplex &>( rootFrequencies->getRevObject() ).getDagNode();
    }
    
    if ( dt == "DNA" )
    {
        RevBayesCore::PhyloCTMCEpoch<RevBayesCore::DnaState> *dist =
        new RevBayesCore::PhyloCTMCEpoch<RevBayesCore::DnaState>(tau, 4, true, n);
        
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
//        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) )
//        {
//            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
//            
//            // sanity check
//            if ( (nNodes-1) != rm->getValue().size() )
//            {
//                throw RbException( "The number of substitution matrices does not match the number of branches" );
//            }
//            
//            dist->setRateMatrix( rm );
//        }
//        else
//        {
//            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
//            dist->setRateMatrix( rm );
//        }
        
        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }

        dist->setEpochTimes( et );
        dist->setEpochClockRates( ecr );
        dist->setEpochRateMatrices( erm );

        
        d = dist;
    }
    else if ( dt == "RNA" )
    {
        RevBayesCore::PhyloCTMCEpoch<RevBayesCore::RnaState> *dist = new RevBayesCore::PhyloCTMCEpoch<RevBayesCore::RnaState>(tau, 4, true, n);
        
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
//        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) )
//        {
//            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
//            
//            // sanity check
//            if ( (nNodes-1) != rm->getValue().size() )
//            {
//                throw RbException( "The number of substitution matrices does not match the number of branches" );
//            }
//            
//            dist->setRateMatrix( rm );
//        }
//        else
//        {
//            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
//            dist->setRateMatrix( rm );
//        }
        
        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }
        
        dist->setEpochClockRates( ecr );
        dist->setEpochRateMatrices( erm );
        dist->setEpochTimes( et );

        d = dist;
    }
    else if ( dt == "AA" || dt == "Protein" )
    {
        RevBayesCore::PhyloCTMCEpoch<RevBayesCore::AminoAcidState> *dist = new RevBayesCore::PhyloCTMCEpoch<RevBayesCore::AminoAcidState>(tau, 20, true, n);
        
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
//        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) )
//        {
//            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
//            
//            // sanity check
//            if ( (nNodes-1) != rm->getValue().size() )
//            {
//                throw RbException( "The number of substitution matrices does not match the number of branches" );
//            }
//            
//            dist->setRateMatrix( rm );
//        }
//        else
//        {
//            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
//            dist->setRateMatrix( rm );
//        }
        
        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }
        
        dist->setEpochClockRates( ecr );
        dist->setEpochRateMatrices( erm );
        dist->setEpochTimes( et );
        
        d = dist;
    }
    else if ( dt == "Pomo" )
    {
        
        // we get the number of states from the rate matrix (we don't know, because Pomo is flexible about its rates)
        // set the rate matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }
        
        RevBayesCore::PhyloCTMCEpoch<RevBayesCore::PomoState> *dist = new RevBayesCore::PhyloCTMCEpoch<RevBayesCore::PomoState>(tau, nChars, true, n);
        
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
//        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) )
//        {
//            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
//            
//            // sanity check
//            if ( (nNodes-1) != rm->getValue().size() )
//            {
//                throw RbException( "The number of substitution matrices does not match the number of branches" );
//            }
//            
//            dist->setRateMatrix( rm );
//        }
//        else
//        {
//            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
//            dist->setRateMatrix( rm );
//        }
        
        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }
        
        dist->setEpochClockRates( ecr );
        dist->setEpochRateMatrices( erm );
        dist->setEpochTimes( et );

        d = dist;
    }
    else if ( dt == "Standard" )
    {
        // we get the number of states from the rates matrix
        // set the rate matrix
        size_t nChars = 1;
        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        }
        else
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }
        
        RevBayesCore::PhyloCTMCEpoch<RevBayesCore::StandardState>  *dist = new RevBayesCore::PhyloCTMCEpoch<RevBayesCore::StandardState>(tau, nChars, true, n);
        
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
//        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) )
//        {
//            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
//            
//            // sanity check
//            if ( (nNodes-1) != rm->getValue().size() )
//            {
//                throw RbException( "The number of substitution matrices does not match the number of branches" );
//            }
//            
//            dist->setRateMatrix( rm );
//        }
//        else
//        {
//            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
//            dist->setRateMatrix( rm );
//        }
        
        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
        {
            dist->setSiteRates( siteRatesNode );
        }
        
        dist->setEpochClockRates( ecr );
        dist->setEpochRateMatrices( erm );
        dist->setEpochTimes( et );

        d = dist;
    }
    
    
    return d;
}



/* Get Rev type of object */const std::string& RevLanguage::Dist_phyloCTMCEpoch::getClassType(void) {
    
    static std::string revType = "Dist_phyloCTMCEpoch";
    
	return revType;
}

/* Get class type spec describing type of object */const RevLanguage::TypeSpec& RevLanguage::Dist_phyloCTMCEpoch::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}




/** Return member rules (no members) */const RevLanguage::MemberRules& RevLanguage::Dist_phyloCTMCEpoch::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        // epoch model requires time tree
        distMemberRules.push_back( new ArgumentRule( "tree"           , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        // epoch model requires vector of Q
        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( ModelVector<RateMatrix>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "Q"              , rateMatrixTypes             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        // optional argument for the root frequencies
        distMemberRules.push_back( new ArgumentRule( "rootFrequencies", Simplex::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        // branch rates
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates"    , branchRateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        // epoch rates
        distMemberRules.push_back( new ArgumentRule( "epochRates"    , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>(std::vector<double>(1, 1.0)) ) );
        
        // epoch times
        distMemberRules.push_back( new ArgumentRule( "epochTimes"    , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>(std::vector<double>(1, 0.0)) ) );
        
        
        ModelVector<RealPos> *defaultSiteRates = new ModelVector<RealPos>();
        distMemberRules.push_back( new ArgumentRule( "siteRates"      , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );
        distMemberRules.push_back( new ArgumentRule( "pInv"           , Probability::getClassTypeSpec()         , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(0.0) ) );
        
        distMemberRules.push_back( new ArgumentRule( "nSites"         , Natural::getClassTypeSpec()             , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );
        
        
        std::vector<std::string> options;
        options.push_back( "DNA" );
        options.push_back( "RNA" );
        options.push_back( "AA" );
        options.push_back( "Pomo" );
        options.push_back( "Protein" );
        options.push_back( "Standard" );
        distMemberRules.push_back( new OptionRule( "type", new RlString("DNA"), options ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}

const RevLanguage::TypeSpec& RevLanguage::Dist_phyloCTMCEpoch::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */void RevLanguage::Dist_phyloCTMCEpoch::printValue(std::ostream& o) const
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


/** Set a member variable */void RevLanguage::Dist_phyloCTMCEpoch::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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
    else if ( name == "nStates" )
    {
        nStates = var;
    }
    else if ( name == "type" )
    {
        type = var;
    }
    else if ( name == "epochTimes" )
    {
        epochTimes = var;
    }
    else if ( name == "epochRates" )
    {
        epochRates = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
    
}


#endif
