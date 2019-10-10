/*
 * File:   ExponentialErrorDistribution.h
 * Author: DavidCerny
 *
 * Created on October 8, 2019
 */

#ifndef EXPONENTIALERRORDISTRIBUTION_H
#define EXPONENTIALERRORDISTRIBUTION_H

#include <stddef.h>

#include "TypedDistribution.h"
#include "MatrixReal.h"
#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
class DagNode;
template <class valueType> class RbVector;
    
    class ExponentialErrorDistribution : public TypedDistribution<DistanceMatrix>   {
        
    public:
        
        // Exponential error distribution on an average distance matrix with parameter beta
        ExponentialErrorDistribution(const TypedDagNode<DistanceMatrix> *adm, const TypedDagNode<double> *l);

        virtual                                     ~ExponentialErrorDistribution(void) {}
        
        // Public member functions

        ExponentialErrorDistribution*               clone(void) const; // !< creates a clone
        double                                      computeLnProbability(void);
        void                                        redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP); // !< swaps a parameter

    private:

        const TypedDagNode<DistanceMatrix>*                 avgDistMat;
        const TypedDagNode<double>*                         lambda;
                
    };
    
}


#endif    /* EXPONENTIALERRORDISTRIBUTION_H */
