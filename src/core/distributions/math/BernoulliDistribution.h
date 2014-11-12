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
        virtual                                            ~BernoulliDistribution(void);                                              //!< Virtual destructor
        
        // public member functions
        BernoulliDistribution*                              clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<double>*                         p;
    };
    
}

#endif
