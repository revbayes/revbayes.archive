/**
 * @file
 * This file contains the declaration of the binomially distribution class.
 *
 * @brief Declaration of the binomial distribution.
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



#ifndef BinomialDistribution_H
#define BinomialDistribution_H

#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class BinomialDistribution : public TypedDistribution<int> {
        
    public:
        BinomialDistribution(const TypedDagNode<int> *n, const TypedDagNode<double> *p);
        virtual                                            ~BinomialDistribution(void);                                             //!< Virtual destructor
        
        // public member functions
        BinomialDistribution*                               clone(void) const;                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);        //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<int>*                            n;
        const TypedDagNode<double>*                         p;
    };
    
}

#endif
