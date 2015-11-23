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


#include "Sample.h"
#include "TypedDistribution.h"
#include "Tree.h"
#include "TraceTree.h"

namespace RevBayesCore {
    
    class EmpiricalTreeDistribution : public TypedDistribution<Tree> {
        
    public:
		
		EmpiricalTreeDistribution(const TraceTree &t, int b );
		
		virtual                                             ~EmpiricalTreeDistribution(void); 

		EmpiricalTreeDistribution*                          clone(void) const;                                          //!< Create an independent clone
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
		TraceTree                                           trace;
		
	};

} 


#endif