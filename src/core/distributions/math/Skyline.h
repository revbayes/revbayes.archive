/**
 * @file
 * This file contains the declaration of the skyline path random variable.
 * The random variable itself is the random path distributed by the skyline, that is values are 
 * uncorrelated and drawn from a uniform distribution.
 *
 * @brief Declaration of the Skyline path.
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



#ifndef Skyline_H
#define Skyline_H

#include "ConstantStepFunction.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Skyline : public TypedDistribution<UnivariateFunction> {
        
    public:
        Skyline(const TypedDagNode<double> *l, const TypedDagNode<double> *u, const TypedDagNode<double> *t, int n);
        Skyline(const Skyline &n);                                                                                              //!< Copy constructor
        virtual                                            ~Skyline(void);                                                                        //!< Virtual destructor
        
        // public member functions
        Skyline*                                            clone(void) const;                                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        int                                                 getNumberOfIntervals(void) const;
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                                //!< Implementation of swaping parameters
        
    private:
        ConstantStepFunction*                               simulate(void);
        
        // members
        const TypedDagNode<double>*                         lower;
        const TypedDagNode<double>*                         upper;
        const TypedDagNode<double>*                         time;
        int                                                 blocks;
    };
    
}

#endif
