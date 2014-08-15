#ifndef Dist_mixture_H
#define Dist_mixture_H

#include "MixtureDistribution.h"
#include "RealPos.h"
#include "RlTypedDistribution.h"
#include "TypeSpec.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the MixtureDistribution
     *
     * The RevLanguage wrapper of the mixture distribution connects
     * the variables/parameters of the distribution and creates the internal MixtureDistribution object.
     * Please read the MixtureDistribution.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-08-14, version 1.0
     *
     */
    template <typename valType>
    class Dist_mixture :  public TypedDistribution< valType >{
        
    public:
                                                        Dist_mixture( void );
        virtual                                        ~Dist_mixture();
        
        // Basic utility functions
        Dist_mixture*                                   clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::MixtureDistribution<typename valType::valueType>*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          values;
        RevPtr<const Variable>                          probabilities;
        
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlSimplex.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"


template <typename valType>
RevLanguage::Dist_mixture<valType>::Dist_mixture() : TypedDistribution< valType >(),
    values( NULL ),
    probabilities( NULL )
{
    
}


template <typename valType>
RevLanguage::Dist_mixture<valType>::~Dist_mixture()
{
    
}



template <typename valType>
RevLanguage::Dist_mixture<valType>* RevLanguage::Dist_mixture<valType>::clone( void ) const
{
    
    return new Dist_mixture(*this);
}


template <typename valType>
RevBayesCore::MixtureDistribution<typename valType::valueType>* RevLanguage::Dist_mixture<valType>::createDistribution( void ) const
{
	
    // get the parameters
    RevBayesCore::TypedDagNode< std::vector<typename valType::valueType> >* v   = static_cast<const ModelVector<valType> &>( values->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< std::vector<double> >*                      p   = static_cast<const Simplex &>( probabilities->getRevObject() ).getDagNode();
    
    RevBayesCore::MixtureDistribution<typename valType::valueType>* d		= new RevBayesCore::MixtureDistribution<typename valType::valueType>(v,p);
    
    return d;
}



/* Get Rev type of object */
template <typename valType>
const std::string& RevLanguage::Dist_mixture<valType>::getClassType(void)
{
    
    static std::string revType = "Dist_mixture";
    
	return revType;
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_mixture<valType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< valType >::getClassTypeSpec() ) );
    
	return revTypeSpec;
}




/** Return member rules (no members) */
template <typename valType>
const RevLanguage::MemberRules& RevLanguage::Dist_mixture<valType>::getMemberRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "values",          true, ModelVector<valType>::getClassTypeSpec() ) );
        distMemberRules.push_back( new ArgumentRule( "probabilities",   true, Simplex::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_mixture<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}



/** Set a member variable */
template <typename valType>
void RevLanguage::Dist_mixture<valType>::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "values" )
    {
        values = var;
    }
    else if ( name == "probabilities" )
    {
        probabilities = var;
    }
    else
    {
        TypedDistribution< valType >::setConstMemberVariable(name, var);
    }
}


#endif
