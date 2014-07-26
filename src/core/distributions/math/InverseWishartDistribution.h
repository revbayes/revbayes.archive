/* 
 * File:   InverseInverseWishartDistribution.h
 * Author: nl
 *
 * Created on 15 juillet 2014, 10:08
 */

#ifndef INVERSEWISHARTDISTRIBUTION_H
#define	INVERSEWISHARTDISTRIBUTION_H


#include <iostream>

#include "TypedDistribution.h"
#include "PrecisionMatrix.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class InverseWishartDistribution : public TypedDistribution<PrecisionMatrix>   {
        
    public:
        
        // inverse InverseWishart distribution of parameter sigma0 et df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<PrecisionMatrix> *insigma0, const TypedDagNode<int>* indf);
        InverseWishartDistribution(const InverseWishartDistribution& from);
        // specialized version: inverse InverseWishart of parameter sigma0=kappa*I and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf);
        
        virtual                                            ~InverseWishartDistribution(void) {}
        
        // public member functions

        InverseWishartDistribution*                                clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
        int                                                 getDF() const {return df->getValue();}
        
        // Parameter management functions
        std::set<const DagNode*>                            getParameters(void) const;                                          //!< Return parameters
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

        
    private:

        // members
        
        const TypedDagNode<PrecisionMatrix>*                sigma0;
        const TypedDagNode<double>*                         kappa;
        const TypedDagNode<int>*                            df;
        const TypedDagNode<int>*                            dim;
        
        // double bklnProb;
        
    };
    
}


#endif	/* INVERSEWISHARTDISTRIBUTION_H */

