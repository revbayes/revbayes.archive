#ifndef Dist_rlmixture_H
#define Dist_rlmixture_H

#include "RbMixtureDistribution.h"
#include "RealPos.h"
#include "RlTypedDistribution.h"
#include "TypeSpec.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the RbMixtureDistribution
     *
     * The RevLanguage wrapper of the mixture distribution connects
     * the variables/parameters of the distribution and creates the internal RbMixtureDistribution object.
     * Please read the RbMixtureDistribution.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-08-14, version 1.0
     *
     */
    template <typename valType>
    class Dist_rlmixture :  public TypedDistribution< valType >{
        
    public:
        Dist_rlmixture( void );
        virtual                                        ~Dist_rlmixture();
        
        // Basic utility functions
        Dist_rlmixture*                                 clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
        const MethodTable&                              getMethods(void) const;                                                         //!< Get member methods
        MethodTable                                     makeMethods(void) const;                                                        //!< Make member methods

        
        // Distribution functions you have to override
        RevBayesCore::RbMixtureDistribution<typename valType::valueType>*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          values;
        RevPtr<const Variable>                          probabilities;
        
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlDistributionMemberFunction.h"
#include "RlSimplex.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"


template <typename valType>
RevLanguage::Dist_rlmixture<valType>::Dist_rlmixture() : TypedDistribution< valType >(),
values( NULL ),
probabilities( NULL )
{
    
}


template <typename valType>
RevLanguage::Dist_rlmixture<valType>::~Dist_rlmixture()
{
    
}



template <typename valType>
RevLanguage::Dist_rlmixture<valType>* RevLanguage::Dist_rlmixture<valType>::clone( void ) const
{
    
    return new Dist_rlmixture(*this);
}


template <typename valType>
RevBayesCore::RbMixtureDistribution<typename valType::valueType>* RevLanguage::Dist_rlmixture<valType>::createDistribution( void ) const
{
	
    // get the parameters
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<typename valType::valueType> >*  v   = static_cast<const ModelVectorAbstractElement<valType> &>( values->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< std::vector<double> >*                                  p   = static_cast<const Simplex &>( probabilities->getRevObject() ).getDagNode();
    
    RevBayesCore::RbMixtureDistribution<typename valType::valueType>* d = new RevBayesCore::RbMixtureDistribution<typename valType::valueType>(v,p);
    
    return d;
}



/* Get Rev type of object */
template <typename valType>
const std::string& RevLanguage::Dist_rlmixture<valType>::getClassType(void)
{
    
    static std::string revType = "Dist_rlmixture";
    
	return revType;
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_rlmixture<valType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< valType >::getClassTypeSpec() ) );
    
	return revTypeSpec;
}




/** Return member rules (no members) */
template <typename valType>
const RevLanguage::MemberRules& RevLanguage::Dist_rlmixture<valType>::getMemberRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "values"       , ModelVectorAbstractElement<valType>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "probabilities", Simplex::getClassTypeSpec()                            , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
template <typename valType>
const RevLanguage::MethodTable& Dist_rlmixture<valType>::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_rlmixture<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/* Make member methods for this class */
template <typename valType>
RevLanguage::MethodTable Dist_rlmixture<valType>::makeMethods(void) const
{
    
    MethodTable methods = MethodTable();
    
    ArgumentRules* argRules = new ArgumentRules();
    
//    methods.addFunction("getAllocationIndex", new DistributionMemberFunction<Dist_rlmixture<valType> ,Natural>( this, argRules ) );
    
    // Insert inherited methods
    methods.insertInheritedMethods( TypedDistribution<valType>::makeMethods() );
    
    return methods;
}



/** Set a member variable */
template <typename valType>
void RevLanguage::Dist_rlmixture<valType>::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
