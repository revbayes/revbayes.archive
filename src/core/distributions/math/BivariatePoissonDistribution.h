#ifndef BivariatePoissonDistribution_H
#define BivariatePoissonDistribution_H

#include "RbVector.h"
#include "TypedDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {

    /**
     * @brief Bivariate Poisson distribution class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-12, version 1.0
     *
     */
  class BivariatePoissonDistribution : public TypedDistribution< RbVector<long> > {

    public:
    BivariatePoissonDistribution(const TypedDagNode< double > *theta1, const TypedDagNode< double > *theta2, const TypedDagNode< double > *theta0);
        virtual                                            ~BivariatePoissonDistribution(void);                                                  //!< Virtual destructor

        // public member functions
        BivariatePoissonDistribution*                       clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:

        // members
        const TypedDagNode<double>*                         theta1;
        const TypedDagNode<double>*                         theta2;
        const TypedDagNode<double>*                         theta0;

    };

}

#endif
