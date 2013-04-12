/**
 * @file
 * This file contains the declaration of the Mixture class. 
 * A mixture object holds the mapping between parameter values and the indix of this parameters.
 *
 *
 * @brief Declaration of the Mixture class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */


#ifndef MixtureDistribution_H
#define MixtureDistribution_H

#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template <class mixtureType>
    class MixtureDistribution : public TypedDistribution<mixtureType> {
        
    public:
        // constructor(s)
        MixtureDistribution(const TypedDagNode<std::vector<mixtureType> > *v, const TypedDagNode<std::vector<double> > *p);
        MixtureDistribution(const MixtureDistribution<mixtureType> &n);
        
        // public member functions
        MixtureDistribution*                                clone(void) const;                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        const std::vector<mixtureType>&                     getParameterValues(void) const;
        size_t                                              getCurrentIndex(void) const;
        size_t                                              getNumberOfCategories(void) const;
        void                                                redrawValue(void);
        void                                                setCurrentIndex(size_t i);
        void                                                setValue(const mixtureType &v);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
        
        // special handling of state changes
        void                                                getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        void                                                keepSpecialization(DagNode* affecter);
        void                                                restoreSpecialization(DagNode *restorer);
        void                                                touchSpecialization(DagNode *toucher);
        
        
    private:
        // helper methods
        const mixtureType&                                  simulate();
        
        // private members
        const TypedDagNode<std::vector<mixtureType> >*      parameterValues;
        const TypedDagNode<std::vector<double> >*           probabilities;
        
        size_t                                              index;
    };
    
}

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "VectorIndexOperator.h"

#include <cmath>

template <class mixtureType>
RevBayesCore::MixtureDistribution<mixtureType>::MixtureDistribution(const TypedDagNode<std::vector<mixtureType> > *v, const TypedDagNode<std::vector<double> > *p) : TypedDistribution<mixtureType>( new mixtureType( v->getValue()[0]) ), parameterValues( v ), probabilities( p ), index( 0 ) {
    
    this->addParameter( parameterValues );
    this->addParameter( probabilities );
    
    *this->value = simulate();
}


template <class mixtureType>
RevBayesCore::MixtureDistribution<mixtureType>::MixtureDistribution(const MixtureDistribution<mixtureType> &n) : TypedDistribution<mixtureType>( n ), parameterValues( n.parameterValues ), probabilities( n.probabilities ), index( n.index ) {
    // parameters are added automatically
    
    *(this->value) = simulate();
}



template <class mixtureType>
RevBayesCore::MixtureDistribution<mixtureType>* RevBayesCore::MixtureDistribution<mixtureType>::clone( void ) const {
    return new MixtureDistribution<mixtureType>( *this );
}



template <class mixtureType>
double RevBayesCore::MixtureDistribution<mixtureType>::computeLnProbability( void ) {
    
    return probabilities->getValue()[index];
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    // only delegate when the toucher was our parameters
    if ( affecter == parameterValues )
        this->dagNode->getAffectedNodes( affected );
}


template <class mixtureType>
size_t RevBayesCore::MixtureDistribution<mixtureType>::getCurrentIndex( void ) const {
    return index;
}


template <class mixtureType>
size_t RevBayesCore::MixtureDistribution<mixtureType>::getNumberOfCategories( void ) const {
    return probabilities->getValue().size();
}


template <class mixtureType>
const std::vector<mixtureType>& RevBayesCore::MixtureDistribution<mixtureType>::getParameterValues( void ) const {
    return parameterValues->getValue();
}

template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::keepSpecialization( DagNode* affecter ) {
    // only do this when the toucher was our parameters
    if ( affecter == parameterValues )
        this->dagNode->keepAffected();
}


template <class mixtureType>
const mixtureType& RevBayesCore::MixtureDistribution<mixtureType>::simulate() {
    
    const std::vector<double> &probs = probabilities->getValue();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double u = rng->uniform01();
    index = 0;
    while ( u > probs[index] ) {
        u -= probs[index];
        index++;
    }
    return parameterValues->getValue()[index];
    
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::redrawValue( void ) {
    *(this->value) = simulate();
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::setCurrentIndex(size_t i) {
    index = i;
    *(this->value) = parameterValues->getValue()[i];
}



template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == parameterValues) {
        parameterValues = static_cast<const TypedDagNode<std::vector<mixtureType> >* >( newP );
    }
    else if (oldP == probabilities) {
        probabilities = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::restoreSpecialization( DagNode *restorer ) {
    // only do this when the toucher was our parameters
    if ( restorer == parameterValues ) {
        *this->value = parameterValues->getValue()[index];
        this->dagNode->restoreAffected();
    }
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::setValue(mixtureType const &v) {
    
    const std::vector<mixtureType> &vals = parameterValues->getValue();
    // we need to catch the value and increment the index
    for (index = 0; index < vals.size(); ++index) {
        if ( vals[index] == v ) {
            break;
        }
    }
    
    // delegate class
//    StochasticNode<mixtureType>::setValue( v );
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::touchSpecialization( DagNode *toucher ) {
    // only do this when the toucher was our parameters
    if ( toucher == parameterValues ) {
        *this->value = parameterValues->getValue()[index];
        this->dagNode->touchAffected();
    }
}

#endif
