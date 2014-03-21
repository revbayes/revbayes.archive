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



#ifndef ExponentialDistribution_H
#define ExponentialDistribution_H

#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class ExponentialDistribution : public ContinuousDistribution {
        
    public:
        ExponentialDistribution(const TypedDagNode<double> *l);
        ExponentialDistribution(const TypedDagNode<double> *l, const TypedDagNode<double> *o);
        ExponentialDistribution(const ExponentialDistribution &n);                                                                      //!< Copy constructor
        virtual                                            ~ExponentialDistribution(void);                                              //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        ExponentialDistribution*                            clone(void) const;                                                          //!< Create an independent clone
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
