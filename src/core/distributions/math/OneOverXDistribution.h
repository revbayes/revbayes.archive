/**
 * @file
 * This file contains the declaration of the 1/x distributed random variable class. Note,
 * to obtain a proper distribution we truncate this distribution (x in (0,a]).
 * This class is derived from the stochastic node and each instance will represent a random variable
 * from a normal distribution in the model graph.
 *
 * @brief Declaration of the stochastic DAG node base class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id:$
 */



#ifndef OneOverXDistribution_H
#define OneOverXDistribution_H

#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class OneOverXDistribution : public ContinuousDistribution {
        
    public:
        OneOverXDistribution(const TypedDagNode<double> *min, const TypedDagNode<double> *max);
        OneOverXDistribution(const OneOverXDistribution &n);                                                                            //!< Copy constructor
        virtual                                            ~OneOverXDistribution(void);                                                 //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        OneOverXDistribution*                               clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        double                                              getMax(void) const;
        double                                              getMin(void) const;
        double                                              quantile(double p) const;                                                       //!< Qu
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<double>*                         min;
        const TypedDagNode<double>*                         max;
        
    };
    
}

#endif
