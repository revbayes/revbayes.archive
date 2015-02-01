 /**
 * @file
 * This file contains the declaration and implementation
 * of the EmpiricalTreeDistribution class. The distribution 
 * is constructed from an input tree trace.
 * 
 *
 * @brief Declaration and implementation of the EmpiricalTreeDistribution class
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0
 *
 */

#ifndef EmpiricalTreeDistribution_H
#define EmpiricalTreeDistribution_H

#include "TreeTrace.h"
#include "Sample.h"
#include "NewickConverter.h"

namespace RevBayesCore {
    
    template<typename treeType>
    class EmpiricalTreeDistribution : public TypedDistribution<treeType> {
        
    public:
		
		EmpiricalTreeDistribution(const TreeTrace<treeType> &t, int& b );
		
		virtual                                             ~EmpiricalTreeDistribution(void); 

		EmpiricalTreeDistribution<treeType>*                clone(void) const;                                          //!< Create an independent clone
		double                                              computeLnProbability(void);                                 //!< Compute ln prob of current value
		void                                                redrawValue(void);                                          //!< Draw a new random value from distribution
		size_t                                              getCurrentTreeIndex(void);
		void                                                setCurrentTree(size_t index);
		
	protected:

		void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
		
	private:
		
		double                                              probability;
		size_t                                              burnin;
		size_t                                              current_tree_index;
		TreeTrace<treeType>                                 trace;
		
	};

} 



#include "Tree.h"
#include "TreeTrace.h"

using namespace RevBayesCore;



template <typename treeType>
RevBayesCore::EmpiricalTreeDistribution<treeType>::EmpiricalTreeDistribution(const TreeTrace<treeType> &t, int &b) : TypedDistribution<treeType>( new treeType() ),
    probability( 0.0 ),
    burnin( b ),
    trace( t )
{
	// make sure burnin is proper
	if (trace.size() <= burnin)
    {
		throw RbException("Burnin size is too large for the trace.");
	}
	
    if (burnin == -1)
    {
		burnin = trace.size() / 4;
	}
    else
    {
		burnin = (size_t)(b);
	}
	
	// all the trees have the same probability
	probability = 1 / (double)(trace.size() - burnin);
	
	// draw the first value
	redrawValue();
}


template <typename treeType>
RevBayesCore::EmpiricalTreeDistribution<treeType>::~EmpiricalTreeDistribution()
{
    
}


template <typename treeType>
RevBayesCore::EmpiricalTreeDistribution<treeType>* RevBayesCore::EmpiricalTreeDistribution<treeType>::clone( void ) const {
    
    return new RevBayesCore::EmpiricalTreeDistribution<treeType>( *this );
}


template <typename treeType>
void RevBayesCore::EmpiricalTreeDistribution<treeType>::redrawValue( void ) {
	
	// draw a random tree
	RandomNumberGenerator* rng = GLOBAL_RNG;
	size_t total_tree_samples = trace.size();
	current_tree_index = (size_t)( rng->uniform01() * (total_tree_samples - burnin) + burnin - 1 );
	delete this->value;
	this->value = new treeType(*trace.objectAt(current_tree_index));
	
}


template <typename treeType>
size_t RevBayesCore::EmpiricalTreeDistribution<treeType>::getCurrentTreeIndex( void ) {
	
    return current_tree_index;
	
}


template <typename treeType>
void RevBayesCore::EmpiricalTreeDistribution<treeType>::setCurrentTree( size_t index ) {

	current_tree_index = index;
	delete this->value;
    this->value = new treeType(*trace.objectAt(index));
		
}


template <typename treeType>
double RevBayesCore::EmpiricalTreeDistribution<treeType>::computeLnProbability( void ) {
	
    return log( probability );
	
}


template <typename treeType>
void RevBayesCore::EmpiricalTreeDistribution<treeType>::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
}

#endif