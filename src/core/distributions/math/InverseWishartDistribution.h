//
//  InverseWishartDistribution.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__InverseWishartDistribution__
#define __revbayes__InverseWishartDistribution__

#include <iostream>


#include "CovarianceMatrix.h"

#include "TypedDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class InverseWishartDistribution : public TypedDistribution<CovarianceMatrix>   {
        
    public:
        
        // inverse Wishart distribution of parameter sigma0 et df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<CovarianceMatrix> *insigma0, const TypedDagNode<int> *indf);

        // specialized version: inverse Wishart of parameter sigma0=kappa*I and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<double> *inkappa, const TypedDagNode<int> *indf);
        
        virtual                                            ~InverseWishartDistribution(void);
        
        // public member functions

        InverseWishartDistribution*                         clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);
        
    private:

        // resample itself based on a sufficient statistic (empirical covariance, or scatter matrix)
        void                                                GibbsResample(const CovarianceMatrix& empcov);
        // members

        const TypedDagNode<CovarianceMatrix>*               sigma0;
        const TypedDagNode<double>*                         kappa;
        const TypedDagNode<int>*                            df;
        
    };
    
}


#endif /* defined(__revbayes__InverseWishartDistribution__) */
