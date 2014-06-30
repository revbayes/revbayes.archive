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
    
    template <typename valueType>
    class TypedDistribution : public Distribution {
        
    public:
        virtual                                         ~TypedDistribution(void);                                                                  //!< Destructor
        TypedDistribution<valueType>(const TypedDistribution<valueType> &x);                                                                //!< Copy constuctor
        
        virtual valueType*                              createRandomVariable(void) const;                                                   //!< Create a random variable from this distribution        
        
        // Basic utility functions you have to override
        virtual TypedDistribution<valueType>*           clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                       getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec
        
        
        // Distribution functions you have to override
        virtual RevBayesCore::TypedDistribution<typename valueType::valueType>*     createDistribution(void) const = 0;                                                 //!< Create a random variable from this distribution
        
        
    protected:
        TypedDistribution<valueType>(void);                                                                                                 //!< Basic constructor
        
    };
    
    
}

#include "StochasticNode.h"
#include "TypedDistribution.h"

template <typename valueType>
RevLanguage::TypedDistribution<valueType>::TypedDistribution() : Distribution() {
    
}



template <typename valueType>
RevLanguage::TypedDistribution<valueType>::TypedDistribution( const TypedDistribution<valueType> &d ) : Distribution(d) {
    
}



template <typename valueType>
RevLanguage::TypedDistribution<valueType>::~TypedDistribution() {
    
}


template <typename valueType>
valueType* RevLanguage::TypedDistribution<valueType>::createRandomVariable(void) const { 
    
    RevBayesCore::TypedDistribution<typename valueType::valueType>* d = createDistribution();
    RevBayesCore::TypedDagNode<typename valueType::valueType>* rv  = new RevBayesCore::StochasticNode<typename valueType::valueType>("", d);
    
    return new valueType(rv);
}


/* Get class name of object */
template <typename valueType>
const std::string& RevLanguage::TypedDistribution<valueType>::getClassName(void) { 
    
    static std::string rbClassName = "Distribution<"+ valueType::getClassName() +">";
    
	return rbClassName; 
}


/* Get class type spec describing type of object */
template <typename valueType>
const RevLanguage::TypeSpec& RevLanguage::TypedDistribution<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}


#endif

