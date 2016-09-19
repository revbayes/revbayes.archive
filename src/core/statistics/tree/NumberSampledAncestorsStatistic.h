/**
 * @file
 * This function take a time tree and returns the number of sampled ancestor "fossils"
 * in the tree
 *
 * @brief Declaration of the NumberSampledAncestorsStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: NumberSampledAncestorsStatistic.h by trayc7 $
 */

#ifndef NumberSampledAncestorsStatistic_H
#define NumberSampledAncestorsStatistic_H

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class NumberSampledAncestorsStatistic : public TypedFunction<int> {
        
    public:
        NumberSampledAncestorsStatistic(const TypedDagNode<Tree> *t);                                                                                   //!< Default constructor
        virtual                                    ~NumberSampledAncestorsStatistic(void);                                                                  //!< Destructor
                
        // Basic utility functions
        NumberSampledAncestorsStatistic*            clone(void) const;                                                                          //!< Clone object
        void                                        update(void);                                                                               //!< Clone the function
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<Tree>*                   tree;
        size_t                                      sampAncCount;
    };
    
}

#endif

