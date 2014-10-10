/**
 * @file
 * This file contains the declaration of the MultivariateNormally distributed random variable class.
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



#ifndef MultivariateNormalDistribution_H
#define MultivariateNormalDistribution_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "MatrixRealSymmetric.h"

#include <vector>

namespace RevBayesCore {
    
    class MultivariateNormalDistribution : public TypedDistribution< RbVector<double> > {
        
    public:
        MultivariateNormalDistribution(const TypedDagNode< RbVector<double> > *inmean, const TypedDagNode<MatrixRealSymmetric>* inprec);
        virtual                                            ~MultivariateNormalDistribution(void);                                                //!< Virtual destructor
        
        
        // public member functions
        MultivariateNormalDistribution*                     clone(void) const;
        
 
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);

        // Parameter management functions
        std::set<const DagNode*>                            getParameters(void) const;                                          //!< Return parameters
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             mean;
        const TypedDagNode< MatrixRealSymmetric  >*         precision;

    };
    
}

#endif
