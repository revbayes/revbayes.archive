//
//  ExponentialBranchTree.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__ExponentialBranchTree__
#define __revbayes__ExponentialBranchTree__

#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include "RateMatrix.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "TimeTree.h"
#include "MatrixReal.h"
#include "RealNodeContainer.h"
#include "MultivariateRealNodeContainer.h"
#include <vector>

namespace RevBayesCore {
    
    class ExponentialBranchTree : public TypedFunction< RbVector<double> > {
        
    public:
        ExponentialBranchTree(const TypedDagNode<TimeTree> *t, const TypedDagNode< MultivariateRealNodeContainer > *p, const TypedDagNode< RealNodeContainer > *up, const TypedDagNode<double>* o, const TypedDagNode< int > *i);
        virtual                                            ~ExponentialBranchTree(void) {}                                                         //!< Virtual destructor
        
        // public member functions
        ExponentialBranchTree*         clone(void) const;                                                                        //!< Create an independent clone
        void                                                update(void);
        
        int                                                 getTraitIndex();
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                                //!< Implementation of swaping parameters
        
        void                                                recursiveUpdate(const TopologyNode& n);

        /*
        void                                                touch(DagNode *toucher);
        void                                                restore(DagNode *restorer);
        */
    private:

        
        void                                                flagNodes(DagNode* toucher);
        void                                                corruptAll();
        void                                                recursiveCorruptAll(const TopologyNode& n);
        
        
        // members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< MultivariateRealNodeContainer >*    process;
        const TypedDagNode< RealNodeContainer >*                uniprocess;
        const TypedDagNode< double >*                           offset;
        const TypedDagNode< int >*                              traitindex;
    };
    
}

#endif /* defined(__revbayes__ExponentialBranchTree__) */
