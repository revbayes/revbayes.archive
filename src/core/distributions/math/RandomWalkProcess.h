/**
 * @file
 * This file contains the declaration of the random walk interface. Random variables representing a random walk,
 * such as the Brownian motion should derive from this class.
 *
 * @brief Declaration of the random walk base class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-14, version 1.0
 * @interface TypedDagNode
 *
 * $Id$
 */



#ifndef RandomWalkProcess_H
#define RandomWalkProcess_H

#include "LinearStepFunction.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class RandomWalkProcess : public TypedDistribution<UnivariateFunction> {
        
    public:
        
        RandomWalkProcess(LinearStepFunction *v);
        RandomWalkProcess(const RandomWalkProcess &n);                                                                             //!< Copy constructor
        virtual                                            ~RandomWalkProcess(void);                                                       //!< Virtual destructor
        
        // pure virtual methods
        virtual RandomWalkProcess*                          clone(void) const = 0;
        virtual double                                      computeLnProbability(void) = 0;                                             //!< Compute the log-probability for this random variable
        virtual int                                         getNumberOfIntervals(void) const = 0;
        
    };
    
}


#endif
