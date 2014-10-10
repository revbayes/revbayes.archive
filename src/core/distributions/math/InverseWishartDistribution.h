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
#include "MatrixRealSymmetric.h"
#include "RbVector.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class InverseWishartDistribution : public TypedDistribution<MatrixRealSymmetric>   {
        
    public:
        
        // inverse InverseWishart distribution of parameter sigma0 and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<MatrixRealSymmetric> *insigma0, const TypedDagNode<int>* indf);

        // inverse InverseWishart distribution of parameter sigma0 = Diagonal(kappaVector) and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<RbVector<double> > *inkappaVector, const TypedDagNode<int>* indf);

        // inverse InverseWishart distribution of parameter sigma0 kappa * Identitymatrix and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf);

        // copy constructor
        InverseWishartDistribution(const InverseWishartDistribution& from);

        virtual                                            ~InverseWishartDistribution(void) {}
        
        // public member functions

        InverseWishartDistribution*                         clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
        int                                                 getDF() const {return df->getValue();}
        
        // Parameter management functions
        std::set<const DagNode*>                            getParameters(void) const;                                          //!< Return parameters
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:

        const TypedDagNode<MatrixRealSymmetric>*            sigma0;
        const TypedDagNode<RbVector<double> >*              kappaVector;
        const TypedDagNode<double>*                         kappa;
        const TypedDagNode<int>*                            df;
        const TypedDagNode<int>*                            dim;
                
    };
    
}


#endif	/* INVERSEWISHARTDISTRIBUTION_H */

