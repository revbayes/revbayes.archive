/**
 * @file
 * This file contains the declaration of EmpiricalTreeMove, which draws
 * a new tree from the EmpiricalTreeDistribution.
 *
 * @brief Declaration of EmpiricalTreeMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */

#ifndef EmpiricalTreeMove_H
#define EmpiricalTreeMove_H

#include <ostream>

#include "SimpleMove.h"

namespace RevBayesCore {
	
    template <class treeType>
    class EmpiricalTreeMove : public SimpleMove {
		
    public:
        EmpiricalTreeMove(StochasticNode<treeType>* node, double weight);                                                                      //!< Internal constructor
		
        // Basic utility functions
        EmpiricalTreeMove<treeType>*                            clone(void) const;                                                                  //!< Clone object
        void                                                    swapNode(DagNode *oldN, DagNode *newN);
		
    protected:
        const std::string&                                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                                  performSimpleMove(void);                                                            //!< Perform move
        void                                                    rejectSimpleMove(void);
		
    private:
		
        StochasticNode<treeType>*                               variable;
		size_t                                                  oldTreeIndex;
		
    };
    
}


#include "EmpiricalTreeDistribution.h"

#include <cmath>

template <class treeType>
RevBayesCore::EmpiricalTreeMove<treeType>::EmpiricalTreeMove(StochasticNode<treeType> *v, double w) : SimpleMove( v, w ), variable( v ) {
	
}


/** Clone object */
template <class treeType>
RevBayesCore::EmpiricalTreeMove<treeType>* RevBayesCore::EmpiricalTreeMove<treeType>::clone( void ) const {
    
    return new EmpiricalTreeMove<treeType>( *this );
}



template <class treeType>
const std::string& RevBayesCore::EmpiricalTreeMove<treeType>::getMoveName( void ) const
{
    static std::string name = "EmpiricalTreeMove";
    
    return name;
}



/** Perform the move */
template <class treeType>
double RevBayesCore::EmpiricalTreeMove<treeType>::performSimpleMove( void ) {
	    
    EmpiricalTreeDistribution<treeType>& dist = static_cast<EmpiricalTreeDistribution<treeType> &>( variable->getDistribution() );
	
	// get the current tree index
	oldTreeIndex = dist.getCurrentTreeIndex();
    
    // draw a new tree
    dist.redrawValue();
    
    variable->touch( true );
    
    return 0.0;
}


template <class treeType>
void RevBayesCore::EmpiricalTreeMove<treeType>::rejectSimpleMove( void )
{
	
	// reset to the old tree
    EmpiricalTreeDistribution<treeType>& dist = static_cast<EmpiricalTreeDistribution<treeType> &>( variable->getDistribution() );
	dist.setCurrentTree( oldTreeIndex );
    
}


template <class treeType>
void RevBayesCore::EmpiricalTreeMove<treeType>::swapNode(DagNode *oldN, DagNode *newN)
{
    
    SimpleMove::swapNode(oldN, newN);

	variable = static_cast<StochasticNode<treeType>* >( newN );
    
}

#endif

