/**
 * @file
 * This file contains the declaration of the probability density function of some distribution.
 * The distribution is given as a parameter so this file is the wrapper to call the pdf of the distribution.
 * Hence, this function can be used inside deterministic nodes.
 *
 * @brief Declaration of the probability density function.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-13, version 1.0
 *
 * $Id: RandomNumberFactory.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef DistributionConstructorFunction_H
#define DistributionConstructorFunction_H

#include "TypedFunction.h"
#include "TypedDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    template <class valueType>
    class DistributionConstructorFunction : public TypedFunction< TypedDistribution<valueType> > {
        
    public:
        DistributionConstructorFunction(const TypedDistribution<valueType> *d, const std::vector< DagNode * > &p);
        DistributionConstructorFunction(const DistributionConstructorFunction &pdf);
        virtual                            ~DistributionConstructorFunction(void);
        
        DistributionConstructorFunction*	clone(void) const;                                                  //!< Create a clon.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        std::vector< DagNode* >				params;
        TypedDistribution<valueType>*       dist;
    };
}

template <class valueType>
RevBayesCore::DistributionConstructorFunction<valueType>::DistributionConstructorFunction(const TypedDistribution<valueType> *d, const std::vector< DagNode * > &p) : TypedDistribution<valueType>( NULL ), params( p ), dist( d ) {

	for (std::vector< DagNode * >::const_iterator it = params.begin(); it != params.end(); ++it) {
		this->addParameter( *it );
	}
    
}


template <class valueType>
RevBayesCore::DistributionConstructorFunction<valueType>::DistributionConstructorFunction(const DistributionConstructorFunction &dcf) : TypedDistribution<valueType>( dcf ), params( dcf.params ), dist( dcf.dist->clone() ) {
    
}

template <class valueType>
RevBayesCore::DistributionConstructorFunction<valueType>::~DistributionConstructorFunction(void)
ß{
    delete dist;
}


template <class valueType>
RevBayesCore::DistributionConstructorFunction<valueType>* RevBayesCore::DistributionConstructorFunction<valueType>::clone( void ) const
{
    return new DistributionConstructorFunction<valueType>(*this);
}


template <class valueType>
void RevBayesCore::DistributionConstructorFunction<valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
//    if (oldP == x) {
//        x = static_cast<const TypedDagNode<valueType>* >( newP );
//    }
//    else {
//        dist->swapParameter(oldP, newP);
//    }
}

template <class valueType>
void RevBayesCore::DistributionConstructorFunction<valueType>::update( void )
{
//    dist->setValue( new valueType(x->getValue()) );
//    *value = dist->computeLnProbability();
}

#endif
