//
//  LengthTree.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-21.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef revbayes_LengthTree_h
#define revbayes_LengthTree_h

#include "TimeTree.h"
#include "TypedFunction.h"
#include "TypedDistribution.h"


namespace RevBayesCore {
    
    class LengthTree : public TypedFunction< RbVector<double> > {
        
    public:
        LengthTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< RbVector<double> > * v);
        
        LengthTree(const LengthTree& l);
        
        // public member functions
        LengthTree*                                         clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:

        void                                                recursiveUpdate(const TopologyNode& from);
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*           vals;
        const TypedDagNode< TimeTree >*                     tree;
    };
    
}



#endif
