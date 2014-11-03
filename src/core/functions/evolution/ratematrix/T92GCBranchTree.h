/* 
 * File:   T92GCBranchTree.h
 * Author: nl
 *
 * Created on 25 juillet 2014, 20:20
 */

#ifndef T92GCBRANCHTREE_H
#define	T92GCBRANCHTREE_H

#include <iostream>

#include "RateMatrix_HKY.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "TimeTree.h"
#include "MatrixReal.h"
#include "MultivariateRealNodeContainer.h"
#include "RbVector.h"
#include <vector>

namespace RevBayesCore {
    
    class T92GCBranchTree : public TypedFunction< RbVector<RateMatrix> > {
        
    public:
        T92GCBranchTree(const TypedDagNode<TimeTree> *t, const TypedDagNode< RbVector<double> > *g, const TypedDagNode<double>* k);
        virtual                                            ~T92GCBranchTree(void) {}                                                         //!< Virtual destructor
        
        // public member functions
        T92GCBranchTree*         clone(void) const;                                                                        //!< Create an independent clone
        void                                                update(void);
        
    protected:

        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                                //!< Implementation of swaping parameters        
        void                                                recursiveUpdate(const TopologyNode& n);

        /*
        void                                                touch(DagNode *toucher);
        void                                                restore(DagNode *restorer);
        */
        
    private:
        
        /*
        void                                                flagNodes();
        void                                                corruptAll();
        void                                                recursiveCorruptAll(const TopologyNode& n);
        */
        
        // members
        const TypedDagNode< TimeTree >*                     tau;
        const TypedDagNode< RbVector<double> >*             gctree;
        const TypedDagNode< double >*                       kappa;
    };
    
}

#endif	/* T92GCBRANCHTREE_H */

