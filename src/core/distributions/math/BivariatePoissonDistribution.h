//
//  BivariatePoissonDistribution.h
//  revbayes
//
//  Created by Alexander Zarebski on 2018-12-18.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#ifndef BivariatePoissonDistribution_H
#define BivariatePoissonDistribution_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "MatrixReal.h"

#include <vector>

namespace RevBayesCore {

    class BivariatePoissonDistribution : public TypedDistribution< RbVector<int> > {

    public:
      BivariatePoissonDistribution( const TypedDagNode<double> *theta0, const TypedDagNode<double> *theta1, const TypedDagNode<double> *theta2);
      virtual                                                         ~BivariatePoissonDistribution(void);


      BivariatePoissonDistribution*                                   clone(void) const;

      double                                                          computeLnProbability(void);
      void                                                            redrawValue(void);

    protected:
        // Parameter management functions
        void                                                            swapParameterInternal(const DagNode *oldP, const DagNode *newP);                    //!< Swap a parameter

    private:

        // members
        const TypedDagNode< double >*                                   theta0;
        const TypedDagNode< double >*                                   theta1;
        const TypedDagNode< double >*                                   theta2;

    };

}

#endif
