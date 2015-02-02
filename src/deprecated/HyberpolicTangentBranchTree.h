/* 
 * File:   HyberpolicTangentBranchTree.h
 * Author: nl
 *
 * Created on 25 juillet 2014, 19:52
 */

#ifndef HYBERPOLICTANGENTBRANCHTREE_H
#define	HYBERPOLICTANGENTBRANCHTREE_H

#include <iostream>


#include "RateMatrix.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "TimeTree.h"
#include "MatrixReal.h"
#include "MultivariateRealNodeContainer.h"
#include "RbVector.h"
#include <vector>

namespace RevBayesCore {
    
    class HyperbolicTangentBranchTree : public TypedFunction< RbVector<double> > {
        
    public:
        HyperbolicTangentBranchTree(const TypedDagNode<TimeTree> *t, const TypedDagNode< MultivariateRealNodeContainer > *p, const TypedDagNode<double>* o, const TypedDagNode< int > *i);
        virtual                                            ~HyperbolicTangentBranchTree(void) {}                                                         //!< Virtual destructor
        
        // public member functions
        HyperbolicTangentBranchTree*         clone(void) const;                                                                        //!< Create an independent clone
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

        /*
        void                                                flagNodes();
        void                                                corruptAll();
        void                                                recursiveCorruptAll(const TopologyNode& n);
        */
        
        // members
        const TypedDagNode< TimeTree >*                     tau;
        const TypedDagNode< MultivariateRealNodeContainer >*     process;
        const TypedDagNode< double >*                       offset;
        const TypedDagNode< int >*                          traitindex;
    };
    
}

#endif	/* HYBERPOLICTANGENTBRANCHTREE_H */

