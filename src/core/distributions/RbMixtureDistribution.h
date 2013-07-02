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


#ifndef RbMixtureDistribution_H
#define RbMixtureDistribution_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template <class mixtureType>
    class RbMixtureDistribution : public TypedDistribution<mixtureType> {
        
    public:
        // constructor(s)
        RbMixtureDistribution(const TypedDagNode< RbVector<mixtureType> > *v, const TypedDagNode<std::vector<double> > *p);
        RbMixtureDistribution(const RbMixtureDistribution<mixtureType> &n);
        
        // public member functions
        RbMixtureDistribution*                              clone(void) const;                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        const RbVector<mixtureType>&                        getParameterValues(void) const;
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
        mixtureType*                                        simulate();
        
        // private members
        const TypedDagNode< RbVector<mixtureType> >*        parameterValues;
        const TypedDagNode<std::vector<double> >*           probabilities;
        
        size_t                                              index;
    };
    
}

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "VectorIndexOperator.h"

#include <cmath>

template <class mixtureType>
RevBayesCore::RbMixtureDistribution<mixtureType>::RbMixtureDistribution(const TypedDagNode< RbVector<mixtureType> > *v, const TypedDagNode<std::vector<double> > *p) : TypedDistribution<mixtureType>( v->getValue()[0].clone() ), parameterValues( v ), probabilities( p ), index( 0 ) {
    
    this->addParameter( parameterValues );
    this->addParameter( probabilities );
    
    delete this->value;
    this->value = simulate();
}


template <class mixtureType>
RevBayesCore::RbMixtureDistribution<mixtureType>::RbMixtureDistribution(const RbMixtureDistribution<mixtureType> &n) : TypedDistribution<mixtureType>( n ), parameterValues( n.parameterValues ), probabilities( n.probabilities ), index( n.index ) {
    // parameters are added automatically
    
    delete this->value;
    this->value = simulate();
}



template <class mixtureType>
RevBayesCore::RbMixtureDistribution<mixtureType>* RevBayesCore::RbMixtureDistribution<mixtureType>::clone( void ) const {
    return new RbMixtureDistribution<mixtureType>( *this );
}



template <class mixtureType>
double RevBayesCore::RbMixtureDistribution<mixtureType>::computeLnProbability( void ) {
    
    return probabilities->getValue()[index];
}


template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    // only delegate when the toucher was our parameters
    if ( affecter == parameterValues )
        this->dagNode->getAffectedNodes( affected );
}


template <class mixtureType>
size_t RevBayesCore::RbMixtureDistribution<mixtureType>::getCurrentIndex( void ) const {
    return index;
}


template <class mixtureType>
size_t RevBayesCore::RbMixtureDistribution<mixtureType>::getNumberOfCategories( void ) const {
    return probabilities->getValue().size();
}


template <class mixtureType>
const RevBayesCore::RbVector<mixtureType>& RevBayesCore::RbMixtureDistribution<mixtureType>::getParameterValues( void ) const {
    
    return parameterValues->getValue();
}

template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::keepSpecialization( DagNode* affecter ) {
    // only do this when the toucher was our parameters
    if ( affecter == parameterValues )
        this->dagNode->keepAffected();
    
}


template <class mixtureType>
mixtureType* RevBayesCore::RbMixtureDistribution<mixtureType>::simulate() {
    
    const std::vector<double> &probs = probabilities->getValue();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double u = rng->uniform01();
    index = 0;
    while ( u > probs[index] ) 
    {
        u -= probs[index];
        index++;
    }
    return parameterValues->getValue()[index].clone();
    
}


template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::redrawValue( void ) {
    
    delete this->value;
    this->value = simulate();

}


template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::restoreSpecialization( DagNode *restorer ) {
    
    // only do this when the toucher was our parameters
    if ( restorer == parameterValues ) 
    {
        delete this->value;
        this->value = parameterValues->getValue()[index].clone();
        this->dagNode->restoreAffected();
    }
    
}


template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::setCurrentIndex(size_t i) {
    
    index = i;
    delete this->value;
    this->value = parameterValues->getValue()[i].clone();

}



template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == parameterValues) 
    {
        parameterValues = static_cast<const TypedDagNode< RbVector<mixtureType> >* >( newP );
    }
    else if (oldP == probabilities) 
    {
        probabilities = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    
}


template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::setValue(mixtureType const &v) {
    
    // TODO: Reimplement this!!! (Sebastian)
//    const std::vector<mixtureType> &vals = parameterValues->getValue();
//    // we need to catch the value and increment the index
//    for (index = 0; index < vals.size(); ++index) 
//    {
//        if ( vals[index] == v ) 
//        {
//            break;
//        }
//    }
    
    // delegate class
    //    StochasticNode<mixtureType>::setValue( v );
}


template <class mixtureType>
void RevBayesCore::RbMixtureDistribution<mixtureType>::touchSpecialization( DagNode *toucher ) {
    // only do this when the toucher was our parameters
    if ( toucher == parameterValues ) 
    {
        delete this->value;
        this->value = parameterValues->getValue()[index].clone();
        this->dagNode->touchAffected();
    }
}

#endif
