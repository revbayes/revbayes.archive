#ifndef Dist_phyloCTMC_H
#define Dist_phyloCTMC_H

#include "AbstractDiscreteCharacterData.h"
#include "ModelVectorAbstractElement.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class treeType>
    class Dist_phyloCTMC :  public TypedDistribution< AbstractDiscreteCharacterData > {
        
    public:
        Dist_phyloCTMC( void );
        virtual ~Dist_phyloCTMC();
        
        // Basic utility functions
        Dist_phyloCTMC*                                 clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          q;
        RevPtr<const Variable>                          rate;
        RevPtr<const Variable>                          siteRates;
        RevPtr<const Variable>                          rootFrequencies;
        RevPtr<const Variable>                          nSites;
        RevPtr<const Variable>                          type;
        
    };
    
}


#include "OptionRule.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "NucleotideBranchHeterogeneousCharEvoModel.h"
#include "RevNullObject.h"
#include "RlRateMatrix.h"
#include "RlString.h"
#include "RateMatrix.h"
#include "StandardState.h"


template <class treeType>
RevLanguage::Dist_phyloCTMC<treeType>::Dist_phyloCTMC() : TypedDistribution< AbstractDiscreteCharacterData >() {
    
}


template <class treeType>
RevLanguage::Dist_phyloCTMC<treeType>::~Dist_phyloCTMC() {
    
}



template <class treeType>
RevLanguage::Dist_phyloCTMC<treeType>* RevLanguage::Dist_phyloCTMC<treeType>::clone( void ) const {
  
    return new Dist_phyloCTMC(*this);
}


template <class treeType>
RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData >* RevLanguage::Dist_phyloCTMC<treeType>::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    
    size_t nNodes = tau->getValue().getNumberOfNodes();
    
    RevBayesCore::TypedDagNode< std::vector<double> >* siteRatesNode = static_cast<const ModelVector<RealPos> &>( siteRates->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractCharacterData > *d = NULL;
    const RevBayesCore::TypedDagNode< std::vector< double > > *rf = NULL;
    if ( rootFrequencies->getRevObject() != RevNullObject::getInstance() )
    {
        rf = static_cast<const Simplex &>( rootFrequencies->getRevObject() ).getDagNode();
    }
    
    if ( dt == "DNA" ) 
    {
        RevBayesCore::NucleotideBranchHeterogeneousCharEvoModel<RevBayesCore::DnaState, typename treeType::valueType> *dist =
            new RevBayesCore::NucleotideBranchHeterogeneousCharEvoModel<RevBayesCore::DnaState, typename treeType::valueType>(tau, true, n);
        
        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );
        
        // set the clock rates
        if ( rate->getRevObjectTypeSpec().isDerivedOf( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< std::vector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();
            
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
        if ( q->getRevObjectTypeSpec().isDerivedOf( ModelVectorAbstractElement<RateMatrix>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVectorAbstractElement<RateMatrix> &>( q->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != rm->getValue().size() ) 
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            
            dist->setRateMatrix( rm );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }
        
        if ( siteRatesNode->getValue().size() > 0 ) 
        {
            dist->setSiteRates( siteRatesNode );
        }
        
        d = dist;
    } 
    else if ( dt == "RNA" )
    {
        RevBayesCore::NucleotideBranchHeterogeneousCharEvoModel<RevBayesCore::RnaState, typename treeType::valueType> *dist = new RevBayesCore::NucleotideBranchHeterogeneousCharEvoModel<RevBayesCore::RnaState, typename treeType::valueType>(tau, true, n);
        
        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );
        
        if ( rate->getRevObjectTypeSpec().isDerivedOf( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< std::vector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();
            
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
        if ( q->getRevObjectTypeSpec().isDerivedOf( ModelVectorAbstractElement<RateMatrix>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVectorAbstractElement<RateMatrix> &>( q->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != rm->getValue().size() ) 
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            
            dist->setRateMatrix( rm );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }
        
        if ( siteRatesNode->getValue().size() > 0 ) 
        {
            dist->setSiteRates( siteRatesNode );
        }

        d = dist;
    }
    else if ( dt == "AA" || dt == "Protein" )
    {
        RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::AminoAcidState, typename treeType::valueType> *dist = new RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::AminoAcidState, typename treeType::valueType>(tau, 20, true, n);
        
        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );
        
        if ( rate->getRevObjectTypeSpec().isDerivedOf( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< std::vector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();
            
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
        if ( q->getRevObjectTypeSpec().isDerivedOf( ModelVectorAbstractElement<RateMatrix>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVectorAbstractElement<RateMatrix> &>( q->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != rm->getValue().size() ) 
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            
            dist->setRateMatrix( rm );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }
        
        if ( siteRatesNode->getValue().size() > 0 ) 
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
        if ( q->getRevObjectTypeSpec().isDerivedOf( ModelVectorAbstractElement<RateMatrix>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVectorAbstractElement<RateMatrix> &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue()[0].getNumberOfStates();
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            nChars = rm->getValue().getNumberOfStates();
        }
        
        RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::StandardState, typename treeType::valueType> *dist = new RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<RevBayesCore::StandardState, typename treeType::valueType>(tau, nChars, true, n);
        
        // set the root frequencies (by default these are NULL so this is OK)
        dist->setRootFrequencies( rf );
        
        if ( rate->getRevObjectTypeSpec().isDerivedOf( ModelVector<RealPos>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< std::vector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();
            
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
        if ( q->getRevObjectTypeSpec().isDerivedOf( ModelVectorAbstractElement<RateMatrix>::getClassTypeSpec() ) )
        {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVectorAbstractElement<RateMatrix> &>( q->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != rm->getValue().size() ) 
            {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            
            dist->setRateMatrix( rm );
        } 
        else 
        {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }
        
        if ( siteRatesNode->getValue().size() > 0 ) 
        {
            dist->setSiteRates( siteRatesNode );
        }
        
        d = dist;
    }
    
    
    return d;
}



/* Get Rev type of object */
template <class treeType>
const std::string& RevLanguage::Dist_phyloCTMC<treeType>::getClassType(void) { 
    
    static std::string revType = "Dist_phyloCTMC";
    
	return revType; 
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloCTMC<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::Dist_phyloCTMC<treeType>::getMemberRules(void) const {
    
    static MemberRules distCharStateEvolutionMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "tree"           , true, treeType::getClassTypeSpec() ) );
        
        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( RateMatrix::getClassTypeSpec() );
        rateMatrixTypes.push_back( ModelVectorAbstractElement<RateMatrix>::getClassTypeSpec() );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "Q"              , true, rateMatrixTypes ) );
        
        // optional argument for the root frequencies
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "rootFrequencies", true, Simplex::getClassTypeSpec(), NULL ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "branchRates"    , true, branchRateTypes, new RealPos(1.0) ) );
        
        ModelVector<RealPos> *defaultSiteRates = new ModelVector<RealPos>();
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "siteRates"    , true, ModelVector<RealPos>::getClassTypeSpec(), defaultSiteRates ) );
        
        distCharStateEvolutionMemberRules.push_back( new ArgumentRule( "nSites"         , true, Natural::getClassTypeSpec() ) );
        
        std::vector<RlString> options;
        options.push_back( RlString("DNA") );
        options.push_back( RlString("RNA") );
        options.push_back( RlString("AA") );
        options.push_back( RlString("Protein") );
        options.push_back( RlString("Standard") );
        distCharStateEvolutionMemberRules.push_back( new OptionRule( "type", new RlString("DNA"), options ) );
        
        rulesSet = true;
    }
    
    return distCharStateEvolutionMemberRules;
}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloCTMC<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class treeType>
void RevLanguage::Dist_phyloCTMC<treeType>::printValue(std::ostream& o) const {
    
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
    if ( rate != NULL ) {
        o << siteRates->getName();
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
template <class treeType>
void RevLanguage::Dist_phyloCTMC<treeType>::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
    else if ( name == "nSites" ) 
    {
        nSites = var;
    }
    else if ( name == "type" ) 
    {
        type = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}


#endif
