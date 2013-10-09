/**
 * @file
 * This file contains the declaration of the Bernoulli distribution class.
 *
 * @brief Declaration of the Bernoulli distribution.
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



#ifndef BernoulliDistribution_H
#define BernoulliDistribution_H

#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class BernoulliDistribution : public TypedDistribution<int> {
        
    public:
        BernoulliDistribution(const TypedDagNode<double> *p);
        BernoulliDistribution(const BernoulliDistribution &n);                                                                      //!< Copy constructor
        virtual                                            ~BernoulliDistribution(void);                                              //!< Virtual destructor
        
        // public member functions
        BernoulliDistribution*                              clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                     //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<double>*                         p;
    };
    
}

#endif
