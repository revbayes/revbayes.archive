/**
 * @file
 * This file contains the declaration of Distribution, which is
 * the interface and abstract base class for RevLanguage distribution objects.
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface Distribution
 * @package distributions
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Function.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef RlTypedDistribution_H
#define RlTypedDistribution_H

#include "Real.h"
#include "RlDistribution.h"
#include "TypedDistribution.h"

namespace RevLanguage {
    
    template <typename rlType>
    class TypedDistribution : public Distribution {
        
    public:
        virtual                                         ~TypedDistribution(void);                                                                  //!< Destructor
        TypedDistribution<rlType>(const TypedDistribution<rlType> &x);                                                                //!< Copy constuctor
        
        // The value type definition
        typedef typename rlType::valueType valueType;
        
        virtual const TypeSpec&                         getVariableTypeSpec(void) const;                                                    //!< Get the variable type spec of this distribution
        virtual rlType*                                 createRandomVariable(void) const;                                                   //!< Create a random variable from this distribution
        
        // Basic utility functions you have to override
        virtual TypedDistribution<rlType>*              clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                       getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec
        
        
        // Distribution functions you have to override
        virtual RevBayesCore::TypedDistribution<valueType>*     createDistribution(void) const = 0;                                                 //!< Create a random variable from this distribution
        
        
    protected:
        TypedDistribution<rlType>(void);                                                                                                 //!< Basic constructor
        
    };
    
    
}

#include "RlStochasticNode.h"
#include "TypedDistribution.h"

template <typename rlType>
RevLanguage::TypedDistribution<rlType>::TypedDistribution() : Distribution() {
    
}



template <typename rlType>
RevLanguage::TypedDistribution<rlType>::TypedDistribution( const TypedDistribution<rlType> &d ) : Distribution(d) {
    
}



template <typename rlType>
RevLanguage::TypedDistribution<rlType>::~TypedDistribution() {
    
}


template <typename rlType>
rlType* RevLanguage::TypedDistribution<rlType>::createRandomVariable( void ) const
{
    
    RevBayesCore::TypedDistribution<typename rlType::valueType>* d = createDistribution();
    RevBayesCore::TypedDagNode<typename rlType::valueType>* rv  = new StochasticNode<typename rlType::valueType>("", d, this->clone() );
    
    return new rlType(rv);
}


/* Get Rev type of object */
template <typename rlType>
const std::string& RevLanguage::TypedDistribution<rlType>::getClassType(void) {
    
//    static std::string revType = "Distribution<"+ rlType::getClassType() +">";
    static std::string revType = "Distribution__"+ rlType::getClassType();
    
	return revType; 
}


/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::TypedDistribution<rlType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get Rev type of distribution variable */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::TypedDistribution<rlType>::getVariableTypeSpec( void ) const
{
    
    return rlType::getClassTypeSpec();
}


#endif

