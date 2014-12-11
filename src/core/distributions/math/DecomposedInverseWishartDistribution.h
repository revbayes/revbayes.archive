/* 
 * File:   DecomposedInverseInverseWishartDistribution.h
 * Author: nl
 *
 * Created on November 21, 2014
 */

#ifndef DecomposedInverseWishartDistribution_H
#define	DecomposedInverseWishartDistribution_H

#include <iostream>

#include "TypedDistribution.h"
#include "MatrixRealSymmetric.h"
#include "RbVector.h"
#include "TypedDagNode.h"



namespace RevBayesCore {
    
    class DecomposedInverseWishartDistribution : public TypedDistribution<MatrixRealSymmetric>   {
        
    public:
                                                    DecomposedInverseWishartDistribution(const TypedDagNode<MatrixRealSymmetric> *insigma0, const TypedDagNode<int>* indf);
                                                    DecomposedInverseWishartDistribution(const TypedDagNode<RbVector<double> > *inkappaVector, const TypedDagNode<int>* indf);
                                                    DecomposedInverseWishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf);
        virtual                                    ~DecomposedInverseWishartDistribution(void) {}
        DecomposedInverseWishartDistribution*       clone(void) const;                                                          //!< Create an independent clone
        double                                      computeLnProbability(void);
        void                                        redrawValue(void);
        int                                         getDF(void) const {return df->getValue();}
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:
        const TypedDagNode<MatrixRealSymmetric>*    sigma0;
        const TypedDagNode<RbVector<double> >*      kappaVector;
        const TypedDagNode<double>*                 kappa;
        const TypedDagNode<int>*                    df;
        const TypedDagNode<int>*                    dim;
    };
    
}

#endif

