#ifndef Dist_reversibleJumpMixtureConstant_H
#define Dist_reversibleJumpMixtureConstant_H

#include "ReversibleJumpMixtureConstantDistribution.h"
#include "RealPos.h"
#include "RlTypedDistribution.h"
#include "TypeSpec.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the ReversibleJumpMixtureConstantDistribution
     *
     * The RevLanguage wrapper of the mixture distribution connects
     * the variables/parameters of the distribution and creates the internal ReversibleJumpMixtureConstantDistribution object.
     * Please read the ReversibleJumpMixtureConstantDistribution.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-08-14, version 1.0
     *
     */
    template <typename valType>
    class Dist_reversibleJumpMixtureConstant :  public TypedDistribution< valType >{
        
    public:
        Dist_reversibleJumpMixtureConstant( void );
        virtual                                        ~Dist_reversibleJumpMixtureConstant();
        
        // Basic utility functions
        Dist_reversibleJumpMixtureConstant*             clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                  //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::ReversibleJumpMixtureConstantDistribution<typename valType::valueType>*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                       constValue;
        RevPtr<const RevVariable>                       baseDistribution;
        RevPtr<const RevVariable>                       probability;
        
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Probability.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"


template <typename valType>
RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::Dist_reversibleJumpMixtureConstant() : TypedDistribution< valType >(),
    constValue( NULL ),
    baseDistribution( NULL ),
    probability( NULL )
{
    
}


template <typename valType>
RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::~Dist_reversibleJumpMixtureConstant()
{
    
}



template <typename valType>
RevLanguage::Dist_reversibleJumpMixtureConstant<valType>* RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::clone( void ) const
{
    
    return new Dist_reversibleJumpMixtureConstant(*this);
}


template <typename valType>
RevBayesCore::ReversibleJumpMixtureConstantDistribution<typename valType::valueType>* RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode< typename valType::valueType >*      cv              = static_cast<const valType &>( constValue->getRevObject() ).getDagNode();
    const Distribution&                                             rlDistribution  = static_cast<const Distribution &>( baseDistribution->getRevObject() );
    RevBayesCore::TypedDistribution<typename valType::valueType>*   g0              = dynamic_cast<RevBayesCore::TypedDistribution<typename valType::valueType>* >( rlDistribution.createDistribution() );

    // error checking
    if ( g0 == NULL )
    {
        throw RbException("Could not cast distribution in reversible jump mixture.");
    }
    RevBayesCore::TypedDagNode< double >*                           p               = static_cast<const Probability &>( probability->getRevObject() ).getDagNode();
    
    RevBayesCore::ReversibleJumpMixtureConstantDistribution<typename valType::valueType>* d		= new RevBayesCore::ReversibleJumpMixtureConstantDistribution<typename valType::valueType>(cv,g0,p);
    
    return d;
}



/* Get Rev type of object */
template <typename valType>
const std::string& RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::getClassType(void)
{
    
    static std::string revType = "Dist_reversibleJumpMixtureConstant";
    
    return revType;
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< valType >::getClassTypeSpec() ) );
    
    return revTypeSpec;
}




/** Return member rules (no members) */
template <typename valType>
const RevLanguage::MemberRules& RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        //distMemberRules.push_back( new ArgumentRule( "constantValue", ModelVector<valType>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "constantValue", valType::getClassTypeSpec(), "The fixed value this distribution can take.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "baseDistribution", TypedDistribution<valType>::getClassTypeSpec(), "The distribution from which the value is alternatively drawn.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "p", Probability::getClassTypeSpec(), "The probability of being the fixed value.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}



/** Set a member variable */
template <typename valType>
void RevLanguage::Dist_reversibleJumpMixtureConstant<valType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "constantValue" )
    {
        constValue = var;
    }
    else if ( name == "baseDistribution" )
    {
        baseDistribution = var;
    }
    else if ( name == "p" )
    {
        probability = var;
    }
    else
    {
        TypedDistribution< valType >::setConstParameter(name, var);
    }
}


#endif
