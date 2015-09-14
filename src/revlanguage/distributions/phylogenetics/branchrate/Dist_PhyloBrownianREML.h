#ifndef Dist_PhyloBrownianREML_H
#define Dist_PhyloBrownianREML_H

#include "ContinuousCharacterData.h"
#include "RlContinuousCharacterData.h"
#include "RlTypedDistribution.h"
#include "TimeTree.h"

namespace RevLanguage {
    
    template <class treeType>
    class Dist_PhyloBrownianREML :  public TypedDistribution< ContinuousCharacterData > {
        
    public:
        Dist_PhyloBrownianREML( void );
        virtual ~Dist_PhyloBrownianREML();
        
        // Basic utility functions
        Dist_PhyloBrownianREML*                         clone(void) const;                                                              //!< Clone the object
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
        RevPtr<const RevVariable>                       nSites;
        
        
    };
    
}


#include "PhyloBrownianProcessREML.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"

template <class treeType>
RevLanguage::Dist_PhyloBrownianREML<treeType>::Dist_PhyloBrownianREML() : TypedDistribution< ContinuousCharacterData >()
{
    
}


template <class treeType>
RevLanguage::Dist_PhyloBrownianREML<treeType>::~Dist_PhyloBrownianREML()
{
    
}



template <class treeType>
RevLanguage::Dist_PhyloBrownianREML<treeType>* RevLanguage::Dist_PhyloBrownianREML<treeType>::clone( void ) const {
    
    return new Dist_PhyloBrownianREML(*this);
}


template <class treeType>
RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >* RevLanguage::Dist_PhyloBrownianREML<treeType>::createDistribution( void ) const
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
    
    RevBayesCore::PhyloBrownianProcessREML< typename treeType::valueType> *dist = new RevBayesCore::PhyloBrownianProcessREML<typename treeType::valueType>(tau, homBranchRatesNode, hetBranchRatesNode, homSiteRatesNode, hetSiteRatesNode, n);
    
    return dist;
}



/* Get Rev type of object */
template <class treeType>
const std::string& RevLanguage::Dist_PhyloBrownianREML<treeType>::getClassType(void)
{
    
    static std::string revType = "Dist_PhyloBrownianREML";
    
    return revType;
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_PhyloBrownianREML<treeType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}




/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::Dist_PhyloBrownianREML<treeType>::getParameterRules(void) const
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
       
        distMemberRules.push_back( new ArgumentRule( "nSites"         , Natural::getClassTypeSpec()             , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );

        rulesSet = true;
    }
    
    return distMemberRules;
}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_PhyloBrownianREML<treeType>::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class treeType>
void RevLanguage::Dist_PhyloBrownianREML<treeType>::printValue(std::ostream& o) const
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
template <class treeType>
void RevLanguage::Dist_PhyloBrownianREML<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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


#endif
