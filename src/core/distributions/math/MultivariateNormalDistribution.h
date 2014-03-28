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

#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "PrecisionMatrix.h"

#include <vector>

namespace RevBayesCore {
    
    class MultivariateNormalDistribution : public TypedDistribution< std::vector<double> > {
        
    public:
        MultivariateNormalDistribution(const TypedDagNode< std::vector<double> > *inmean, const TypedDagNode<PrecisionMatrix>* inprec);
        MultivariateNormalDistribution(const MultivariateNormalDistribution &n);                                                                          //!< Copy constructor
        virtual                                            ~MultivariateNormalDistribution(void);                                                //!< Virtual destructor
        
        
        // public member functions
        MultivariateNormalDistribution*                     clone(void) const;
        
 
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<std::vector<double> >*           mean;
        const TypedDagNode<PrecisionMatrix  >*              precision;

    };
    
}

#endif
