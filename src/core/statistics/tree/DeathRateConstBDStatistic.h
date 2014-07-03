/**
 * @file
 * This file contains the declaration of the statistic for calculating the death rate given the diversification and turnover
 *
 * @brief Declaration of the DeathRateConstBDStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: DeathRateConstBDStatistic.h tracyh $
 */

#ifndef DeathRateConstBDStatistic_H
#define DeathRateConstBDStatistic_H

#include "StochasticNode.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class DeathRateConstBDStatistic : public TypedFunction<double> {
        
    public:
		DeathRateConstBDStatistic(const TypedDagNode< double >* d, const TypedDagNode< double >* t);	//!< Default constructor
		DeathRateConstBDStatistic(const DeathRateConstBDStatistic& t);			//!< Copy constructor
        virtual													~DeathRateConstBDStatistic(void);											//!< Destructor
        
        DeathRateConstBDStatistic&							operator=(const DeathRateConstBDStatistic& t);
        
        // Basic utility functions
        DeathRateConstBDStatistic*							clone(void) const;																//!< Clone object
        void													update(void);																	//!< Clone the function
		
    protected:
        void													swapParameterInternal(const DagNode *oldP, const DagNode *newP);				//!< Implementation of swaping parameters
        
    private:
        // members
        const TypedDagNode< double >*			diversification;
        const TypedDagNode< double >*			turnover;
    };
}


#endif

