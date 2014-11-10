#ifndef Dist_phyloCTMCFiltered_H
#define Dist_phyloCTMCFiltered_H

#include "AbstractDiscreteCharacterData.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlTypedDistribution.h"
#include "FilteredPhyloCTMCSiteHomogeneous.h"
#include "PhyloCTMCSiteHomogeneous.h"
#include "JcRateMatrixFunction.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class treeType>
    class Dist_phyloCTMCFiltered :  public TypedDistribution< AbstractDiscreteCharacterData > {
        
    public:
        Dist_phyloCTMCFiltered( void );
        virtual ~Dist_phyloCTMCFiltered();
        
        // Basic utility functions
        Dist_phyloCTMCFiltered*                                 clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          tree;
        RevPtr<const Variable>                          q;
        RevPtr<const Variable>                          rate;
        RevPtr<const Variable>                          siteRates;
        RevPtr<const Variable>                          rootFrequencies;
        RevPtr<const Variable>                          pInv;
        RevPtr<const Variable>                          nSites;
        RevPtr<const Variable>                          type;
        
    };
    
}


#include "PhyloCTMCSiteHomogeneous.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "OptionRule.h"
#include "RateMatrix.h"
#include "RevNullObject.h"
#include "RlRateMatrix.h"
#include "RlString.h"
#include "StandardState.h"
#include "PomoState.h"


template <class treeType>
RevLanguage::Dist_phyloCTMCFiltered<treeType>::Dist_phyloCTMCFiltered() : TypedDistribution< AbstractDiscreteCharacterData >() {
    
}


template <class treeType>
RevLanguage::Dist_phyloCTMCFiltered<treeType>::~Dist_phyloCTMCFiltered() {
    
}



template <class treeType>
RevLanguage::Dist_phyloCTMCFiltered<treeType>* RevLanguage::Dist_phyloCTMCFiltered<treeType>::clone( void ) const {
  
    return new Dist_phyloCTMCFiltered(*this);
}


template <class treeType>
RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData >* RevLanguage::Dist_phyloCTMCFiltered<treeType>::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    const std::string& dt = static_cast<const RlString &>( type->getRevObject() ).getValue();
    
    size_t nNodes = tau->getValue().getNumberOfNodes();
    
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* siteRatesNode = NULL;
    if ( siteRates != NULL && siteRates->getRevObject() != RevNullObject::getInstance() ){
        siteRatesNode = static_cast<const ModelVector<RealPos> &>( siteRates->getRevObject() ).getDagNode();
    }
	
    RevBayesCore::TypedDagNode< double >* pInvNode = NULL;
    if ( pInv != NULL && pInv->getRevObject() != RevNullObject::getInstance() ){
        pInvNode = static_cast<const Probability &>( pInv->getRevObject() ).getDagNode();
    }
    
    RevBayesCore::TypedDistribution< RevBayesCore::AbstractDiscreteCharacterData > *d = NULL;
	
    if ( dt == "Standard" ){
        size_t numStates = 1;
        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) ) {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
            numStates = rm->getValue()[0].getNumberOfStates();
        } 
        else {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            numStates = rm->getValue().getNumberOfStates();
        }

		RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<RevBayesCore::StandardState,  typename treeType::valueType> *dist =
			new RevBayesCore::FilteredPhyloCTMCSiteHomogeneous<RevBayesCore::StandardState,  typename treeType::valueType>(tau, numStates, false, n);
        
        // set the probability for invariant site (by default this pInv=0.0)
        dist->setPInv( pInvNode );
        
        // set the clock rates
        if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) ) {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();
            
            // sanity check
            size_t nRates = clockRates->getValue().size();
            if ( (nNodes-1) != nRates ) {
                throw RbException( "The number of clock rates does not match the number of branches" );
            }
            
            dist->setClockRate( clockRates );
        }
        else {
            RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
            dist->setClockRate( clockRate );
        }
        
        // set the rate matrix
        if ( q->getRevObject().isType( ModelVector<RateMatrix>::getClassTypeSpec() ) ) {
            RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* rm = static_cast<const ModelVector<RateMatrix> &>( q->getRevObject() ).getDagNode();
            
            // sanity check
            if ( (nNodes-1) != rm->getValue().size() ) {
                throw RbException( "The number of substitution matrices does not match the number of branches" );
            }
            
            dist->setRateMatrix( rm );
        } 
        else {
            RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix &>( q->getRevObject() ).getDagNode();
            dist->setRateMatrix( rm );
        }
        
        if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 ) {
            dist->setSiteRates( siteRatesNode );
        }
        
        d = dist;
    } 
	
    
    return d;
}



/* Get Rev type of object */
template <class treeType>
const std::string& RevLanguage::Dist_phyloCTMCFiltered<treeType>::getClassType(void) { 
    
    static std::string revType = "Dist_phyloCTMCFiltered";
    
	return revType; 
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloCTMCFiltered<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::Dist_phyloCTMCFiltered<treeType>::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distMemberRules.push_back( new ArgumentRule( "tree"           , treeType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( RateMatrix::getClassTypeSpec() );
        rateMatrixTypes.push_back( ModelVector<RateMatrix>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "Q"              , rateMatrixTypes             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        // optional argument for the root frequencies
        distMemberRules.push_back( new ArgumentRule( "rootFrequencies", Simplex::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates"    , branchRateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
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
        distMemberRules.push_back( new OptionRule( "type", new RlString("Standard"), options ) );
        
//		std::vector<std::string> options2;
//        options2.push_back( "Variable" );
//        options2.push_back( "ParsInform" );
//        options2.push_back( "All" );
//        distMemberRules.push_back( new OptionRule( "filter-by", new RlString("Variable"), options2 ) );
        
       rulesSet = true;
    }
    
    return distMemberRules;
}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloCTMCFiltered<treeType>::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class treeType>
void RevLanguage::Dist_phyloCTMCFiltered<treeType>::printValue(std::ostream& o) const
{
    
    o << "Character-State-Evolution-Along-Tree w/Filtered Data Process(tree=";
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
template <class treeType>
void RevLanguage::Dist_phyloCTMCFiltered<treeType>::setConstParameter(const std::string& name, const RevPtr<const Variable> &var)
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
    else
    {
        Distribution::setConstParameter(name, var);
    }
    
}


#endif
