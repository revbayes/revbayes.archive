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


#include "RateMatrix.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "TimeTree.h"
#include "MatrixReal.h"
#include "MultivariatePhyloProcess.h"
#include <vector>

namespace RevBayesCore {
    
    class ExponentialBranchTree : public TypedFunction< std::vector<double> > {
        
    public:
        ExponentialBranchTree(const TypedDagNode<TimeTree> *t, const TypedDagNode< MultivariatePhyloProcess > *p, const TypedDagNode<double>* o, const TypedDagNode< int > *i);
//         ExponentialBranchTree(const TypedDagNode< TimeTree > *t, /*const TypedDagNode<std::vector<double> > *n, */ const TypedDagNode< MatrixReal > *m, const TypedDagNode<double>* o, const TypedDagNode< int > *i);
        ExponentialBranchTree(const ExponentialBranchTree &n);                                                                              //!< Copy constructor
        virtual                                            ~ExponentialBranchTree(void) {}                                                         //!< Virtual destructor
        
        // public member functions
        ExponentialBranchTree*         clone(void) const;                                                                        //!< Create an independent clone
        void                                                update(void);
        
        int                                                 getTraitIndex();
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                                //!< Implementation of swaping parameters
        
        void                                                recursiveUpdate(const TopologyNode& n);
        
 //        void                                                touch(DagNode* toucher);

    private:
        
        // members
        const TypedDagNode< TimeTree >*                     tau;
        const TypedDagNode< MultivariatePhyloProcess >*     process;
        const TypedDagNode< double >*                       offset;
        const TypedDagNode< int >*                          traitindex;
    };
    
}

#endif /* defined(__revbayes__ExponentialBranchTree__) */
