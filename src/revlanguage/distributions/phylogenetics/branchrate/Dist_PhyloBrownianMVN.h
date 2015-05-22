#ifndef Dist_PhyloBrownianMVN_H
#define Dist_PhyloBrownianMVN_H

#include "ContinuousCharacterData.h"
#include "RlContinuousCharacterData.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class treeType>
    class Dist_PhyloBrownianMVN :  public TypedDistribution< ContinuousCharacterData > {
        
    public:
        Dist_PhyloBrownianMVN( void );
        virtual ~Dist_PhyloBrownianMVN();
        
        // Basic utility functions
        Dist_PhyloBrownianMVN*                          clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                       tree;
        RevPtr<const RevVariable>                       branchRates;
        RevPtr<const RevVariable>                       siteRates;
        RevPtr<const RevVariable>                       rootStates;
        RevPtr<const RevVariable>                       nSites;
        
        
    };
    
}


#include "PhyloBrownianProcessMVN.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"

template <class treeType>
RevLanguage::Dist_PhyloBrownianMVN<treeType>::Dist_PhyloBrownianMVN() : TypedDistribution< ContinuousCharacterData >()
{
    
}


template <class treeType>
RevLanguage::Dist_PhyloBrownianMVN<treeType>::~Dist_PhyloBrownianMVN()
{
    
}



template <class treeType>
RevLanguage::Dist_PhyloBrownianMVN<treeType>* RevLanguage::Dist_PhyloBrownianMVN<treeType>::clone( void ) const {
    
    return new Dist_PhyloBrownianMVN(*this);
}


template <class treeType>
RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >* RevLanguage::Dist_PhyloBrownianMVN<treeType>::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();
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
    
    RevBayesCore::PhyloBrownianProcessMVN< typename treeType::valueType> *dist = new RevBayesCore::PhyloBrownianProcessMVN<typename treeType::valueType>(tau, homBranchRatesNode, hetBranchRatesNode, homSiteRatesNode, hetSiteRatesNode, homRootStatesNode, hetRootStatesNode, n);
    
    return dist;
}



/* Get Rev type of object */
template <class treeType>
const std::string& RevLanguage::Dist_PhyloBrownianMVN<treeType>::getClassType(void)
{
    
    static std::string revType = "Dist_PhyloBrownianMVN";
    
    return revType;
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_PhyloBrownianMVN<treeType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}




/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::Dist_PhyloBrownianMVN<treeType>::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "tree" , treeType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates" , branchRateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> siteRateTypes;
        siteRateTypes.push_back( RealPos::getClassTypeSpec() );
        siteRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        RealPos *defaultSiteRates = new RealPos(1.0);
        distMemberRules.push_back( new ArgumentRule( "siteRates" , siteRateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );

        std::vector<TypeSpec> rootStateTypes;
        rootStateTypes.push_back( Real::getClassTypeSpec() );
        rootStateTypes.push_back( ModelVector<Real>::getClassTypeSpec() );
        Real *defaultRootStates = new Real(0.0);
        distMemberRules.push_back( new ArgumentRule( "rootStates" , rootStateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultRootStates ) );

        distMemberRules.push_back( new ArgumentRule( "nSites"         , Natural::getClassTypeSpec()             , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_PhyloBrownianMVN<treeType>::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class treeType>
void RevLanguage::Dist_PhyloBrownianMVN<treeType>::printValue(std::ostream& o) const
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
template <class treeType>
void RevLanguage::Dist_PhyloBrownianMVN<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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


#endif
