//
//  AbstractCladogenicStateFunction.h
//  RevBayes_development_branch
//
//  Created by will freyman on 3/22/16.
//  Copyright (c) 2016 will freyman. All rights reserved.
//

#ifndef __RevBayes_development_branch__AbstractCladogenicStateFunction__
#define __RevBayes_development_branch__AbstractCladogenicStateFunction__

#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    class AbstractCladogenicStateFunction {
    
    public:
        
        virtual const std::map< std::vector<unsigned>, double >&        getEventMap(void) const = 0;
        
    protected:
        
        virtual void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP) = 0;
        
    };
    
}

#endif /* defined(__RevBayes_development_branch__AbstractCladogenicStateFunction__) */

