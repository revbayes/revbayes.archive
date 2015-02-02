//
//  CompoundPoissonNormalDistribution.h
//  revbayes-proj
//
//  Created by Michael Landis on 11/19/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__CompoundPoissonNormalDistribution__
#define __revbayes_proj__CompoundPoissonNormalDistribution__

#include "TypedDagNode.h"
#include "ContinuousDistribution.h"

namespace RevBayesCore {
    
    class CompoundPoissonNormalDistribution : public ContinuousDistribution {
        
    public:
        CompoundPoissonNormalDistribution(const TypedDagNode<double> *r, const TypedDagNode<double> *m, const TypedDagNode<double>* s);
        virtual                                            ~CompoundPoissonNormalDistribution(void);                                             //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        CompoundPoissonNormalDistribution*                  clone(void) const;                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        double                                              getMax(void) const;
        double                                              getMin(void) const;
        void                                                redrawValue(void);
        double                                              quantile(double p) const;
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);        //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<double>*                         rate;
        const TypedDagNode<double>*                         sigma;
        const TypedDagNode<double>*                         mu;
    };
    
}

#endif /* defined(__revbayes_proj__CompoundPoissonNormalDistribution__) */
