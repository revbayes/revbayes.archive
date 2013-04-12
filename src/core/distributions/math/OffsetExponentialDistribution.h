/**
 * @file
 * This file contains the declaration of the exponentially distributed random variable class.
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



#ifndef OffsetExponentialDistribution_H
#define OffsetExponentialDistribution_H

#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class OffsetExponentialDistribution : public ContinuousDistribution {
        
    public:
        OffsetExponentialDistribution(const TypedDagNode<double> *l, const TypedDagNode<double> *o);
        OffsetExponentialDistribution(const OffsetExponentialDistribution &n);                                                                      //!< Copy constructor
        virtual                                            ~OffsetExponentialDistribution(void);                                              //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        OffsetExponentialDistribution*                      clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        double                                              getMax(void) const;
        double                                              getMin(void) const;
        double                                              quantile(double p) const;                                                       //!< Qu
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                     //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<double>*                         lambda;
        const TypedDagNode<double>*                         offset;
    };
    
}

#endif
