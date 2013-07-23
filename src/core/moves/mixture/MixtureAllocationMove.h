/**
 * @file
 * This file contains the declaration of MixtureAllocationMove, which changes
 * one value in the allocation vector of a mixture.
 *
 * @brief Declaration of MixtureAllocationMove
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

#ifndef MixtureAllocationMove_H
#define MixtureAllocationMove_H

#include <ostream>

#include "MixtureDistribution.h"
#include "SimpleMove.h"

namespace RevBayesCore {

    template <class valueType>
    class MixtureAllocationMove : public SimpleMove {
    
    public:
        MixtureAllocationMove(StochasticNode<valueType>* node, double weight);                                                                      //!< Internal constructor
    
        // Basic utility functions
        MixtureAllocationMove<valueType>*                       clone(void) const;                                                                  //!< Clone object
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
RevBayesCore::MixtureAllocationMove<valueType>::MixtureAllocationMove(StochasticNode<valueType> *v, double w) : SimpleMove( v, w ), variable( v ) {
    
}


/** Clone object */
template <class valueType>
RevBayesCore::MixtureAllocationMove<valueType>* RevBayesCore::MixtureAllocationMove<valueType>::clone( void ) const {
    
    return new MixtureAllocationMove<valueType>( *this );
}



template <class valueType>
const std::string& RevBayesCore::MixtureAllocationMove<valueType>::getMoveName( void ) const {
    static std::string name = "Mixture-Reallocation";
    
    return name;
}



/** Perform the move */
template <class valueType>
double RevBayesCore::MixtureAllocationMove<valueType>::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    MixtureDistribution<valueType>& dist = static_cast<MixtureDistribution<valueType> &>( variable->getDistribution() );
    
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
void RevBayesCore::MixtureAllocationMove<valueType>::rejectSimpleMove( void ) {
    // just reset the index

    MixtureDistribution<valueType>& dist = static_cast<MixtureDistribution<valueType> &>( variable->getDistribution() );
    dist.setCurrentIndex( oldCategory );
    
}


template <class valueType>
void RevBayesCore::MixtureAllocationMove<valueType>::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<valueType>* >( newN );
}


#endif

