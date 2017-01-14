//
//  GeographicalDistanceFunction.hpp
//  revbayes-proj
//
//  Created by Michael Landis on 12/23/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef GeographicalDistanceFunction_hpp
#define GeographicalDistanceFunction_hpp

#include <stdio.h>
#include "ContinuousFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Hyperbolic Sine of a real number.
     *
     * Compute the hyperbolic Sine of a real number x. (sinh(x) = (exp(x) - exp(-x))/2).
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Wade Dismukes)
     * \since Version 1.0, 2015-01-31
     *
     */
    class GeographicalDistanceFunction : public ContinuousFunction {
        
    public:
        GeographicalDistanceFunction(const TypedDagNode<double> *lat1, const TypedDagNode<double> *lon1, const TypedDagNode<double> *lat2, const TypedDagNode<double> *lon2);
        
        GeographicalDistanceFunction*       clone(void) const;                                                  //!< Create a clone.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<double>*         latitude1;
        const TypedDagNode<double>*         longitude1;
        const TypedDagNode<double>*         latitude2;
        const TypedDagNode<double>*         longitude2;
    };
}


#endif /* GeographicalDistanceFunction_hpp */
