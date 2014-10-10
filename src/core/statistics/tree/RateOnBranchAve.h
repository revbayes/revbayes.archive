/**
 * @file
 * This file contains the declaration of the tree-height statistic, which simply computes
 * the height of a tree from root to tips.
 *
 * @brief Declaration of the RateOnBranchAve
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: TimeTree.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef RateOnBranchAve_H
#define RateOnBranchAve_H

//#include "Statistic.h"
#include "RbVector.h"
#include "TimeTree.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "StochasticNode.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class RateOnBranchAve : public TypedFunction<double> {
        
    public:
        RateOnBranchAve(const TypedDagNode< RbVector<double> > *rts, const TypedDagNode<TimeTree> *t, const TypedDagNode<double> *rr, const size_t idx);                                                                                   //!< Default constructor
        RateOnBranchAve(const TypedDagNode< RbVector<double> > *rts, const TypedDagNode<TimeTree> *t, const TypedDagNode<double> *rr, const TypedDagNode< double >* sv, const size_t idx);                                                                                   //!< Default constructor
        virtual                                    ~RateOnBranchAve(void);                                                                  //!< Destructor
        
        RateOnBranchAve&                        operator=(const RateOnBranchAve& t);
        
        // Basic utility functions
        RateOnBranchAve*                            clone(void) const;                                                                          //!< Clone object
        void                                        update(void);                                                                               //!< Clone the function
		
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
        // members
		const TypedDagNode< RbVector<double> >*     nodeRates;
        const TypedDagNode<TimeTree>*               tree;
		const TypedDagNode<double>*					rootRate;
		const TypedDagNode< double >*				scaleValue;
		const size_t								nodeID;
		
    };
	
}

#endif

