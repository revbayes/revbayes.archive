/**
 * @file
 * This file contains the declaration of RbMixtureAllocationMove, which changes
 * one value in the allocation vector of a RbMixture.
 *
 * @brief Declaration of RbMixtureAllocationMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-20, version 1.0
 *
 * $Id: SimplexMove.h 1535 2012-05-11 12:54:35Z hoehna $
 */

#ifndef RbMixtureAllocationMove_H
#define RbMixtureAllocationMove_H

#include <ostream>

#include "RbMixtureDistribution.h"
#include "SimpleMove.h"

namespace RevBayesCore {
    
    template <class valueType>
    class RbMixtureAllocationMove : public SimpleMove {
        
    public:
        RbMixtureAllocationMove(StochasticNode<valueType>* node, double weight);                                                                      //!< Internal constructor
        
        // Basic utility functions
        RbMixtureAllocationMove<valueType>*                     clone(void) const;                                                                  //!< Clone object
        void                                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                                  performSimpleMove(void);                                                            //!< Perform move
        void                                                    rejectSimpleMove(void);
        
    private:
        
        StochasticNode<valueType>*                              variable;
        size_t                                                  newCategory;
        size_t                                                  oldCategory;
        
    };
    
}


#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include <cmath>

template <class valueType>
RevBayesCore::RbMixtureAllocationMove<valueType>::RbMixtureAllocationMove(StochasticNode<valueType> *v, double w) : SimpleMove( v, w ), variable( v ) {
    
}


/** Clone object */
template <class valueType>
RevBayesCore::RbMixtureAllocationMove<valueType>* RevBayesCore::RbMixtureAllocationMove<valueType>::clone( void ) const {
    
    return new RbMixtureAllocationMove<valueType>( *this );
}



template <class valueType>
const std::string& RevBayesCore::RbMixtureAllocationMove<valueType>::getMoveName( void ) const {
    static std::string name = "RbMixture-Reallocation";
    
    return name;
}



/** Perform the move */
template <class valueType>
double RevBayesCore::RbMixtureAllocationMove<valueType>::performSimpleMove( void ) {
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    RbMixtureDistribution<valueType>& dist = static_cast<RbMixtureDistribution<valueType> &>( variable->getDistribution() );
    
    // get the number of categories
    size_t n = dist.getNumberOfCategories();
    
    // get the current index
    oldCategory = dist.getCurrentIndex();
    
    // draw a new category
    newCategory = (int)(rng->uniform01() * n);
    
    // set our new value
    dist.setCurrentIndex( newCategory );
    
    return 0.0;
}


template <class valueType>
void RevBayesCore::RbMixtureAllocationMove<valueType>::rejectSimpleMove( void ) {
    // just reset the index
    
    RbMixtureDistribution<valueType>& dist = static_cast<RbMixtureDistribution<valueType> &>( variable->getDistribution() );
    dist.setCurrentIndex( oldCategory );
    
}


template <class valueType>
void RevBayesCore::RbMixtureAllocationMove<valueType>::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<valueType>* >( newN );
}


#endif

