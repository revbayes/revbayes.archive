//
//  WishartDistribution.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__WishartDistribution__
#define __revbayes__WishartDistribution__

#include <iostream>



#include "TypedDistribution.h"
#include "PrecisionMatrix.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class WishartDistribution : public TypedDistribution<PrecisionMatrix>   {
        
    public:
        
        // inverse Wishart distribution of parameter sigma0 et df degrees of freedom
        WishartDistribution(const TypedDagNode<PrecisionMatrix> *insigma0, const TypedDagNode<int>* indf);
        WishartDistribution(const WishartDistribution& from);
        // specialized version: inverse Wishart of parameter sigma0=kappa*I and df degrees of freedom
        WishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf);
        
        virtual                                            ~WishartDistribution(void) {}
        
        // public member functions

        WishartDistribution*                                clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);
        
//         size_t                                              getDim() const;
        
        int                                                 getDF() const {return df->getValue();}
        
        // double                                              getLogDet() const;
        
        void                                                drawNormalSample(std::vector<double>& v);
        
    private:

        // resample itself based on a sufficient statistic (empirical Precision, or scatter matrix)
        // void                                             GibbsResample(const PrecisionMatrix& empcov);
        // members
        
        const TypedDagNode<PrecisionMatrix>*                omega0;
        const TypedDagNode<double>*                         kappa;
        const TypedDagNode<int>*                            df;
        const TypedDagNode<int>*                            dim;
        
    };
    
}


#endif /* defined(__revbayes__WishartDistribution__) */
