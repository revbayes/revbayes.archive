/**
 * @file
 * This file contains the declaration of the statistic for calculating the birth rate given div and turnover,
 *
 * @brief Declaration of the BirthRateConstBDStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: BirthRateConstBDStatistic.h tracyh $
 */

#ifndef BirthRateConstBDStatistic_H
#define BirthRateConstBDStatistic_H

#include "StochasticNode.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class BirthRateConstBDStatistic : public TypedFunction<double> {
        
    public:
		BirthRateConstBDStatistic(const TypedDagNode< double >* d, const TypedDagNode< double >* t);	//!< Default constructor
		BirthRateConstBDStatistic(const BirthRateConstBDStatistic& t);			//!< Copy constructor
        virtual													~BirthRateConstBDStatistic(void);											//!< Destructor
        
        BirthRateConstBDStatistic&							operator=(const BirthRateConstBDStatistic& t);
        
        // Basic utility functions
        BirthRateConstBDStatistic*							clone(void) const;																//!< Clone object
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

