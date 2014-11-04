/**
 * @file
 * This file contains the declaration of the Dirichletly distributed random variable class.
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



#ifndef DirichletDistribution_H
#define DirichletDistribution_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <vector>

namespace RevBayesCore {
    
    class DirichletDistribution : public TypedDistribution< RbVector<double> > {
        
    public:
        DirichletDistribution(const TypedDagNode< RbVector<double> > *l);
        virtual                                            ~DirichletDistribution(void);                                                //!< Virtual destructor
        
        // public member functions
        DirichletDistribution*                              clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             alpha;
    };
    
}

#endif
