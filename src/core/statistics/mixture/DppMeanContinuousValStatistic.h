/**
 * @file
 * This file contains the declaration of the average value statistic for DPP-distributed continuous variables, 
 *
 * @brief Declaration of the DppNumTablesStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: DppMeanContinuousValStatistic.h tracyh $
 */

#ifndef DppMeanContinuousValStatistic_H
#define DppMeanContinuousValStatistic_H

#include "StochasticNode.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class DppMeanContinuousValStatistic : public TypedFunction<double> {
        
    public:
																DppMeanContinuousValStatistic(const StochasticNode< std::vector<double> >* v);	//!< Default constructor
																DppMeanContinuousValStatistic(const DppMeanContinuousValStatistic& t);			//!< Copy constructor
        virtual													~DppMeanContinuousValStatistic(void);											//!< Destructor
        
        DppMeanContinuousValStatistic&							operator=(const DppMeanContinuousValStatistic& t);
        
        // Basic utility functions
        DppMeanContinuousValStatistic*							clone(void) const;																//!< Clone object
        void													update(void);																	//!< Clone the function
		
    protected:
        void													swapParameterInternal(const DagNode *oldP, const DagNode *newP);				//!< Implementation of swaping parameters
        
    private:
        // members
        const StochasticNode< std::vector<double> >*			elementVals;
    };
}


#endif

