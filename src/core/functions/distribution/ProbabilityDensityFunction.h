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

#ifndef ProbabilityDensityFunction_H
#define ProbabilityDensityFunction_H

#include "ContinuousFunction.h"
#include "TypedDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    template <class valueType>
    class ProbabilityDensityFunction : public ContinuousFunction {
        
    public:
        ProbabilityDensityFunction(const TypedDagNode<valueType> *x, TypedDistribution<valueType> *d);
        ProbabilityDensityFunction(const ProbabilityDensityFunction &pdf);
        virtual                            ~ProbabilityDensityFunction(void);
        
        ProbabilityDensityFunction*         clone(void) const;                                                  //!< Create a clon.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<valueType>*      x;
        TypedDistribution<valueType>*       dist;
    };
}

template <class valueType>
RevBayesCore::ProbabilityDensityFunction<valueType>::ProbabilityDensityFunction(const TypedDagNode<valueType> *z, TypedDistribution<valueType>* d) : ContinuousFunction( new double(0.0) ), x( z ), dist( d ) {
    addParameter( x );
    
}


template <class valueType>
RevBayesCore::ProbabilityDensityFunction<valueType>::ProbabilityDensityFunction(const ProbabilityDensityFunction &pdf) : ContinuousFunction( pdf ), x( pdf.x ), dist( pdf.dist->clone() ) {
    
}

template <class valueType>
RevBayesCore::ProbabilityDensityFunction<valueType>::~ProbabilityDensityFunction(void) {
    delete dist;
}


template <class valueType>
RevBayesCore::ProbabilityDensityFunction<valueType>* RevBayesCore::ProbabilityDensityFunction<valueType>::clone( void ) const {
    return new ProbabilityDensityFunction<valueType>(*this);
}


template <class valueType>
void RevBayesCore::ProbabilityDensityFunction<valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == x) {
        x = static_cast<const TypedDagNode<valueType>* >( newP );
    }
    else {
        dist->swapParameter(oldP, newP);
    }
}

template <class valueType>
void RevBayesCore::ProbabilityDensityFunction<valueType>::update( void ) {
    dist->setValue( new valueType(x->getValue()) );
    *value = dist->computeLnProbability();
}

#endif
